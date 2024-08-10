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

/* This struct holds values for argument checking.  */
struct
{
  ZMM_T i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15,
    i16, i17, i18, i19, i20, i21, i22, i23;
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

fun_check_passing_m512_8_values (__m512 i0 ATTRIBUTE_UNUSED,
				 __m512 i1 ATTRIBUTE_UNUSED,
				 __m512 i2 ATTRIBUTE_UNUSED,
				 __m512 i3 ATTRIBUTE_UNUSED,
				 __m512 i4 ATTRIBUTE_UNUSED,
				 __m512 i5 ATTRIBUTE_UNUSED,
				 __m512 i6 ATTRIBUTE_UNUSED,
				 __m512 i7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m512);
  compare (values.i1, i1, __m512);
  compare (values.i2, i2, __m512);
  compare (values.i3, i3, __m512);
  compare (values.i4, i4, __m512);
  compare (values.i5, i5, __m512);
  compare (values.i6, i6, __m512);
  compare (values.i7, i7, __m512);
}

fun_check_passing_m512h_8_values (__m512h i0 ATTRIBUTE_UNUSED,
				  __m512h i1 ATTRIBUTE_UNUSED,
				  __m512h i2 ATTRIBUTE_UNUSED,
				  __m512h i3 ATTRIBUTE_UNUSED,
				  __m512h i4 ATTRIBUTE_UNUSED,
				  __m512h i5 ATTRIBUTE_UNUSED,
				  __m512h i6 ATTRIBUTE_UNUSED,
				  __m512h i7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m512h);
  compare (values.i1, i1, __m512h);
  compare (values.i2, i2, __m512h);
  compare (values.i3, i3, __m512h);
  compare (values.i4, i4, __m512h);
  compare (values.i5, i5, __m512h);
  compare (values.i6, i6, __m512h);
  compare (values.i7, i7, __m512h);
}

