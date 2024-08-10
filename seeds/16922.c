#include <stdio.h>
#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

/* Get __m64 and __m128. */
#include <xmmintrin.h>

typedef unsigned long ulong;
typedef long double ldouble;

/* These defines determines what part of the test should be run.  When
   GCC implements these parts, the defines should be uncommented to
   enable testing.  */

/* Scalar type __int128.  */
/* #define CHECK_INT128 */

/* Scalar type long double.  */
#define CHECK_LONG_DOUBLE

/* Scalar type __float128.  */
/* #define CHECK_FLOAT128 */

/* Scalar types __m64 and __m128.  */
#define CHECK_M64_M128

/* Returning of complex type.  */
#define CHECK_COMPLEX

/* Structs with size >= 16.  */
#define CHECK_LARGER_STRUCTS

/* Checks for passing floats and doubles.  */
#define CHECK_FLOAT_DOUBLE_PASSING

/* Union passing with not-extremely-simple unions.  */
#define CHECK_LARGER_UNION_PASSING

/* Variable args.  */
#define CHECK_VARARGS

/* Check argument passing and returning for scalar types with sizeof = 16.  */
/* TODO: Implement these tests. Don't activate them for now.  */
#define CHECK_LARGE_SCALAR_PASSING

/* Defines for sizing and alignment.  */

#define TYPE_SIZE_CHAR         1
#define TYPE_SIZE_SHORT        2
#define TYPE_SIZE_INT          4
#define TYPE_SIZE_LONG         8
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_INT128       16
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  16
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_M64          8
#define TYPE_SIZE_M128         16
#define TYPE_SIZE_ENUM         4
#define TYPE_SIZE_POINTER      8

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#define TYPE_ALIGN_LONG        8
#define TYPE_ALIGN_LONG_LONG   8
#define TYPE_ALIGN_INT128      16
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      8
#define TYPE_ALIGN_LONG_DOUBLE 16
#define TYPE_ALIGN_FLOAT128    16
#define TYPE_ALIGN_M64         8
#define TYPE_ALIGN_M128        16
#define TYPE_ALIGN_ENUM        4
#define TYPE_ALIGN_POINTER     8

/* These defines control the building of the list of types to check. There
   is a string identifying the type (with a comma after), a size of the type
   (also with a comma and an integer for adding to the total amount of types)
   and an alignment of the type (which is currently not really needed since
   the abi specifies that alignof == sizeof for all scalar types).  */
#ifdef CHECK_INT128
#define CI128_STR "__int128",
#define CI128_SIZ TYPE_SIZE_INT128,
#define CI128_ALI TYPE_ALIGN_INT128,
#define CI128_RET "???",
#else
#define CI128_STR
#define CI128_SIZ
#define CI128_ALI
#define CI128_RET
#endif
#ifdef CHECK_LONG_DOUBLE
#define CLD_STR "long double",
#define CLD_SIZ TYPE_SIZE_LONG_DOUBLE,
#define CLD_ALI TYPE_ALIGN_LONG_DOUBLE,
#define CLD_RET "x87_regs[0]._ldouble",
#else
#define CLD_STR
#define CLD_SIZ
#define CLD_ALI
#define CLD_RET
#endif
#ifdef CHECK_FLOAT128
#define CF128_STR "__float128",
#define CF128_SIZ TYPE_SIZE_FLOAT128,
#define CF128_ALI TYPE_ALIGN_FLOAT128, 
#define CF128_RET "???",
#else
#define CF128_STR
#define CF128_SIZ
#define CF128_ALI
#define CF128_RET
#endif
#ifdef CHECK_M64_M128
#define CMM_STR "__m64", "__m128",
#define CMM_SIZ TYPE_SIZE_M64, TYPE_SIZE_M128,
#define CMM_ALI TYPE_ALIGN_M64, TYPE_ALIGN_M128,
#define CMM_RET "???", "???",
#else
#define CMM_STR
#define CMM_SIZ
#define CMM_ALI
#define CMM_RET
#endif

/* Used in size and alignment tests.  */
enum dummytype { enumtype };

extern void abort (void);

/* Assertion macro.  */
#define assert(test) if (!(test)) abort()

#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define ATTRIBUTE_UNUSED
#endif

#ifdef __GNUC__
#define PACKED __attribute__((__packed__))
#else
#warning Some tests will fail due to missing __packed__ support
#define PACKED
#endif

