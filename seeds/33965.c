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

/* Exponent values that might cause problems with a particular
   implementation.  */

CONVERT_VALID (101, dd, df, 1.e309dd, 1.e309, 0.) 
CONVERT_VALID (102, dd, df, 1.e308dd, 1.e308, 0.)
CONVERT_VALID (103, dd, df, 1.e307dd, 1.e307, 0.) 
CONVERT_VALID (104, dd, df, 1.e306dd, 1.e306, 0.) 
CONVERT_VALID (105, dd, df, 1.e305dd, 1.e305, 0.)
CONVERT_VALID (106, dd, df, 1.e304dd, 1.e304, 0.)
CONVERT_VALID (107, dd, df, 1.e303dd, 1.e303, 0.) 
CONVERT_VALID (108, dd, df, 1.e302dd, 1.e302, 0.) 
CONVERT_VALID (109, dd, df, 1.e301dd, 1.e301, 0.) 
CONVERT_VALID (110, dd, df, 1.e300dd, 1.e300, 0.) 
CONVERT_VALID (111, dd, df, 1.e299dd, 1.e299, 0.) 
CONVERT_VALID (112, dd, df, 1.e298dd, 1.e298, 0.) 
CONVERT_VALID (113, dd, df, 1.e297dd, 1.e297, 0.) 
CONVERT_VALID (114, dd, df, 1.e296dd, 1.e296, 0.) 
CONVERT_VALID (115, dd, df, 1.e295dd, 1.e295, 0.) 
CONVERT_VALID (116, dd, df, 1.e294dd, 1.e294, 0.) 
CONVERT_VALID (117, dd, df, 1.e293dd, 1.e293, 0.) 
CONVERT_VALID (118, dd, df, 1.e292dd, 1.e292, 0.) 
CONVERT_VALID (119, dd, df, 1.e291dd, 1.e291, 0.)
CONVERT_VALID (120, dd, df, 1.e290dd, 1.e290, 0.)

CONVERT_VALID (201, dd, df, 1.e-309dd, 1.e-309, 0.) 
CONVERT_VALID (202, dd, df, 1.e-308dd, 1.e-308, 0.) 
CONVERT_VALID (203, dd, df, 1.e-307dd, 1.e-307, 0.) 
CONVERT_VALID (204, dd, df, 1.e-306dd, 1.e-306, 0.) 
CONVERT_VALID (205, dd, df, 1.e-305dd, 1.e-305, 0.) 
CONVERT_VALID (206, dd, df, 1.e-304dd, 1.e-304, 0.) 
CONVERT_VALID (207, dd, df, 1.e-303dd, 1.e-303, 0.) 
CONVERT_VALID (208, dd, df, 1.e-302dd, 1.e-302, 0.) 
CONVERT_VALID (209, dd, df, 1.e-301dd, 1.e-301, 0.) 
CONVERT_VALID (210, dd, df, 1.e-300dd, 1.e-300, 0.) 
CONVERT_VALID (211, dd, df, 1.e-299dd, 1.e-299, 0.) 
CONVERT_VALID (212, dd, df, 1.e-298dd, 1.e-298, 0.) 
CONVERT_VALID (213, dd, df, 1.e-297dd, 1.e-297, 0.) 
CONVERT_VALID (214, dd, df, 1.e-296dd, 1.e-296, 0.) 
CONVERT_VALID (215, dd, df, 1.e-295dd, 1.e-295, 0.)
CONVERT_VALID (216, dd, df, 1.e-294dd, 1.e-294, 0.)
CONVERT_VALID (217, dd, df, 1.e-293dd, 1.e-293, 0.) 
CONVERT_VALID (218, dd, df, 1.e-292dd, 1.e-292, 0.) 
CONVERT_VALID (219, dd, df, 1.e-291dd, 1.e-291, 0.) 
CONVERT_VALID (220, dd, df, 1.e-290dd, 1.e-290, 0.) 