void
fun_check_passing_m512_8_regs (__m512 i0 ATTRIBUTE_UNUSED,
			       __m512 i1 ATTRIBUTE_UNUSED,
			       __m512 i2 ATTRIBUTE_UNUSED,
			       __m512 i3 ATTRIBUTE_UNUSED,
			       __m512 i4 ATTRIBUTE_UNUSED,
			       __m512 i5 ATTRIBUTE_UNUSED,
			       __m512 i6 ATTRIBUTE_UNUSED,
			       __m512 i7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
fun_check_passing_m512h_8_regs (__m512h i0 ATTRIBUTE_UNUSED,
				__m512h i1 ATTRIBUTE_UNUSED,
				__m512h i2 ATTRIBUTE_UNUSED,
				__m512h i3 ATTRIBUTE_UNUSED,
				__m512h i4 ATTRIBUTE_UNUSED,
				__m512h i5 ATTRIBUTE_UNUSED,
				__m512h i6 ATTRIBUTE_UNUSED,
				__m512h i7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
fun_check_passing_m512_20_values (__m512 i0 ATTRIBUTE_UNUSED,
				  __m512 i1 ATTRIBUTE_UNUSED,
				  __m512 i2 ATTRIBUTE_UNUSED,
				  __m512 i3 ATTRIBUTE_UNUSED,
				  __m512 i4 ATTRIBUTE_UNUSED,
				  __m512 i5 ATTRIBUTE_UNUSED,
				  __m512 i6 ATTRIBUTE_UNUSED,
				  __m512 i7 ATTRIBUTE_UNUSED,
				  __m512 i8 ATTRIBUTE_UNUSED,
				  __m512 i9 ATTRIBUTE_UNUSED,
				  __m512 i10 ATTRIBUTE_UNUSED,
				  __m512 i11 ATTRIBUTE_UNUSED,
				  __m512 i12 ATTRIBUTE_UNUSED,
				  __m512 i13 ATTRIBUTE_UNUSED,
				  __m512 i14 ATTRIBUTE_UNUSED,
				  __m512 i15 ATTRIBUTE_UNUSED,
				  __m512 i16 ATTRIBUTE_UNUSED,
				  __m512 i17 ATTRIBUTE_UNUSED,
				  __m512 i18 ATTRIBUTE_UNUSED,
				  __m512 i19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m512);
  compare (values.i1, i1, __m512);
  compare (values.i2, i2, __m512);
  compare (values.i3, i3, __m512);
  compare (values.i4, i4, __m512);
  compare (values.i5, i5, __m512);
  compare (values.i6, i6, __m512);
  compare (values.i7, i7, __m512);
  compare (values.i8, i8, __m512);
  compare (values.i9, i9, __m512);
  compare (values.i10, i10, __m512);
  compare (values.i11, i11, __m512);
  compare (values.i12, i12, __m512);
  compare (values.i13, i13, __m512);
  compare (values.i14, i14, __m512);
  compare (values.i15, i15, __m512);
  compare (values.i16, i16, __m512);
  compare (values.i17, i17, __m512);
  compare (values.i18, i18, __m512);
  compare (values.i19, i19, __m512);
}

void
fun_check_passing_m512h_20_values (__m512h i0 ATTRIBUTE_UNUSED,
				   __m512h i1 ATTRIBUTE_UNUSED,
				   __m512h i2 ATTRIBUTE_UNUSED,
				   __m512h i3 ATTRIBUTE_UNUSED,
				   __m512h i4 ATTRIBUTE_UNUSED,
				   __m512h i5 ATTRIBUTE_UNUSED,
				   __m512h i6 ATTRIBUTE_UNUSED,
				   __m512h i7 ATTRIBUTE_UNUSED,
				   __m512h i8 ATTRIBUTE_UNUSED,
				   __m512h i9 ATTRIBUTE_UNUSED,
				   __m512h i10 ATTRIBUTE_UNUSED,
				   __m512h i11 ATTRIBUTE_UNUSED,
				   __m512h i12 ATTRIBUTE_UNUSED,
				   __m512h i13 ATTRIBUTE_UNUSED,
				   __m512h i14 ATTRIBUTE_UNUSED,
				   __m512h i15 ATTRIBUTE_UNUSED,
				   __m512h i16 ATTRIBUTE_UNUSED,
				   __m512h i17 ATTRIBUTE_UNUSED,
				   __m512h i18 ATTRIBUTE_UNUSED,
				   __m512h i19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m512h);
  compare (values.i1, i1, __m512h);
  compare (values.i2, i2, __m512h);
  compare (values.i3, i3, __m512h);
  compare (values.i4, i4, __m512h);
  compare (values.i5, i5, __m512h);
  compare (values.i6, i6, __m512h);
  compare (values.i7, i7, __m512h);
  compare (values.i8, i8, __m512h);
  compare (values.i9, i9, __m512h);
  compare (values.i10, i10, __m512h);
  compare (values.i11, i11, __m512h);
  compare (values.i12, i12, __m512h);
  compare (values.i13, i13, __m512h);
  compare (values.i14, i14, __m512h);
  compare (values.i15, i15, __m512h);
  compare (values.i16, i16, __m512h);
  compare (values.i17, i17, __m512h);
  compare (values.i18, i18, __m512h);
  compare (values.i19, i19, __m512h);
}

