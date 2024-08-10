/* This is a small test case for returning a complex number. Written by
   Andreas Jaeger.  */

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

#define BUILD_F16_COMPLEX(real, imag) \
  ({ __complex__ _Float16 __retval; \
     __real__ __retval = (real); \
     __imag__ __retval = (imag); \
     __retval; })

__complex__ _Float16
aj_f16_times2 (__complex__ _Float16 x)
{
  __complex__ _Float16 res;

  __real__ res = (2.0 * __real__ x);
  __imag__ res = (2.0 * __imag__ x);

  return res;
}

#define BUILD_F_COMPLEX(real, imag) \
  ({ __complex__ float __retval; \
     __real__ __retval = (real); \
     __imag__ __retval = (imag); \
     __retval; })

#define BUILD_D_COMPLEX(real, imag) \
  ({ __complex__ double __retval; \
     __real__ __retval = (real); \
     __imag__ __retval = (imag); \
     __retval; })

#define BUILD_LD_COMPLEX(real, imag) \
  ({ __complex__ long double __retval; \
     __real__ __retval = (real); \
     __imag__ __retval = (imag); \
     __retval; })

__complex__ float
aj_f_times2 (__complex__ float x)
{
  __complex__ float res;

  __real__ res = (2.0 * __real__ x);
  __imag__ res = (2.0 * __imag__ x);

  return res;
}

__complex__ double
aj_d_times2 (__complex__ double x)
{
  __complex__ double res;

  __real__ res = (2.0 * __real__ x);
  __imag__ res = (2.0 * __imag__ x);

  return res;
}

__complex__ long double
aj_ld_times2 (__complex__ long double x)
{
  __complex__ long double res;

  __real__ res = (2.0 * __real__ x);
  __imag__ res = (2.0 * __imag__ x);

  return res;
}

static void
do_test (void)
{
#ifdef CHECK_COMPLEX
  _Complex _Float16 f16c, f16d;
  _Complex float fc, fd;
  _Complex double dc, dd;
  _Complex long double ldc, ldd;

  f16c = BUILD_F16_COMPLEX (2.0, 3.0);
  f16d = aj_f16_times2 (f16c);

  assert (__real__ f16d == 4.0f16 && __imag__ f16d == 6.0f16);

  fc = BUILD_LD_COMPLEX (2.0f, 3.0f);
  fd = aj_f_times2 (fc);

  assert (__real__ fd == 4.0f && __imag__ fd == 6.0f);

  dc = BUILD_LD_COMPLEX (2.0, 3.0);
  dd = aj_ld_times2 (dc);

  assert (__real__ dd == 4.0 && __imag__ dd == 6.0);

  ldc = BUILD_LD_COMPLEX (2.0L, 3.0L);
  ldd = aj_ld_times2 (ldc);

  assert (__real__ ldd == 4.0L && __imag__ ldd == 6.0L);
#endif
}
