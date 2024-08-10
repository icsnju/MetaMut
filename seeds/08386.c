/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-msse2" } */
/* { dg-require-effective-target sse2_runtime } */

typedef double __m128d __attribute__ ((__vector_size__ (16), __may_alias__));

#define TYPE __m128d

typedef TYPE __attribute__((aligned(1))) unaligned;

extern void abort (void);


TYPE  __attribute__((noinline))
foo (TYPE a1, TYPE a2, TYPE a3, TYPE a4,
     TYPE a5, TYPE a6, TYPE a7, TYPE a8,
     int b1, int b2, int b3, int b4, int b5, int b6, int b7, unaligned y)
{
  return y;
}

void
do_test (void)
{
  unaligned x;
  TYPE y = { 0 };
  x = y; 
  y = foo (y, y, y, y, y, y, y, y, 1, 2, 3, 4, 5, 6, -1, x);
  if (__builtin_memcmp (&y, &x, sizeof (y)) != 0)
    abort ();
}

int
main (void)
{
  do_test ();
  return 0;
}