void
fun_check_passing_m512_20_regs (__m512 i0 ATTRIBUTE_UNUSED,
				__m512 i1 ATTRIBUTE_UNUSED,
				__m512 i2 ATTRIBUTE_UNUSED,
				__m512 i3 ATTRIBUTE_UNUSED,
				__m512 i4 ATTRIBUTE_UNUSED,
				__m512 i5 ATTRIBUTE_UNUSED,
				__m512 i6 ATTRIBUTE_UNUSED,
				__m512 i7 ATTRIBUTE_UNUSED,
				__m512 i8 ATTRIBUTE_UNUSED,
				__m512 i9 ATTRIBUTE_UNUSED,
				__m512 i10 ATTRIBUTE_UNUSED,
				__m512 i11 ATTRIBUTE_UNUSED,
				__m512 i12 ATTRIBUTE_UNUSED,
				__m512 i13 ATTRIBUTE_UNUSED,
				__m512 i14 ATTRIBUTE_UNUSED,
				__m512 i15 ATTRIBUTE_UNUSED,
				__m512 i16 ATTRIBUTE_UNUSED,
				__m512 i17 ATTRIBUTE_UNUSED,
				__m512 i18 ATTRIBUTE_UNUSED,
				__m512 i19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
fun_check_passing_m512h_20_regs (__m512h i0 ATTRIBUTE_UNUSED,
				 __m512h i1 ATTRIBUTE_UNUSED,
				 __m512h i2 ATTRIBUTE_UNUSED,
				 __m512h i3 ATTRIBUTE_UNUSED,
				 __m512h i4 ATTRIBUTE_UNUSED,
				 __m512h i5 ATTRIBUTE_UNUSED,
				 __m512h i6 ATTRIBUTE_UNUSED,
				 __m512h i7 ATTRIBUTE_UNUSED,
				 __m512h i8 ATTRIBUTE_UNUSED,
				 __m512h i9 ATTRIBUTE_UNUSED,
				 __m512h i10 ATTRIBUTE_UNUSED,
				 __m512h i11 ATTRIBUTE_UNUSED,
				 __m512h i12 ATTRIBUTE_UNUSED,
				 __m512h i13 ATTRIBUTE_UNUSED,
				 __m512h i14 ATTRIBUTE_UNUSED,
				 __m512h i15 ATTRIBUTE_UNUSED,
				 __m512h i16 ATTRIBUTE_UNUSED,
				 __m512h i17 ATTRIBUTE_UNUSED,
				 __m512h i18 ATTRIBUTE_UNUSED,
				 __m512h i19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

#define def_check_passing8(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _func1, _func2, TYPE) \
  values.i0.TYPE[0] = _i0; \
  values.i1.TYPE[0] = _i1; \
  values.i2.TYPE[0] = _i2; \
  values.i3.TYPE[0] = _i3; \
  values.i4.TYPE[0] = _i4; \
  values.i5.TYPE[0] = _i5; \
  values.i6.TYPE[0] = _i6; \
  values.i7.TYPE[0] = _i7; \
  WRAP_CALL(_func1) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7); \
  \
  clear_struct_registers; \
  fregs.F0.TYPE[0] = _i0; \
  fregs.F1.TYPE[0] = _i1; \
  fregs.F2.TYPE[0] = _i2; \
  fregs.F3.TYPE[0] = _i3; \
  fregs.F4.TYPE[0] = _i4; \
  fregs.F5.TYPE[0] = _i5; \
  fregs.F6.TYPE[0] = _i6; \
  fregs.F7.TYPE[0] = _i7; \
  num_fregs = 8; \
  WRAP_CALL(_func2) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7);

#define def_check_passing20(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, \
			    _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, \
			    _i18, _i19, _func1, _func2, TYPE) \
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
  values.i10.TYPE[0] = _i10; \
  values.i11.TYPE[0] = _i11; \
  values.i12.TYPE[0] = _i12; \
  values.i13.TYPE[0] = _i13; \
  values.i14.TYPE[0] = _i14; \
  values.i15.TYPE[0] = _i15; \
  values.i16.TYPE[0] = _i16; \
  values.i17.TYPE[0] = _i17; \
  values.i18.TYPE[0] = _i18; \
  values.i19.TYPE[0] = _i19; \
  WRAP_CALL(_func1) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, \
		     _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, \
		     _i18, _i19); \
  \
  clear_struct_registers; \
  fregs.F0.TYPE[0] = _i0; \
  fregs.F1.TYPE[0] = _i1; \
  fregs.F2.TYPE[0] = _i2; \
  fregs.F3.TYPE[0] = _i3; \
  fregs.F4.TYPE[0] = _i4; \
  fregs.F5.TYPE[0] = _i5; \
  fregs.F6.TYPE[0] = _i6; \
  fregs.F7.TYPE[0] = _i7; \
  num_fregs = 8; \
  WRAP_CALL(_func2) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, \
		     _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, \
		     _i18, _i19);