#endif /* DEFINED_DEFINES_H */
#ifndef MACROS_H

#define check_size(_t, _size) assert(sizeof(_t) == (_size))

#define check_align(_t, _align) assert(__alignof__(_t) == (_align))

#define check_align_lv(_t, _align) assert(__alignof__(_t) == (_align) \
					  && (((unsigned long)&(_t)) & ((_align) - 1) ) == 0)

#define check_basic_struct_size_and_align(_type, _size, _align) { \
  struct _str { _type dummy; } _t; \
  check_size(_t, _size); \
  check_align_lv(_t, _align); \
}

#define check_array_size_and_align(_type, _size, _align) { \
  _type _a[1]; _type _b[2]; _type _c[16]; \
  struct _str { _type _a[1]; } _s; \
  check_align_lv(_a[0], _align); \
  check_size(_a, _size); \
  check_size(_b, (_size*2)); \
  check_size(_c, (_size*16)); \
  check_size(_s, _size); \
  check_align_lv(_s._a[0], _align); \
}

#define check_basic_union_size_and_align(_type, _size, _align) { \
  union _union { _type dummy; } _u; \
  check_size(_u, _size); \
  check_align_lv(_u, _align); \
}

#define run_signed_tests2(_function, _arg1, _arg2) \
  _function(_arg1, _arg2); \
  _function(signed _arg1, _arg2); \
  _function(unsigned _arg1, _arg2);

#define run_signed_tests3(_function, _arg1, _arg2, _arg3) \
  _function(_arg1, _arg2, _arg3); \
  _function(signed _arg1, _arg2, _arg3); \
  _function(unsigned _arg1, _arg2, _arg3);

/* Check size of a struct and a union of three types.  */

#define check_struct_and_union3(type1, type2, type3, struct_size, align_size) \
{ \
  struct _str { type1 t1; type2 t2; type3 t3; } _t; \
  union _uni { type1 t1; type2 t2; type3 t3; } _u; \
  check_size(_t, struct_size); \
  check_size(_u, align_size); \
}

#endif // MACROS_H
#ifndef INCLUDED_ARGS_H
#define INCLUDED_ARGS_H

#include <string.h>

/* This defines the calling sequences for integers and floats.  */
#define I0 rdi
#define I1 rsi
#define I2 rdx
#define I3 rcx
#define I4 r8
#define I5 r9
#define F0 xmm0
#define F1 xmm1
#define F2 xmm2
#define F3 xmm3
#define F4 xmm4
#define F5 xmm5
#define F6 xmm6
#define F7 xmm7

typedef union {
  float _float[4];
  double _double[2];
  long _long[2];
  int _int[4];
  unsigned long _ulong[2];
#ifdef CHECK_M64_M128
  __m64 _m64[2];
  __m128 _m128[1];
#endif
} XMM_T;

