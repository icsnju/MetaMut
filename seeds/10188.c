#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

typedef unsigned long long ulonglong;
typedef long double ldouble;

/* These defines determines what part of the test should be run.  When
   GCC implements these parts, the defines should be uncommented to
   enable testing.  */

/* Scalar type long double.  */
#define CHECK_LONG_DOUBLE

/* Scalar type __float128.  */
#define CHECK_FLOAT128

/* Returning of complex type.  */
#define CHECK_COMPLEX

/* Structs with size > 8.  */
#define CHECK_LARGER_STRUCTS

/* Checks for passing floats and doubles.  */
#define CHECK_FLOAT_DOUBLE_PASSING

/* Union passing with not-extremely-simple unions.  */
#define CHECK_LARGER_UNION_PASSING

/* Variable args.  */
#define CHECK_VARARGS

/* Check argument passing and returning for scalar types with sizeof > 8.  */
#define CHECK_LARGE_SCALAR_PASSING

/* Defines for sizing and alignment.  */

#define TYPE_SIZE_CHAR         1
#define TYPE_SIZE_SHORT        2
#define TYPE_SIZE_INT          4
#define TYPE_SIZE_LONG         4
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  8
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_ENUM         4
#define TYPE_SIZE_POINTER      4

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#define TYPE_ALIGN_LONG        4
#define TYPE_ALIGN_LONG_LONG   4
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      4
#define TYPE_ALIGN_LONG_DOUBLE 4
#define TYPE_ALIGN_FLOAT128    4
#define TYPE_ALIGN_ENUM        4
#define TYPE_ALIGN_POINTER     4

/* These defines control the building of the list of types to check. There
   is a string identifying the type (with a comma after), a size of the type
   (also with a comma and an integer for adding to the total amount of types)
   and an alignment of the type (which is currently not really needed since
   the abi specifies that alignof == sizeof for all scalar types).  */
#ifdef CHECK_LONG_DOUBLE
#define CLD_STR "long double",
#define CLD_SIZ TYPE_SIZE_LONG_DOUBLE,
#define CLD_ALI TYPE_ALIGN_LONG_DOUBLE,
#define CLD_RET "???",
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

/* This defines the calling sequences for integers and floats.  */
#define I0 eax
#define I1 edx
#define I2 ecx

typedef unsigned int size_t;

extern void (*callthis)(void);
extern unsigned long eax,ebx,ecx,edx,esi,edi,esp,ebp;
extern unsigned long sret_eax;
extern volatile unsigned long volatile_var;
extern void snapshot (void);
extern void snapshot_ret (void);
extern void *iamcu_memset (void *, int, size_t);
#define WRAP_CALL(N) \
  (callthis = (void (*)()) (N), (typeof (&N)) snapshot)
#define WRAP_RET(N) \
  (callthis = (void (*)()) (N), (typeof (&N)) snapshot_ret)

/* Clear all scratch integer registers.  */
#define clear_int_hardware_registers \
  asm __volatile__ ("xor %%eax, %%eax\n\t" \
		    "xor %%edx, %%edx\n\t" \
		    "xor %%ecx, %%ecx\n\t" \
		    ::: "eax", "edx", "ecx");

/* Clear all scratch integer registers, excluding the one used to return
   aggregate.  */
#define clear_non_sret_int_hardware_registers \
  asm __volatile__ ("xor %%edx, %%edx\n\t" \
		    "xor %%ecx, %%ecx\n\t" \
		    ::: "edx", "ecx");

/* This is the list of registers available for passing arguments. Not all of
   these are used or even really available.  */
struct IntegerRegisters
{
  unsigned long eax, ebx, ecx, edx, esi, edi;
};

/* Implemented in scalarargs.c  */
extern struct IntegerRegisters iregs, iregbits;
extern unsigned int num_iregs;

#define check_int_arguments do { \
  assert (num_iregs <= 0 || (iregs.I0 & iregbits.I0) == (I0 & iregbits.I0)); \
  assert (num_iregs <= 1 || (iregs.I1 & iregbits.I1) == (I1 & iregbits.I1)); \
  assert (num_iregs <= 2 || (iregs.I2 & iregbits.I2) == (I2 & iregbits.I2)); \
  } while (0)

