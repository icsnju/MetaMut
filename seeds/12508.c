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
  int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
} values_int;

struct
{
  long i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23;
} values_long;

void
fun_check_int_passing_int6_values (int i0 ATTRIBUTE_UNUSED, int i1 ATTRIBUTE_UNUSED, int i2 ATTRIBUTE_UNUSED, int i3 ATTRIBUTE_UNUSED, int i4 ATTRIBUTE_UNUSED, int i5 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_int.i0 == i0);
  assert (values_int.i1 == i1);
  assert (values_int.i2 == i2);
  assert (values_int.i3 == i3);
  assert (values_int.i4 == i4);
  assert (values_int.i5 == i5);

}

void
fun_check_int_passing_int6_regs (int i0 ATTRIBUTE_UNUSED, int i1 ATTRIBUTE_UNUSED, int i2 ATTRIBUTE_UNUSED, int i3 ATTRIBUTE_UNUSED, int i4 ATTRIBUTE_UNUSED, int i5 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_int_arguments;
}

void
fun_check_int_passing_int12_values (int i0 ATTRIBUTE_UNUSED, int i1 ATTRIBUTE_UNUSED, int i2 ATTRIBUTE_UNUSED, int i3 ATTRIBUTE_UNUSED, int i4 ATTRIBUTE_UNUSED, int i5 ATTRIBUTE_UNUSED, int i6 ATTRIBUTE_UNUSED, int i7 ATTRIBUTE_UNUSED, int i8 ATTRIBUTE_UNUSED, int i9 ATTRIBUTE_UNUSED, int i10 ATTRIBUTE_UNUSED, int i11 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_int.i0 == i0);
  assert (values_int.i1 == i1);
  assert (values_int.i2 == i2);
  assert (values_int.i3 == i3);
  assert (values_int.i4 == i4);
  assert (values_int.i5 == i5);
  assert (values_int.i6 == i6);
  assert (values_int.i7 == i7);
  assert (values_int.i8 == i8);
  assert (values_int.i9 == i9);
  assert (values_int.i10 == i10);
  assert (values_int.i11 == i11);

}

void
fun_check_int_passing_int12_regs (int i0 ATTRIBUTE_UNUSED, int i1 ATTRIBUTE_UNUSED, int i2 ATTRIBUTE_UNUSED, int i3 ATTRIBUTE_UNUSED, int i4 ATTRIBUTE_UNUSED, int i5 ATTRIBUTE_UNUSED, int i6 ATTRIBUTE_UNUSED, int i7 ATTRIBUTE_UNUSED, int i8 ATTRIBUTE_UNUSED, int i9 ATTRIBUTE_UNUSED, int i10 ATTRIBUTE_UNUSED, int i11 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_int_arguments;
}

void
fun_check_int_passing_long6_values (long i0 ATTRIBUTE_UNUSED, long i1 ATTRIBUTE_UNUSED, long i2 ATTRIBUTE_UNUSED, long i3 ATTRIBUTE_UNUSED, long i4 ATTRIBUTE_UNUSED, long i5 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_long.i0 == i0);
  assert (values_long.i1 == i1);
  assert (values_long.i2 == i2);
  assert (values_long.i3 == i3);
  assert (values_long.i4 == i4);
  assert (values_long.i5 == i5);

}

void
fun_check_int_passing_long6_regs (long i0 ATTRIBUTE_UNUSED, long i1 ATTRIBUTE_UNUSED, long i2 ATTRIBUTE_UNUSED, long i3 ATTRIBUTE_UNUSED, long i4 ATTRIBUTE_UNUSED, long i5 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_long_arguments;
}

