/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

/* Test ARMv8.1-A SWP instruction.  */

/* Support code for atomic instruction tests.  */

/* Define types names without spaces.  */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef __int128_t int128;
typedef __uint128_t uint128;

#define FNNAME(NAME,TY) NAME

/* Expand one-model functions.  */
#define TEST_M1(NAME, FN, TY, MODEL, DUMMY)	\
  FN (test_##NAME##_##TY, TY, MODEL)

/* Expand two-model functions.  */
#define TEST_M2(NAME, FN, TY, MODEL1, MODEL2)	\
  FN (test_##NAME##_##TY, TY, MODEL1, MODEL2)

/* Typest to test.  */
#define TEST_TY(NAME, FN, N, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, char, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, uchar, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, short, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, ushort, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, int, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, uint, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, longlong, MODEL1, MODEL2)	\
  TEST_M##N (NAME, FN, ulonglong, MODEL1, MODEL2)	\
  TEST_M##N (NAME, FN, int128, MODEL1, MODEL2)		\
  TEST_M##N (NAME, FN, uint128, MODEL1, MODEL2)

/* Models to test.  */
#define TEST_MODEL(NAME, FN, N)					\
  TEST_TY (NAME##_relaxed, FN, N, __ATOMIC_RELAXED, DUMMY)	\
  TEST_TY (NAME##_consume, FN, N, __ATOMIC_CONSUME, DUMMY)	\
  TEST_TY (NAME##_acquire, FN, N, __ATOMIC_ACQUIRE, DUMMY)	\
  TEST_TY (NAME##_release, FN, N, __ATOMIC_RELEASE, DUMMY)	\
  TEST_TY (NAME##_acq_rel, FN, N, __ATOMIC_ACQ_REL, DUMMY)	\
  TEST_TY (NAME##_seq_cst, FN, N, __ATOMIC_SEQ_CST, DUMMY)	\

/* Cross-product of models to test.  */
#define TEST_MODEL_M1(NAME, FN, N, M)			\
  TEST_TY (NAME##_relaxed, FN, N, M, __ATOMIC_RELAXED)	\
  TEST_TY (NAME##_consume, FN, N, M, __ATOMIC_CONSUME)	\
  TEST_TY (NAME##_acquire, FN, N, M, __ATOMIC_ACQUIRE)	\
  TEST_TY (NAME##_release, FN, N, M, __ATOMIC_RELEASE)	\
  TEST_TY (NAME##_acq_rel, FN, N, M, __ATOMIC_ACQ_REL)	\
  TEST_TY (NAME##_seq_cst, FN, N, M, __ATOMIC_SEQ_CST)	\

#define TEST_MODEL_M2(NAME, FN)					\
  TEST_MODEL_M1 (NAME##_relaxed, FN, 2, __ATOMIC_RELAXED)	\
  TEST_MODEL_M1 (NAME##_consume, FN, 2, __ATOMIC_CONSUME)	\
  TEST_MODEL_M1 (NAME##_acquire, FN, 2, __ATOMIC_ACQUIRE)	\
  TEST_MODEL_M1 (NAME##_release, FN, 2, __ATOMIC_RELEASE)	\
  TEST_MODEL_M1 (NAME##_acq_rel, FN, 2, __ATOMIC_ACQ_REL)	\
  TEST_MODEL_M1 (NAME##_seq_cst, FN, 2, __ATOMIC_SEQ_CST)	\

/* Expand functions for a cross-product of memory models and types.  */
#define TEST_TWO(NAME, FN) TEST_MODEL_M2 (NAME, FN)

/* Expand functions for a set of memory models and types.  */
#define TEST_ONE(NAME, FN) TEST_MODEL (NAME, FN, 1)


#define TEST TEST_ONE

#define SWAP_ATOMIC(FN, TY, MODEL)					\
  TY FNNAME (FN, TY) (TY* val, TY foo)					\
  {									\
    return __atomic_exchange_n (val, foo, MODEL);			\
  }

#define SWAP_ATOMIC_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo, TY* bar)			\
  {									\
    __atomic_exchange (val, foo, bar, MODEL);				\
  }


TEST (swap_atomic, SWAP_ATOMIC)
TEST (swap_atomic_noreturn, SWAP_ATOMIC_NORETURN)


/* { dg-final { scan-assembler-times "swpb\t" 4} } */
/* { dg-final { scan-assembler-times "swpab\t" 8} } */
/* { dg-final { scan-assembler-times "swplb\t" 4} } */
/* { dg-final { scan-assembler-times "swpalb\t" 8} } */

/* { dg-final { scan-assembler-times "swph\t" 4} } */
/* { dg-final { scan-assembler-times "swpah\t" 8} } */
/* { dg-final { scan-assembler-times "swplh\t" 4} } */
/* { dg-final { scan-assembler-times "swpalh\t" 8} } */

/* { dg-final { scan-assembler-times "swp\t" 8} } */
/* { dg-final { scan-assembler-times "swpa\t" 16} } */
/* { dg-final { scan-assembler-times "swpl\t" 8} } */
/* { dg-final { scan-assembler-times "swpal\t" 16} } */

/* { dg-final { scan-assembler-not "ldaxr\t" } } */
/* { dg-final { scan-assembler-not "stlxr\t" } } */
/* { dg-final { scan-assembler-not "dmb" } } */
