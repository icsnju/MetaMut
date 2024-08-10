/* { dg-do run } */
/* { dg-options "-O2 -fpredictive-commoning -msse2 -std=c99" } */
/* { dg-require-effective-target sse2 } */

#include <x86intrin.h>

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

int main()
{
    const float mem[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    unsigned int indexes[8];
    for (unsigned int i = 0; i < 8; ++i) indexes[i] = i;

    check_isa ();

    if (!sse_os_support ())
      exit (0);

    __m128 x = _mm_setr_ps(0, 1, 2, 3);
    for (unsigned int i = 0; i + 4 < 6; ++i) {
        const unsigned int *ii = &indexes[i];
        const __m128 tmp = _mm_setr_ps(mem[ii[0]], mem[ii[1]], mem[ii[2]], mem[ii[3]]);
        if (0xf != _mm_movemask_ps(_mm_cmpeq_ps(tmp, x))) {
            __builtin_abort();
        }
        x = _mm_add_ps(x, _mm_set1_ps(1));
    }

    return 0;
}
