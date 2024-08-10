#include <stdio.h>
#define AVX512VL(ebx) 1
#define XSTATE_MASK (XSTATE_SSE | XSTATE_YMM | XSTATE_ZMM \
		     | XSTATE_HI_ZMM | XSTATE_OPMASK)
#include <stdlib.h>
#include <cpuid.h>

/* Check if the OS supports executing AVX512FP16 instructions.  */

#define XCR_XFEATURE_ENABLED_MASK	0x0

#define XSTATE_FP	0x1
#define XSTATE_SSE	0x2
#define XSTATE_YMM	0x4
#define XSTATE_OPMASK	0x20
#define XSTATE_ZMM	0x40
#define XSTATE_HI_ZMM	0x80

static int
check_osxsave (void)
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  return (ecx & bit_OSXSAVE) != 0;
}

static int
avx512fp16_os_support (void)
{
  unsigned int eax, edx;
  unsigned int ecx = XCR_XFEATURE_ENABLED_MASK;
  unsigned int mask = XSTATE_MASK;

  if (!check_osxsave ())
    return 0;

  __asm__ ("xgetbv" : "=a" (eax), "=d" (edx) : "c" (ecx));

  return ((eax & mask) == mask);
}

static void do_test (void);

int
main ()
{
  unsigned int eax, ebx, ecx, edx;

  if (!avx512fp16_os_support ())
    return 0;

  if (__get_cpuid_max (0, NULL) < 7)
    return 0;

  __cpuid_count (7, 0, eax, ebx, ecx, edx);

    /* Run AVX512FP16 test only if host has ISA support.  */
  if (((ebx & (bit_AVX512F | bit_AVX512BW))
       == (bit_AVX512F | bit_AVX512BW))
      && (edx & bit_AVX512FP16)
      && AVX512VL (ebx))
    {
      do_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
      return 0;
    }

#ifdef DEBUG
  printf ("SKIPPED\n");
#endif

  return 0;
}
#ifndef INCLUDED_ARGS_H
#define INCLUDED_ARGS_H

#include <immintrin.h>
#include <string.h>

/* Assertion macro.  */
#define assert(test) if (!(test)) abort()

#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define ATTRIBUTE_UNUSED
#endif

/* This defines the calling sequences for integers and floats.  */
#define I0 rdi
#define I1 rsi
#define I2 rdx
#define I3 rcx
#define I4 r8
#define I5 r9
#define F0 zmm0
#define F1 zmm1
#define F2 zmm2
#define F3 zmm3
#define F4 zmm4
#define F5 zmm5
#define F6 zmm6
#define F7 zmm7

typedef union {
  _Float16 __Float16[32];
  float _float[16];
  double _double[8];
  long long _longlong[8];
  int _int[16];
  unsigned long long _ulonglong[8];
  __m64 _m64[8];
  __m128 _m128[4];
  __m256 _m256[2];
  __m512 _m512[1];
  __m512h _m512h[1];
} ZMM_T;