#define check_char_arguments check_int_arguments
#define check_short_arguments check_int_arguments
#define check_long_arguments check_int_arguments
#define check_float_arguments check_int_arguments
#define check_double_arguments check_int_arguments
#define check_ldouble_arguments check_int_arguments

/* Clear register struct.  */
#define clear_struct_registers \
  eax = edx = ecx = 0; \
  iamcu_memset (&iregs, 0, sizeof iregs);

/* Clear both hardware and register structs for integers.  */
#define clear_int_registers \
  clear_struct_registers \
  clear_int_hardware_registers

/* Clear both hardware and register structs for integers, excluding the
   one used to return aggregate.  */
#define clear_non_sret_int_registers \
  clear_struct_registers \
  clear_non_sret_int_hardware_registers

#endif /* INCLUDED_ARGS_H  */

struct IntegerRegisters iregbits = { ~0, ~0, ~0, ~0, ~0, ~0 };
struct IntegerRegisters iregs;
unsigned int num_iregs;

/* This struct holds values for argument checking.  */
struct
{
  float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23;
} values_float;

struct
{
  double f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23;
} values_double;

struct
{
  ldouble f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23;
} values_ldouble;

void
fun_check_float_passing_float8_values (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_float.f0 == f0);
  assert (values_float.f1 == f1);
  assert (values_float.f2 == f2);
  assert (values_float.f3 == f3);
  assert (values_float.f4 == f4);
  assert (values_float.f5 == f5);
  assert (values_float.f6 == f6);
  assert (values_float.f7 == f7);

}

void
fun_check_float_passing_float8_regs (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_float_arguments;
}

void
fun_check_float_passing_float16_values (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED, float f8 ATTRIBUTE_UNUSED, float f9 ATTRIBUTE_UNUSED, float f10 ATTRIBUTE_UNUSED, float f11 ATTRIBUTE_UNUSED, float f12 ATTRIBUTE_UNUSED, float f13 ATTRIBUTE_UNUSED, float f14 ATTRIBUTE_UNUSED, float f15 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_float.f0 == f0);
  assert (values_float.f1 == f1);
  assert (values_float.f2 == f2);
  assert (values_float.f3 == f3);
  assert (values_float.f4 == f4);
  assert (values_float.f5 == f5);
  assert (values_float.f6 == f6);
  assert (values_float.f7 == f7);
  assert (values_float.f8 == f8);
  assert (values_float.f9 == f9);
  assert (values_float.f10 == f10);
  assert (values_float.f11 == f11);
  assert (values_float.f12 == f12);
  assert (values_float.f13 == f13);
  assert (values_float.f14 == f14);
  assert (values_float.f15 == f15);

}

void
fun_check_float_passing_float16_regs (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED, float f8 ATTRIBUTE_UNUSED, float f9 ATTRIBUTE_UNUSED, float f10 ATTRIBUTE_UNUSED, float f11 ATTRIBUTE_UNUSED, float f12 ATTRIBUTE_UNUSED, float f13 ATTRIBUTE_UNUSED, float f14 ATTRIBUTE_UNUSED, float f15 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_float_arguments;
}

void
fun_check_float_passing_float20_values (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED, float f8 ATTRIBUTE_UNUSED, float f9 ATTRIBUTE_UNUSED, float f10 ATTRIBUTE_UNUSED, float f11 ATTRIBUTE_UNUSED, float f12 ATTRIBUTE_UNUSED, float f13 ATTRIBUTE_UNUSED, float f14 ATTRIBUTE_UNUSED, float f15 ATTRIBUTE_UNUSED, float f16 ATTRIBUTE_UNUSED, float f17 ATTRIBUTE_UNUSED, float f18 ATTRIBUTE_UNUSED, float f19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_float.f0 == f0);
  assert (values_float.f1 == f1);
  assert (values_float.f2 == f2);
  assert (values_float.f3 == f3);
  assert (values_float.f4 == f4);
  assert (values_float.f5 == f5);
  assert (values_float.f6 == f6);
  assert (values_float.f7 == f7);
  assert (values_float.f8 == f8);
  assert (values_float.f9 == f9);
  assert (values_float.f10 == f10);
  assert (values_float.f11 == f11);
  assert (values_float.f12 == f12);
  assert (values_float.f13 == f13);
  assert (values_float.f14 == f14);
  assert (values_float.f15 == f15);
  assert (values_float.f16 == f16);
  assert (values_float.f17 == f17);
  assert (values_float.f18 == f18);
  assert (values_float.f19 == f19);

}

