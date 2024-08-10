/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

/* Test ARMv8.1-A Load-ADD instruction.  */

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

#define LOAD_ADD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_fetch_add (val, foo, MODEL);			\
  }

#define LOAD_ADD_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_fetch_add (val, foo, MODEL);				\
  }

#define LOAD_SUB(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_fetch_sub (val, foo, MODEL);			\
  }

#define LOAD_SUB_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_fetch_sub (val, foo, MODEL);				\
  }

#define ADD_LOAD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_add_fetch (val, foo, MODEL);			\
  }

#define ADD_LOAD_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_add_fetch (val, foo, MODEL);				\
  }

#define SUB_LOAD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_sub_fetch (val, foo, MODEL);			\
  }

#define SUB_LOAD_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_sub_fetch (val, foo, MODEL);				\
  }

TEST (load_add, LOAD_ADD)
TEST (load_add_notreturn, LOAD_ADD_NORETURN)

TEST (load_sub, LOAD_SUB)
TEST (load_sub_notreturn, LOAD_SUB_NORETURN)

TEST (add_load, ADD_LOAD)
TEST (add_load_notreturn, ADD_LOAD_NORETURN)

TEST (sub_load, SUB_LOAD)
TEST (sub_load_notreturn, SUB_LOAD_NORETURN)

/* { dg-final { scan-assembler-times "ldaddb\t" 16} } */
/* { dg-final { scan-assembler-times "ldaddab\t" 32} } */
/* { dg-final { scan-assembler-times "ldaddlb\t" 16} } */
/* { dg-final { scan-assembler-times "ldaddalb\t" 32} } */

/* { dg-final { scan-assembler-times "ldaddh\t" 16} } */
/* { dg-final { scan-assembler-times "ldaddah\t" 32} } */
/* { dg-final { scan-assembler-times "ldaddlh\t" 16} } */
/* { dg-final { scan-assembler-times "ldaddalh\t" 32} } */

/* { dg-final { scan-assembler-times "ldadd\t" 32} } */
/* { dg-final { scan-assembler-times "ldadda\t" 64} } */
/* { dg-final { scan-assembler-times "ldaddl\t" 32} } */
/* { dg-final { scan-assembler-times "ldaddal\t" 64} } */

/* { dg-final { scan-assembler-not "ldaxr\t" } } */
/* { dg-final { scan-assembler-not "stlxr\t" } } */
/* { dg-final { scan-assembler-not "dmb" } } */
