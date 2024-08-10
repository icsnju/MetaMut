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

struct m512_struct
{
  __m512 x;
};

struct m512h_struct
{
  __m512h x;
};

struct m512_2_struct
{
  __m512 x1, x2;
};

struct m512h_2_struct
{
  __m512h x1, x2;
};

/* Check that the struct is passed as the individual members in fregs.  */
void
check_struct_passing1 (struct m512_struct ms1 ATTRIBUTE_UNUSED,
		       struct m512_struct ms2 ATTRIBUTE_UNUSED,
		       struct m512_struct ms3 ATTRIBUTE_UNUSED,
		       struct m512_struct ms4 ATTRIBUTE_UNUSED,
		       struct m512_struct ms5 ATTRIBUTE_UNUSED,
		       struct m512_struct ms6 ATTRIBUTE_UNUSED,
		       struct m512_struct ms7 ATTRIBUTE_UNUSED,
		       struct m512_struct ms8 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
check_struct_passing1h (struct m512h_struct ms1 ATTRIBUTE_UNUSED,
			struct m512h_struct ms2 ATTRIBUTE_UNUSED,
			struct m512h_struct ms3 ATTRIBUTE_UNUSED,
			struct m512h_struct ms4 ATTRIBUTE_UNUSED,
			struct m512h_struct ms5 ATTRIBUTE_UNUSED,
			struct m512h_struct ms6 ATTRIBUTE_UNUSED,
			struct m512h_struct ms7 ATTRIBUTE_UNUSED,
			struct m512h_struct ms8 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
check_struct_passing2 (struct m512_2_struct ms ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ms.x1 == rsp+8);
  assert ((unsigned long)&ms.x2 == rsp+72);
}

void
check_struct_passing2h (struct m512h_2_struct ms ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ms.x1 == rsp+8);
  assert ((unsigned long)&ms.x2 == rsp+72);
}

static void
do_test (void)
{
  struct m512_struct m512s [8];
  struct m512h_struct m512hs [8];
  struct m512_2_struct m512_2s = {
      { 48.394, 39.3, -397.9, 3484.9, -8.394, -93.3, 7.9, 84.94,
	48.3941, 39.31, -397.91, 3484.91, -8.3941, -93.31, 7.91, 84.941 },
      { -8.394, -3.3, -39.9, 34.9, 7.9, 84.94, -48.394, 39.3,
	-8.3942, -3.32, -39.92, 34.92, 7.92, 84.942, -48.3942, 39.32 }
  };
  struct m512h_2_struct m512h_2s = {
      { 58.395f16, 39.3f16, -397.9f16, 3585.9f16, -8.395f16, -93.3f16, 7.9f16, 85.95f16,
        58.395f16, 39.3f16, -397.9f16, 3585.9f16, -8.395f16, -93.3f16, 7.9f16, 85.95f16,
        58.395f16, 39.3f16, -397.9f16, 3585.9f16, -8.395f16, -93.3f16, 7.9f16, 85.95f16,
	58.3951f16, 39.31f16, -397.91f16, 3585.91f16, -8.3951f16, -93.31f16, 7.91f16, 85.951f16},
      { 67.396f16, 39.3f16, -397.9f16, 3676.9f16, -7.396f16, -93.3f16, 7.9f16, 76.96f16,
        67.396f16, 39.3f16, -397.9f16, 3676.9f16, -7.396f16, -93.3f16, 7.9f16, 76.96f16,
        67.396f16, 39.3f16, -397.9f16, 3676.9f16, -7.396f16, -93.3f16, 7.9f16, 76.96f16,
	67.3961f16, 39.31f16, -397.91f16, 3676.91f16, -7.3961f16, -93.31f16, 7.91f16, 76.961f16},
  };
  int i;

  for (i = 0; i < 8; i++)
    {
      m512s[i].x = (__m512){32+i, 0, i, 0, -i, 0, i - 12, i + 8,
			    32+i, 0, i, 0, -i, 0, i - 12, i + 8};
      m512hs[i].x = (__m512h){33+i, 1, i, 2, -i, 0, i - 15, i + 9,
			      34+i, 1, i, 2, -i, 0, i - 15, i + 9,
			      35+i, 1, i, 2, -i, 0, i - 15, i + 9,
			      36+i, 1, i, 2, -i, 0, i - 15, i + 9};
    }

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.zmm0)[i]._m512[0] = m512s[i].x;
  num_fregs = 8;
  WRAP_CALL (check_struct_passing1)(m512s[0], m512s[1], m512s[2], m512s[3],
				    m512s[4], m512s[5], m512s[6], m512s[7]);
  WRAP_CALL (check_struct_passing2)(m512_2s);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.zmm0)[i]._m512h[0] = m512hs[i].x;
  num_fregs = 8;
  WRAP_CALL (check_struct_passing1h)(m512hs[0], m512hs[1], m512hs[2], m512hs[3],
				    m512hs[4], m512hs[5], m512hs[6], m512hs[7]);
  WRAP_CALL (check_struct_passing2h)(m512h_2s);
}
