/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O3 -ftree-vectorize -fno-vect-cost-model -ffast-math -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#define TEST_UNARY_CALL(TYPE, CALL)                               \
  void test_##TYPE##_##CALL (TYPE *out, TYPE *in, unsigned count) \
  {                                                               \
    for (unsigned i = 0; i < count; i++)                          \
      out[i] = CALL (in[i]);                                      \
  }

#define TEST_COND_UNARY_CALL(TYPE, CALL)                                     \
  void test_##TYPE##_##CALL (TYPE *out, int *cond, TYPE *in, unsigned count) \
  {                                                                          \
    for (unsigned i = 0; i < count; i++)                                     \
      out[i] = cond[i] ? CALL (in[i]) : in[i];                               \
  }

#define TEST_INIT(TYPE, VAL_IN, VAL_REF, NUM)                        \
  void test_##TYPE##_init_##NUM (TYPE *in, TYPE *ref, unsigned size) \
  {                                                                  \
    for (unsigned i = 0; i < size; i++)                              \
      {                                                              \
	in[i] = VAL_IN;                                              \
	ref[i] = VAL_REF;                                            \
      }                                                              \
  }

#define TEST_ASSERT(TYPE)                                         \
  void test_##TYPE##_assert (TYPE *out, TYPE *ref, unsigned size) \
  {                                                               \
    for (unsigned i = 0; i < size; i++)                           \
      {                                                           \
	if (out[i] != ref[i])                                     \
	  __builtin_abort ();                                     \
      }                                                           \
  }

#define RUN_TEST(TYPE, NUM, CALL, IN, OUT, REF, SIZE) \
  test_##TYPE##_init_##NUM (IN, REF, SIZE);           \
  test_##TYPE##_##CALL (OUT, IN, SIZE);               \
  test_##TYPE##_assert (OUT, REF, SIZE);

/*
** test_double___builtin_ceil:
**   frrm\s+[atx][0-9]+
**   ...
**   fsrmi\s+3
**   ...
**   vsetvli\s+[atx][0-9]+,\s*zero,\s*e64,\s*m1,\s*ta,\s*mu
**   vfabs\.v\s+v[0-9]+,\s*v[0-9]+
**   vmflt\.vf\s+v0,\s*v[0-9]+,\s*[fa]+[0-9]+
**   vfcvt\.x\.f\.v\s+v[0-9]+,\s*v[0-9]+,\s*v0\.t
**   vfcvt\.f\.x\.v\s+v[0-9]+,\s*v[0-9]+,\s*v0\.t
**   vfsgnj\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+
**   ...
**   fsrm\s+[atx][0-9]+
**   ...
*/
TEST_UNARY_CALL (double, __builtin_ceil)
