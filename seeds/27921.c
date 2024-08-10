/* For some targets we end up vectorizing the below loop such that the `sp`
   single integer is loaded into a 4 integer vector.
   While the writes are all safe, without 2 scalar loops being peeled into the
   epilogue we would read past the end of the 31 integer array.  This happens
   because we load a 4 integer chunk to only use the first integer and
   increment by 2 integers at a time, hence the last load needs s[30-33] and
   the penultimate load needs s[28-31].
   This testcase ensures that we do not crash due to that behaviour.  */
/* { dg-require-effective-target mmap } */
#include <sys/mman.h>
#include <stdio.h>
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
   

#define MMAP_SIZE 0x20000
#define ADDRESS 0x1122000000

#define MB_BLOCK_SIZE 16
#define VERT_PRED_16 0
#define HOR_PRED_16 1
#define DC_PRED_16 2
int *sptr;
extern void intrapred_luma_16x16();
unsigned short mprr_2[5][16][16];
void initialise_s(int *s) { }
int main() {
    void *s_mapping;
    void *end_s;
    check_vect ();
    s_mapping = mmap ((void *)ADDRESS, MMAP_SIZE, PROT_READ | PROT_WRITE,
		      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (s_mapping == MAP_FAILED)
      {
	perror ("mmap");
	return 1;
      }
    end_s = (s_mapping + MMAP_SIZE);
    sptr = (int*)(end_s - sizeof(int[31]));
    intrapred_luma_16x16(sptr);
    return 0;
}

void intrapred_luma_16x16(int * restrict sp) {
    for (int j=0; j < MB_BLOCK_SIZE; j++)
      {
	mprr_2[VERT_PRED_16][j][0]=sp[j*2];
	mprr_2[VERT_PRED_16][j][1]=sp[j*2];
	mprr_2[VERT_PRED_16][j][2]=sp[j*2];
	mprr_2[VERT_PRED_16][j][3]=sp[j*2];
	mprr_2[VERT_PRED_16][j][4]=sp[j*2];
	mprr_2[VERT_PRED_16][j][5]=sp[j*2];
	mprr_2[VERT_PRED_16][j][6]=sp[j*2];
	mprr_2[VERT_PRED_16][j][7]=sp[j*2];
	mprr_2[VERT_PRED_16][j][8]=sp[j*2];
	mprr_2[VERT_PRED_16][j][9]=sp[j*2];
	mprr_2[VERT_PRED_16][j][10]=sp[j*2];
	mprr_2[VERT_PRED_16][j][11]=sp[j*2];
	mprr_2[VERT_PRED_16][j][12]=sp[j*2];
	mprr_2[VERT_PRED_16][j][13]=sp[j*2];
	mprr_2[VERT_PRED_16][j][14]=sp[j*2];
	mprr_2[VERT_PRED_16][j][15]=sp[j*2];
      }
}
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" {target vect_int } } } */
