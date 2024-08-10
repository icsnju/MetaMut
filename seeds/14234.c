/* This tests passing and returning of empty structures and unions.  */

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

struct empty_struct
{
};

struct empty_struct
check_struct_passing(struct empty_struct s0 ATTRIBUTE_UNUSED,
		     struct empty_struct s1 ATTRIBUTE_UNUSED,
		     int i0 ATTRIBUTE_UNUSED)
{
  struct empty_struct s;
  check_int_arguments;
  return s;
}

#define check_struct_passing WRAP_CALL(check_struct_passing)

union empty_union
{
};

union empty_union
check_union_passing(union empty_union u0 ATTRIBUTE_UNUSED,
		    union empty_union u1 ATTRIBUTE_UNUSED,
		    int i0 ATTRIBUTE_UNUSED)
{
  union empty_union u;
  check_int_arguments;
  return u;
}

#define check_union_passing WRAP_CALL(check_union_passing)

int
main (void)
{
  struct empty_struct s;
  union empty_union u;

  clear_struct_registers;
  iregs.I0 = 32;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing(u,u,32);

  clear_struct_registers;
  iregs.I0 = 33;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_struct_passing(s,s,33);

  return 0;
}
