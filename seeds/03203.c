/* This tests passing of structs.  */

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

struct int_struct
{
  int i;
};

struct long_struct
{
  long l;
};

union un1
{
  char c;
  int i;
};

union un2
{
  char c1;
  long l;
  char c2;
};

union un3
{
  struct int_struct is;
  struct long_struct ls;
  union un1 un;
};


void
check_union_passing1(union un1 u ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_union_passing2(union un2 u1 ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_union_passing3(union un3 u ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

#define check_union_passing1 WRAP_CALL(check_union_passing1)
#define check_union_passing2 WRAP_CALL(check_union_passing2)
#define check_union_passing3 WRAP_CALL(check_union_passing3)

union un4
{
  int i;
  float f;
};

union un5
{
  long long ll;
  double d;
};

void
check_union_passing4(union un4 u1 ATTRIBUTE_UNUSED,
		     union un4 u2 ATTRIBUTE_UNUSED,
		     union un4 u3 ATTRIBUTE_UNUSED,
		     union un4 u4 ATTRIBUTE_UNUSED,
		     union un4 u5 ATTRIBUTE_UNUSED,
		     union un4 u6 ATTRIBUTE_UNUSED,
		     union un4 u7 ATTRIBUTE_UNUSED,
		     union un4 u8 ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_union_passing5(union un5 u ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

#define check_union_passing4 WRAP_CALL(check_union_passing4)
#define check_union_passing5 WRAP_CALL(check_union_passing5)

#ifdef CHECK_FLOAT128
union un6
{
  __float128 f128;
  int i;
};


void
check_union_passing6(union un6 u ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&u.f128 == esp+4);
  assert ((unsigned long)&u.i == esp+4);
}

#define check_union_passing6 WRAP_CALL(check_union_passing6)
#endif

int
main (void)
{
  union un1 u1;
#ifdef CHECK_LARGER_UNION_PASSING
  union un2 u2;
  union un3 u3;
  struct int_struct is;
  struct long_struct ls;
#endif /* CHECK_LARGER_UNION_PASSING */
  union un4 u4[8];
  union un5 u5;
  int i;
#ifdef CHECK_FLOAT128
  union un6 u6;
#endif

  /* Check a union with char, int.  */
  clear_struct_registers;
  u1.i = 0;  /* clear the struct to not have high bits left */
  u1.c = 32;
  iregs.I0 = 32;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing1(u1);
  u1.i = 0;  /* clear the struct to not have high bits left */
  u1.i = 33;
  iregs.I0 = 33;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing1(u1);

  /* Check a union with char, long, char.  */
#ifdef CHECK_LARGER_UNION_PASSING
  clear_struct_registers;
  u2.l = 0;  /* clear the struct to not have high bits left */
  u2.c1 = 34;
  iregs.I0 = 34;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing2(u2);
  u2.l = 0;  /* clear the struct to not have high bits left */
  u2.l = 35;
  iregs.I0 = 35;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing2(u2);
  u2.l = 0;  /* clear the struct to not have high bits left */
  u2.c2 = 36;
  iregs.I0 = 36;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing2(u2);

  /* check a union containing two structs and a union.  */
  clear_struct_registers;
  is.i = 37;
  u3.ls.l = 0;  /* clear the struct to not have high bits left */
  u3.is = is;
  iregs.I0 = 37;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing3(u3);
  ls.l = 38;
  u3.ls.l = 0;  /* clear the struct to not have high bits left */
  u3.ls = ls;
  iregs.I0 = 38;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing3(u3);
  u1.c = 39;
  u3.ls.l = 0;  /* clear the struct to not have high bits left */
  u3.un = u1;
  iregs.I0 = 39;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing3(u3);
  u1.i = 40;
  u3.ls.l = 0;  /* clear the struct to not have high bits left */
  u3.un = u1;
  iregs.I0 = 40;
  num_iregs = 1;
  clear_int_hardware_registers;
  check_union_passing3(u3);
#endif /* CHECK_LARGER_UNION_PASSING */

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    u4[i].f = 32 + i;
  iregs.I0 = u4[0].i;
  iregs.I1 = u4[1].i;
  iregs.I2 = u4[2].i;
  num_iregs = 3;
  clear_int_hardware_registers;
  check_union_passing4(u4[0], u4[1], u4[2], u4[3],
		       u4[4], u4[5], u4[6], u4[7]);

  clear_struct_registers;
  u5.d = 48.394;
  iregs.I0 = u5.ll & 0xffffffff;
  iregs.I1 = (u5.ll >> 32) & 0xffffffff;
  num_iregs = 2;
  clear_int_hardware_registers;
  check_union_passing5(u5);

#ifdef CHECK_FLOAT128
  u6.i = 2;
  check_union_passing6(u6);
#endif

  return 0;
}
