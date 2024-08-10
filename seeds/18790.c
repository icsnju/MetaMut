/* Test complex arithmetic with signed zeros.  Pure complex
   multiplication with -1.0 + 0.0i.  */
/* { dg-do run } */
/* { dg-options "-std=gnu99" { target c } } */
/* { dg-skip-if "double support is incomplete" { "avr-*-*" } } */

/* Common header for complex arithmetic sign tests.  */

#ifdef __cplusplus
extern "C" {
#endif
extern void abort (void);
extern void exit (int);
#ifdef __cplusplus
}
#endif

#define CHECK_RES(VALUE, COPY, SIGN_REAL, SIGN_IMAG)		\
  do {								\
    if ((VALUE) != 0						\
	|| COPY (1.0, __real__ (VALUE)) != SIGN_REAL 1.0	\
	|| COPY (1.0, __imag__ (VALUE)) != SIGN_IMAG 1.0)	\
      abort ();							\
  } while (0)

/* This definition is intended to work with or without imaginary
   types, as long as mixed real/complex arithmetic is handled
   correctly.  */
#define ENCODE(ZERO, ZEROI, SA, SB)			\
  (SA 1 == 1						\
   ? SB 1 == 1 ? ZERO + ZEROI : ZERO - ZEROI		\
   : SB 1 == 1 ? -(ZERO - ZEROI) : -(ZERO + ZEROI))

#define CHECK_ARITH(TYPE, COPY, ZERO, ZEROI, OP, S1, S2, S3, S4, SR, SI) \
  do {									\
    _Complex TYPE a1, b1, c1;						\
    volatile _Complex TYPE a2, b2, c2;					\
    a1 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a1, COPY, S1, S2);					\
    b1 = ENCODE(ZERO, ZEROI, S3, S4);					\
    CHECK_RES (b1, COPY, S3, S4);					\
    c1 = a1 OP b1;							\
    CHECK_RES (c1, COPY, SR, SI);					\
    a2 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a2, COPY, S1, S2);					\
    b2 = ENCODE(ZERO, ZEROI, S3, S4);					\
    CHECK_RES (b2, COPY, S3, S4);					\
    c2 = a2 OP b2;							\
    CHECK_RES (c2, COPY, SR, SI);					\
  } while (0)

#define CHECK_ARITH_RC(TYPE, COPY, ZERO, ZEROI, OP, S1, S3, S4, SR, SI) \
  do {									\
    TYPE a1;								\
    _Complex TYPE b1, c1;						\
    volatile TYPE a2;							\
    volatile _Complex TYPE b2, c2;					\
    a1 = S1 ZERO;							\
    b1 = ENCODE(ZERO, ZEROI, S3, S4);					\
    CHECK_RES (b1, COPY, S3, S4);					\
    c1 = a1 OP b1;							\
    CHECK_RES (c1, COPY, SR, SI);					\
    a2 = S1 ZERO;							\
    b2 = ENCODE(ZERO, ZEROI, S3, S4);					\
    CHECK_RES (b2, COPY, S3, S4);					\
    c2 = a2 OP b2;							\
    CHECK_RES (c2, COPY, SR, SI);					\
  } while (0)

#define CHECK_ARITH_CR(TYPE, COPY, ZERO, ZEROI, OP, S1, S2, S3, V3, SR, SI) \
  do {									\
    _Complex TYPE a1, c1;						\
    TYPE b1;								\
    volatile _Complex TYPE a2, c2;					\
    volatile TYPE b2;							\
    a1 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a1, COPY, S1, S2);					\
    b1 = S3 V3;								\
    c1 = a1 OP b1;							\
    CHECK_RES (c1, COPY, SR, SI);					\
    a2 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a2, COPY, S1, S2);					\
    b2 = S3 V3;								\
    c2 = a2 OP b2;							\
    CHECK_RES (c2, COPY, SR, SI);					\
  } while (0)

#define CHECK_MUL_INT(TYPE, COPY, ZERO, ZEROI, ONE, S1, S2, SR, SI)	\
  do {									\
    _Complex TYPE a1, b1, c1;						\
    volatile _Complex TYPE a2, b2, c2;					\
    a1 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a1, COPY, S1, S2);					\
    b1 = -ONE + ZEROI;							\
    c1 = a1 * b1;							\
    CHECK_RES (c1, COPY, SR, SI);					\
    c1 = a1 * (-ONE + ZEROI);						\
    CHECK_RES (c1, COPY, SR, SI);					\
    a2 = ENCODE(ZERO, ZEROI, S1, S2);					\
    CHECK_RES (a2, COPY, S1, S2);					\
    b2 = -ONE + ZEROI;							\
    c2 = a2 * b2;							\
    CHECK_RES (c2, COPY, SR, SI);					\
    c2 = a2 * (-ONE + ZEROI);						\
    CHECK_RES (c2, COPY, SR, SI);					\
  } while (0)

#define CHECK_MUL(TYPE, COPY, ZERO, ZEROI, ONE)			\
  do {								\
    CHECK_MUL_INT (TYPE, COPY, ZERO, ZEROI, ONE, +, +, -, +);	\
    CHECK_MUL_INT (TYPE, COPY, ZERO, ZEROI, ONE, +, -, +, +);	\
    CHECK_MUL_INT (TYPE, COPY, ZERO, ZEROI, ONE, -, +, +, -);	\
    CHECK_MUL_INT (TYPE, COPY, ZERO, ZEROI, ONE, -, -, +, +);	\
  } while (0)

void
check_mul_float (void)
{
  CHECK_MUL (float, __builtin_copysignf, 0.0f, 0.0if, 1.0f);
}

void
check_mul_double (void)
{
  CHECK_MUL (double, __builtin_copysign, 0.0, 0.0i, 1.0);
}

void
check_mul_long_double (void)
{
  CHECK_MUL (long double, __builtin_copysignl, 0.0l, 0.0il, 1.0l);
}

int
main (void)
{
  check_mul_float ();
  check_mul_double ();
  check_mul_long_double ();
  exit (0);
}