typedef union {
  float _float;
  double _double;
  long double _ldouble;
  unsigned long long _ulonglong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
ZMM_T zmm_regs[32];
X87_T x87_regs[8];
extern volatile unsigned long long volatile_var;
extern void snapshot (void);
extern void snapshot_ret (void);
#define WRAP_CALL(N) \
  (callthis = (void (*)()) (N), (typeof (&N)) snapshot)
#define WRAP_RET(N) \
  (callthis = (void (*)()) (N), (typeof (&N)) snapshot_ret)

/* Clear all integer registers.  */
#define clear_int_hardware_registers \
  asm __volatile__ ("xor %%rax, %%rax\n\t" \
		    "xor %%rbx, %%rbx\n\t" \
		    "xor %%rcx, %%rcx\n\t" \
		    "xor %%rdx, %%rdx\n\t" \
		    "xor %%rsi, %%rsi\n\t" \
		    "xor %%rdi, %%rdi\n\t" \
		    "xor %%r8, %%r8\n\t" \
		    "xor %%r9, %%r9\n\t" \
		    "xor %%r10, %%r10\n\t" \
		    "xor %%r11, %%r11\n\t" \
		    "xor %%r12, %%r12\n\t" \
		    "xor %%r13, %%r13\n\t" \
		    "xor %%r14, %%r14\n\t" \
		    "xor %%r15, %%r15\n\t" \
		    ::: "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", \
		    "r9", "r10", "r11", "r12", "r13", "r14", "r15");

/* This is the list of registers available for passing arguments. Not all of
   these are used or even really available.  */
struct IntegerRegisters
{
  unsigned long long rax, rbx, rcx, rdx, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15;
};
struct FloatRegisters
{
  double mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
  long double st0, st1, st2, st3, st4, st5, st6, st7;
  ZMM_T zmm0, zmm1, zmm2, zmm3, zmm4, zmm5, zmm6, zmm7, zmm8, zmm9,
        zmm10, zmm11, zmm12, zmm13, zmm14, zmm15, zmm16, zmm17, zmm18,
	zmm19, zmm20, zmm21, zmm22, zmm23, zmm24, zmm25, zmm26, zmm27,
	zmm28, zmm29, zmm30, zmm31;
};

/* Implemented in scalarargs.c  */
extern struct IntegerRegisters iregs;
extern struct FloatRegisters fregs;
extern unsigned int num_iregs, num_fregs;

#define check_int_arguments do { \
  assert (num_iregs <= 0 || iregs.I0 == I0); \
  assert (num_iregs <= 1 || iregs.I1 == I1); \
  assert (num_iregs <= 2 || iregs.I2 == I2); \
  assert (num_iregs <= 3 || iregs.I3 == I3); \
  assert (num_iregs <= 4 || iregs.I4 == I4); \
  assert (num_iregs <= 5 || iregs.I5 == I5); \
  } while (0)

#define check_char_arguments check_int_arguments
#define check_short_arguments check_int_arguments
#define check_long_arguments check_int_arguments

/* Clear register struct.  */
#define clear_struct_registers \
  rax = rbx = rcx = rdx = rdi = rsi = rbp = rsp \
    = r8 = r9 = r10 = r11 = r12 = r13 = r14 = r15 = 0; \
  memset (&iregs, 0, sizeof (iregs)); \
  memset (&fregs, 0, sizeof (fregs)); \
  memset (zmm_regs, 0, sizeof (zmm_regs)); \
  memset (x87_regs, 0, sizeof (x87_regs));

/* Clear both hardware and register structs for integers.  */
#define clear_int_registers \
  clear_struct_registers \
  clear_int_hardware_registers