void
fun_check_float_passing_float20_regs (float f0 ATTRIBUTE_UNUSED, float f1 ATTRIBUTE_UNUSED, float f2 ATTRIBUTE_UNUSED, float f3 ATTRIBUTE_UNUSED, float f4 ATTRIBUTE_UNUSED, float f5 ATTRIBUTE_UNUSED, float f6 ATTRIBUTE_UNUSED, float f7 ATTRIBUTE_UNUSED, float f8 ATTRIBUTE_UNUSED, float f9 ATTRIBUTE_UNUSED, float f10 ATTRIBUTE_UNUSED, float f11 ATTRIBUTE_UNUSED, float f12 ATTRIBUTE_UNUSED, float f13 ATTRIBUTE_UNUSED, float f14 ATTRIBUTE_UNUSED, float f15 ATTRIBUTE_UNUSED, float f16 ATTRIBUTE_UNUSED, float f17 ATTRIBUTE_UNUSED, float f18 ATTRIBUTE_UNUSED, float f19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_float_arguments;
}

void
fun_check_float_passing_double8_values (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_double.f0 == f0);
  assert (values_double.f1 == f1);
  assert (values_double.f2 == f2);
  assert (values_double.f3 == f3);
  assert (values_double.f4 == f4);
  assert (values_double.f5 == f5);
  assert (values_double.f6 == f6);
  assert (values_double.f7 == f7);

}

void
fun_check_float_passing_double8_regs (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_double_arguments;
}

void
fun_check_float_passing_double16_values (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED, double f8 ATTRIBUTE_UNUSED, double f9 ATTRIBUTE_UNUSED, double f10 ATTRIBUTE_UNUSED, double f11 ATTRIBUTE_UNUSED, double f12 ATTRIBUTE_UNUSED, double f13 ATTRIBUTE_UNUSED, double f14 ATTRIBUTE_UNUSED, double f15 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_double.f0 == f0);
  assert (values_double.f1 == f1);
  assert (values_double.f2 == f2);
  assert (values_double.f3 == f3);
  assert (values_double.f4 == f4);
  assert (values_double.f5 == f5);
  assert (values_double.f6 == f6);
  assert (values_double.f7 == f7);
  assert (values_double.f8 == f8);
  assert (values_double.f9 == f9);
  assert (values_double.f10 == f10);
  assert (values_double.f11 == f11);
  assert (values_double.f12 == f12);
  assert (values_double.f13 == f13);
  assert (values_double.f14 == f14);
  assert (values_double.f15 == f15);

}

void
fun_check_float_passing_double16_regs (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED, double f8 ATTRIBUTE_UNUSED, double f9 ATTRIBUTE_UNUSED, double f10 ATTRIBUTE_UNUSED, double f11 ATTRIBUTE_UNUSED, double f12 ATTRIBUTE_UNUSED, double f13 ATTRIBUTE_UNUSED, double f14 ATTRIBUTE_UNUSED, double f15 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_double_arguments;
}

