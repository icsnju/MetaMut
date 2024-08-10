/* { dg-do compile } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-O3" } */

#define TEST(name, float_type, int_type, pref) void f_##name (float_type x) \
{									    \
  volatile float_type a = __builtin_rint (x);				    \
  volatile int_type   b = __builtin_l##pref##rint (x);			    \
}

TEST (dld, double, long, )
TEST (flf, float , long, )

TEST (did, double, int, )
TEST (fif, float , int, )

TEST (dlld, double, long long, l)
TEST (fllf, float , long long, l)

/* { dg-final { scan-assembler-times "frintx\t\[d,s\]\[0-9\]+, \[d,s\]\[0-9\]+" 6 } } */
/* { dg-final { scan-assembler-times "bl\tlrint"  4 } } */
/* { dg-final { scan-assembler-times "bl\tllrint" 2 } } */
/* { dg-final { scan-assembler-not "fcvtzs" } } */