/* TODO: Do the checking.  */
#define check_f_arguments(T) do { \
  assert (num_fregs <= 0 || fregs.zmm0._ ## T [0] == zmm_regs[0]._ ## T [0]); \
  assert (num_fregs <= 1 || fregs.zmm1._ ## T [0] == zmm_regs[1]._ ## T [0]); \
  assert (num_fregs <= 2 || fregs.zmm2._ ## T [0] == zmm_regs[2]._ ## T [0]); \
  assert (num_fregs <= 3 || fregs.zmm3._ ## T [0] == zmm_regs[3]._ ## T [0]); \
  assert (num_fregs <= 4 || fregs.zmm4._ ## T [0] == zmm_regs[4]._ ## T [0]); \
  assert (num_fregs <= 5 || fregs.zmm5._ ## T [0] == zmm_regs[5]._ ## T [0]); \
  assert (num_fregs <= 6 || fregs.zmm6._ ## T [0] == zmm_regs[6]._ ## T [0]); \
  assert (num_fregs <= 7 || fregs.zmm7._ ## T [0] == zmm_regs[7]._ ## T [0]); \
  } while (0)

#define check_float_arguments check_f_arguments(float)
#define check_double_arguments check_f_arguments(double)

#define check_vector_arguments(T,O) do { \
  assert (num_fregs <= 0 \
	  || memcmp (((char *) &fregs.zmm0) + (O), \
		     &zmm_regs[0], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 1 \
	  || memcmp (((char *) &fregs.zmm1) + (O), \
		     &zmm_regs[1], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 2 \
	  || memcmp (((char *) &fregs.zmm2) + (O), \
		     &zmm_regs[2], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 3 \
	  || memcmp (((char *) &fregs.zmm3) + (O), \
		     &zmm_regs[3], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 4 \
	  || memcmp (((char *) &fregs.zmm4) + (O), \
		     &zmm_regs[4], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 5 \
	  || memcmp (((char *) &fregs.zmm5) + (O), \
		     &zmm_regs[5], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 6 \
	  || memcmp (((char *) &fregs.zmm6) + (O), \
		     &zmm_regs[6], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 7 \
	  || memcmp (((char *) &fregs.zmm7) + (O), \
		     &zmm_regs[7], \
		     sizeof (__ ## T) - (O)) == 0); \
  } while (0)

#define check_m64_arguments check_vector_arguments(m64, 0)
#define check_m128_arguments check_vector_arguments(m128, 0)
#define check_m256_arguments check_vector_arguments(m256, 0)
#define check_m512_arguments check_vector_arguments(m512, 0)

#endif /* INCLUDED_ARGS_H  */

struct IntegerRegisters iregs;
struct FloatRegisters fregs;
unsigned int num_iregs, num_fregs;

__m512
fun_test_returning___m512 (void)
{
  volatile_var++;
  return (__m512){73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}

__m512h
fun_test_returning___m512h (void)
{
  volatile_var++;
  return (__m512h){ 1.1f16, 2.2f16, 3.3f16, 4.4f16,
                    5.5f16, 6.6f16, 7.7f16, 8.8f16,
                    9.9f16,  10.10f16,   11.11f16, 12.12f16,
                    13.13f16, 14.14f16,  15.15f16, 16.16f16,
                    17.17f16, 18.18f16,  19.19f16, 20.20f16,
                    21.21f16, 22.22f16,  23.23f16, 24.24f16,
                    25.25f16, 26.26f16,  27.27f16, 28.28f16,
                    29.29f16, 30.30f16,  31.31f16, 32.32f16};
}

__m512 test_512;
__m512h test_512h;

static void
do_test (void)
{
  unsigned failed = 0;
  ZMM_T zmmt1, zmmt2;

  clear_struct_registers;
  test_512 = (__m512){73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  zmmt1._m512[0] = test_512;
  zmmt2._m512[0] = WRAP_RET (fun_test_returning___m512)();
  if (memcmp (&zmmt1, &zmmt2, sizeof (zmmt2)) != 0)
    printf ("fail m512\n"), failed++;

  clear_struct_registers;
  test_512h = (__m512h){ 1.1f16, 2.2f16, 3.3f16, 4.4f16,
                         5.5f16, 6.6f16, 7.7f16, 8.8f16,
                         9.9f16,  10.10f16,   11.11f16, 12.12f16,
                         13.13f16, 14.14f16,  15.15f16, 16.16f16,
                         17.17f16, 18.18f16,  19.19f16, 20.20f16,
                         21.21f16, 22.22f16,  23.23f16, 24.24f16,
                         25.25f16, 26.26f16,  27.27f16, 28.28f16,
                         29.29f16, 30.30f16,  31.31f16, 32.32f16};
  zmmt1._m512h[0] = test_512h;
  zmmt2._m512h[0] = WRAP_RET (fun_test_returning___m512h)();
  if (memcmp (&zmmt1, &zmmt2, sizeof (zmmt2)) != 0)
    printf ("fail m512h\n"), failed++;

  if (failed)
    abort ();
}