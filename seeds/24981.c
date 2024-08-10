/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -O3 -ftree-vectorize -fno-vect-cost-model -ffast-math" } */

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

#define ARRAY_SIZE 128

double in[ARRAY_SIZE];
double out[ARRAY_SIZE];
double ref[ARRAY_SIZE];

TEST_UNARY_CALL (double, __builtin_floor)
TEST_ASSERT (double)

TEST_INIT (double, 1.2, 1.0, 1)
TEST_INIT (double, -1.2, -2.0, 2)
TEST_INIT (double, 3.0, 3.0, 3)
TEST_INIT (double, 4503599627370495.5, 4503599627370495.0, 4)
TEST_INIT (double, 4503599627370497.0, 4503599627370497.0, 5)
TEST_INIT (double, 0.0, 0.0, 6)
TEST_INIT (double, -0.0, -0.0, 7)
TEST_INIT (double, -4503599627370495.5, -4503599627370496.0, 8)
TEST_INIT (double, -4503599627370496.0, -4503599627370496.0, 9)

int
main ()
{
  RUN_TEST (double, 1, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 2, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 3, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 4, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 5, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 6, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 7, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 8, __builtin_floor, in, out, ref, ARRAY_SIZE);
  RUN_TEST (double, 9, __builtin_floor, in, out, ref, ARRAY_SIZE);

  return 0;
}
