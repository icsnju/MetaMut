/* { dg-do compile } */
/* { dg-require-effective-target persistent } */
/* { dg-skip-if "data LMA != VMA" { msp430-*-* } { "-mlarge" } } */
/* { dg-options "-fdata-sections -save-temps" } */
/* { dg-final { scan-assembler ".section\t.persistent.var_persistent,\"aw\"\n" } } */

/* Test the "persistent" attribute with -fdata-sections.  */
/* This test checks that data marked with the "persistent" attribute is handled
   correctly.
   If data LMA != VMA (e.g. for simulating the copy of data from ROM to RAM),
   then var_init will always be re-initialized to 2 and this test will loop
   forever, so it must be skipped for those targets.  */

#ifdef __cplusplus
extern "C" {
#endif
extern void _start (void) __attribute__ ((noreturn));
#ifdef __cplusplus
}
#endif

int var_common;
int var_zero = 0;
int var_one = 1;
int __attribute__((persistent)) var_persistent = 2;
int var_init = 2;

int
main (void)
{
  /* Make sure that the C startup code has correctly initialized the ordinary variables.  */
  if (var_common != 0)
    __builtin_abort ();

  /* Initialized variables are not re-initialized during startup, so
     check their original values only during the first run of this
     test.  */
  if (var_init == 2)
    if (var_zero != 0 || var_one != 1 || var_persistent != 2)
      __builtin_abort ();

  switch (var_init)
    {
    case 2:
      /* First time through - change all the values.  */
      var_common = var_zero = var_one = var_persistent = var_init = 3;
      break;

    case 3:
      /* Second time through - make sure that var_persistent has not been reset.  */
      if (var_persistent != 3)
	__builtin_abort ();
      __builtin_exit (0);

    default:
      /* Any other value for var_init is an error.  */
      __builtin_abort ();
    }

  /* Simulate a processor reset by calling the C startup code.  */
  _start ();

  /* Should never reach here.  */
  __builtin_abort ();
}