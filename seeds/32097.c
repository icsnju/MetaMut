/* This tests passing of structs. */

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
#include <complex.h>

struct IntegerRegisters iregs;
struct FloatRegisters fregs;
unsigned int num_iregs, num_fregs;

struct int_struct
{
  int i;
};

struct long_struct
{
  long l;
};

struct long2_struct
{
  long l1, l2;
};

struct long3_struct
{
  long l1, l2, l3;
};


/* Check that the struct is passed as the individual members in iregs.  */
void
check_struct_passing1 (struct int_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_struct_passing2 (struct long_struct ls ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_struct_passing3 (struct long2_struct ls ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_struct_passing4 (struct long3_struct ls ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ls.l1 == rsp+8);
  assert ((unsigned long)&ls.l2 == rsp+16);
  assert ((unsigned long)&ls.l3 == rsp+24);
}

#ifdef CHECK_M64_M128
struct m128_struct
{
  __m128 x;
};

struct m128_2_struct
{
  __m128 x1, x2;
};

/* Check that the struct is passed as the individual members in fregs.  */
void
check_struct_passing5 (struct m128_struct ms1 ATTRIBUTE_UNUSED,
		       struct m128_struct ms2 ATTRIBUTE_UNUSED,
		       struct m128_struct ms3 ATTRIBUTE_UNUSED,
		       struct m128_struct ms4 ATTRIBUTE_UNUSED,
		       struct m128_struct ms5 ATTRIBUTE_UNUSED,
		       struct m128_struct ms6 ATTRIBUTE_UNUSED,
		       struct m128_struct ms7 ATTRIBUTE_UNUSED,
		       struct m128_struct ms8 ATTRIBUTE_UNUSED)
{
  check_m128_arguments;
}

void
check_struct_passing6 (struct m128_2_struct ms ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ms.x1 == rsp+8);
  assert ((unsigned long)&ms.x2 == rsp+24);
}
#endif

struct flex1_struct
{
  long i;
  long flex[];
};

struct flex2_struct
{
  long i;
  long flex[0];
};

void
check_struct_passing7 (struct flex1_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_struct_passing8 (struct flex2_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

struct complex1_struct
{
  int c;
  __complex__ float x;
};

struct complex1a_struct
{
  long l;
  float f;
};

struct complex2_struct
{
  int c;
  __complex__ float x;
  float y;
};

struct complex2a_struct
{
  long l;
  double d;
};

void
check_struct_passing9 (struct complex1_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
  check_float_arguments;
}

void
check_struct_passing10 (struct complex2_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
  check_double_arguments;
}

static struct flex1_struct f1s = { 60, { } };
static struct flex2_struct f2s = { 61, { } };

int
main (void)
{
  struct int_struct is = { 48 };
  struct long_struct ls = { 49 };
#ifdef CHECK_LARGER_STRUCTS
  struct long2_struct l2s = { 50, 51 };
  struct long3_struct l3s = { 52, 53, 54 };
#endif
#ifdef CHECK_M64_M128
  struct m128_struct m128s[8];
  struct m128_2_struct m128_2s = { 
      { 48.394, 39.3, -397.9, 3484.9 },
      { -8.394, -93.3, 7.9, 84.94 }
  };
  int i;
#endif
  struct complex1_struct c1s = { 4, ( -13.4 + 3.5*I ) };
  union
    {
      struct complex1_struct c;
      struct complex1a_struct u;
    } c1u;
  struct complex2_struct c2s = { 4, ( -13.4 + 3.5*I ), -34.5 };
  union
    {
      struct complex2_struct c;
      struct complex2a_struct u;
    } c2u;

  clear_struct_registers;
  iregs.I0 = is.i;
  num_iregs = 1;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing1)(is);

  clear_struct_registers;
  iregs.I0 = ls.l;
  num_iregs = 1;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing2)(ls);

#ifdef CHECK_LARGER_STRUCTS
  clear_struct_registers;
  iregs.I0 = l2s.l1;
  iregs.I1 = l2s.l2;
  num_iregs = 2;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing3)(l2s);
  WRAP_CALL (check_struct_passing4)(l3s);
#endif

#ifdef CHECK_M64_M128
  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      m128s[i].x = (__m128){32+i, 0, i, 0};
      (&fregs.xmm0)[i]._m128[0] = m128s[i].x;
    }
  num_fregs = 8;
  clear_float_hardware_registers;
  WRAP_CALL (check_struct_passing5)(m128s[0], m128s[1], m128s[2], m128s[3],
				    m128s[4], m128s[5], m128s[6], m128s[7]);
  WRAP_CALL (check_struct_passing6)(m128_2s);
#endif

  clear_struct_registers;
  iregs.I0 = f1s.i;
  num_iregs = 1;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing7)(f1s);

  clear_struct_registers;
  iregs.I0 = f2s.i;
  num_iregs = 1;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing8)(f2s);

  clear_struct_registers;
  c1u.c = c1s;
  iregs.I0 = c1u.u.l;
  num_iregs = 1;
  fregs.xmm0._float [0] = c1u.u.f;
  num_fregs = 1;
  clear_int_hardware_registers;
  clear_float_hardware_registers;
  WRAP_CALL (check_struct_passing9)(c1s);

  clear_struct_registers;
  c2u.c = c2s;
  iregs.I0 = c2u.u.l;
  num_iregs = 1;
  fregs.xmm0._double[0] = c2u.u.d;
  num_fregs = 1;
  clear_int_hardware_registers;
  clear_float_hardware_registers;
  WRAP_CALL (check_struct_passing10)(c2s);

  return 0;
}
