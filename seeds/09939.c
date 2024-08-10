/* { dg-xfail-run-if "" { lax_strtofp } } */
/* { dg-options "-w" } */

/* This test assumes IEEE float and double.  */

#define __STDC_WANT_DEC_FP__
#include <float.h>

#ifdef __cplusplus
typedef float _Decimal32 __attribute__((mode(SD)));
typedef float _Decimal64 __attribute__((mode(DD)));
typedef float _Decimal128 __attribute__((mode(TD)));

#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#define FINISH if (failures != 0) __builtin_abort (); return 0;
#else
#define FAILURE __builtin_abort ();
#define FINISH return 0;
#endif

/* Macros are set up to skip using long double, which doesn't necessarily
   map to TF mode.  If there's a reason to skip those for a test, the
   test itself can define USE_TF to be zero.  */
#ifndef USE_TF
#define USE_TF 1
#endif

/* Provide more information with FAILURE than what is available with
   the version of that macro in dfp-dbg.h.  */

#undef FAILURE
#if defined(DBG) || defined(DBG2)
#include <stdio.h>
#define FAILURE(NUM) \
  { printf ("failed for test %s\n", NUM); failures++; }
#else
#define FAILURE(N) __builtin_abort ();
#endif

/* This is useful when modifying the test to make sure that tests are
   actually run.  */
#if defined(DBG2)
#define REPORT(NUM) \
  { printf ("%s\n", NUM); }
#else
#define REPORT(N) ;
#endif

