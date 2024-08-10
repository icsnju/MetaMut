/* { dg-do compile } */
/* { dg-options "-O2 -march=armv8-a+lse" } */

/* Test ARMv8.1-A LD<logic-op> instruction.  */

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

#define LOAD_OR(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_fetch_or (val, foo, MODEL);				\
  }

#define LOAD_OR_NORETURN(FN, TY, MODEL)					\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_fetch_or (val, foo, MODEL);				\
  }

#define LOAD_AND(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_fetch_and (val, foo, MODEL);			\
  }

#define LOAD_AND_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_fetch_and (val, foo, MODEL);				\
  }

#define LOAD_XOR(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_fetch_xor (val, foo, MODEL);			\
  }

#define LOAD_XOR_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_fetch_xor (val, foo, MODEL);				\
  }

#define OR_LOAD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_or_fetch (val, foo, MODEL);				\
  }

#define OR_LOAD_NORETURN(FN, TY, MODEL)					\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_or_fetch (val, foo, MODEL);				\
  }

#define AND_LOAD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_and_fetch (val, foo, MODEL);			\
  }

#define AND_LOAD_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_and_fetch (val, foo, MODEL);				\
  }

#define XOR_LOAD(FN, TY, MODEL)						\
  TY FNNAME (FN, TY) (TY* val, TY* foo)					\
  {									\
    return __atomic_xor_fetch (val, foo, MODEL);			\
  }

#define XOR_LOAD_NORETURN(FN, TY, MODEL)				\
  void FNNAME (FN, TY) (TY* val, TY* foo)				\
  {									\
    __atomic_xor_fetch (val, foo, MODEL);				\
  }


TEST (load_or, LOAD_OR)
TEST (load_or_notreturn, LOAD_OR_NORETURN)

TEST (load_and, LOAD_AND)
TEST (load_and_notreturn, LOAD_AND_NORETURN)

TEST (load_xor, LOAD_XOR)
TEST (load_xor_notreturn, LOAD_XOR_NORETURN)

TEST (or_load, OR_LOAD)
TEST (or_load_notreturn, OR_LOAD_NORETURN)

TEST (and_load, AND_LOAD)
TEST (and_load_notreturn, AND_LOAD_NORETURN)

TEST (xor_load, XOR_LOAD)
TEST (xor_load_notreturn, XOR_LOAD_NORETURN)


/* Load-OR.  */

/* { dg-final { scan-assembler-times "ldsetb\t" 8} } */
/* { dg-final { scan-assembler-times "ldsetab\t" 16} } */
/* { dg-final { scan-assembler-times "ldsetlb\t" 8} } */
/* { dg-final { scan-assembler-times "ldsetalb\t" 16} } */

/* { dg-final { scan-assembler-times "ldseth\t" 8} } */
/* { dg-final { scan-assembler-times "ldsetah\t" 16} } */
/* { dg-final { scan-assembler-times "ldsetlh\t" 8} } */
/* { dg-final { scan-assembler-times "ldsetalh\t" 16} } */

/* { dg-final { scan-assembler-times "ldset\t" 16} } */
/* { dg-final { scan-assembler-times "ldseta\t" 32} } */
/* { dg-final { scan-assembler-times "ldsetl\t" 16} } */
/* { dg-final { scan-assembler-times "ldsetal\t" 32} } */

/* Load-AND.  */

/* { dg-final { scan-assembler-times "ldclrb\t" 8} } */
/* { dg-final { scan-assembler-times "ldclrab\t" 16} } */
/* { dg-final { scan-assembler-times "ldclrlb\t" 8} } */
/* { dg-final { scan-assembler-times "ldclralb\t" 16} } */

/* { dg-final { scan-assembler-times "ldclrh\t" 8} } */
/* { dg-final { scan-assembler-times "ldclrah\t" 16} } */
/* { dg-final { scan-assembler-times "ldclrlh\t" 8} } */
/* { dg-final { scan-assembler-times "ldclralh\t" 16} } */

/* { dg-final { scan-assembler-times "ldclr\t" 16} */
/* { dg-final { scan-assembler-times "ldclra\t" 32} } */
/* { dg-final { scan-assembler-times "ldclrl\t" 16} } */
/* { dg-final { scan-assembler-times "ldclral\t" 32} } */

/* Load-XOR.  */

/* { dg-final { scan-assembler-times "ldeorb\t" 8} } */
/* { dg-final { scan-assembler-times "ldeorab\t" 16} } */
/* { dg-final { scan-assembler-times "ldeorlb\t" 8} } */
/* { dg-final { scan-assembler-times "ldeoralb\t" 16} } */

/* { dg-final { scan-assembler-times "ldeorh\t" 8} } */
/* { dg-final { scan-assembler-times "ldeorah\t" 16} } */
/* { dg-final { scan-assembler-times "ldeorlh\t" 8} } */
/* { dg-final { scan-assembler-times "ldeoralh\t" 16} } */

/* { dg-final { scan-assembler-times "ldeor\t" 16} */
/* { dg-final { scan-assembler-times "ldeora\t" 32} } */
/* { dg-final { scan-assembler-times "ldeorl\t" 16} } */
/* { dg-final { scan-assembler-times "ldeoral\t" 32} } */

/* { dg-final { scan-assembler-not "ldaxr\t" } } */
/* { dg-final { scan-assembler-not "stlxr\t" } } */
/* { dg-final { scan-assembler-not "dmb" } } */
