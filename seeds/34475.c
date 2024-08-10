/* { dg-require-effective-target vect_double } */

/* Check if system supports SIMD */
#include <signal.h>

#if defined(__i386__) || defined(__x86_64__)
# include "cpuid.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void abort (void);
extern void exit (int);
#ifdef __cplusplus
}
#endif

static void
sig_ill_handler (int sig)
{
  exit(0);
}

static void __attribute__((noinline))
check_vect (void)
{
  signal(SIGILL, sig_ill_handler);
#if defined(__PAIRED__)
  /* 750CL paired-single instruction, 'ps_mul %v0,%v0,%v0'.  */
 asm volatile (".long 0x10000032");
#elif defined(__ppc__) || defined(__ppc64__) || defined(__powerpc__) || defined(powerpc)
  /* Altivec instruction, 'vor %v0,%v0,%v0'.  */
  asm volatile (".long 0x10000484");
#elif defined(__i386__) || defined(__x86_64__)
  {
    unsigned int a, b, c, d,
      want_level, want_b = 0, want_c = 0, want_d = 0;

    /* Determine what instruction set we've been compiled for, and detect
       that we're running with it.  This allows us to at least do a compile
       check for, e.g. SSE4.1 when the machine only supports SSE2.  */
# if defined(__AVX2__)
    want_level = 7, want_b = bit_AVX2;
# elif defined(__AVX__)
    want_level = 1, want_c = bit_AVX;
# elif defined(__SSE4_2__)
    want_level = 1, want_c = bit_SSE4_2;
# elif defined(__SSE4_1__)
    want_level = 1, want_c = bit_SSE4_1;
# elif defined(__SSSE3__)
    want_level = 1, want_c = bit_SSSE3;
# else
    want_level = 1, want_d = bit_SSE2;
# endif

    if (!__get_cpuid_count (want_level, 0, &a, &b, &c, &d)
	|| ((b & want_b) | (c & want_c) | (d & want_d)) == 0)
      exit (0);
  }
#elif defined(__sparc__)
  asm volatile (".word\t0x81b007c0");
#elif defined(__arm__)
  {
    /* On some processors without NEON support, this instruction may
       be a no-op, on others it may trap, so check that it executes
       correctly.  */
    long long a = 0, b = 1;
    asm ("vorr %P0, %P1, %P2"
	 : "=w" (a)
	 : "0" (a), "w" (b));
    if (a != 1)
      exit (0);
  }
#elif defined(__mips_msa)
  asm volatile ("or.v $w0,$w0,$w0");
#endif
  signal (SIGILL, SIG_DFL);
}

#if defined (__ARM_FEATURE_SVE)
#  if __ARM_FEATURE_SVE_BITS == 0
#    define VECTOR_BITS 1024
#  else
#    define VECTOR_BITS __ARM_FEATURE_SVE_BITS
#  endif
#else
#  define VECTOR_BITS 128
#endif

/* Which most of our tests are going to #define for internal use, and
   which might be exposed by system headers related to signal.h on some
   targets, notably VxWorks.  */
#undef OFFSET
   

#define N 8

typedef __SIZE_TYPE__ size_t;

extern void *malloc (size_t __size) __attribute__ ((__nothrow__,
						    __malloc__));

typedef double num_t;
static const num_t num__infty = ((num_t)1.0)/((num_t)0.0);

struct oct_tt;
typedef struct oct_tt oct_t;

typedef unsigned int var_t;
typedef enum {
  OCT_EMPTY = 0,
  OCT_NORMAL = 1,
  OCT_CLOSED = 2
} oct_state;

struct oct_tt {
  var_t n;

  int ref;

  oct_state state;
  struct oct_tt* closed;

  num_t* c;
};

void* octfapg_mm_malloc (size_t t);
oct_t* octfapg_alloc (var_t n);
oct_t* octfapg_full_copy (oct_t* m);

struct mmalloc_tt;
typedef struct mmalloc_tt mmalloc_t;

struct mmalloc_tt
{
  int id;

  int nb_alloc;
  int nb_realloc;
  int nb_free;

  size_t rem;
  size_t max;
  size_t tot;

};

typedef struct
{
  size_t size;

  mmalloc_t* mm;
  int id;

  double dummy;

} mmheader_t;

void*
octfapg_mm_malloc (size_t t)
{
  char* m = (char*)malloc(t+sizeof(mmheader_t));
  return m+sizeof(mmheader_t);
}

oct_t* octfapg_empty (var_t n);

oct_t*
octfapg_empty (const var_t n)
{
  oct_t* m;
  /*octfapg_timing_enter("oct_empty",3);*/
  m = ((oct_t*) octfapg_mm_malloc (sizeof(oct_t)));
  m->n = n;
  m->ref = 1;
  m->state = OCT_EMPTY;
  m->closed = (oct_t*)((void *)0);
  m->c = (num_t*)((void *)0);
  /*octfapg_timing_exit("oct_empty",3);*/
  return m;
}

oct_t*
octfapg_alloc (const var_t n)
{
  size_t nn = (2*(size_t)(n)*((size_t)(n)+1));
  oct_t* m;
  m = octfapg_empty(n);
  m->c = ((num_t*) octfapg_mm_malloc (sizeof(num_t)*(nn)));
  ;
  m->state = OCT_NORMAL;
  m->closed = (oct_t*)((void *)0);
  return m;
}

oct_t*
octfapg_universe (const var_t n)
{
  oct_t* m;
  size_t i, nn = (2*(size_t)(n)*((size_t)(n)+1));
  m = octfapg_alloc(n);
  for (i=0;i<nn;i++) *(m->c+i) = num__infty;
  for (i=0;i<2*n;i++) *(m->c+((size_t)(i)+(((size_t)(i)+1)*((size_t)(i)+1))/2)) = (num_t)(0);
  m->state = OCT_CLOSED;
  return m;
}

int main (void)
{ 
  int i;
  check_vect ();
  
  oct_t *p = octfapg_universe(10);
  return 0;
} 

/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "vector alignment may not be reachable" 1 "vect" { target { ! vector_alignment_reachable  } } } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 1 "vect" { target { ! vector_alignment_reachable } } } } */
