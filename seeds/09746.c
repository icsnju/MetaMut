/* { dg-do compile { target x86_64-*-* } } */
/* { dg-require-effective-target lp64 } */

#ifndef ANALYZER_DECLS_H
#define ANALYZER_DECLS_H

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

/* Function decls with special meaning to the analyzer.
   None of these are actually implemented.  */

/* Trigger a breakpoint in the analyzer when reached.  */
extern void __analyzer_break (void);

/* Emit a warning describing the 2nd argument (which can be of any
   type), at the given verbosity level.  This is for use when
   debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_describe (int verbosity, ...);

/* Dump copious information about the analyzer’s state when reached.  */
extern void __analyzer_dump (void);

/* Emit a warning describing the size of the base region of (*ptr).  */
extern void __analyzer_dump_capacity (const void *ptr);

/* Dump information about what decls have escaped at this point on the path.  */
extern void __analyzer_dump_escaped (void);

/* Dump information after analysis on all of the exploded nodes at this
   program point.

   __analyzer_dump_exploded_nodes (0);
   will dump just the number of nodes, and their IDs.

   __analyzer_dump_exploded_nodes (1);
   will also dump all of the states within those nodes.  */
extern void __analyzer_dump_exploded_nodes (int);

/* Emit a warning describing what is known about the value of NAME.  */
extern void __analyzer_dump_named_constant (const char *name);

/* Emit a placeholder "note" diagnostic with a path to this call site,
   if the analyzer finds a feasible path to it.  */
extern void __analyzer_dump_path (void);

/* Dump the region_model's state to stderr.  */
extern void __analyzer_dump_region_model (void);

/* Emit a warning describing the state of the 2nd argument
   (which can be of any type) with respect to NAME.
   This is for use when debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_dump_state (const char *name, ...);

/* Emit a warning with text "TRUE", FALSE" or "UNKNOWN" based on the
   truthfulness of the argument.  */
extern void __analyzer_eval (int);

/* Obtain an "unknown" void *.  */
extern void *__analyzer_get_unknown_ptr (void);

/* Complain if PTR doesn't point to a null-terminated string.
   TODO: eventually get the strlen of the buffer (without the
   optimizer touching it).  */
extern __SIZE_TYPE__ __analyzer_get_strlen (const char *ptr);

#endif /* #ifndef ANALYZER_DECLS_H.  */

#include <stdint.h>

int test_1 (int src)
{
  int dst;
  asm ("mov %1, %0\n\t"
       "add $1, %0"
       : "=r" (dst)
       : "r" (src));
  return dst;
}

uint32_t test_2 (uint32_t Mask)
{
  uint32_t Index;
  asm ("bsfl %[aMask], %[aIndex]"
       : [aIndex] "=r" (Index)
       : [aMask] "r" (Mask)
       : "cc");
  return Index;
}

int test_3a (int p1, int p2)
{
  asm goto ("btl %1, %0\n\t"
	    "jc %l2"
	    : // No outputs
	    : "r" (p1), "r" (p2)
	    : "cc"
	    : carry);

  return 0;

 carry:
  return 1;
}

int test_3b (int p1, int p2)
{
  asm goto ("btl %1, %0\n\t"
	    "jc %l[carry]"
	    : // No outputs
	    : "r" (p1), "r" (p2)
	    : "cc"
	    : carry);

  return 0;

 carry:
  return 1;
}

uint64_t test_4 (void)
{
  uint64_t start_time, end_time;

  // Get start time
  asm volatile ("rdtsc\n\t"    // Returns the time in EDX:EAX.
		"shl $32, %%rdx\n\t"  // Shift the upper bits left.
		"or %%rdx, %0"        // 'Or' in the lower bits.
		: "=a" (start_time)
		:
		: "rdx");

  // could do other work here

  // Get end time
  asm volatile ("rdtsc\n\t"    // Returns the time in EDX:EAX.
		"shl $32, %%rdx\n\t"  // Shift the upper bits left.
		"or %%rdx, %0"        // 'Or' in the lower bits.
		: "=a" (end_time)
		:
		: "rdx");

  __analyzer_eval (start_time == end_time); /* { dg-warning "UNKNOWN" } */

  // Get elapsed time
  return end_time - start_time;
}

static uint64_t get_time (void)
{
  uint64_t result;
  asm volatile ("rdtsc\n\t"    // Returns the time in EDX:EAX.
		"shl $32, %%rdx\n\t"  // Shift the upper bits left.
		"or %%rdx, %0"        // 'Or' in the lower bits.
		: "=a" (result)
		:
		: "rdx");
  return result;
}

uint64_t test_4a (void)
{
  uint64_t start_time, end_time;

  start_time = get_time ();
  // could do other work here
  end_time = get_time ();

  __analyzer_eval (start_time == end_time); /* { dg-warning "UNKNOWN" } */

  // Get elapsed time
  return end_time - start_time;
}

asm ("\t.pushsection .text\n"
     "\t.globl add_asm\n"
     "\t.type add_asm, @function\n"
     "add_asm:\n"
     "\tmovq %rdi, %rax\n"
     "\tadd %rsi, %rax\n"
     "\tret\n"
     "\t.popsection\n");

int test_5 (int count)
{
  asm goto ("dec %0; jb %l[stop]"
	    : "+r" (count)
	    :
	    :
	    : stop);
  return count;
stop:
  return 0;
}
