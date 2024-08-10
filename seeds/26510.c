/* Check that appropriate exceptions are raised for int to DFP conversions.  */

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
volatile signed int si;
volatile unsigned int ui;
volatile signed long long sll;
volatile unsigned long long ull;

CONVERT (100, si, d32, 9999999, 0)
CONVERT (101, si, d32, 11111111, FE_INEXACT)
CONVERT (102, si, d32, -9999999, 0)
CONVERT (103, si, d32, -10000001, FE_INEXACT)
CONVERT (110, ui, d32, 9999999, 0)
CONVERT (111, ui, d32, 10000001, FE_INEXACT)
CONVERT (200, sll, d64, 9999999999999999, 0)
CONVERT (201, sll, d64, 10000000000000001, FE_INEXACT)
CONVERT (202, sll, d64, -9999999999999999, 0)
CONVERT (203, sll, d64, -10000000000000001, FE_INEXACT)
CONVERT (210, ull, d64, 9999999999999999, 0)
CONVERT (211, ull, d64, 10000000000000001, FE_INEXACT)

int
main ()
{
  if (sizeof (long long) != 16)
    return 0;

  convert_100 ();
  convert_101 ();
  convert_102 ();
  convert_103 ();
  convert_110 ();
  convert_111 ();
  convert_200 ();
  convert_201 ();
  convert_202 ();
  convert_203 ();
  convert_210 ();
  convert_211 ();

  FINISH
}