CONVERT_VALID (301, td, df, 1.e309dl, 1.e309, 0.) 
CONVERT_VALID (302, td, df, 1.e308dl, 1.e308, 0.)
CONVERT_VALID (303, td, df, 1.e307dl, 1.e307, 0.) 
CONVERT_VALID (304, td, df, 1.e306dl, 1.e306, 0.) 
CONVERT_VALID (305, td, df, 1.e305dl, 1.e305, 0.)
CONVERT_VALID (306, td, df, 1.e304dl, 1.e304, 0.)
CONVERT_VALID (307, td, df, 1.e303dl, 1.e303, 0.) 
CONVERT_VALID (308, td, df, 1.e302dl, 1.e302, 0.) 
CONVERT_VALID (309, td, df, 1.e301dl, 1.e301, 0.)
CONVERT_VALID (310, td, df, 1.e300dl, 1.e300, 0.) 
CONVERT_VALID (311, td, df, 1.e299dl, 1.e299, 0.)
CONVERT_VALID (312, td, df, 1.e298dl, 1.e298, 0.)
CONVERT_VALID (313, td, df, 1.e297dl, 1.e297, 0.) 
CONVERT_VALID (314, td, df, 1.e296dl, 1.e296, 0.) 
CONVERT_VALID (315, td, df, 1.e295dl, 1.e295, 0.)
CONVERT_VALID (316, td, df, 1.e294dl, 1.e294, 0.)
CONVERT_VALID (317, td, df, 1.e293dl, 1.e293, 0.) 
CONVERT_VALID (318, td, df, 1.e292dl, 1.e292, 0.) 
CONVERT_VALID (319, td, df, 1.e291dl, 1.e291, 0.) 
CONVERT_VALID (320, td, df, 1.e290dl, 1.e290, 0.)

CONVERT_VALID (401, td, df, 1.e-309dl, 1.e-309, 0.) 
CONVERT_VALID (402, td, df, 1.e-308dl, 1.e-308, 0.) 
CONVERT_VALID (403, td, df, 1.e-307dl, 1.e-307, 0.) 
CONVERT_VALID (404, td, df, 1.e-306dl, 1.e-306, 0.) 
CONVERT_VALID (405, td, df, 1.e-305dl, 1.e-305, 0.) 
CONVERT_VALID (406, td, df, 1.e-304dl, 1.e-304, 0.) 
CONVERT_VALID (407, td, df, 1.e-303dl, 1.e-303, 0.) 
CONVERT_VALID (408, td, df, 1.e-302dl, 1.e-302, 0.) 
CONVERT_VALID (409, td, df, 1.e-301dl, 1.e-301, 0.) 
CONVERT_VALID (410, td, df, 1.e-300dl, 1.e-300, 0.) 
CONVERT_VALID (411, td, df, 1.e-299dl, 1.e-299, 0.) 
CONVERT_VALID (412, td, df, 1.e-298dl, 1.e-298, 0.) 
CONVERT_VALID (413, td, df, 1.e-297dl, 1.e-297, 0.) 
CONVERT_VALID (414, td, df, 1.e-296dl, 1.e-296, 0.) 
CONVERT_VALID (415, td, df, 1.e-295dl, 1.e-295, 0.) 
CONVERT_VALID (416, td, df, 1.e-294dl, 1.e-294, 0.) 
CONVERT_VALID (417, td, df, 1.e-293dl, 1.e-293, 0.) 
CONVERT_VALID (418, td, df, 1.e-292dl, 1.e-292, 0.)
CONVERT_VALID (419, td, df, 1.e-291dl, 1.e-291, 0.) 
CONVERT_VALID (420, td, df, 1.e-290dl, 1.e-290, 0.)

int
main ()
{
  convert_101 ();
  convert_102 ();
  convert_103 ();
  convert_104 ();
  convert_105 ();
  convert_106 ();
  convert_107 ();
  convert_108 ();
  convert_109 ();
  convert_110 ();
  convert_111 ();
  convert_112 ();
  convert_113 ();
  convert_114 ();
  convert_115 ();
  convert_116 ();
  convert_117 ();
  convert_118 ();
  convert_119 ();
  convert_120 ();

  convert_201 ();
  convert_202 ();
  convert_203 ();
  convert_204 ();
  convert_205 ();
  convert_206 ();
  convert_207 ();
  convert_208 ();
  convert_209 ();
  convert_210 ();
  convert_211 ();
  convert_212 ();
  convert_213 ();
  convert_214 ();
  convert_215 ();
  convert_216 ();
  convert_217 ();
  convert_218 ();
  convert_219 ();
  convert_220 ();

  convert_301 ();
  convert_302 ();
  convert_303 ();
  convert_304 ();
  convert_305 ();
  convert_306 ();
  convert_307 ();
  convert_308 ();
  convert_309 ();
  convert_310 ();
  convert_311 ();
  convert_312 ();
  convert_313 ();
  convert_314 ();
  convert_315 ();
  convert_316 ();
  convert_317 ();
  convert_318 ();
  convert_319 ();
  convert_320 ();

  convert_401 ();
  convert_402 ();
  convert_403 ();
  convert_404 ();
  convert_405 ();
  convert_406 ();
  convert_407 ();
  convert_408 ();
  convert_409 ();
  convert_410 ();
  convert_411 ();
  convert_412 ();
  convert_413 ();
  convert_414 ();
  convert_415 ();
  convert_416 ();
  convert_417 ();
  convert_418 ();
  convert_419 ();
  convert_420 ();

  FINISH
}