void
fun_check_int_passing_long12_values (long i0 ATTRIBUTE_UNUSED, long i1 ATTRIBUTE_UNUSED, long i2 ATTRIBUTE_UNUSED, long i3 ATTRIBUTE_UNUSED, long i4 ATTRIBUTE_UNUSED, long i5 ATTRIBUTE_UNUSED, long i6 ATTRIBUTE_UNUSED, long i7 ATTRIBUTE_UNUSED, long i8 ATTRIBUTE_UNUSED, long i9 ATTRIBUTE_UNUSED, long i10 ATTRIBUTE_UNUSED, long i11 ATTRIBUTE_UNUSED)
{
  /* Check argument values.  */
  assert (values_long.i0 == i0);
  assert (values_long.i1 == i1);
  assert (values_long.i2 == i2);
  assert (values_long.i3 == i3);
  assert (values_long.i4 == i4);
  assert (values_long.i5 == i5);
  assert (values_long.i6 == i6);
  assert (values_long.i7 == i7);
  assert (values_long.i8 == i8);
  assert (values_long.i9 == i9);
  assert (values_long.i10 == i10);
  assert (values_long.i11 == i11);

}

void
fun_check_int_passing_long12_regs (long i0 ATTRIBUTE_UNUSED, long i1 ATTRIBUTE_UNUSED, long i2 ATTRIBUTE_UNUSED, long i3 ATTRIBUTE_UNUSED, long i4 ATTRIBUTE_UNUSED, long i5 ATTRIBUTE_UNUSED, long i6 ATTRIBUTE_UNUSED, long i7 ATTRIBUTE_UNUSED, long i8 ATTRIBUTE_UNUSED, long i9 ATTRIBUTE_UNUSED, long i10 ATTRIBUTE_UNUSED, long i11 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_long_arguments;
}

#define def_check_int_passing6(_i0, _i1, _i2, _i3, _i4, _i5, _func1, _func2, TYPE) \
  values_ ## TYPE .i0 = _i0; \
  values_ ## TYPE .i1 = _i1; \
  values_ ## TYPE .i2 = _i2; \
  values_ ## TYPE .i3 = _i3; \
  values_ ## TYPE .i4 = _i4; \
  values_ ## TYPE .i5 = _i5; \
  WRAP_CALL(_func1) (_i0, _i1, _i2, _i3, _i4, _i5); \
  \
  clear_int_registers; \
  iregs.I0 = _i0; \
  iregs.I1 = _i1; \
  iregs.I2 = _i2; \
  num_iregs = 3; \
  WRAP_CALL(_func2) (_i0, _i1, _i2, _i3, _i4, _i5);

#define def_check_int_passing12(_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11, _func1, _func2, TYPE) \
  values_ ## TYPE .i0 = _i0; \
  values_ ## TYPE .i1 = _i1; \
  values_ ## TYPE .i2 = _i2; \
  values_ ## TYPE .i3 = _i3; \
  values_ ## TYPE .i4 = _i4; \
  values_ ## TYPE .i5 = _i5; \
  values_ ## TYPE .i6 = _i6; \
  values_ ## TYPE .i7 = _i7; \
  values_ ## TYPE .i8 = _i8; \
  values_ ## TYPE .i9 = _i9; \
  values_ ## TYPE .i10 = _i10; \
  values_ ## TYPE .i11 = _i11; \
  WRAP_CALL(_func1) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11); \
  \
  clear_int_registers; \
  iregs.I0 = _i0; \
  iregs.I1 = _i1; \
  iregs.I2 = _i2; \
  num_iregs = 3; \
  WRAP_CALL(_func2) (_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9, _i10, _i11);

void
test_ints_on_stack ()
{
  def_check_int_passing6(32, 33, 34, 35, 36, 37, fun_check_int_passing_int6_values, fun_check_int_passing_int6_regs, int);
}

void
test_too_many_ints ()
{
  def_check_int_passing12(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, fun_check_int_passing_int12_values, fun_check_int_passing_int12_regs, int);
}

void
test_longs_on_stack ()
{
  def_check_int_passing6(32, 33, 34, 35, 36, 37, fun_check_int_passing_long6_values, fun_check_int_passing_long6_regs, long);
}

void
test_too_many_longs ()
{
  def_check_int_passing12(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, fun_check_int_passing_long12_values, fun_check_int_passing_long12_regs, long);
}

int
main (void)
{
  test_ints_on_stack ();
  test_too_many_ints ();
  test_longs_on_stack ();
  test_too_many_longs ();
  return 0;
}