void
fun_check_float_passing_double20_values (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED, double f8 ATTRIBUTE_UNUSED, double f9 ATTRIBUTE_UNUSED, double f10 ATTRIBUTE_UNUSED, double f11 ATTRIBUTE_UNUSED, double f12 ATTRIBUTE_UNUSED, double f13 ATTRIBUTE_UNUSED, double f14 ATTRIBUTE_UNUSED, double f15 ATTRIBUTE_UNUSED, double f16 ATTRIBUTE_UNUSED, double f17 ATTRIBUTE_UNUSED, double f18 ATTRIBUTE_UNUSED, double f19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_double.f0 == f0);
  assert (values_double.f1 == f1);
  assert (values_double.f2 == f2);
  assert (values_double.f3 == f3);
  assert (values_double.f4 == f4);
  assert (values_double.f5 == f5);
  assert (values_double.f6 == f6);
  assert (values_double.f7 == f7);
  assert (values_double.f8 == f8);
  assert (values_double.f9 == f9);
  assert (values_double.f10 == f10);
  assert (values_double.f11 == f11);
  assert (values_double.f12 == f12);
  assert (values_double.f13 == f13);
  assert (values_double.f14 == f14);
  assert (values_double.f15 == f15);
  assert (values_double.f16 == f16);
  assert (values_double.f17 == f17);
  assert (values_double.f18 == f18);
  assert (values_double.f19 == f19);

}

void
fun_check_float_passing_double20_regs (double f0 ATTRIBUTE_UNUSED, double f1 ATTRIBUTE_UNUSED, double f2 ATTRIBUTE_UNUSED, double f3 ATTRIBUTE_UNUSED, double f4 ATTRIBUTE_UNUSED, double f5 ATTRIBUTE_UNUSED, double f6 ATTRIBUTE_UNUSED, double f7 ATTRIBUTE_UNUSED, double f8 ATTRIBUTE_UNUSED, double f9 ATTRIBUTE_UNUSED, double f10 ATTRIBUTE_UNUSED, double f11 ATTRIBUTE_UNUSED, double f12 ATTRIBUTE_UNUSED, double f13 ATTRIBUTE_UNUSED, double f14 ATTRIBUTE_UNUSED, double f15 ATTRIBUTE_UNUSED, double f16 ATTRIBUTE_UNUSED, double f17 ATTRIBUTE_UNUSED, double f18 ATTRIBUTE_UNUSED, double f19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_double_arguments;
}

void
fun_check_x87_passing_ldouble8_values (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_ldouble.f0 == f0);
  assert (values_ldouble.f1 == f1);
  assert (values_ldouble.f2 == f2);
  assert (values_ldouble.f3 == f3);
  assert (values_ldouble.f4 == f4);
  assert (values_ldouble.f5 == f5);
  assert (values_ldouble.f6 == f6);
  assert (values_ldouble.f7 == f7);

}

void
fun_check_x87_passing_ldouble8_regs (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_ldouble_arguments;
}

void
fun_check_x87_passing_ldouble16_values (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED, ldouble f8 ATTRIBUTE_UNUSED, ldouble f9 ATTRIBUTE_UNUSED, ldouble f10 ATTRIBUTE_UNUSED, ldouble f11 ATTRIBUTE_UNUSED, ldouble f12 ATTRIBUTE_UNUSED, ldouble f13 ATTRIBUTE_UNUSED, ldouble f14 ATTRIBUTE_UNUSED, ldouble f15 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_ldouble.f0 == f0);
  assert (values_ldouble.f1 == f1);
  assert (values_ldouble.f2 == f2);
  assert (values_ldouble.f3 == f3);
  assert (values_ldouble.f4 == f4);
  assert (values_ldouble.f5 == f5);
  assert (values_ldouble.f6 == f6);
  assert (values_ldouble.f7 == f7);
  assert (values_ldouble.f8 == f8);
  assert (values_ldouble.f9 == f9);
  assert (values_ldouble.f10 == f10);
  assert (values_ldouble.f11 == f11);
  assert (values_ldouble.f12 == f12);
  assert (values_ldouble.f13 == f13);
  assert (values_ldouble.f14 == f14);
  assert (values_ldouble.f15 == f15);

}

void
fun_check_x87_passing_ldouble16_regs (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED, ldouble f8 ATTRIBUTE_UNUSED, ldouble f9 ATTRIBUTE_UNUSED, ldouble f10 ATTRIBUTE_UNUSED, ldouble f11 ATTRIBUTE_UNUSED, ldouble f12 ATTRIBUTE_UNUSED, ldouble f13 ATTRIBUTE_UNUSED, ldouble f14 ATTRIBUTE_UNUSED, ldouble f15 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_ldouble_arguments;
}

