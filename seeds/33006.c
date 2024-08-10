/* This tests returning of structures.  */

#include <stdio.h>
#define AVX512VL(ebx) (ebx & bit_AVX512VL)
#define XSTATE_MASK (XSTATE_SSE | XSTATE_OPMASK)
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
#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

/* Get __m64 and __m128. */
#include <immintrin.h>

typedef unsigned long long ulonglong;
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
#ifdef __ILP32__
# define TYPE_SIZE_LONG        4
#else
# define TYPE_SIZE_LONG        8
#endif
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_INT128       16
#define TYPE_SIZE_FLOAT16      2
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  16
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_M64          8
#define TYPE_SIZE_M128         16
#define TYPE_SIZE_ENUM         4
#ifdef __ILP32__
# define TYPE_SIZE_POINTER     4
#else
# define TYPE_SIZE_POINTER     8
#endif

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#ifdef __ILP32__
# define TYPE_ALIGN_LONG       4
#else
# define TYPE_ALIGN_LONG       8
#endif
#define TYPE_ALIGN_LONG_LONG   8
#define TYPE_ALIGN_INT128      16
#define TYPE_ALIGN_FLOAT16     2
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      8
#define TYPE_ALIGN_LONG_DOUBLE 16
#define TYPE_ALIGN_FLOAT128    16
#define TYPE_ALIGN_M64         8
#define TYPE_ALIGN_M128        16
#define TYPE_ALIGN_ENUM        4
#ifdef __ILP32__
# define TYPE_ALIGN_POINTER    4
#else
# define TYPE_ALIGN_POINTER    8
#endif

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
  _Float16 __Float16[8];
  float _float[4];
  double _double[2];
  long long _longlong[2];
  int _int[4];
  ulonglong _ulonglong[2];
#ifdef CHECK_M64_M128
  __m64 _m64[2];
  __m128 _m128[1];
  __m128h _m128h[1];
#endif
} XMM_T;