#define CONVERT_VALID(NUM,FROM,TO,FROMVAL,TOVAL,DIFF)		\
void								\
convert_##NUM (void)						\
{								\
  REPORT(#NUM " " #FROMVAL)					\
  FROM = FROMVAL;						\
  TO = FROM;							\
  if (TO < (TOVAL - DIFF) || TO > (TOVAL + DIFF))		\
    FAILURE (#NUM);						\
}

#define CONVERT_TO_PINF(NUM,FROM,TO,FROMVAL,TOSUFFIX)		\
void								\
convert_##NUM (void)						\
{								\
  REPORT(#NUM " " #FROMVAL)					\
  FROM = FROMVAL;						\
  TO = FROM;							\
  if (__builtin_isinf##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " pinf: isinf");				\
  if (__builtin_signbit##TOSUFFIX (TO) != 0)			\
    FAILURE (#NUM " pinf: sign");				\
}

#define CONVERT_TO_MINF(NUM,FROM,TO,FROMVAL,TOSUFFIX)		\
void								\
convert_##NUM (void)						\
{								\
  REPORT(#NUM " " #FROMVAL)					\
  FROM = FROMVAL;						\
  TO = FROM;							\
  if (__builtin_isinf##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " pinf: isinf");				\
  if (__builtin_signbit##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " pinf: sign");				\
}

#define CONVERT_TO_PZERO(NUM,FROM,TO,FROMVAL,TOVAL,TOSUFFIX)	\
void								\
convert_##NUM (void)						\
{								\
  REPORT(#NUM " " #FROMVAL)					\
  FROM = FROMVAL;						\
  TO = FROM;							\
  if (TO != TOVAL)						\
    FAILURE (#NUM "_pzero: zero")				\
  if (__builtin_signbit##TOSUFFIX (TO) != 0)			\
    FAILURE (#NUM " _pzero: sign");				\
}

#define CONVERT_TO_MZERO(NUM,FROM,TO,FROMVAL,TOVAL,TOSUFFIX)	\
void								\
convert_##NUM (void)						\
{								\
  REPORT(#NUM " " #FROMVAL)					\
  FROM = FROMVAL;						\
  TO = FROM;							\
  if (TO != TOVAL)						\
    FAILURE (#NUM "_mzero: zero")				\
  if (__builtin_signbit##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " _mzero: sign");				\
}

#define CONVERT_NAN(NUM,FROM,TO,FROMSUFFIX,TOSUFFIX)		\
void								\
convert_##NUM##_nan (void)					\
{								\
  REPORT(#NUM "_nan")						\
  FROM = __builtin_nan##FROMSUFFIX ("");			\
  TO = FROM;							\
  if (__builtin_isnan##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " nan");					\
}

#define CONVERT_PINF(NUM,FROM,TO,FROMSUFFIX,TOSUFFIX)		\
void								\
convert_##NUM##_pinf (void)					\
{								\
  REPORT (#NUM "_pinf")						\
  FROM = __builtin_inf##FROMSUFFIX ();				\
  TO = FROM;							\
  if (__builtin_isinf##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " pinf: isinf");				\
  if (__builtin_signbit##TOSUFFIX (TO) != 0)			\
    FAILURE (#NUM " pinf: sign");				\
}

#define CONVERT_MINF(NUM,FROM,TO,FROMSUFFIX,TOSUFFIX)		\
void								\
convert_##NUM##_minf (void)					\
{								\
  REPORT (#NUM "_minf")						\
  FROM = -__builtin_inf##FROMSUFFIX ();				\
  TO = FROM;							\
  if (__builtin_isinf##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " minf: isinf");				\
  if (__builtin_signbit##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " minf: sign");				\
}

#define CONVERT_PZERO(NUM,FROM,TO,FROMVALUE,TOVALUE,TOSUFFIX)	\
void								\
convert_##NUM##_pzero (void)					\
{								\
  REPORT (#NUM "_pzero")					\
  FROM = FROMVALUE;						\
  TO = FROM;							\
  if (TO != TOVALUE)						\
    FAILURE (#NUM "pzero: zero")				\
  if (__builtin_signbit##TOSUFFIX (TO) != 0)			\
    FAILURE (#NUM " pzero: sign");				\
}

#define CONVERT_MZERO(NUM,FROM,TO,FROMVALUE,TOVALUE,TOSUFFIX)	\
void								\
convert_##NUM##_mzero (void)					\
{								\
  REPORT (#NUM "_mzero")					\
  FROM = FROMVALUE;						\
  TO = FROM;							\
  if (TO != TOVALUE)						\
    FAILURE (#NUM "mzero: zero")				\
  if (__builtin_signbit##TOSUFFIX (TO) == 0)			\
    FAILURE (#NUM " mzero: sign");				\
}

#define CONVERT_VALID_NOTF(NUM,VAL,DIFF)			\
CONVERT_VALID (NUM##_sdsf, sd, sf, VAL##df, VAL##f, DIFF##f)	\
CONVERT_VALID (NUM##_sddf, sd, df, VAL##df, VAL, DIFF)		\
CONVERT_VALID (NUM##_ddsf, dd, sf, VAL##dd, VAL##f, DIFF##f)	\
CONVERT_VALID (NUM##_dddf, dd, df, VAL##dd, VAL, DIFF)		\
CONVERT_VALID (NUM##_tdsf, td, sf, VAL##dl, VAL##f, DIFF##f)	\
CONVERT_VALID (NUM##_tddf, td, df, VAL##dl, VAL, DIFF)		\
CONVERT_VALID (NUM##_sfsd, sf, sd, VAL##f, VAL##df, DIFF##df)	\
CONVERT_VALID (NUM##_sfdd, sf, dd, VAL##f, VAL##dd, DIFF##dd)	\
CONVERT_VALID (NUM##_sftd, sf, td, VAL##f, VAL##dl, DIFF##dl)	\
CONVERT_VALID (NUM##_dfsd, df, sd, VAL, VAL##df, DIFF##df)	\
CONVERT_VALID (NUM##_dfdd, df, dd, VAL, VAL##dd, DIFF##dd)	\
CONVERT_VALID (NUM##_dftd, df, td, VAL, VAL##dl, DIFF##dl)	\
CONVERT_VALID (NUM##_sddd, sd, dd, VAL##df, VAL##dd, DIFF##dd)	\
CONVERT_VALID (NUM##_sdtd, sd, dd, VAL##df, VAL##dd, DIFF##dd)	\
CONVERT_VALID (NUM##_ddsd, dd, sd, VAL##dd, VAL##df, DIFF##dd)	\
CONVERT_VALID (NUM##_ddtd, dd, td, VAL##dd, VAL##dl, DIFF##dl)	\
CONVERT_VALID (NUM##_tdsd, td, sd, VAL##dl, VAL##df, DIFF##df)	\
CONVERT_VALID (NUM##_tddd, td, dd, VAL##dl, VAL##dd, DIFF##dd)

#if USE_TF == 0
#define CONVERT_VALID_TF(NUM,VAL,DIFF)
#else
#define CONVERT_VALID_TF(NUM,VAL,DIFF)				\
CONVERT_VALID (NUM##_sdtf, sd, tf, VAL##df, VAL##l, DIFF##l)	\
CONVERT_VALID (NUM##_tdtf, td, tf, VAL##dl, VAL##l, DIFF##l)	\
CONVERT_VALID (NUM##_ddtf, dd, tf, VAL##dd, VAL##l, DIFF##l)	\
CONVERT_VALID (NUM##_tfsd, tf, sd, VAL##l, VAL##df, DIFF##df)	\
CONVERT_VALID (NUM##_tfdd, tf, dd, VAL##l, VAL##dd, DIFF##dd)	\
CONVERT_VALID (NUM##_tftd, tf, td, VAL##l, VAL##dl, DIFF##dl)
#endif

#define CONVERT_VALID_ALL(NUM,VAL,DIFF)				\
  CONVERT_VALID_NOTF(NUM,VAL,DIFF)				\
  CONVERT_VALID_TF(NUM,VAL,DIFF)

#define CALL_VALID_NOTF(NUM)					\
  convert_##NUM##_sdsf ();					\
  convert_##NUM##_sddf ();					\
  convert_##NUM##_ddsf ();					\
  convert_##NUM##_dddf ();					\
  convert_##NUM##_tdsf ();					\
  convert_##NUM##_tddf ();					\
  convert_##NUM##_sfsd ();					\
  convert_##NUM##_sfdd ();					\
  convert_##NUM##_sftd ();					\
  convert_##NUM##_dfsd ();					\
  convert_##NUM##_dfdd ();					\
  convert_##NUM##_dftd ();					\
  convert_##NUM##_sddd ();					\
  convert_##NUM##_sdtd ();					\
  convert_##NUM##_ddsd ();					\
  convert_##NUM##_ddtd ();					\
  convert_##NUM##_tdsd ();					\
  convert_##NUM##_tddd ();

#if USE_TF == 0
#define CALL_VALID_TF(NUM)
#else
#define CALL_VALID_TF(NUM)					\
  convert_##NUM##_sdtf ();					\
  convert_##NUM##_ddtf ();					\
  convert_##NUM##_tdtf ();					\
  convert_##NUM##_tfsd ();					\
  convert_##NUM##_tfdd ();					\
  convert_##NUM##_tftd ();
#endif

#define CALL_VALID_ALL(NUM)					\
  CALL_VALID_NOTF(NUM)						\
  CALL_VALID_TF(NUM)

#define CONVERT_ZEROES(NUM,FROM,TO,FROMVALUE,TOVALUE,TOSUFFIX)	\
CONVERT_PZERO(NUM, FROM, TO, FROMVALUE, TOVALUE, TOSUFFIX)	\
CONVERT_MZERO(NUM, FROM, TO, -FROMVALUE, -TOVALUE, TOSUFFIX)

#define CONVERT_ZEROES_NOTF(NUM)				\
CONVERT_ZEROES (NUM##_sdsf, sd, sf, 0.0df, 0.0f, f)		\
CONVERT_ZEROES (NUM##_sddf, sd, df, 0.0df, 0.0, )		\
CONVERT_ZEROES (NUM##_ddsf, dd, sf, 0.0dd, 0.0f, f)		\
CONVERT_ZEROES (NUM##_dddf, dd, df, 0.0dd, 0.0, )		\
CONVERT_ZEROES (NUM##_tdsf, td, sf, 0.0dl, 0.0f, f)		\
CONVERT_ZEROES (NUM##_tddf, td, df, 0.0dl, 0.0, )		\
CONVERT_ZEROES (NUM##_sfsd, sf, sd, 0.0f, 0.0df, d32)		\
CONVERT_ZEROES (NUM##_sfdd, sf, dd, 0.0f, 0.0dd, d64)		\
CONVERT_ZEROES (NUM##_sftd, sf, td, 0.0f, 0.0dl, d128)		\
CONVERT_ZEROES (NUM##_dfsd, df, sd, 0.0, 0.0df, d32)		\
CONVERT_ZEROES (NUM##_dfdd, df, dd, 0.0, 0.0dd, d64)		\
CONVERT_ZEROES (NUM##_dftd, df, td, 0.0, 0.0dl, d128)		\
CONVERT_ZEROES (NUM##_sddd, sd, dd, 0.0df, 0.0dd, d64)		\
CONVERT_ZEROES (NUM##_sdtd, sd, td, 0.0dl, 0.0dl, d128)		\
CONVERT_ZEROES (NUM##_ddsd, dd, sd, 0.0dd, 0.0df, d32)		\
CONVERT_ZEROES (NUM##_ddtd, dd, td, 0.0dd, 0.0dl, d128)		\
CONVERT_ZEROES (NUM##_tdsd, td, sd, 0.0dl, 0.0df, d32)		\
CONVERT_ZEROES (NUM##_tddd, td, dd, 0.0dl, 0.0dd, d64)

#if USE_TF == 0
#define CONVERT_ZEROES_TF(NUM)
#else
#define CONVERT_ZEROES_TF(NUM)					\
CONVERT_ZEROES (NUM##_sdtf, sd, tf, 0.0df, 0.0l, l)		\
CONVERT_ZEROES (NUM##_ddtf, dd, tf, 0.0dd, 0.0l, l)		\
CONVERT_ZEROES (NUM##_tdtf, td, tf, 0.0dl, 0.0l, l)		\
CONVERT_ZEROES (NUM##_tfsd, tf, sd, 0.0l, 0.0df, d32)		\
CONVERT_ZEROES (NUM##_tfdd, tf, dd, 0.0l, 0.0dd, d64)		\
CONVERT_ZEROES (NUM##_tftd, tf, td, 0.0l, 0.0dl, d128)
#endif

#define CONVERT_ZEROES_ALL(NUM)					\
  CONVERT_ZEROES_NOTF(NUM)					\
  CONVERT_ZEROES_TF(NUM)

#define CALL_ZEROES(NUM)					\
  convert_##NUM##_pzero ();					\
  convert_##NUM##_mzero ();

#define CALL_ZEROES_NOTF(NUM)					\
  CALL_ZEROES (NUM##_sdsf)					\
  CALL_ZEROES (NUM##_sddf)					\
  CALL_ZEROES (NUM##_ddsf)					\
  CALL_ZEROES (NUM##_dddf)					\
  CALL_ZEROES (NUM##_tdsf)					\
  CALL_ZEROES (NUM##_tddf)					\
  CALL_ZEROES (NUM##_sfsd)					\
  CALL_ZEROES (NUM##_sfdd)					\
  CALL_ZEROES (NUM##_sftd)					\
  CALL_ZEROES (NUM##_dfsd)					\
  CALL_ZEROES (NUM##_dfdd)					\
  CALL_ZEROES (NUM##_dftd)					\
  CALL_ZEROES (NUM##_sddd)					\
  CALL_ZEROES (NUM##_sdtd)					\
  CALL_ZEROES (NUM##_ddsd)					\
  CALL_ZEROES (NUM##_ddtd)					\
  CALL_ZEROES (NUM##_tdsd)					\
  CALL_ZEROES (NUM##_tddd)

#if USE_TF == 0
#define CALL_ZEROES_TF(NUM)
#else
#define CALL_ZEROES_TF(NUM)					\
  CALL_ZEROES (NUM##_sdtf)					\
  CALL_ZEROES (NUM##_ddtf)					\
  CALL_ZEROES (NUM##_tdtf)					\
  CALL_ZEROES (NUM##_tfsd)					\
  CALL_ZEROES (NUM##_tfdd)					\
  CALL_ZEROES (NUM##_tftd)
#endif

#define CALL_ZEROES_ALL(NUM)					\
  CALL_ZEROES_NOTF(NUM)						\
  CALL_ZEROES_TF(NUM)

#define CONVERT_INF(NUM,FROM,TO,FROMSUFFIX,TOSUFFIX)		\
CONVERT_PINF (NUM, FROM, TO, FROMSUFFIX, TOSUFFIX)		\
CONVERT_MINF (NUM, FROM, TO, FROMSUFFIX, TOSUFFIX)

#define CONVERT_INF_NOTF(NUM)					\
CONVERT_INF (NUM##_sdsf, sd, sf, d32, f)			\
CONVERT_INF (NUM##_sddf, sd, df, d32, )				\
CONVERT_INF (NUM##_ddsf, dd, sf, d64, f)			\
CONVERT_INF (NUM##_dddf, dd, df, d64, )				\
CONVERT_INF (NUM##_tdsf, td, sf, d128, f)			\
CONVERT_INF (NUM##_tddf, td, df, d128, )			\
CONVERT_INF (NUM##_sfsd, sf, sd, f, d32)			\
CONVERT_INF (NUM##_sfdd, sf, dd, f, d64)			\
CONVERT_INF (NUM##_sftd, sf, td, f, d128)			\
CONVERT_INF (NUM##_dfsd, df, sd, , d32)				\
CONVERT_INF (NUM##_dfdd, df, dd, , d64)				\
CONVERT_INF (NUM##_dftd, df, td, , d128)			\
CONVERT_INF (NUM##_sddd, sd, dd, d32, d64)			\
CONVERT_INF (NUM##_sdtd, sd, td, d32, d128)			\
CONVERT_INF (NUM##_ddsd, dd, sd, d64, d32)			\
CONVERT_INF (NUM##_ddtd, dd, td, d64, d128)			\
CONVERT_INF (NUM##_tdsd, td, sd, d128, d32)			\
CONVERT_INF (NUM##_tddd, td, dd, d128, d64)

#if USE_TF == 0
#define CONVERT_INF_TF(NUM)
#else
#define CONVERT_INF_TF(NUM)					\
CONVERT_INF (NUM##_sdtf, sd, tf, d32, l)			\
CONVERT_INF (NUM##_ddtf, dd, tf, d64, l)			\
CONVERT_INF (NUM##_tdtf, td, tf, d128, l)			\
CONVERT_INF (NUM##_tfsd, tf, sd, l, d32)			\
CONVERT_INF (NUM##_tfdd, tf, dd, l, d64)			\
CONVERT_INF (NUM##_tftd, tf, td, l, d128)
#endif

#define CONVERT_INF_ALL(NUM)					\
  CONVERT_INF_NOTF(NUM)						\
  CONVERT_INF_TF(NUM)

#define CALL_INF(NUM)						\
  convert_##NUM##_pinf ();					\
  convert_##NUM##_minf ();

#define CALL_INF_NOTF(NUM)					\
  CALL_INF (NUM##_sdsf)						\
  CALL_INF (NUM##_sddf)						\
  CALL_INF (NUM##_ddsf)						\
  CALL_INF (NUM##_dddf)						\
  CALL_INF (NUM##_tdsf)						\
  CALL_INF (NUM##_tddf)						\
  CALL_INF (NUM##_sfsd)						\
  CALL_INF (NUM##_sfdd)						\
  CALL_INF (NUM##_sftd)						\
  CALL_INF (NUM##_dfsd)						\
  CALL_INF (NUM##_dfdd)						\
  CALL_INF (NUM##_dftd)						\
  CALL_INF (NUM##_sddd)						\
  CALL_INF (NUM##_sdtd)						\
  CALL_INF (NUM##_ddsd)						\
  CALL_INF (NUM##_ddtd)						\
  CALL_INF (NUM##_tdsd)						\
  CALL_INF (NUM##_tddd)

#if USE_TF == 0
#define CALL_INF_TF(NUM)
#else
#define CALL_INF_TF(NUM)					\
  CALL_INF (NUM##_sdtf)						\
  CALL_INF (NUM##_ddtf)						\
  CALL_INF (NUM##_tdtf)						\
  CALL_INF (NUM##_tfsd)						\
  CALL_INF (NUM##_tfdd)						\
  CALL_INF (NUM##_tftd)
#endif

#define CALL_INF_ALL(NUM)					\
  CALL_INF_NOTF(NUM)						\
  CALL_INF_TF(NUM)

#define CONVERT_NAN_NOTF(NUM)					\
CONVERT_NAN (NUM##_sdsf, sd, sf, d32, f)			\
CONVERT_NAN (NUM##_sddf, sd, df, d32, )				\
CONVERT_NAN (NUM##_ddsf, dd, sf, d64, f)			\
CONVERT_NAN (NUM##_dddf, dd, df, d64, )				\
CONVERT_NAN (NUM##_tdsf, td, sf, d128, f)			\
CONVERT_NAN (NUM##_tddf, td, df, d128, )			\
CONVERT_NAN (NUM##_sfsd, sf, sd, f, d32)			\
CONVERT_NAN (NUM##_sfdd, sf, dd, f, d64)			\
CONVERT_NAN (NUM##_sftd, sf, td, f, d128)			\
CONVERT_NAN (NUM##_dfsd, df, sd, , d32)				\
CONVERT_NAN (NUM##_dfdd, df, dd, , d64)				\
CONVERT_NAN (NUM##_dftd, df, td, , d128)			\
CONVERT_NAN (NUM##_sddd, sd, dd, d32, d64)			\
CONVERT_NAN (NUM##_sdtd, sd, td, d32, d128)			\
CONVERT_NAN (NUM##_ddsd, dd, sd, d64, d32)			\
CONVERT_NAN (NUM##_ddtd, dd, td, d64, d128)			\
CONVERT_NAN (NUM##_tdsd, td, sd, d128, d32)			\
CONVERT_NAN (NUM##_tddd, td, dd, d128, d64)

#if USE_TF == 0
#define CONVERT_NAN_TF(NUM)
#else
#define CONVERT_NAN_TF(NUM)					\
CONVERT_NAN (NUM##_sdtf, sd, tf, d32, l)			\
CONVERT_NAN (NUM##_ddtf, dd, tf, d64, l)			\
CONVERT_NAN (NUM##_tdtf, td, tf, d128, l)			\
CONVERT_NAN (NUM##_tfsd, tf, sd, l, d32)			\
CONVERT_NAN (NUM##_tfdd, tf, dd, l, d64)			\
CONVERT_NAN (NUM##_tftd, tf, td, l, d128)
#endif

#define CONVERT_NAN_ALL(NUM)					\
  CONVERT_NAN_NOTF(NUM)						\
  CONVERT_NAN_TF(NUM)

#define CALL_NAN(NUM)						\
  convert_##NUM##_nan ();

#define CALL_NAN_NOTF(NUM)					\
  CALL_NAN (NUM##_sdsf)						\
  CALL_NAN (NUM##_sddf)						\
  CALL_NAN (NUM##_ddsf)						\
  CALL_NAN (NUM##_dddf)						\
  CALL_NAN (NUM##_tdsf)						\
  CALL_NAN (NUM##_tddf)						\
  CALL_NAN (NUM##_sfsd)						\
  CALL_NAN (NUM##_sfdd)						\
  CALL_NAN (NUM##_sftd)						\
  CALL_NAN (NUM##_dfsd)						\
  CALL_NAN (NUM##_dfdd)						\
  CALL_NAN (NUM##_dftd)						\
  CALL_NAN (NUM##_sddd)						\
  CALL_NAN (NUM##_sdtd)						\
  CALL_NAN (NUM##_ddsd)						\
  CALL_NAN (NUM##_ddtd)						\
  CALL_NAN (NUM##_tdsd)						\
  CALL_NAN (NUM##_tddd)

#if USE_TF == 0
#define CALL_NAN_TF(NUM)
#else
#define CALL_NAN_TF(NUM)					\
  CALL_NAN (NUM##_sdtf)						\
  CALL_NAN (NUM##_ddtf)						\
  CALL_NAN (NUM##_tdtf)						\
  CALL_NAN (NUM##_tfsd)						\
  CALL_NAN (NUM##_tfdd)						\
  CALL_NAN (NUM##_tftd)
#endif

#define CALL_NAN_ALL(NUM)					\
  CALL_NAN_NOTF(NUM)						\
  CALL_NAN_TF(NUM)

volatile _Decimal32 sd;
volatile _Decimal64 dd;
volatile _Decimal128 td;
volatile float sf;
volatile double df;
volatile long double tf;

CONVERT_VALID (101, td, sf, 0.000488281251dl, 0.00048828125f, 0.f)

/* 2**(-25) = 0.298023223876953125E-7.  */
CONVERT_VALID (102, td, sf, 2.98023223876953125e-8dl, 2.9802322387695312e-08f,
               01.e-13f)

/* Fractional part doesn't fit.  */
CONVERT_VALID (103, df, sd, 1.0e-20, 1.0e-20df, 0.df)

/* Exact power of 2.  */
CONVERT_VALID (104, df, sd, 0.00048828125, 0.00048828125df, 0.df)
CONVERT_VALID (105, df, sd, 1.0e-96, 0.dd, DEC32_MIN)

/* A value slightly less than FLT_MAX can be converted in both directions.  */
CONVERT_VALID (201, sf, sd, 3.402819e+38f, 3.402819e+38df, 0.df)
CONVERT_VALID (202, sd, sf, 3.402819e+38df, 3.402819e+38f, 0.f)
CONVERT_VALID (203, sf, dd, 3.402819e+38f, 3.402819e+38dd, 1.e+30dd)
CONVERT_VALID (204, dd, sf, 3.402819e+38dd, 3.402819e+38f, 0.f)
CONVERT_VALID (205, sf, td, 3.402819e+38f, 3.402819e+38dl, 1.e+30dl)
CONVERT_VALID (206, td, sf, 3.402819e+38dl, 3.402819e+38f, 0.f)

/* A value slightly less than DEC32_MAX can be converted in both directions.  */
CONVERT_VALID (211, sd, df, 9.999998e96df, 9.999998e96, 0.)
CONVERT_VALID (212, df, sd, 9.999998e96, 9.999998e96df, 0.df)

/* A value slightly less than DBL_MAX can be converted in both directions.  */
CONVERT_VALID (221, df, dd, 1.79768e+308, 1.79768e+308dd, 0.dd)
CONVERT_VALID (222, dd, df, 1.79768e+308dd, 1.79768e+308, 0.)
CONVERT_VALID (223, df, td, 1.79768e+308, 1.79768e+308dl, 1.e292dl)
CONVERT_VALID (224, td, df, 1.79768e+308dl, 1.79768e+308, 0.)

/* An integral value with 6 digits (FLT_DIG) can be converted between float
   and _Decimal32 in both directions.  */
CONVERT_VALID (301, sd, sf, 100000.DF, 100000.F, 0.F)
CONVERT_VALID (302, sf, sd, 100000.F, 100000.DF, 0.DF)
CONVERT_VALID (303, sd, sf, 999999.DF, 999999.F, 0.F)
CONVERT_VALID (304, sf, sd, 999999.F, 999999.DF, 0.DF)

/* An integral value with 7 digits (DEC32_MANT_DIG) can be converted between
   _Decimal32 and double in both directions.  */
CONVERT_VALID (311, sd, df, 1000000.DF, 1000000., 0.)
CONVERT_VALID (312, df, sd, 1000000., 1000000.DF, 0.DF)
CONVERT_VALID (313, sd, df, 9999999.DF, 9999999., 0.)
CONVERT_VALID (314, df, sd, 9999999., 9999999.DF, 0.DF)

/* An integral value with 15 digits (DBL_DIG) can be converted between
   double and _Decimal64 in both directions.  */
CONVERT_VALID (321, dd, df, 100000000000000.DD, 100000000000000., 0.)
CONVERT_VALID (322, df, dd, 100000000000000., 100000000000000.DD, 0.DD);
CONVERT_VALID (323, dd, df, 999999999999999.DD, 999999999999999., 0.);
CONVERT_VALID (324, df, dd, 999999999999999., 999999999999999.DD, 0.DD);

/* If LDBL_DIG is at least 16, an integral value with 16 digits can be
   converted between _Decimal64 and long double in both directions.  */
CONVERT_VALID (331, dd, tf, 1000000000000000.DD, 1000000000000000.L, 0.L)
CONVERT_VALID (332, td, dd, 1000000000000000.L, 1000000000000000.DD, 0.DD)
CONVERT_VALID (333, dd, tf, 9999999999999999.DD, 9999999999999999.L, 0.L)
CONVERT_VALID (334, td, dd, 9999999999999999.L, 9999999999999999.DD, 0.DD)

/* If LDBL_DIG is at least 18, an integral value with 18 digits can be
   converted between long double and _Decimal128 in both directions.  */
CONVERT_VALID (341, td, tf, 100000000000000000.DL, 100000000000000000.L, 0.L)
CONVERT_VALID (342, tf, td, 100000000000000000.L, 100000000000000000.DL, 0.DL)
CONVERT_VALID (343, td, tf, 999999999999999999.DL, 999999999999999999.L, 0.L)
CONVERT_VALID (344, tf, td, 999999999999999999.L, 999999999999999999.DL, 0.DL)

/* If LDBL_DIG is at least 31, an integral value with 31 digits can be
   converted between long double and _Decimal128 in both directions.  */
CONVERT_VALID (351, td, tf, 1000000000000000000000000000000.DL,
			    1000000000000000000000000000000.L, 0.L)
CONVERT_VALID (352, tf, td, 1000000000000000000000000000000.L,
			    1000000000000000000000000000000.DL, 0.DL)
CONVERT_VALID (353, td, tf, 9999999999999999999999999999999.DL,
			    9999999999999999999999999999999.L, 0.L)
CONVERT_VALID (354, tf, td, 9999999999999999999999999999999.L,
			    9999999999999999999999999999999.DL, 0.DL)

/* If LDBL_DIG is at least 33, an integral value with 33 digits can be
   converted between long double and _Decimal128 in both directions.  */
CONVERT_VALID (361, td, tf, 100000000000000000000000000000000.DL,
			    100000000000000000000000000000000.L, 0.L)
CONVERT_VALID (362, tf, td, 100000000000000000000000000000000.L,
			    100000000000000000000000000000000.DL, 0.DL)
CONVERT_VALID (363, td, tf, 999999999999999999999999999999999.DL,
			    999999999999999999999999999999999.L, 0.L)
CONVERT_VALID (364, tf, td, 999999999999999999999999999999999.L,
			    999999999999999999999999999999999.DL, 0.DL)

int
main ()
{
  convert_101 ();
  convert_102 ();
  convert_103 ();
  convert_104 ();
  convert_105 ();

  convert_201 ();
  convert_202 ();
  convert_203 ();
  convert_204 ();
  convert_205 ();
  convert_206 ();

  convert_211 ();
  convert_212 ();

  convert_221 ();
  convert_222 ();
  convert_223 ();
  convert_224 ();

  convert_301 ();
  convert_302 ();
  convert_303 ();
  convert_304 ();

  convert_311 ();
  convert_312 ();
  convert_313 ();
  convert_314 ();

  convert_321 ();
  convert_322 ();
  convert_323 ();
  convert_324 ();

  if (LDBL_DIG >= 16)
    {
      convert_331 ();
      convert_332 ();
      convert_333 ();
      convert_334 ();
    }

  if (LDBL_DIG >= 18)
    {
      convert_341 ();
      convert_342 ();
      convert_343 ();
      convert_344 ();
    }

  if (LDBL_DIG >= 31)
    {
      convert_351 ();
      convert_352 ();
      convert_353 ();
      convert_354 ();
    }

  if (LDBL_DIG >= 33)
    {
      convert_361 ();
      convert_362 ();
      convert_363 ();
      convert_364 ();
    }

  FINISH
}
