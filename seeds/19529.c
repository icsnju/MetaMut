#define AVX512VL(ebx) (ebx & bit_AVX512VL)
#define XSTATE_MASK (XSTATE_SSE | XSTATE_YMM | XSTATE_OPMASK)
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
  _Float16 __Float16[16];
  float _float[8];
  double _double[4];
  long long _longlong[4];
  int _int[8];
  unsigned long long _ulonglong[4];
  __m64 _m64[4];
  __m128 _m128[2];
  __m256 _m256[1];
  __m256h _m256h[1];
} YMM_T;

typedef union {
  float _float;
  double _double;
  long double _ldouble;
  unsigned long long _ulonglong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
YMM_T ymm_regs[16];
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

union un1
{
  __m256 x;
  float f;
};

union un2
{
  __m256 x;
  double d;
};

union un3
{
  __m256 x;
  __m128 v;
};

union un4
{
  __m256 x;
  long double ld;
};

union un5
{
  __m256 x;
  int i;
};

union un1a
{
  __m256 x;
  _Float16 f;
};

union un1h
{
  __m256h x;
  float f;
};

union un1hh
{
  __m256h x;
  _Float16 f;
};

union un2h
{
  __m256h x;
  double d;
};

union un3h
{
  __m256h x;
  __m128 v;
};

union un4h
{
  __m256h x;
  long double ld;
};

union un5h
{
  __m256h x;
  int i;
};

void
check_union_passing1(union un1 u1 ATTRIBUTE_UNUSED,
		     union un1 u2 ATTRIBUTE_UNUSED,
		     union un1 u3 ATTRIBUTE_UNUSED,
		     union un1 u4 ATTRIBUTE_UNUSED,
		     union un1 u5 ATTRIBUTE_UNUSED,
		     union un1 u6 ATTRIBUTE_UNUSED,
		     union un1 u7 ATTRIBUTE_UNUSED,
		     union un1 u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing1a(union un1a u1 ATTRIBUTE_UNUSED,
		      union un1a u2 ATTRIBUTE_UNUSED,
		      union un1a u3 ATTRIBUTE_UNUSED,
		      union un1a u4 ATTRIBUTE_UNUSED,
		      union un1a u5 ATTRIBUTE_UNUSED,
		      union un1a u6 ATTRIBUTE_UNUSED,
		      union un1a u7 ATTRIBUTE_UNUSED,
		      union un1a u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing1h(union un1h u1 ATTRIBUTE_UNUSED,
		      union un1h u2 ATTRIBUTE_UNUSED,
		      union un1h u3 ATTRIBUTE_UNUSED,
		      union un1h u4 ATTRIBUTE_UNUSED,
		      union un1h u5 ATTRIBUTE_UNUSED,
		      union un1h u6 ATTRIBUTE_UNUSED,
		      union un1h u7 ATTRIBUTE_UNUSED,
		      union un1h u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing1hh(union un1hh u1 ATTRIBUTE_UNUSED,
		       union un1hh u2 ATTRIBUTE_UNUSED,
		       union un1hh u3 ATTRIBUTE_UNUSED,
		       union un1hh u4 ATTRIBUTE_UNUSED,
		       union un1hh u5 ATTRIBUTE_UNUSED,
		       union un1hh u6 ATTRIBUTE_UNUSED,
		       union un1hh u7 ATTRIBUTE_UNUSED,
		       union un1hh u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing2(union un2 u1 ATTRIBUTE_UNUSED,
		     union un2 u2 ATTRIBUTE_UNUSED,
		     union un2 u3 ATTRIBUTE_UNUSED,
		     union un2 u4 ATTRIBUTE_UNUSED,
		     union un2 u5 ATTRIBUTE_UNUSED,
		     union un2 u6 ATTRIBUTE_UNUSED,
		     union un2 u7 ATTRIBUTE_UNUSED,
		     union un2 u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing2h(union un2h u1 ATTRIBUTE_UNUSED,
		      union un2h u2 ATTRIBUTE_UNUSED,
		      union un2h u3 ATTRIBUTE_UNUSED,
		      union un2h u4 ATTRIBUTE_UNUSED,
		      union un2h u5 ATTRIBUTE_UNUSED,
		      union un2h u6 ATTRIBUTE_UNUSED,
		      union un2h u7 ATTRIBUTE_UNUSED,
		      union un2h u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing3(union un3 u1 ATTRIBUTE_UNUSED,
		     union un3 u2 ATTRIBUTE_UNUSED,
		     union un3 u3 ATTRIBUTE_UNUSED,
		     union un3 u4 ATTRIBUTE_UNUSED,
		     union un3 u5 ATTRIBUTE_UNUSED,
		     union un3 u6 ATTRIBUTE_UNUSED,
		     union un3 u7 ATTRIBUTE_UNUSED,
		     union un3 u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing3h(union un3h u1 ATTRIBUTE_UNUSED,
		      union un3h u2 ATTRIBUTE_UNUSED,
		      union un3h u3 ATTRIBUTE_UNUSED,
		      union un3h u4 ATTRIBUTE_UNUSED,
		      union un3h u5 ATTRIBUTE_UNUSED,
		      union un3h u6 ATTRIBUTE_UNUSED,
		      union un3h u7 ATTRIBUTE_UNUSED,
		      union un3h u8 ATTRIBUTE_UNUSED)
{
  check_m256_arguments;
}

void
check_union_passing4(union un4 u ATTRIBUTE_UNUSED)
{
   /* Check the passing on the stack by comparing the address of the
      stack elements to the expected place on the stack.  */
  assert ((unsigned long)&u.x == rsp+8);
  assert ((unsigned long)&u.ld == rsp+8);
}

void
check_union_passing4h(union un4h u ATTRIBUTE_UNUSED)
{
   /* Check the passing on the stack by comparing the address of the
      stack elements to the expected place on the stack.  */
  assert ((unsigned long)&u.x == rsp+8);
  assert ((unsigned long)&u.ld == rsp+8);
}

void
check_union_passing5(union un5 u ATTRIBUTE_UNUSED)
{
   /* Check the passing on the stack by comparing the address of the
      stack elements to the expected place on the stack.  */
  assert ((unsigned long)&u.x == rsp+8);
  assert ((unsigned long)&u.i == rsp+8);
}

void
check_union_passing5h(union un5h u ATTRIBUTE_UNUSED)
{
   /* Check the passing on the stack by comparing the address of the
      stack elements to the expected place on the stack.  */
  assert ((unsigned long)&u.x == rsp+8);
  assert ((unsigned long)&u.i == rsp+8);
}

#define check_union_passing1 WRAP_CALL(check_union_passing1)
#define check_union_passing2 WRAP_CALL(check_union_passing2)
#define check_union_passing3 WRAP_CALL(check_union_passing3)
#define check_union_passing4 WRAP_CALL(check_union_passing4)
#define check_union_passing5 WRAP_CALL(check_union_passing5)

#define check_union_passing1h WRAP_CALL(check_union_passing1h)
#define check_union_passing1a WRAP_CALL(check_union_passing1a)
#define check_union_passing1hh WRAP_CALL(check_union_passing1hh)
#define check_union_passing2h WRAP_CALL(check_union_passing2h)
#define check_union_passing3h WRAP_CALL(check_union_passing3h)
#define check_union_passing4h WRAP_CALL(check_union_passing4h)
#define check_union_passing5h WRAP_CALL(check_union_passing5h)

static void
do_test (void)
{
  union un1 u1[8];
  union un2 u2[8];
  union un3 u3[8];
  union un4 u4;
  union un5 u5;
  union un1a u1a[8];
  union un1h u1h[8];
  union un1hh u1hh[8];
  union un2h u2h[8];
  union un3h u3h[8];
  union un4h u4h;
  union un5h u5h;
  int i;

  for (i = 0; i < 8; i++)
    {
      u1[i].x = (__m256){32+i, 0, i, 0, -i, 0, i - 12, i + 8};
      u1h[i].x = (__m256h){32+i, 0, i, 0, -i, 0, i - 12, i + 8,
                           33+i, 1, i, 2, -i, 4, i - 11, i + 9};
    }

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.ymm0)[i]._m256[0] = u1[i].x;
  num_fregs = 8;
  check_union_passing1(u1[0], u1[1], u1[2], u1[3],
		       u1[4], u1[5], u1[6], u1[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u1a[i].x = u1[i].x;
      (&fregs.ymm0)[i]._m256[0] = u1a[i].x;
    }
  num_fregs = 8;
  check_union_passing1a(u1a[0], u1a[1], u1a[2], u1a[3],
		        u1a[4], u1a[5], u1a[6], u1a[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.ymm0)[i]._m256h[0] = u1h[i].x;
  num_fregs = 8;
  check_union_passing1h(u1h[0], u1h[1], u1h[2], u1h[3],
		        u1h[4], u1h[5], u1h[6], u1h[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u1hh[i].x = u1h[i].x;
      (&fregs.ymm0)[i]._m256h[0] = u1hh[i].x;
    }
  num_fregs = 8;
  check_union_passing1hh(u1hh[0], u1hh[1], u1hh[2], u1hh[3],
		         u1hh[4], u1hh[5], u1hh[6], u1hh[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u2[i].x = u1[i].x;
      (&fregs.ymm0)[i]._m256[0] = u2[i].x;
    }
  num_fregs = 8;
  check_union_passing2(u2[0], u2[1], u2[2], u2[3],
		       u2[4], u2[5], u2[6], u2[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u2h[i].x = u1h[i].x;
      (&fregs.ymm0)[i]._m256h[0] = u2h[i].x;
    }
  num_fregs = 8;
  check_union_passing2h(u2h[0], u2h[1], u2h[2], u2h[3],
		        u2h[4], u2h[5], u2h[6], u2h[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u3[i].x = u1[i].x;
      (&fregs.ymm0)[i]._m256[0] = u3[i].x;
    }
  num_fregs = 8;
  check_union_passing3(u3[0], u3[1], u3[2], u3[3],
		       u3[4], u3[5], u3[6], u3[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u3h[i].x = u1h[i].x;
      (&fregs.ymm0)[i]._m256h[0] = u3h[i].x;
    }
  num_fregs = 8;
  check_union_passing3h(u3h[0], u3h[1], u3h[2], u3h[3],
		        u3h[4], u3h[5], u3h[6], u3h[7]);

  check_union_passing4(u4);
  check_union_passing5(u5);

  check_union_passing4h(u4h);
  check_union_passing5h(u5h);
}