typedef union {
  _Float16 __Float16;
  float _float;
  double _double;
  ldouble _ldouble;
  ulonglong _ulonglong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
XMM_T xmm_regs[16];
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

#define check_float16_arguments check_f_arguments(_Float16)
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

int current_test;
int num_failed = 0;

#undef assert
#define assert(test) do { if (!(test)) {fprintf (stderr, "failed in test %d\n", current_test); num_failed++; } } while (0)

#define xmm0h xmm_regs[0].__Float16
#define xmm1h xmm_regs[1].__Float16
#define xmm0f xmm_regs[0]._float
#define xmm0d xmm_regs[0]._double
#define xmm1f xmm_regs[1]._float
#define xmm1d xmm_regs[1]._double

typedef enum {
  INT = 0,
  SSE_H,
  SSE_F,
  SSE_D,
  X87,
  MEM,
  INT_SSE,
  SSE_INT,
  SSE_F_V,
  SSE_F_H,
  SSE_F_H8
} Type;

/* Structures which should be returned in INTEGER.  */
#define D(I,MEMBERS,B) struct S_ ## I { MEMBERS ; }; Type class_ ## I = INT; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s; memset (&s, 0, sizeof(s)); B; return s; }

D(1,char m1, s.m1=42)
D(2,short m1, s.m1=42)
D(3,int m1, s.m1=42)
D(4,long m1, s.m1=42)
D(5,long long m1, s.m1=42)
D(6,char m1;short s, s.m1=42)
D(7,char m1;int i, s.m1=42)
D(8,char m1; long l, s.m1=42)
D(9,char m1; long long l, s.m1=42)
D(10,char m1[16], s.m1[0]=42)
D(11,short m1[8], s.m1[0]=42)
D(12,int m1[4], s.m1[0]=42)
D(13,long m1[2], s.m1[0]=42)
D(14,long long m1[2], s.m1[0]=42)

#undef D

/* Structures which should be returned in SSE.  */
#define D(I,MEMBERS,C,B) struct S_ ## I { MEMBERS ; }; Type class_ ## I = C; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s; memset (&s, 0, sizeof(s)); B; return s; }

D(100,float f,SSE_F, s.f=42)
D(101,double d,SSE_D, s.d=42)
D(102,float f;float f2,SSE_F, s.f=42)
D(103,float f;double d,SSE_F, s.f=42)
D(104,double d; float f,SSE_D, s.d=42)
D(105,double d; double d2,SSE_D, s.d=42)
D(106,float f[2],SSE_F, s.f[0]=42)
D(107,float f[3],SSE_F, s.f[0]=42)
D(108,float f[4],SSE_F, s.f[0]=42)
D(109,double d[2],SSE_D, s.d[0]=42)
D(110,float f[2]; double d,SSE_F, s.f[0]=42)
D(111,double d;float f[2],SSE_D, s.d=42)

D(120,_Float16 f,SSE_H, s.f=42)
D(121,_Float16 f;_Float16 f2,SSE_H, s.f=42)
D(122,_Float16 f;float d,SSE_H, s.f=42)
D(123,_Float16 f;double d,SSE_H, s.f=42)
D(124,double d; _Float16 f,SSE_D, s.d=42)
D(125,_Float16 f[2],SSE_H, s.f[0]=42)
D(126,_Float16 f[3],SSE_H, s.f[0]=42)
D(127,_Float16 f[4],SSE_H, s.f[0]=42)
D(128,_Float16 f[2]; double d,SSE_H, s.f[0]=42)
D(129,double d;_Float16 f[2],SSE_D, s.d=42)

#undef D

/* Structures which should be returned on x87 stack.  */
#define D(I,MEMBERS) struct S_ ## I { MEMBERS ; }; Type class_ ## I = X87; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s = { 42 }; return s; }

/* The only struct containing a long double, which is returned in
   registers at all, is the singleton struct.  All others are too large.
   This includes a struct containing complex long double, which is passed
   in memory, although a complex long double type itself is returned in
   two registers.  */
D(200,long double ld)

#undef D

/* Structures which should be returned in INT (low) and SSE (high).  */
#define D(I,MEMBERS) struct S_ ## I { MEMBERS ; }; Type class_ ## I = INT_SSE; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s = { 42,43 }; return s; }

D(300,char m1; float m2)
D(301,char m1; double m2)
D(302,short m1; float m2)
D(303,short m1; double m2)
D(304,int m1; float m2)
D(305,int m1; double m2)
D(306,long long m1; float m2)
D(307,long long m1; double m2)

D(310,char m1; _Float16 m2)
D(311,short m1; _Float16 m2)
D(312,int m1; _Float16 m2)
D(313,long long m1; _Float16 m2)

#undef D

void check_300 (void)
{
  XMM_T x;
  x._ulonglong[0] = rax;
  switch (current_test) {
    case 300: assert ((rax & 0xff) == 42 && x._float[1] == 43); break;
    case 301: assert ((rax & 0xff) == 42 && xmm0d[0] == 43); break;
    case 302: assert ((rax & 0xffff) == 42 && x._float[1] == 43); break;
    case 303: assert ((rax & 0xffff) == 42 && xmm0d[0] == 43); break;
    case 304: assert ((rax & 0xffffffff) == 42 && x._float[1] == 43); break;
    case 305: assert ((rax & 0xffffffff) == 42 && xmm0d[0] == 43); break;
    case 306: assert (rax == 42 && xmm0f[0] == 43); break;
    case 307: assert (rax == 42 && xmm0d[0] == 43); break;
    case 310: assert ((rax & 0xff) == 42 && x.__Float16[1] == 43); break;
    case 311: assert ((rax & 0xffff) == 42 && x.__Float16[1] == 43); break;
    case 312: assert ((rax & 0xffffffff) == 42 && x.__Float16[2] == 43); break;
    case 313: assert (rax == 42 && xmm0h[0] == 43); break;

    default: assert (0); break;
  }
}

/* Structures which should be returned in SSE (low) and INT (high).  */
#define D(I,MEMBERS,B) struct S_ ## I { MEMBERS ; }; Type class_ ## I = SSE_INT; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s; memset (&s, 0, sizeof(s));  B; return s; }

D(400,float f[2];char c, s.f[0]=42; s.c=43)
D(401,double d;char c, s.d=42; s.c=43)

D(402,_Float16 f[4];char c, s.f[0]=42; s.c=43)

#undef D

void check_400 (void)
{
  switch (current_test) {
    case 400: assert (xmm0f[0] == 42 && (rax & 0xff) == 43); break;
    case 401: assert (xmm0d[0] == 42 && (rax & 0xff) == 43); break;
    case 402: assert (xmm0h[0] == 42 && (rax & 0xff) == 43); break;

    default: assert (0); break;
  }
}

/* Structures which should be returned in MEM.  */
void *struct_addr;
#define D(I,MEMBERS) struct S_ ## I { MEMBERS ; }; Type class_ ## I = MEM; \
struct S_ ## I f_ ## I (void) { union {unsigned char c; struct S_ ## I s;} u; memset (&u.s, 0, sizeof(u.s)); u.c = 42; return u.s; }

/* Too large.  */
D(500,char m1[17])
D(501,short m1[9])
D(502,int m1[5])
D(503,long m1[3])
D(504,short m1[8];char c)
D(505,char m1[1];int i[4])
D(506,float m1[5])
D(507,double m1[3])
D(508,char m1[1];float f[4])
D(509,char m1[1];double d[2])
D(510,__complex long double m1[1])

/* Too large due to padding.  */
D(520,char m1[1];int i;char c2; int i2; char c3)

/* Unnaturally aligned members.  */
D(530,short m1[1];int i PACKED)

D(540,_Float16 m1[10])
D(541,char m1[1];_Float16 f[8])

#undef D


/* Special tests.  */
#define D(I,MEMBERS,C,B) struct S_ ## I { MEMBERS ; }; Type class_ ## I = C; \
struct S_ ## I f_ ## I (void) { struct S_ ## I s; B; return s; }
D(600,float f[4], SSE_F_V, s.f[0] = s.f[1] = s.f[2] = s.f[3] = 42)
D(601,_Float16 f[4], SSE_F_H, s.f[0] = s.f[1] = s.f[2] = s.f[3] = 42)
D(602,_Float16 f[8], SSE_F_H8,
  s.f[0] = s.f[1] = s.f[2] = s.f[3] = s.f[4] = s.f[5] = s.f[6] = s.f[7] = 42)
#undef D

void clear_all (void)
{
  clear_int_registers;
  clear_float_registers;
  clear_x87_registers;
}

void check_all (Type class, unsigned long size)
{
  switch (class) {
    case INT: if (size < 8) rax &= ~0UL >> (64-8*size); assert (rax == 42); break;
    case SSE_H: assert (xmm0h[0] == 42); break;
    case SSE_F: assert (xmm0f[0] == 42); break;
    case SSE_D: assert (xmm0d[0] == 42); break;
    case SSE_F_V: assert (xmm0f[0] == 42 && xmm0f[1]==42 && xmm1f[0] == 42 && xmm1f[1] == 42); break;
    case SSE_F_H: assert (xmm0h[0] == 42 && xmm0h[1]==42 && xmm0h[2] == 42 && xmm0h[3] == 42); break;
    case SSE_F_H8: assert (xmm0h[0] == 42 && xmm0h[1]==42 && xmm0h[2] == 42 && xmm0h[3] == 42
			   && xmm1h[0] == 42 && xmm1h[1]==42 && xmm1h[2] == 42 && xmm1h[3] == 42); break;
    case X87: assert (x87_regs[0]._ldouble == 42); break;
    case INT_SSE: check_300(); break;
    case SSE_INT: check_400(); break;
    /* Ideally we would like to check that rax == struct_addr.
       Unfortunately the address of the target struct escapes (for setting
       struct_addr), so the return struct is a temporary one whose address
       is given to the f_* functions, otherwise a conforming program
       could notice the struct changing already before the function returns.
       This temporary struct could be anywhere.  For GCC it will be on
       stack, but no one is forbidding that it could be a static variable
       if there's no threading or proper locking.  Nobody in his right mind
       will not use the stack for that.  */
    case MEM: assert (*(unsigned char*)struct_addr == 42 && rdi == rax); break;
  }
}

#define D(I) { struct S_ ## I s; current_test = I; struct_addr = (void*)&s; \
  clear_all(); \
  s = WRAP_RET(f_ ## I) (); \
  check_all(class_ ## I, sizeof(s)); \
}

static void
do_test (void)
{
  D(1) D(2) D(3) D(4) D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14)
  
  D(100) D(101) D(102) D(103) D(104) D(105) D(106) D(107) D(108) D(109) D(110)
  D(111)
  
  D(120) D(121) D(122) D(123) D(124) D(125) D(126) D(127) D(128) D(129)

  D(200)

  D(300) D(301) D(302) D(303) D(304) D(305) D(306) D(307)
  D(310) D(311) D(312) D(313)

  D(400) D(401) D(402)

  D(500) D(501) D(502) D(503) D(504) D(505) D(506) D(507) D(508) D(509)
  D(520)
  D(530)

  D(540) D(541)

  D(600) D(601) D(602)
  if (num_failed)
    abort ();
}
#undef D
