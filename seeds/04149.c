/* { dg-options "-O -msse2" } */

#include <xmmintrin.h>

typedef struct
{
  __m128 x;
} SS_struct_mi128;

typedef union
{
  __m128 x;
} SS_union_mi128;

typedef union
{
  __m128 x;
  unsigned long long u[2];
} union_mi128;

SS_union_mi128 un;
SS_struct_mi128 st;

extern void bar ();
extern void foo ();

void
union_m128_1_x ()
{
  union_mi128 x;

  x.u [0] = 0x123456789abcedf0LL;
  x.u [1] = 0xfedcba9876543210LL;
  un.x = x.x;
  st.x = x.x;
  bar(un);
  bar(st);
  foo(un);
  foo(st);
}
