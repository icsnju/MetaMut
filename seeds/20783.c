/* This tests passing of structs. */

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
#include <complex.h>

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

struct longlong2_struct
{
  long long ll1, ll2;
};

struct longlong3_struct
{
  long long ll1, ll2, ll3;
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
check_struct_passing3 (struct longlong2_struct ls ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

void
check_struct_passing4 (struct longlong3_struct ls ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ls.ll1 == esp+4);
  assert ((unsigned long)&ls.ll2 == esp+12);
  assert ((unsigned long)&ls.ll3 == esp+20);
}

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
  __complex__ float x;
};

struct complex1a_struct
{
  long l;
  union
    {
      float f;
      int i;
    } u;
};

void
check_struct_passing9 (struct complex1_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

struct long3_struct
{
  long l1, l2, l3;
};

void
check_struct_passing10 (struct long3_struct ls ATTRIBUTE_UNUSED)
{
  /* Check the passing on the stack by comparing the address of the
     stack elements to the expected place on the stack.  */
  assert ((unsigned long)&ls.l1 == esp+4);
  assert ((unsigned long)&ls.l2 == esp+8);
  assert ((unsigned long)&ls.l3 == esp+12);
}

struct char3_struct
{
  char c1, c2, c3;
};

void
check_struct_passing11 (struct char3_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

struct char7_struct
{
  char c1, c2, c3, c4, c5, c6, c7;
};

void
check_struct_passing12 (struct char7_struct is ATTRIBUTE_UNUSED)
{
  check_int_arguments;
}

static struct flex1_struct f1s = { 60, { } };
static struct flex2_struct f2s = { 61, { } };

int
main (void)
{
  struct int_struct is = { 48 };
  struct long_struct ls = { 49 };
#ifdef CHECK_LARGER_STRUCTS
  struct longlong2_struct ll2s = { 50, 51 };
  struct longlong3_struct ll3s = { 52, 53, 54 };
  struct long3_struct l3s = { 60, 61, 62 };
#endif
  struct complex1_struct c1s = { ( -13.4 + 3.5*I ) };
  union
    {
      struct complex1_struct c;
      struct complex1a_struct u;
    } c1u;
  struct char3_struct c3 = { 0x12, 0x34, 0x56 };
  union
    {
      struct char3_struct c;
      int i;
    } c3u;
  struct char7_struct c7 = { 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56 };
  union
    {
      struct char7_struct c;
      struct
	{
	  int i0, i1;
	} i;
    } c7u;

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
  num_iregs = 0;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing3)(ll2s);
  WRAP_CALL (check_struct_passing4)(ll3s);
  WRAP_CALL (check_struct_passing10)(l3s);
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
  iregs.I1 = c1u.u.u.i;
  num_iregs = 2;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing9)(c1s);

  clear_struct_registers;
  c3u.c = c3;
  iregs.I0 = c3u.i;
  iregbits.I0 = 0xffffff;
  num_iregs = 1;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing11)(c3);

  clear_struct_registers;
  c7u.c = c7;
  iregs.I0 = c7u.i.i0;
  iregs.I1 = c7u.i.i1;
  iregbits.I0 = 0xffffffff;
  iregbits.I1 = 0xffffff;
  num_iregs = 2;
  clear_int_hardware_registers;
  WRAP_CALL (check_struct_passing12)(c7);

  return 0;
}
