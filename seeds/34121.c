/* { dg-skip-if "test is for emulation" { hard_dfp } } */

/* Touch tests that check for raising appropriate exceptions for binary
   arithmetic operations on decimal float values.  */

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

volatile _Decimal32 a32, b32, c32;
volatile _Decimal64 a64, b64, c64;
volatile _Decimal128 a128, b128, c128;
_Decimal32 inf32;
_Decimal64 inf64;
_Decimal128 inf128;

BINOP (100, /, a32, 2.0df, b32, 0.df, c32, FE_DIVBYZERO)
BINOP (101, /, a64, 2.0dd, b64, 0.dd, c64, FE_DIVBYZERO)
BINOP (102, /, a128, 2.0dl, b128, 0.dl, c128, FE_DIVBYZERO)

BINOP (200, /, a32, 0.df, b32, 0.df, c32, FE_INVALID)
BINOP (201, /, a64, 0.dd, b64, 0.dd, c64, FE_INVALID)
BINOP (202, /, a128, 0.dl, b128, 0.dl, c128, FE_INVALID)
BINOP (203, /, a32, inf32, b32, inf32, c32, FE_INVALID)
BINOP (204, /, a64, inf64, b64, inf64, c64, FE_INVALID)
BINOP (205, /, a128, inf128, b128, inf128, c128, FE_INVALID)
BINOP (206, *, a32, 0.df, b32, __builtin_infd32(), c32, FE_INVALID)
BINOP (207, *, a32, __builtin_infd32(), b32, 0.df, c32, FE_INVALID)
BINOP (208, *, a64, 0.df, b64, __builtin_infd64(), c64, FE_INVALID)
BINOP (209, *, a64, __builtin_infd64(), b64, 0.df, c64, FE_INVALID)
BINOP (210, *, a128, 0.df, b128, __builtin_infd128(), c128, FE_INVALID)
BINOP (211, *, a128, __builtin_infd128(), b128, 0.df, c128, FE_INVALID)
BINOP (212, +, a32, inf32, b32, -inf32, c32, FE_INVALID)
BINOP (213, +, a64, inf64, b64, -inf64, c64, FE_INVALID)
BINOP (214, +, a128, inf128, b128, -inf128, c128, FE_INVALID)
BINOP (215, -, a32, inf32, b32, inf32, c32, FE_INVALID)
BINOP (216, -, a64, inf64, b64, inf64, c64, FE_INVALID)
BINOP (217, -, a128, inf128, b128, inf128, c128, FE_INVALID)

BINOP (300, /, a32, 9.9e94df, b32, 1.e-3df, c32, FE_OVERFLOW|FE_INEXACT)
BINOP (301, /, a64, 9.9e382dd, b64, 1.e-3dd, c64, FE_OVERFLOW|FE_INEXACT)
BINOP (302, /, a128, 9.9e6142dl, b128, 1.e-3dl, c128, FE_OVERFLOW|FE_INEXACT)
BINOP (303, +, a32, 9.9e96df, b32, 1.e96df, c32, FE_OVERFLOW|FE_INEXACT)
BINOP (304, +, a64, 9.9e384dd, b64, 1.e384dd, c64, FE_OVERFLOW|FE_INEXACT)
BINOP (305, +, a128, 9.9e6144dl, b128, 1.e6144dl, c128, FE_OVERFLOW|FE_INEXACT)

BINOP (400, /, a32, 1.e-3df, b32, 9.9e94df, c32, FE_UNDERFLOW|FE_INEXACT)
BINOP (401, /, a64, 1.e-3dd, b64, 9.9e382dd, c64, FE_UNDERFLOW|FE_INEXACT)
BINOP (402, /, a128, 1.e-3dl, b128, 9.9e6142dl, c128, FE_UNDERFLOW|FE_INEXACT)
BINOP (403, *, a32, 1.e-95df, b32, 1.e-7df, c32, FE_UNDERFLOW|FE_INEXACT)
BINOP (404, *, a64, 1.e-383dd, b64, 1.e-16dd, c64, FE_UNDERFLOW|FE_INEXACT)
BINOP (405, *, a128, 1.e-6143dl, b128, 1.e-34dl, c128, FE_UNDERFLOW|FE_INEXACT)

BINOP (500, /, a32, 1.df, b32, 3.df, c32, FE_INEXACT)
BINOP (501, /, a64, 1.dd, b64, 3.dd, c64, FE_INEXACT)
BINOP (502, /, a128, 1.dl, b128, 3.dl, c128, FE_INEXACT)

int
main ()
{
  inf32 = __builtin_infd32();
  inf64 = __builtin_infd64();
  inf128 = __builtin_infd128();

  binop_100 ();
  binop_101 ();
  binop_102 ();

  binop_200 ();
  binop_201 ();
  binop_202 ();
  binop_203 ();
  binop_204 ();
  binop_205 ();
  binop_206 ();
  binop_207 ();
  binop_208 ();
  binop_209 ();
  binop_210 ();
  binop_211 ();
  binop_212 ();
  binop_213 ();
  binop_214 ();
  binop_215 ();
  binop_216 ();
  binop_217 ();

  binop_300 ();
  binop_301 ();
  binop_302 ();
  binop_303 ();
  binop_304 ();
  binop_305 ();

  binop_400 ();
  binop_401 ();
  binop_402 ();
  binop_403 ();
  binop_404 ();
  binop_405 ();

  binop_500 ();
  binop_501 ();
  binop_502 ();

  FINISH
}