void
fun_check_x87_passing_ldouble20_values (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED, ldouble f8 ATTRIBUTE_UNUSED, ldouble f9 ATTRIBUTE_UNUSED, ldouble f10 ATTRIBUTE_UNUSED, ldouble f11 ATTRIBUTE_UNUSED, ldouble f12 ATTRIBUTE_UNUSED, ldouble f13 ATTRIBUTE_UNUSED, ldouble f14 ATTRIBUTE_UNUSED, ldouble f15 ATTRIBUTE_UNUSED, ldouble f16 ATTRIBUTE_UNUSED, ldouble f17 ATTRIBUTE_UNUSED, ldouble f18 ATTRIBUTE_UNUSED, ldouble f19 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_ldouble.f0 == f0);
  assert (values_ldouble.f1 == f1);
  assert (values_ldouble.f2 == f2);
  assert (values_ldouble.f3 == f3);
  assert (values_ldouble.f4 == f4);
  assert (values_ldouble.f5 == f5);
  assert (values_ldouble.f6 == f6);
  assert (values_ldouble.f7 == f7);
  assert (values_ldouble.f8 == f8);
  assert (values_ldouble.f9 == f9);
  assert (values_ldouble.f10 == f10);
  assert (values_ldouble.f11 == f11);
  assert (values_ldouble.f12 == f12);
  assert (values_ldouble.f13 == f13);
  assert (values_ldouble.f14 == f14);
  assert (values_ldouble.f15 == f15);
  assert (values_ldouble.f16 == f16);
  assert (values_ldouble.f17 == f17);
  assert (values_ldouble.f18 == f18);
  assert (values_ldouble.f19 == f19);

}

void
fun_check_x87_passing_ldouble20_regs (ldouble f0 ATTRIBUTE_UNUSED, ldouble f1 ATTRIBUTE_UNUSED, ldouble f2 ATTRIBUTE_UNUSED, ldouble f3 ATTRIBUTE_UNUSED, ldouble f4 ATTRIBUTE_UNUSED, ldouble f5 ATTRIBUTE_UNUSED, ldouble f6 ATTRIBUTE_UNUSED, ldouble f7 ATTRIBUTE_UNUSED, ldouble f8 ATTRIBUTE_UNUSED, ldouble f9 ATTRIBUTE_UNUSED, ldouble f10 ATTRIBUTE_UNUSED, ldouble f11 ATTRIBUTE_UNUSED, ldouble f12 ATTRIBUTE_UNUSED, ldouble f13 ATTRIBUTE_UNUSED, ldouble f14 ATTRIBUTE_UNUSED, ldouble f15 ATTRIBUTE_UNUSED, ldouble f16 ATTRIBUTE_UNUSED, ldouble f17 ATTRIBUTE_UNUSED, ldouble f18 ATTRIBUTE_UNUSED, ldouble f19 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_ldouble_arguments;
}

#define def_check_float_passing8(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7);

#define def_check_float_passing16(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  values_ ## TYPE .f8 = _f8; \
  values_ ## TYPE .f9 = _f9; \
  values_ ## TYPE .f10 = _f10; \
  values_ ## TYPE .f11 = _f11; \
  values_ ## TYPE .f12 = _f12; \
  values_ ## TYPE .f13 = _f13; \
  values_ ## TYPE .f14 = _f14; \
  values_ ## TYPE .f15 = _f15; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15);

#define def_check_float_passing20(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  values_ ## TYPE .f8 = _f8; \
  values_ ## TYPE .f9 = _f9; \
  values_ ## TYPE .f10 = _f10; \
  values_ ## TYPE .f11 = _f11; \
  values_ ## TYPE .f12 = _f12; \
  values_ ## TYPE .f13 = _f13; \
  values_ ## TYPE .f14 = _f14; \
  values_ ## TYPE .f15 = _f15; \
  values_ ## TYPE .f16 = _f16; \
  values_ ## TYPE .f17 = _f17; \
  values_ ## TYPE .f18 = _f18; \
  values_ ## TYPE .f19 = _f19; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19);

#define def_check_x87_passing8(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7);

