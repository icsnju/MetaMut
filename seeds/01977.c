/* { dg-do run } */
/* { dg-options "-O -msse2" } */
/* { dg-require-effective-target sse2 } */

#include "cpuid.h"

extern void exit (int) __attribute__((noreturn));

/* Determine what instruction set we've been compiled for,
   and detect that we're running with it.  */
static void __attribute__((constructor))
check_isa (void)
{
  int a, b, c, d;
  int c1, d1, c1e, d1e;

  c1 = d1 = c1e = d1e = 0;

#ifdef __MMX__
  d1 |= bit_MMX;
#endif
#ifdef __3dNOW__
  d1e |= bit_3DNOW;
#endif
#ifdef __3dNOW_A__
  d1e |= bit_3DNOWP;
#endif
#ifdef __SSE__
  d1 |= bit_SSE;
#endif
#ifdef __SSE2__
  d1 |= bit_SSE2;
#endif
#ifdef __SSE3__
  c1 |= bit_SSE3;
#endif
#ifdef __SSSE3__
  c1 |= bit_SSSE3;
#endif
#ifdef __SSE4_1__
  c1 |= bit_SSE4_1;
#endif
#ifdef __SSE4_2__
  c1 |= bit_SSE4_2;
#endif
#ifdef __AES__
  c1 |= bit_AES;
#endif
#ifdef __PCLMUL__
  c1 |= bit_PCLMUL;
#endif
#ifdef __AVX__
  c1 |= bit_AVX;
#endif
#ifdef __FMA__
  c1 |= bit_FMA;
#endif
#ifdef __SSE4A__
  c1e |= bit_SSE4a;
#endif
#ifdef __FMA4__
  c1e |= bit_FMA4;
#endif
#ifdef __XOP__
  c1e |= bit_XOP;
#endif
#ifdef __LWP__
  c1e |= bit_LWP;
#endif

  if (c1 | d1)
    {
      if (!__get_cpuid (1, &a, &b, &c, &d))
	goto fail;
      if ((c & c1) != c1 || (d & d1) != d1)
	goto fail;
    }
  if (c1e | d1e)
    {
      if (!__get_cpuid (0x80000001, &a, &b, &c, &d))
	goto fail;
      if ((c & c1e) != c1e || (d & d1e) != d1e)
	goto fail;
    }
  return;

 fail:
  exit (0);
}
/* Check if the OS supports executing SSE instructions.  */

static int
sse_os_support (void)
{
  /* All currently supported OSes do.  */
  return 1;
}

typedef long long S;
typedef long long V __attribute__((vector_size(16)));
typedef long long IV __attribute__((vector_size(16)));
typedef union { S s[2]; V v; } U;

static U i[2], b, c;

extern int memcmp (const void *, const void *, __SIZE_TYPE__);
#define assert(T) ((T) || (__builtin_trap (), 0))

#define TEST(E0, E1) \
  b.v = __builtin_shuffle (i[0].v, i[1].v, (IV){E0, E1}); \
  c.s[0] = i[0].s[E0]; \
  c.s[1] = i[0].s[E1]; \
  __asm__("" : : : "memory"); \
  assert (memcmp (&b, &c, sizeof(c)) == 0);

/* This file auto-generated with ./vperm.pl 2 2.  */

void check0(void)
{
  TEST (0, 0)
  TEST (1, 0)
  TEST (2, 0)
  TEST (3, 0)
  TEST (0, 1)
  TEST (1, 1)
  TEST (2, 1)
  TEST (3, 1)
  TEST (0, 2)
  TEST (1, 2)
  TEST (2, 2)
  TEST (3, 2)
  TEST (0, 3)
  TEST (1, 3)
  TEST (2, 3)
  TEST (3, 3)
}

void check(void)
{
  check0 ();
}


int main()
{
  check_isa ();

  if (!sse_os_support ())
    exit (0);

  i[0].s[0] = 0;
  i[0].s[1] = 1;
  i[0].s[2] = 2;
  i[0].s[3] = 3;

  check();
  return 0;
}
