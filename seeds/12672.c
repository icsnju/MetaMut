/* { dg-skip-if "test is for emulation" { hard_dfp } } */

/* Check that appropriate exceptions are raised for conversions involving
   decimal float values.  */

/* Common support for checking that appropriate floating point exceptions
   are raised for decimal float operations.  These tests are here to test
   the software decimal float support in libgcc.  */

#define EXTERN extern

int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#define FINISH if (failures != 0) __builtin_abort (); return 0;
#else
#define FAILURE __builtin_abort ();
#define FINISH return 0;
#endif
/* Use undocumented functions in libgcc to clear and test dummy floating
   point exception flags.  That functionality is in libgcc just for
   testing purposes.

   If fesetexcept and feclearexcept are available, use those instead.  */

/* Get names of exception flags.  */
#include <fenv.h>

extern void __dfp_clear_except (int);
#define DFP_CLEAR_EXCEPT(M) __dfp_clear_except(M)
extern int __dfp_test_except (int);
#define DFP_TEST_EXCEPT(M) __dfp_test_except(M)

#if defined(DBG) || defined(DBG2)
#include <stdio.h>
#undef FAILURE
#define FAILURE(NUM,KIND,EXCEPT) \
  { printf ("failed for test %d: %s %s\n", NUM, KIND, EXCEPT); failures++; }
#else
#undef FAILURE
#define FAILURE(N,K,E) __builtin_abort ();
#endif

/* This is useful when modifying the test to make sure that tests are
   actually run.  */
#if defined(DBG2)
#define SUCCESS(NUM,EXCEPT) \
  { printf ("passed for test %d: %s\n", NUM, EXCEPT); }
#else
#define SUCCESS(N,E) ;
#endif

#define CHECKFLAG(NUM,EXCEPT,GOT,WANT)				\
  if ((WANT & EXCEPT) != (GOT & EXCEPT))			\
    {								\
      if ((WANT & EXCEPT) != 0)					\
        FAILURE (NUM, "missing", #EXCEPT)			\
      else							\
        FAILURE (NUM, "unexpected", #EXCEPT)			\
    }								\
  else								\
    SUCCESS (NUM, #EXCEPT)

void
checkflags (int num, int want)
{
  int got = DFP_TEST_EXCEPT (FE_ALL_EXCEPT);
  CHECKFLAG (num, FE_INVALID, got, want)
  CHECKFLAG (num, FE_OVERFLOW, got, want)
  CHECKFLAG (num, FE_UNDERFLOW, got, want)
  CHECKFLAG (num, FE_DIVBYZERO, got, want)
  CHECKFLAG (num, FE_INEXACT, got, want)
}

#define BINOP(NUM,OP,VAR1,VAL1,VAR2,VAL2,VAR3,EXCEPT)		\
void								\
binop_##NUM (void)						\
{								\
  VAR1 = VAL1;							\
  VAR2 = VAL2;							\
  DFP_CLEAR_EXCEPT (FE_ALL_EXCEPT);				\
  VAR3 = VAR1 OP VAR2;						\
  checkflags (NUM, EXCEPT);					\
}

#define CONVERT(NUM,FROM,TO,VALUE,EXCEPT)			\
void								\
convert_##NUM (void)						\
{								\
  FROM = VALUE;							\
  DFP_CLEAR_EXCEPT (FE_ALL_EXCEPT);				\
  TO = FROM;							\
  checkflags (NUM, EXCEPT);					\
}

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;

/* Conversions between decimal float types should raise an invalid
   exception if the value doesn't fit, either because the value
   is too large or the result can't hold the full precision.  */

CONVERT (100, d64, d32, 9.999999e96DD, 0)
CONVERT (101, d64, d32, 10.000000e96DD, FE_INEXACT|FE_OVERFLOW)
CONVERT (102, d64, d32, 1.1111111DD, FE_INEXACT)
CONVERT (110, d128, d32, 9.999999e96DL, 0)
CONVERT (111, d128, d32, 10.000000e96DL, FE_INEXACT|FE_OVERFLOW)
CONVERT (112, d128, d32, 1.1111111DL, FE_INEXACT)
CONVERT (120, d128, d64, 9.999999999999999E384DL, 0)
CONVERT (121, d128, d64, 10.00000000000000E384DL, FE_INEXACT|FE_OVERFLOW)
CONVERT (122, d128, d64, 1.1111111111111111DL, FE_INEXACT)

int
main ()
{
  convert_100 ();
  convert_101 ();
  convert_102 ();
  convert_110 ();
  convert_111 ();
  convert_112 ();
  convert_120 ();
  convert_121 ();
  convert_122 ();

  FINISH
}