#define def_check_x87_passing16(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  values_ ## TYPE .f8 = _f8; \
  values_ ## TYPE .f9 = _f9; \
  values_ ## TYPE .f10 = _f10; \
  values_ ## TYPE .f11 = _f11; \
  values_ ## TYPE .f12 = _f12; \
  values_ ## TYPE .f13 = _f13; \
  values_ ## TYPE .f14 = _f14; \
  values_ ## TYPE .f15 = _f15; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15);

#define def_check_x87_passing20(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19, _func1, _func2, TYPE) \
  values_ ## TYPE .f0 = _f0; \
  values_ ## TYPE .f1 = _f1; \
  values_ ## TYPE .f2 = _f2; \
  values_ ## TYPE .f3 = _f3; \
  values_ ## TYPE .f4 = _f4; \
  values_ ## TYPE .f5 = _f5; \
  values_ ## TYPE .f6 = _f6; \
  values_ ## TYPE .f7 = _f7; \
  values_ ## TYPE .f8 = _f8; \
  values_ ## TYPE .f9 = _f9; \
  values_ ## TYPE .f10 = _f10; \
  values_ ## TYPE .f11 = _f11; \
  values_ ## TYPE .f12 = _f12; \
  values_ ## TYPE .f13 = _f13; \
  values_ ## TYPE .f14 = _f14; \
  values_ ## TYPE .f15 = _f15; \
  values_ ## TYPE .f16 = _f16; \
  values_ ## TYPE .f17 = _f17; \
  values_ ## TYPE .f18 = _f18; \
  values_ ## TYPE .f19 = _f19; \
  WRAP_CALL(_func1) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19); \
  \
  clear_int_registers; \
  if (sizeof (TYPE) == 4) \
    { \
      u.f = _f0; \
      iregs.I0 = u.i[0]; \
      u.f = _f1; \
      iregs.I1 = u.i[0]; \
      u.f = _f2; \
      iregs.I2 = u.i[0]; \
      num_iregs = 3; \
    } \
  else \
    { \
      u.d = _f0; \
      iregs.I0 = u.i[0]; \
      iregs.I1 = u.i[1]; \
      num_iregs = 2; \
    } \
  WRAP_CALL(_func2) (_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, _f17, _f18, _f19);

void
test_floats_on_stack ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_float_passing8(32, 33, 34, 35, 36, 37, 38, 39, fun_check_float_passing_float8_values, fun_check_float_passing_float8_regs, float);

  def_check_float_passing16(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, fun_check_float_passing_float16_values, fun_check_float_passing_float16_regs, float);
}

void
test_too_many_floats ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_float_passing20(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, fun_check_float_passing_float20_values, fun_check_float_passing_float20_regs, float);
}

void
test_doubles_on_stack ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_float_passing8(32, 33, 34, 35, 36, 37, 38, 39, fun_check_float_passing_double8_values, fun_check_float_passing_double8_regs, double);

  def_check_float_passing16(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, fun_check_float_passing_double16_values, fun_check_float_passing_double16_regs, double);
}

void
test_too_many_doubles ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_float_passing20(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, fun_check_float_passing_double20_values, fun_check_float_passing_double20_regs, double);
}

void
test_long_doubles_on_stack ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_x87_passing8(32, 33, 34, 35, 36, 37, 38, 39, fun_check_x87_passing_ldouble8_values, fun_check_x87_passing_ldouble8_regs, ldouble);
}

void
test_too_many_long_doubles ()
{
  union
    {
      float f;
      double d;
      int i[2];
    } u;
  def_check_x87_passing20(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, fun_check_x87_passing_ldouble20_values, fun_check_x87_passing_ldouble20_regs, ldouble);
}

void
test_float128s_on_stack ()
{
}

void
test_too_many_float128s ()
{
}


int
main (void)
{
  test_floats_on_stack ();
  test_too_many_floats ();
  test_doubles_on_stack ();
  test_too_many_doubles ();
  test_long_doubles_on_stack ();
  test_too_many_long_doubles ();
  test_float128s_on_stack ();
  test_too_many_float128s ();
  return 0;
}
