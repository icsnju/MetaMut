/* { dg-do compile } */
/* { dg-options "-O3" } */

#define N 1024

#define ABD_ABS
#ifdef ABD_IDIOM

#define TEST1(S, TYPE)				\
__attribute__((noipa))				\
void fn_##S##_##TYPE (S TYPE * restrict a,	\
		      S TYPE * restrict b,	\
		      S TYPE * restrict out) {	\
  for (int i = 0; i < N; i++) {			\
    signed TYPE diff = b[i] - a[i];		\
    out[i] = diff > 0 ? diff : -diff;		\
} }

#define TEST2(S, TYPE1, TYPE2)			\
__attribute__((noipa))				\
void fn_##S##_##TYPE1##_##TYPE1##_##TYPE2	\
    (S TYPE1 * restrict a,			\
     S TYPE1 * restrict b,			\
     S TYPE2 * restrict out) {			\
  for (int i = 0; i < N; i++) {			\
    signed TYPE2 diff = b[i] - a[i];		\
    out[i] = diff > 0 ? diff : -diff;		\
} }

#define TEST3(S, TYPE1, TYPE2, TYPE3)		\
__attribute__((noipa))				\
void fn_##S##_##TYPE1##_##TYPE2##_##TYPE3	\
    (S TYPE1 * restrict a,			\
     S TYPE2 * restrict b,			\
     S TYPE3 * restrict out) {			\
  for (int i = 0; i < N; i++) {			\
    signed TYPE3 diff = b[i] - a[i];		\
    out[i] = diff > 0 ? diff : -diff;		\
} }

#endif

#ifdef ABD_ABS

#define TEST1(S, TYPE)				\
__attribute__((noipa))				\
void fn_##S##_##TYPE (S TYPE * restrict a,	\
		      S TYPE * restrict b,	\
		      S TYPE * restrict out) {	\
  for (int i = 0; i < N; i++)			\
    out[i] = __builtin_abs(a[i] - b[i]);	\
}

#define TEST2(S, TYPE1, TYPE2)			\
__attribute__((noipa))				\
void fn_##S##_##TYPE1##_##TYPE1##_##TYPE2	\
    (S TYPE1 * restrict a,			\
     S TYPE1 * restrict b,			\
     S TYPE2 * restrict out) {			\
  for (int i = 0; i < N; i++)			\
    out[i] = __builtin_abs(a[i] - b[i]);	\
}

#define TEST3(S, TYPE1, TYPE2, TYPE3)		\
__attribute__((noipa))				\
void fn_##S##_##TYPE1##_##TYPE2##_##TYPE3	\
    (S TYPE1 * restrict a,			\
     S TYPE2 * restrict b,			\
     S TYPE3 * restrict out) {			\
  for (int i = 0; i < N; i++)			\
    out[i] = __builtin_abs(a[i] - b[i]);	\
}

#endif

TEST1(signed, int)
TEST1(signed, short)
TEST1(signed, char)

TEST2(signed, char, short)
TEST2(signed, char, int)
TEST2(signed, char, long)
TEST2(signed, short, int)
TEST2(signed, short, long)
TEST2(signed, int, long)

TEST2(signed, int, short)
TEST2(signed, int, char)
TEST2(signed, short, char)

TEST3(signed, char, short, int)
TEST3(signed, char, int, short)
TEST3(signed, char, short, long)
TEST3(signed, char, int, long)
TEST3(signed, char, short, char)
TEST3(signed, char, int, char)

TEST3(signed, short, char, int)
TEST3(signed, short, int, char)
TEST3(signed, short, char, long)
TEST3(signed, short, int, long)
TEST3(signed, short, char, short)
TEST3(signed, short, int, short)

TEST3(signed, int, char, short)
TEST3(signed, int, short, char)
TEST3(signed, int, char, long)
TEST3(signed, int, short, long)
TEST3(signed, int, char, int)
TEST3(signed, int, short, int)

TEST1(unsigned, short)
TEST1(unsigned, char)

TEST2(unsigned, char, short)
TEST2(unsigned, char, int)
TEST2(unsigned, char, long)
TEST2(unsigned, short, int)
TEST2(unsigned, short, long)

TEST2(unsigned, short, char)

TEST3(unsigned, char, short, int)
TEST3(unsigned, char, short, long)
TEST3(unsigned, char, short, char)

TEST3(unsigned, short, char, int)
TEST3(unsigned, short, char, long)
TEST3(unsigned, short, char, short)

/* { dg-final { scan-assembler-times "sabd\\tz\[0-9\]+\.d, p\[0-9\]/m, z\[0-9\]+\.d, z\[0-9\]+\.d" 0 } } */
/* { dg-final { scan-assembler-times "sabd\\tz\[0-9\]+\.s, p\[0-9\]/m, z\[0-9\]+\.s, z\[0-9\]+\.s" 16 } } */
/* { dg-final { scan-assembler-times "sabd\\tz\[0-9\]+\.h, p\[0-9\]/m, z\[0-9\]+\.h, z\[0-9\]+\.h" 10 } } */
/* { dg-final { scan-assembler-times "sabd\\tz\[0-9\]+\.b, p\[0-9\]/m, z\[0-9\]+\.b, z\[0-9\]+\.b" 4 } } */

/* { dg-final { scan-assembler-times "uabd\\tz\[0-9\]+\.d, p\[0-9\]/m, z\[0-9\]+\.d, z\[0-9\]+\.d" 0 } } */
/* { dg-final { scan-assembler-times "uabd\\tz\[0-9\]+\.s, p\[0-9\]/m, z\[0-9\]+\.s, z\[0-9\]+\.s" 0 } } */
/* { dg-final { scan-assembler-times "uabd\\tz\[0-9\]+\.h, p\[0-9\]/m, z\[0-9\]+\.h, z\[0-9\]+\.h" 10 } } */
/* { dg-final { scan-assembler-times "uabd\\tz\[0-9\]+\.b, p\[0-9\]/m, z\[0-9\]+\.b, z\[0-9\]+\.b" 4 } } */

/* { dg-final { scan-assembler-not {\tsabdl\t} } } */
/* { dg-final { scan-assembler-not {\tsabdl2\t} } } */
/* { dg-final { scan-assembler-not {\tuabdl\t} } } */
/* { dg-final { scan-assembler-not {\tuabdl2\t} } } */
/* { dg-final { scan-assembler-not {\tabs\t} } } */