typedef union {
  float _float;
  double _double;
  ldouble _ldouble;
  ulong _ulong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
XMM_T xmm_regs[16];
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
  ldouble st0, st1, st2, st3, st4, st5, st6, st7;
  XMM_T xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9,
        xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
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
  memset (xmm_regs, 0, sizeof (xmm_regs)); \
  memset (x87_regs, 0, sizeof (x87_regs));

/* Clear both hardware and register structs for integers.  */
#define clear_int_registers \
  clear_struct_registers \
  clear_int_hardware_registers

/* TODO: Do the checking.  */
#define check_f_arguments(T) do { \
  assert (num_fregs <= 0 || fregs.xmm0._ ## T [0] == xmm_regs[0]._ ## T [0]); \
  assert (num_fregs <= 1 || fregs.xmm1._ ## T [0] == xmm_regs[1]._ ## T [0]); \
  assert (num_fregs <= 2 || fregs.xmm2._ ## T [0] == xmm_regs[2]._ ## T [0]); \
  assert (num_fregs <= 3 || fregs.xmm3._ ## T [0] == xmm_regs[3]._ ## T [0]); \
  assert (num_fregs <= 4 || fregs.xmm4._ ## T [0] == xmm_regs[4]._ ## T [0]); \
  assert (num_fregs <= 5 || fregs.xmm5._ ## T [0] == xmm_regs[5]._ ## T [0]); \
  assert (num_fregs <= 6 || fregs.xmm6._ ## T [0] == xmm_regs[6]._ ## T [0]); \
  assert (num_fregs <= 7 || fregs.xmm7._ ## T [0] == xmm_regs[7]._ ## T [0]); \
  } while (0)

#define check_float_arguments check_f_arguments(float)
#define check_double_arguments check_f_arguments(double)

#define check_vector_arguments(T,O) do { \
  assert (num_fregs <= 0 \
	  || memcmp (((char *) &fregs.xmm0) + (O), \
		     &xmm_regs[0], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 1 \
	  || memcmp (((char *) &fregs.xmm1) + (O), \
		     &xmm_regs[1], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 2 \
	  || memcmp (((char *) &fregs.xmm2) + (O), \
		     &xmm_regs[2], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 3 \
	  || memcmp (((char *) &fregs.xmm3) + (O), \
		     &xmm_regs[3], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 4 \
	  || memcmp (((char *) &fregs.xmm4) + (O), \
		     &xmm_regs[4], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 5 \
	  || memcmp (((char *) &fregs.xmm5) + (O), \
		     &xmm_regs[5], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 6 \
	  || memcmp (((char *) &fregs.xmm6) + (O), \
		     &xmm_regs[6], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 7 \
	  || memcmp (((char *) &fregs.xmm7) + (O), \
		     &xmm_regs[7], \
		     sizeof (__ ## T) - (O)) == 0); \
  } while (0)

#define check_m64_arguments check_vector_arguments(m64, 0)
#define check_m128_arguments check_vector_arguments(m128, 0)

/* ldoubles are not passed in registers */
#define check_ldouble_arguments

/* TODO: Do the clearing.  */
#define clear_float_hardware_registers
#define clear_x87_hardware_registers

#define clear_float_registers \
  clear_struct_registers \
  clear_float_hardware_registers

#define clear_x87_registers \
  clear_struct_registers \
  clear_x87_hardware_registers


#endif /* INCLUDED_ARGS_H  */

struct IntegerRegisters iregs;
struct FloatRegisters fregs;
unsigned int num_iregs, num_fregs;

/* This struct holds values for argument checking.  */
struct 
{
  XMM_T i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
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
fun_check_passing_m64_8_values (__m64 i0 ATTRIBUTE_UNUSED, __m64 i1 ATTRIBUTE_UNUSED, __m64 i2 ATTRIBUTE_UNUSED, __m64 i3 ATTRIBUTE_UNUSED, __m64 i4 ATTRIBUTE_UNUSED, __m64 i5 ATTRIBUTE_UNUSED, __m64 i6 ATTRIBUTE_UNUSED, __m64 i7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m64);
  compare (values.i1, i1, __m64);
  compare (values.i2, i2, __m64);
  compare (values.i3, i3, __m64);
  compare (values.i4, i4, __m64);
  compare (values.i5, i5, __m64);
  compare (values.i6, i6, __m64);
  compare (values.i7, i7, __m64);
}

void
fun_check_passing_m64_8_regs (__m64 i0 ATTRIBUTE_UNUSED, __m64 i1 ATTRIBUTE_UNUSED, __m64 i2 ATTRIBUTE_UNUSED, __m64 i3 ATTRIBUTE_UNUSED, __m64 i4 ATTRIBUTE_UNUSED, __m64 i5 ATTRIBUTE_UNUSED, __m64 i6 ATTRIBUTE_UNUSED, __m64 i7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m64_arguments;
}

void
fun_check_passing_m64_20_values (__m64 i0 ATTRIBUTE_UNUSED, __m64 i1 ATTRIBUTE_UNUSED, __m64 i2 ATTRIBUTE_UNUSED, __m64 i3 ATTRIBUTE_UNUSED, __m64 i4 ATTRIBUTE_UNUSED, __m64 i5 ATTRIBUTE_UNUSED, __m64 i6 ATTRIBUTE_UNUSED, __m64 i7 ATTRIBUTE_UNUSED, __m64 i8 ATTRIBUTE_UNUSED, __m64 i9 ATTRIBUTE_UNUSED, __m64 i10 ATTRIBUTE_UNUSED, __m64 i11 ATTRIBUTE_UNUSED, __m64 i12 ATTRIBUTE_UNUSED, __m64 i13 ATTRIBUTE_UNUSED, __m64 i14 ATTRIBUTE_UNUSED, __m64 i15 ATTRIBUTE_UNUSED, __m64 i16 ATTRIBUTE_UNUSED, __m64 i17 ATTRIBUTE_UNUSED, __m64 i18 ATTRIBUTE_UNUSED, __m64 i19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0 , i0, __m64);
  compare (values.i1 , i1, __m64);
  compare (values.i2 , i2, __m64);
  compare (values.i3 , i3, __m64);
  compare (values.i4 , i4, __m64);
  compare (values.i5 , i5, __m64);
  compare (values.i6 , i6, __m64);
  compare (values.i7 , i7, __m64);
  compare (values.i8 , i8, __m64);
  compare (values.i9 , i9, __m64);
  compare (values.i10 , i10, __m64);
  compare (values.i11 , i11, __m64);
  compare (values.i12 , i12, __m64);
  compare (values.i13 , i13, __m64);
  compare (values.i14 , i14, __m64);
  compare (values.i15 , i15, __m64);
  compare (values.i16 , i16, __m64);
  compare (values.i17 , i17, __m64);
  compare (values.i18 , i18, __m64);
  compare (values.i19 , i19, __m64);
}

void
fun_check_passing_m64_20_regs (__m64 i0 ATTRIBUTE_UNUSED, __m64 i1 ATTRIBUTE_UNUSED, __m64 i2 ATTRIBUTE_UNUSED, __m64 i3 ATTRIBUTE_UNUSED, __m64 i4 ATTRIBUTE_UNUSED, __m64 i5 ATTRIBUTE_UNUSED, __m64 i6 ATTRIBUTE_UNUSED, __m64 i7 ATTRIBUTE_UNUSED, __m64 i8 ATTRIBUTE_UNUSED, __m64 i9 ATTRIBUTE_UNUSED, __m64 i10 ATTRIBUTE_UNUSED, __m64 i11 ATTRIBUTE_UNUSED, __m64 i12 ATTRIBUTE_UNUSED, __m64 i13 ATTRIBUTE_UNUSED, __m64 i14 ATTRIBUTE_UNUSED, __m64 i15 ATTRIBUTE_UNUSED, __m64 i16 ATTRIBUTE_UNUSED, __m64 i17 ATTRIBUTE_UNUSED, __m64 i18 ATTRIBUTE_UNUSED, __m64 i19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m64_arguments;
}

void
fun_check_passing_m128_8_values (__m128 i0 ATTRIBUTE_UNUSED, __m128 i1 ATTRIBUTE_UNUSED, __m128 i2 ATTRIBUTE_UNUSED, __m128 i3 ATTRIBUTE_UNUSED, __m128 i4 ATTRIBUTE_UNUSED, __m128 i5 ATTRIBUTE_UNUSED, __m128 i6 ATTRIBUTE_UNUSED, __m128 i7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0, i0, __m128);
  compare (values.i1, i1, __m128);
  compare (values.i2, i2, __m128);
  compare (values.i3, i3, __m128);
  compare (values.i4, i4, __m128);
  compare (values.i5, i5, __m128);
  compare (values.i6, i6, __m128);
  compare (values.i7, i7, __m128);
}

void
fun_check_passing_m128_8_regs (__m128 i0 ATTRIBUTE_UNUSED, __m128 i1 ATTRIBUTE_UNUSED, __m128 i2 ATTRIBUTE_UNUSED, __m128 i3 ATTRIBUTE_UNUSED, __m128 i4 ATTRIBUTE_UNUSED, __m128 i5 ATTRIBUTE_UNUSED, __m128 i6 ATTRIBUTE_UNUSED, __m128 i7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m128_arguments;
}

void
fun_check_passing_m128_20_values (__m128 i0 ATTRIBUTE_UNUSED, __m128 i1 ATTRIBUTE_UNUSED, __m128 i2 ATTRIBUTE_UNUSED, __m128 i3 ATTRIBUTE_UNUSED, __m128 i4 ATTRIBUTE_UNUSED, __m128 i5 ATTRIBUTE_UNUSED, __m128 i6 ATTRIBUTE_UNUSED, __m128 i7 ATTRIBUTE_UNUSED, __m128 i8 ATTRIBUTE_UNUSED, __m128 i9 ATTRIBUTE_UNUSED, __m128 i10 ATTRIBUTE_UNUSED, __m128 i11 ATTRIBUTE_UNUSED, __m128 i12 ATTRIBUTE_UNUSED, __m128 i13 ATTRIBUTE_UNUSED, __m128 i14 ATTRIBUTE_UNUSED, __m128 i15 ATTRIBUTE_UNUSED, __m128 i16 ATTRIBUTE_UNUSED, __m128 i17 ATTRIBUTE_UNUSED, __m128 i18 ATTRIBUTE_UNUSED, __m128 i19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  compare (values.i0 , i0, __m128);
  compare (values.i1 , i1, __m128);
  compare (values.i2 , i2, __m128);
  compare (values.i3 , i3, __m128);
  compare (values.i4 , i4, __m128);
  compare (values.i5 , i5, __m128);
  compare (values.i6 , i6, __m128);
  compare (values.i7 , i7, __m128);
  compare (values.i8 , i8, __m128);
  compare (values.i9 , i9, __m128);
  compare (values.i10 , i10, __m128);
  compare (values.i11 , i11, __m128);
  compare (values.i12 , i12, __m128);
  compare (values.i13 , i13, __m128);
  compare (values.i14 , i14, __m128);
  compare (values.i15 , i15, __m128);
  compare (values.i16 , i16, __m128);
  compare (values.i17 , i17, __m128);
  compare (values.i18 , i18, __m128);
  compare (values.i19 , i19, __m128);
}

void
fun_check_passing_m128_20_regs (__m128 i0 ATTRIBUTE_UNUSED, __m128 i1 ATTRIBUTE_UNUSED, __m128 i2 ATTRIBUTE_UNUSED, __m128 i3 ATTRIBUTE_UNUSED, __m128 i4 ATTRIBUTE_UNUSED, __m128 i5 ATTRIBUTE_UNUSED, __m128 i6 ATTRIBUTE_UNUSED, __m128 i7 ATTRIBUTE_UNUSED, __m128 i8 ATTRIBUTE_UNUSED, __m128 i9 ATTRIBUTE_UNUSED, __m128 i10 ATTRIBUTE_UNUSED, __m128 i11 ATTRIBUTE_UNUSED, __m128 i12 ATTRIBUTE_UNUSED, __m128 i13 ATTRIBUTE_UNUSED, __m128 i14 ATTRIBUTE_UNUSED, __m128 i15 ATTRIBUTE_UNUSED, __m128 i16 ATTRIBUTE_UNUSED, __m128 i17 ATTRIBUTE_UNUSED, __m128 i18 ATTRIBUTE_UNUSED, __m128 i19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m128_arguments;
}


#define def_check_int_passing8(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _func1, _func2, TYPE) \
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
  clear_float_registers; \
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

#define def_check_int_passing20(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _i12, _i13, _i14, _i15, _i16, _i17, _i18, _i19, _func1, _func2, TYPE) \
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
  clear_float_registers; \
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
test_m64_on_stack ()
{
  __m64 x[8];
  int i;
  for (i = 0; i < 8; i++)
    x[i] = (__m64){32+i, 0};
  pass = "m64-8";
  def_check_int_passing8(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], fun_check_passing_m64_8_values, fun_check_passing_m64_8_regs, _m64);
}

void
test_too_many_m64 ()
{
  __m64 x[20];
  int i;
  for (i = 0; i < 20; i++)
    x[i] = (__m64){32+i, 0};
  pass = "m64-20";
  def_check_int_passing20(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16], x[17], x[18], x[19], fun_check_passing_m64_20_values, fun_check_passing_m64_20_regs, _m64);
}

void
test_m128_on_stack ()
{
  __m128 x[8];
  int i;
  for (i = 0; i < 8; i++)
    x[i] = (__m128){32+i, 0, 0, 0};
  pass = "m128-8";
  def_check_int_passing8(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], fun_check_passing_m128_8_values, fun_check_passing_m128_8_regs, _m128);
}

void
test_too_many_m128 ()
{
  __m128 x[20];
  int i;
  for (i = 0; i < 20; i++)
    x[i] = (__m128){32+i, 0, 0, 0};
  pass = "m128-20";
  def_check_int_passing20(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16], x[17], x[18], x[19], fun_check_passing_m128_20_values, fun_check_passing_m128_20_regs, _m128);
}

int
main (void)
{
  test_m64_on_stack ();
  test_too_many_m64 ();
  test_m128_on_stack ();
  test_too_many_m128 ();
  if (failed)
    abort ();
  return 0;
}