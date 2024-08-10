/* { dg-do link } */
/* { dg-require-effective-target sync_int_long } */
/* { dg-final { simulate-thread } } */


#include <stdio.h>
int simulate_thread_fini = 0;

void __attribute__((noinline))
simulate_thread_done ()
{
  simulate_thread_fini = 1;
}

/* A hostile thread is one which changes a memory location so quickly
   that another thread may never see the same value again.  This is
   simulated when simulate_thread_other_thread() is defined to modify
   a memory location every cycle.

   A process implementing a dependency on this value can run into
   difficulties with such a hostile thread.  For instance,
   implementing an add with a compare_and_swap loop goes something
   like:

     expected = *mem;
   loop:
     new = expected += value;
     if (!succeed (expected = compare_and_swap (mem, expected, new)))
       goto loop;

   If the content of 'mem' are changed every cycle by
   simulate_thread_other_thread () this will become an infinite loop
   since the value *mem will never be 'expected' by the time the
   compare_and_swap is executed.

   HOSTILE_THREAD_THRESHOLD defines the number of intructions which a
   program will execute before triggering the hostile thread
   pause. The pause will last for HOSTILE_THREAD_PAUSE instructions,
   and then the counter will reset and begin again.  During the pause
   period, simulate_thread_other_thread will not be called.

   This provides a chance for forward progress to be made and the
   infinite loop to be avoided.

   If the testcase defines HOSTILE_PAUSE_ERROR, then it will be
   considered a RUNTIME FAILURE if the hostile pause is triggered.
   This will allow to test for guaranteed forward progress routines.

   If the default values for HOSTILE_THREAD_THRESHOLD or
   HOSTILE_THREAD_PAUSE are insufficient, then the testcase may
   override these by defining the values before including this file.

   Most testcase are intended to run for very short periods of time,
   so these defaults are considered to be high enough to not trigger
   on a typical case, but not drag the test time out too much if a
   hostile condition is interferring.  */

  
/* Define the threshold instruction count to start pausing the hostile 
   thread.  To avoid huge potential log files when things are not going well,
   set this number very low.  If a test specifically requires that the forward
   progress guarantee is made, this number should be raised by the testcase. */
#if !defined (HOSTILE_THREAD_THRESHOLD)
#define HOSTILE_THREAD_THRESHOLD 	50
#endif

/* Define the length of pause in cycles for the hostile thread to pause to
   allow forward progress to be made.  If this number is too low, a 
   compare_and_swap loop may not have time to finish, especially on a
   128 bit operation. */
#if !defined (HOSTILE_THREAD_PAUSE)
#define HOSTILE_THREAD_PAUSE	20
#endif

/* Define the number of instructions which are allowed to be executed before
   the testcase is deemed to fail.  This is primarily to avoid huge log files
   when a testcase goes into an infinte loop.  */
#if !defined (INSN_COUNT_THRESHOLD)
#define INSN_COUNT_THRESHOLD	10000
#endif

void simulate_thread_other_threads (void);
int simulate_thread_final_verify (void);

static int simulate_thread_hostile_pause = 0;

/* This function wraps simulate_thread_other_threads an monitors for
   an infinite loop.  If the threshold value HOSTILE_THREAD_THRESHOLD
   is reached, the other_thread process is paused for
   HOSTILE_THREAD_PAUSE cycles before resuming, and the counters start
   again.  */
int
simulate_thread_wrapper_other_threads()
{
  static int insn_count = 0;
  static int hostile_count = 0;
  static int hostile_pause = 0;

  if (++insn_count >= INSN_COUNT_THRESHOLD)
    {
      printf ("FAIL: Testcase exceeded maximum instruction count threshold\n");
      return 1;
    }

  if (++hostile_count >= HOSTILE_THREAD_THRESHOLD)
    {
      if (!simulate_thread_hostile_pause)
        simulate_thread_hostile_pause = 1;

      /* Count cycles before calling the hostile thread again.  */
      if (hostile_pause++ < HOSTILE_THREAD_PAUSE)
	return 0;

      /* Reset the pause counter, as well as the thread counter.  */
      hostile_pause = 0;
      hostile_count = 0;
    }
  simulate_thread_other_threads ();
  return 0;
}


/* If the test case defines HOSTILE_PAUSE_ERROR, then the test case
   will fail execution if it had a hostile pause.  */
int
simulate_thread_wrapper_final_verify ()
{
  int ret = simulate_thread_final_verify ();
#if defined (HOSTILE_PAUSE_ERROR)
  if (simulate_thread_hostile_pause)
    {
      printf ("FAIL: Forward progress made only by pausing hostile thread\n");
      ret = ret | 1;    /* 0 indicates proper comnpletion.  */
    }
#endif
  return ret;
}


/* Testing load for atomicity is a little trickier.  

   Set up the atomic value so that it changes value after every instruction 
   is executed.

   Simply alternating between 2 values wouldn't be sufficient since a load of
   one part, followed by the load of the second part 2 instructions later would
   appear to be valid.

   set up a table of 16 values which change a bit in every byte of the value 
   each time, this will give us a 16 instruction cycle before repetition
   kicks in, which should be sufficient to detect any issues.  Just to be sure,
   we also change the table cycle size during execution. 
   
   The end result is that all loads should always get one of the values from
   the table. Any other pattern means the load failed.  */

unsigned int ret;
unsigned int value = 0;
unsigned int result = 0;
unsigned int table[16] = {
0x00000000, 
0x11111111, 
0x22222222, 
0x33333333,
0x44444444,
0x55555555,
0x66666666,
0x77777777,
0x88888888,
0x99999999,
0xAAAAAAAA,
0xBBBBBBBB,
0xCCCCCCCC,
0xDDDDDDDD,
0xEEEEEEEE,
0xFFFFFFFF
};

int table_cycle_size = 16;

/* Return 0 if 'result' is a valid value to have loaded.  */
int verify_result ()
{
  int x;
  int found = 0;

  /* Check entire table for valid values.  */
  for (x = 0; x < 16 ; x++)
    if (result == table[x])
      {
	found = 1;
	break;
      }

  if (!found)
    printf("FAIL: Invalid result returned from fetch\n");

  return !found;
}

/* Iterate VALUE through the different valid values. */
void simulate_thread_other_threads ()
{
  static int current = 0;

  if (++current >= table_cycle_size)
    current = 0;
  value = table[current];
}

int simulate_thread_step_verify ()
{
  return verify_result ();
}

int simulate_thread_final_verify ()
{
  return verify_result ();
}

__attribute__((noinline))
void simulate_thread_main()
{
  int x;

  /* Execute loads with value changing at various cyclic values.  */
  for (table_cycle_size = 16; table_cycle_size > 4 ; table_cycle_size--)
    {
      ret = __atomic_load_n (&value, __ATOMIC_SEQ_CST);
      /* In order to verify the returned value (which is not atomic), it needs
	 to be atomically stored into another variable and check that.  */
      __atomic_store_n (&result, ret, __ATOMIC_SEQ_CST);

      /* Execute the fetch/store a couple of times just to ensure the cycles
         have a chance to be interesting.  */
      ret = __atomic_load_n (&value, __ATOMIC_SEQ_CST);
      __atomic_store_n (&result, ret, __ATOMIC_SEQ_CST);
    }
}

int
main()
{
  simulate_thread_main ();
  simulate_thread_done ();
  return 0;
}
