/* { dg-do compile } */
/* { dg-require-effective-target powerpc_altivec_ok } */
/* { dg-options "-maltivec -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fdump-tree-vect-details" } */

/* Test if unpack vectorization succeeds for type signed/unsigned
   short and char.  */

typedef signed long long sll;
typedef unsigned long long ull;
typedef signed int si;
typedef unsigned int ui;
typedef signed short sh;
typedef unsigned short uh;
typedef signed char sc;
typedef unsigned char uc;

#ifndef ALIGN
#define ALIGN 32
#endif

#define ALIGN_ATTR __attribute__((__aligned__(ALIGN)))

#define N 128

#define DEF_ARR(TYPE)                                                         \
  TYPE TYPE##_a[N] ALIGN_ATTR;                                                \
  TYPE TYPE##_b[N] ALIGN_ATTR;                                                \
  TYPE TYPE##_c[N] ALIGN_ATTR;

#define TEST1(NTYPE, WTYPE)                                                    \
  __attribute__((noipa)) void test1_##NTYPE##_##WTYPE() {                      \
    for (int i = 0; i < N; i++)                                                \
      WTYPE##_c[i] = NTYPE##_a[i] + NTYPE##_b[i];                              \
  }

#define CHECK1(NTYPE, WTYPE)                                                   \
  __attribute__((noipa, optimize(0))) void check1_##NTYPE##_##WTYPE() {        \
    for (int i = 0; i < N; i++) {                                              \
      NTYPE##_a[i] = 2 * i * sizeof(NTYPE) + 10;                               \
      NTYPE##_b[i] = 7 * i * sizeof(NTYPE) / 5 - 10;                           \
    }                                                                          \
    test1_##NTYPE##_##WTYPE();                                                 \
    for (int i = 0; i < N; i++) {                                              \
      WTYPE exp = NTYPE##_a[i] + NTYPE##_b[i];                                 \
      if (WTYPE##_c[i] != exp)                                                 \
        __builtin_abort();                                                     \
    }                                                                          \
  }


DEF_ARR (si)
DEF_ARR (ui)
DEF_ARR (sh)
DEF_ARR (uh)
DEF_ARR (sc)
DEF_ARR (uc)

TEST1 (sh, si)
TEST1 (uh, ui)
TEST1 (sc, sh)
TEST1 (uc, uh)


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 4 "vect" } } */
/* { dg-final { scan-assembler-times {\mvupkhsb\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvupklsb\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvupkhsh\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvupklsh\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvmrghb\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvmrglb\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvmrghh\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvmrglh\M} 2 } } */
