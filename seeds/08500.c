#include <stdio.h>
#include <stdlib.h>
#include "cpuid.h"

static void avx_test (void);

int
main ()
{
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run AVX test only if host has AVX support.  */
  if ((ecx & (bit_AVX | bit_OSXSAVE)) == (bit_AVX | bit_OSXSAVE))
    {
      avx_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
    }
#ifdef DEBUG
  else
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
  float _float[8];
  double _double[4];
  long _long[4];
  int _int[8];
  unsigned long _ulong[4];
  __m64 _m64[4];
  __m128 _m128[2];
  __m256 _m256[1];
} YMM_T;

typedef union {
  float _float;
  double _double;
  long double _ldouble;
  unsigned long _ulong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
YMM_T ymm_regs[16];
X87_T x87_regs[8];
extern volatile unsigned long volatile_var;
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
  unsigned long rax, rbx, rcx, rdx, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15;
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
unsigned int num_iregs, num_fregs;

/* This struct holds values for argument checking.  */
struct 
{
  YMM_T i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
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

fun_check_passing_m256_8_values (__m256 i0 ATTRIBUTE_UNUSED, __m256 i1 ATTRIBUTE_UNUSED, __m256 i2 ATTRIBUTE_UNUSED, __m256 i3 ATTRIBUTE_UNUSED, __m256 i4 ATTRIBUTE_UNUSED, __m256 i5 ATTRIBUTE_UNUSED, __m256 i6 ATTRIBUTE_UNUSED, __m256 i7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m256);
  compare (values.i1, i1, __m256);
  compare (values.i2, i2, __m256);
  compare (values.i3, i3, __m256);
  compare (values.i4, i4, __m256);
  compare (values.i5, i5, __m256);
  compare (values.i6, i6, __m256);
  compare (values.i7, i7, __m256);
}

void
fun_check_passing_m256_8_regs (__m256 i0 ATTRIBUTE_UNUSED, __m256 i1 ATTRIBUTE_UNUSED, __m256 i2 ATTRIBUTE_UNUSED, __m256 i3 ATTRIBUTE_UNUSED, __m256 i4 ATTRIBUTE_UNUSED, __m256 i5 ATTRIBUTE_UNUSED, __m256 i6 ATTRIBUTE_UNUSED, __m256 i7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m256_arguments;
}

void
fun_check_passing_m256_20_values (__m256 i0 ATTRIBUTE_UNUSED, __m256 i1 ATTRIBUTE_UNUSED, __m256 i2 ATTRIBUTE_UNUSED, __m256 i3 ATTRIBUTE_UNUSED, __m256 i4 ATTRIBUTE_UNUSED, __m256 i5 ATTRIBUTE_UNUSED, __m256 i6 ATTRIBUTE_UNUSED, __m256 i7 ATTRIBUTE_UNUSED, __m256 i8 ATTRIBUTE_UNUSED, __m256 i9 ATTRIBUTE_UNUSED, __m256 i10 ATTRIBUTE_UNUSED, __m256 i11 ATTRIBUTE_UNUSED, __m256 i12 ATTRIBUTE_UNUSED, __m256 i13 ATTRIBUTE_UNUSED, __m256 i14 ATTRIBUTE_UNUSED, __m256 i15 ATTRIBUTE_UNUSED, __m256 i16 ATTRIBUTE_UNUSED, __m256 i17 ATTRIBUTE_UNUSED, __m256 i18 ATTRIBUTE_UNUSED, __m256 i19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m256);
  compare (values.i1, i1, __m256);
  compare (values.i2, i2, __m256);
  compare (values.i3, i3, __m256);
  compare (values.i4, i4, __m256);
  compare (values.i5, i5, __m256);
  compare (values.i6, i6, __m256);
  compare (values.i7, i7, __m256);
  compare (values.i8, i8, __m256);
  compare (values.i9, i9, __m256);
  compare (values.i10, i10, __m256);
  compare (values.i11, i11, __m256);
  compare (values.i12, i12, __m256);
  compare (values.i13, i13, __m256);
  compare (values.i14, i14, __m256);
  compare (values.i15, i15, __m256);
  compare (values.i16, i16, __m256);
  compare (values.i17, i17, __m256);
  compare (values.i18, i18, __m256);
  compare (values.i19, i19, __m256);
}

void
fun_check_passing_m256_20_regs (__m256 i0 ATTRIBUTE_UNUSED, __m256 i1 ATTRIBUTE_UNUSED, __m256 i2 ATTRIBUTE_UNUSED, __m256 i3 ATTRIBUTE_UNUSED, __m256 i4 ATTRIBUTE_UNUSED, __m256 i5 ATTRIBUTE_UNUSED, __m256 i6 ATTRIBUTE_UNUSED, __m256 i7 ATTRIBUTE_UNUSED, __m256 i8 ATTRIBUTE_UNUSED, __m256 i9 ATTRIBUTE_UNUSED, __m256 i10 ATTRIBUTE_UNUSED, __m256 i11 ATTRIBUTE_UNUSED, __m256 i12 ATTRIBUTE_UNUSED, __m256 i13 ATTRIBUTE_UNUSED, __m256 i14 ATTRIBUTE_UNUSED, __m256 i15 ATTRIBUTE_UNUSED, __m256 i16 ATTRIBUTE_UNUSED, __m256 i17 ATTRIBUTE_UNUSED, __m256 i18 ATTRIBUTE_UNUSED, __m256 i19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m256_arguments;
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

#define def_check_passing20(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, _i18, _i19, _func1, _func2, TYPE) \
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
  WRAP_CALL(_func1) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, _i18, _i19); \
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
  WRAP_CALL(_func2) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, _i18, _i19);

void
test_m256_on_stack ()
{
  __m256 x[8];
  int i;
  for (i = 0; i < 8; i++)
    x[i] = (__m256){32+i, 0, 0, 0, 0, 0, 0, 0};
  pass = "m256-8";
  def_check_passing8(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], fun_check_passing_m256_8_values, fun_check_passing_m256_8_regs, _m256);
}

void
test_too_many_m256 ()
{
  __m256 x[20];
  int i;
  for (i = 0; i < 20; i++)
    x[i] = (__m256){32+i, 0, 0, 0, 0, 0, 0, 0};
  pass = "m256-20";
  def_check_passing20(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16], x[17], x[18], x[19], fun_check_passing_m256_20_values, fun_check_passing_m256_20_regs, _m256);
}

static void
avx_test (void)
{
  test_m256_on_stack ();
  test_too_many_m256 ();
  if (failed)
    abort ();
}
