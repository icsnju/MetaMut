/* Test variable number of 256-bit vector arguments passed to functions.  */

#include <stdio.h>
#define AVX512VL(ebx) (ebx & bit_AVX512VL)
#define XSTATE_MASK (XSTATE_SSE | XSTATE_YMM | XSTATE_OPMASK)
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
#define F0 ymm0
#define F1 ymm1
#define F2 ymm2
#define F3 ymm3
#define F4 ymm4
#define F5 ymm5
#define F6 ymm6
#define F7 ymm7

typedef union {
  _Float16 __Float16[16];
  float _float[8];
  double _double[4];
  long long _longlong[4];
  int _int[8];
  unsigned long long _ulonglong[4];
  __m64 _m64[4];
  __m128 _m128[2];
  __m256 _m256[1];
  __m256h _m256h[1];
} YMM_T;

typedef union {
  float _float;
  double _double;
  long double _ldouble;
  unsigned long long _ulonglong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
YMM_T ymm_regs[16];
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
  YMM_T ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7, ymm8, ymm9,
        ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;
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
  memset (ymm_regs, 0, sizeof (ymm_regs)); \
  memset (x87_regs, 0, sizeof (x87_regs));

/* Clear both hardware and register structs for integers.  */
#define clear_int_registers \
  clear_struct_registers \
  clear_int_hardware_registers

/* TODO: Do the checking.  */
#define check_f_arguments(T) do { \
  assert (num_fregs <= 0 || fregs.ymm0._ ## T [0] == ymm_regs[0]._ ## T [0]); \
  assert (num_fregs <= 1 || fregs.ymm1._ ## T [0] == ymm_regs[1]._ ## T [0]); \
  assert (num_fregs <= 2 || fregs.ymm2._ ## T [0] == ymm_regs[2]._ ## T [0]); \
  assert (num_fregs <= 3 || fregs.ymm3._ ## T [0] == ymm_regs[3]._ ## T [0]); \
  assert (num_fregs <= 4 || fregs.ymm4._ ## T [0] == ymm_regs[4]._ ## T [0]); \
  assert (num_fregs <= 5 || fregs.ymm5._ ## T [0] == ymm_regs[5]._ ## T [0]); \
  assert (num_fregs <= 6 || fregs.ymm6._ ## T [0] == ymm_regs[6]._ ## T [0]); \
  assert (num_fregs <= 7 || fregs.ymm7._ ## T [0] == ymm_regs[7]._ ## T [0]); \
  } while (0)

#define check_float_arguments check_f_arguments(float)
#define check_double_arguments check_f_arguments(double)

#define check_vector_arguments(T,O) do { \
  assert (num_fregs <= 0 \
	  || memcmp (((char *) &fregs.ymm0) + (O), \
		     &ymm_regs[0], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 1 \
	  || memcmp (((char *) &fregs.ymm1) + (O), \
		     &ymm_regs[1], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 2 \
	  || memcmp (((char *) &fregs.ymm2) + (O), \
		     &ymm_regs[2], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 3 \
	  || memcmp (((char *) &fregs.ymm3) + (O), \
		     &ymm_regs[3], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 4 \
	  || memcmp (((char *) &fregs.ymm4) + (O), \
		     &ymm_regs[4], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 5 \
	  || memcmp (((char *) &fregs.ymm5) + (O), \
		     &ymm_regs[5], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 6 \
	  || memcmp (((char *) &fregs.ymm6) + (O), \
		     &ymm_regs[6], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 7 \
	  || memcmp (((char *) &fregs.ymm7) + (O), \
		     &ymm_regs[7], \
		     sizeof (__ ## T) - (O)) == 0); \
  } while (0)

#define check_m64_arguments check_vector_arguments(m64, 0)
#define check_m128_arguments check_vector_arguments(m128, 0)
#define check_m256_arguments check_vector_arguments(m256, 0)

#endif /* INCLUDED_ARGS_H  */

struct IntegerRegisters iregs;
struct FloatRegisters fregs;

/* This struct holds values for argument checking.  */
struct 
{
  YMM_T i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
} values;

char *pass;
int failed = 0;

#undef assert
#define assert(c) do { \
  if (!(c)) {failed++; printf ("failed %s\n", pass); } \
} while (0)

#define compare(X1,X2,T) do { \
  assert (memcmp (&X1, &X2, sizeof (T)) == 0); \
} while (0)

void
fun_check_passing_m256_varargs (__m256 i0, __m256 i1, __m256 i2,
				__m256 i3, ...)
{
  /* Check argument values.  */
  void **fp = __builtin_frame_address (0);
  void *ra = __builtin_return_address (0);
  __m256 *argp;

  compare (values.i0, i0, __m256);
  compare (values.i1, i1, __m256);
  compare (values.i2, i2, __m256);
  compare (values.i3, i3, __m256);

  /* Get the pointer to the return address on stack.  */
  while (*fp != ra)
    fp++;

  /* Skip the return address stack slot.  */
  argp = (__m256 *)(((char *) fp) + 8);

  /* Check __m256 arguments passed on stack.  */
  compare (values.i4, argp[0], __m256);
  compare (values.i5, argp[1], __m256);
  compare (values.i6, argp[2], __m256);
  compare (values.i7, argp[3], __m256);
  compare (values.i8, argp[4], __m256);
  compare (values.i9, argp[5], __m256);

  /* Check register contents.  */
  compare (fregs.ymm0, ymm_regs[0], __m256);
  compare (fregs.ymm1, ymm_regs[1], __m256);
  compare (fregs.ymm2, ymm_regs[2], __m256);
  compare (fregs.ymm3, ymm_regs[3], __m256);
}

void
fun_check_passing_m256h_varargs (__m256h i0, __m256h i1, __m256h i2,
				 __m256h i3, ...)
{
  /* Check argument values.  */
  void **fp = __builtin_frame_address (0);
  void *ra = __builtin_return_address (0);
  __m256h *argp;

  compare (values.i0, i0, __m256h);
  compare (values.i1, i1, __m256h);
  compare (values.i2, i2, __m256h);
  compare (values.i3, i3, __m256h);

  /* Get the pointer to the return address on stack.  */
  while (*fp != ra)
    fp++;

  /* Skip the return address stack slot.  */
  argp = (__m256h *)(((char *) fp) + 8);

  /* Check __m256h arguments passed on stack.  */
  compare (values.i4, argp[0], __m256h);
  compare (values.i5, argp[1], __m256h);
  compare (values.i6, argp[2], __m256h);
  compare (values.i7, argp[3], __m256h);
  compare (values.i8, argp[4], __m256h);
  compare (values.i9, argp[5], __m256h);

  /* Check register contents.  */
  compare (fregs.ymm0, ymm_regs[0], __m256h);
  compare (fregs.ymm1, ymm_regs[1], __m256h);
  compare (fregs.ymm2, ymm_regs[2], __m256h);
  compare (fregs.ymm3, ymm_regs[3], __m256h);
}

#define def_check_int_passing_varargs(_i0, _i1, _i2, _i3, _i4, _i5, \
				      _i6, _i7, _i8, _i9, \
				      _func, TYPE) \
  values.i0.TYPE[0] = _i0; \
  values.i1.TYPE[0] = _i1; \
  values.i2.TYPE[0] = _i2; \
  values.i3.TYPE[0] = _i3; \
  values.i4.TYPE[0] = _i4; \
  values.i5.TYPE[0] = _i5; \
  values.i6.TYPE[0] = _i6; \
  values.i7.TYPE[0] = _i7; \
  values.i8.TYPE[0] = _i8; \
  values.i9.TYPE[0] = _i9; \
  clear_struct_registers; \
  fregs.F0.TYPE[0] = _i0; \
  fregs.F1.TYPE[0] = _i1; \
  fregs.F2.TYPE[0] = _i2; \
  fregs.F3.TYPE[0] = _i3; \
  WRAP_CALL(_func) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9);

void
test_m256_varargs (void)
{
  __m256 x[10];
  int i;
  for (i = 0; i < 10; i++)
    x[i] = (__m256){32+i, 0, 0, 0, 0, 0, 0, 0};
  pass = "m256-varargs";
  def_check_int_passing_varargs (x[0], x[1], x[2], x[3], x[4], x[5],
				 x[6], x[7], x[8], x[9],
				 fun_check_passing_m256_varargs,
				 _m256);
}

void
test_m256h_varargs (void)
{
  __m256h x[10];
  int i;
  for (i = 0; i < 10; i++)
    x[i] = (__m256h) {
        1.1f16 + i, 2.2f16 + i, 3.3f16 + i, 4.4f16 + i,
	5.5f16 + i, 6.6f16 + i, 7.7f16 + i, 8.8f16 + i,
	9.9f16 + i, 10.10f16 + i, 11.11f16 + i, 12.12f16 + i,
	13.13f16 + i, 14.14f16 + i, 15.15f16 + i, 16.16f16 + i
    };
  pass = "m256h-varargs";
  def_check_int_passing_varargs (x[0], x[1], x[2], x[3], x[4], x[5],
				 x[6], x[7], x[8], x[9],
				 fun_check_passing_m256h_varargs,
				 _m256h);
}

void
do_test (void)
{
  test_m256_varargs ();
  test_m256h_varargs ();
  if (failed)
    abort ();
}