void
test_m512_on_stack ()
{
  __m512 x[8];
  int i;
  for (i = 0; i < 8; i++)
    x[i] = (__m512){32 + i, 0, 0, 0, 0, 0, 0, 0};
  pass = "m512-8";
  def_check_passing8 (x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7],
		      fun_check_passing_m512_8_values,
		      fun_check_passing_m512_8_regs, _m512);
}

void
test_m512h_on_stack ()
{
  __m512h x[8];
  int i;
  for (i = 0; i < 8; i++)
    x[i] = (__m512h){1.1f16 + i, 2.2f16 + i, 3.3f16 + i, 4.4f16 + i,
		     5.5f16 + i, 6.6f16 + i, 7.7f16 + i, 8.8f16 + i,
		     9.9f16 + i, 10.10f16 + i, 11.11f16 + i, 12.12f16 + i,
		     13.13f16 + i, 14.14f16 + i, 15.15f16 + i, 16.16f16 + i,
		     17.17f16 + i, 18.18f16 + i, 19.19f16 + i, 20.20f16 + i,
		     21.21f16 + i, 22.22f16 + i, 23.23f16 + i, 24.24f16 + i,
		     25.25f16 + i, 26.26f16 + i, 27.27f16 + i, 28.28f16 + i,
		     29.29f16 + i, 30.30f16 + i, 31.31f16 + i, 32.32f16 + i};

  pass = "m512h-8";
  def_check_passing8 (x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7],
		      fun_check_passing_m512h_8_values,
		      fun_check_passing_m512h_8_regs, _m512h);
}

void
test_too_many_m512 ()
{
  __m512 x[20];
  int i;
  for (i = 0; i < 20; i++)
    x[i] = (__m512){32 + i, 0, 0, 0, 0, 0, 0, 0};
  pass = "m512-20";
  def_check_passing20 (x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8],
		       x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16],
		       x[17], x[18], x[19], fun_check_passing_m512_20_values,
		       fun_check_passing_m512_20_regs, _m512);
}

void
test_too_many_m512h ()
{
  __m512h x[20];
  int i;
  for (i = 0; i < 20; i++)
    x[i] = (__m512h){ 1.1f16 + i, 2.2f16 + i, 3.3f16 + i, 4.4f16 + i,
		      5.5f16 + i, 6.6f16 + i, 7.7f16 + i, 8.8f16 + i,
		      9.9f16 + i, 10.10f16 + i, 11.11f16 + i, 12.12f16 + i,
		      13.13f16 + i, 14.14f16 + i, 15.15f16 + i, 16.16f16 + i,
		      17.17f16 + i, 18.18f16 + i, 19.19f16 + i, 20.20f16 + i,
		      21.21f16 + i, 22.22f16 + i, 23.23f16 + i, 24.24f16 + i,
		      25.25f16 + i, 26.26f16 + i, 27.27f16 + i, 28.28f16 + i,
		      29.29f16 + i, 30.30f16 + i, 31.31f16 + i, 32.32f16 + i};
  pass = "m512h-20";
  def_check_passing20 (x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8],
		       x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16],
		       x[17], x[18], x[19], fun_check_passing_m512h_20_values,
		       fun_check_passing_m512h_20_regs, _m512h);
}

static void
do_test (void)
{
  test_m512_on_stack ();
  test_too_many_m512 ();
  test_m512h_on_stack ();
  test_too_many_m512h ();
  if (failed)
    abort ();
}
