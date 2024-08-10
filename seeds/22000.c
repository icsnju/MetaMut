#define AVX512VL(ebx) 1
#define XSTATE_MASK (XSTATE_SSE | XSTATE_YMM | XSTATE_ZMM \
		     | XSTATE_HI_ZMM | XSTATE_OPMASK)
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
#define F0 zmm0
#define F1 zmm1
#define F2 zmm2
#define F3 zmm3
#define F4 zmm4
#define F5 zmm5
#define F6 zmm6
#define F7 zmm7

typedef union {
  _Float16 __Float16[32];
  float _float[16];
  double _double[8];
  long long _longlong[8];
  int _int[16];
  unsigned long long _ulonglong[8];
  __m64 _m64[8];
  __m128 _m128[4];
  __m256 _m256[2];
  __m512 _m512[1];
  __m512h _m512h[1];
} ZMM_T;

typedef union {
  float _float;
  double _double;
  long double _ldouble;
  unsigned long long _ulonglong[2];
} X87_T;
extern void (*callthis)(void);
extern unsigned long long rax,rbx,rcx,rdx,rsi,rdi,rsp,rbp,r8,r9,r10,r11,r12,r13,r14,r15;
ZMM_T zmm_regs[32];
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
  ZMM_T zmm0, zmm1, zmm2, zmm3, zmm4, zmm5, zmm6, zmm7, zmm8, zmm9,
        zmm10, zmm11, zmm12, zmm13, zmm14, zmm15, zmm16, zmm17, zmm18,
	zmm19, zmm20, zmm21, zmm22, zmm23, zmm24, zmm25, zmm26, zmm27,
	zmm28, zmm29, zmm30, zmm31;
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
  memset (zmm_regs, 0, sizeof (zmm_regs)); \
  memset (x87_regs, 0, sizeof (x87_regs));

/* Clear both hardware and register structs for integers.  */
#define clear_int_registers \
  clear_struct_registers \
  clear_int_hardware_registers

/* TODO: Do the checking.  */
#define check_f_arguments(T) do { \
  assert (num_fregs <= 0 || fregs.zmm0._ ## T [0] == zmm_regs[0]._ ## T [0]); \
  assert (num_fregs <= 1 || fregs.zmm1._ ## T [0] == zmm_regs[1]._ ## T [0]); \
  assert (num_fregs <= 2 || fregs.zmm2._ ## T [0] == zmm_regs[2]._ ## T [0]); \
  assert (num_fregs <= 3 || fregs.zmm3._ ## T [0] == zmm_regs[3]._ ## T [0]); \
  assert (num_fregs <= 4 || fregs.zmm4._ ## T [0] == zmm_regs[4]._ ## T [0]); \
  assert (num_fregs <= 5 || fregs.zmm5._ ## T [0] == zmm_regs[5]._ ## T [0]); \
  assert (num_fregs <= 6 || fregs.zmm6._ ## T [0] == zmm_regs[6]._ ## T [0]); \
  assert (num_fregs <= 7 || fregs.zmm7._ ## T [0] == zmm_regs[7]._ ## T [0]); \
  } while (0)

#define check_float_arguments check_f_arguments(float)
#define check_double_arguments check_f_arguments(double)

#define check_vector_arguments(T,O) do { \
  assert (num_fregs <= 0 \
	  || memcmp (((char *) &fregs.zmm0) + (O), \
		     &zmm_regs[0], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 1 \
	  || memcmp (((char *) &fregs.zmm1) + (O), \
		     &zmm_regs[1], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 2 \
	  || memcmp (((char *) &fregs.zmm2) + (O), \
		     &zmm_regs[2], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 3 \
	  || memcmp (((char *) &fregs.zmm3) + (O), \
		     &zmm_regs[3], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 4 \
	  || memcmp (((char *) &fregs.zmm4) + (O), \
		     &zmm_regs[4], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 5 \
	  || memcmp (((char *) &fregs.zmm5) + (O), \
		     &zmm_regs[5], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 6 \
	  || memcmp (((char *) &fregs.zmm6) + (O), \
		     &zmm_regs[6], \
		     sizeof (__ ## T) - (O)) == 0); \
  assert (num_fregs <= 7 \
	  || memcmp (((char *) &fregs.zmm7) + (O), \
		     &zmm_regs[7], \
		     sizeof (__ ## T) - (O)) == 0); \
  } while (0)

#define check_m64_arguments check_vector_arguments(m64, 0)
#define check_m128_arguments check_vector_arguments(m128, 0)
#define check_m256_arguments check_vector_arguments(m256, 0)
#define check_m512_arguments check_vector_arguments(m512, 0)

#endif /* INCLUDED_ARGS_H  */

struct IntegerRegisters iregs;
struct FloatRegisters fregs;
unsigned int num_iregs, num_fregs;

union un1
{
  __m512 x;
  float f;
};

union un2
{
  __m512 x;
  double d;
};

union un3
{
  __m512 x;
  __m128 v;
};

union un4
{
  __m512 x;
  long double ld;
};

union un5
{
  __m512 x;
  int i;
};

union un6
{
  __m512 x;
  __m256 v;
};

union un1h
{
  __m512 x;
  _Float16 f;
};

union un1hf
{
  __m512h x;
  float f;
};

union un1hh
{
  __m512h x;
  _Float16 f;
};

union un2h
{
  __m512h x;
  double d;
};

union un3h
{
  __m512h x;
  __m128 v;
};

union un4h
{
  __m512h x;
  long double ld;
};

union un5h
{
  __m512h x;
  int i;
};

union un6h
{
  __m512h x;
  __m256 v;
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
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
}

void
check_union_passing1hf(union un1hf u1 ATTRIBUTE_UNUSED,
		       union un1hf u2 ATTRIBUTE_UNUSED,
		       union un1hf u3 ATTRIBUTE_UNUSED,
		       union un1hf u4 ATTRIBUTE_UNUSED,
		       union un1hf u5 ATTRIBUTE_UNUSED,
		       union un1hf u6 ATTRIBUTE_UNUSED,
		       union un1hf u7 ATTRIBUTE_UNUSED,
		       union un1hf u8 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
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
  /* Check register contents.  */
  check_m512_arguments;
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

void
check_union_passing6(union un6 u1 ATTRIBUTE_UNUSED,
		     union un6 u2 ATTRIBUTE_UNUSED,
		     union un6 u3 ATTRIBUTE_UNUSED,
		     union un6 u4 ATTRIBUTE_UNUSED,
		     union un6 u5 ATTRIBUTE_UNUSED,
		     union un6 u6 ATTRIBUTE_UNUSED,
		     union un6 u7 ATTRIBUTE_UNUSED,
		     union un6 u8 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

void
check_union_passing6h(union un6h u1 ATTRIBUTE_UNUSED,
		      union un6h u2 ATTRIBUTE_UNUSED,
		      union un6h u3 ATTRIBUTE_UNUSED,
		      union un6h u4 ATTRIBUTE_UNUSED,
		      union un6h u5 ATTRIBUTE_UNUSED,
		      union un6h u6 ATTRIBUTE_UNUSED,
		      union un6h u7 ATTRIBUTE_UNUSED,
		      union un6h u8 ATTRIBUTE_UNUSED)
{
  /* Check register contents.  */
  check_m512_arguments;
}

#define check_union_passing1 WRAP_CALL(check_union_passing1)
#define check_union_passing2 WRAP_CALL(check_union_passing2)
#define check_union_passing3 WRAP_CALL(check_union_passing3)
#define check_union_passing4 WRAP_CALL(check_union_passing4)
#define check_union_passing5 WRAP_CALL(check_union_passing5)
#define check_union_passing6 WRAP_CALL(check_union_passing6)

#define check_union_passing1h WRAP_CALL(check_union_passing1h)
#define check_union_passing1hf WRAP_CALL(check_union_passing1hf)
#define check_union_passing1hh WRAP_CALL(check_union_passing1hh)
#define check_union_passing2h WRAP_CALL(check_union_passing2h)
#define check_union_passing3h WRAP_CALL(check_union_passing3h)
#define check_union_passing4h WRAP_CALL(check_union_passing4h)
#define check_union_passing5h WRAP_CALL(check_union_passing5h)
#define check_union_passing6h WRAP_CALL(check_union_passing6h)


static void
do_test (void)
{
  union un1 u1[8];
  union un2 u2[8];
  union un3 u3[8];
  union un4 u4;
  union un5 u5;
  union un6 u6[8];
  union un1h u1h[8];
  union un1hf u1hf[8];
  union un1hh u1hh[8];
  union un2h u2h[8];
  union un3h u3h[8];
  union un4h u4h;
  union un5h u5h;
  union un6h u6h[8];
   int i;

  for (i = 0; i < 8; i++)
    {
      u1[i].x = (__m512){32+i, 0, i, 0, -i, 0, i - 12, i + 8,
	                 32+i, 0, i, 0, -i, 0, i - 12, i + 8};

      u1hf[i].x =  (__m512h){ 33+i, 1, i, 2, -i, 0, i - 15, i + 9,
                              34+i, 1, i, 2, -i, 0, i - 15, i + 9,
                              35+i, 1, i, 2, -i, 0, i - 15, i + 9,
                              36+i, 1, i, 2, -i, 0, i - 15, i + 9};
    }

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.zmm0)[i]._m512[0] = u1[i].x;
  num_fregs = 8;
  check_union_passing1(u1[0], u1[1], u1[2], u1[3],
		       u1[4], u1[5], u1[6], u1[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u1h[i].x = u1[i].x;
      (&fregs.zmm0)[i]._m512[0] = u1h[i].x;
    }
  num_fregs = 8;
  check_union_passing1h(u1h[0], u1h[1], u1h[2], u1h[3],
		        u1h[4], u1h[5], u1h[6], u1h[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    (&fregs.zmm0)[i]._m512h[0] = u1hf[i].x;
  num_fregs = 8;
  check_union_passing1hf(u1hf[0], u1hf[1], u1hf[2], u1hf[3],
		         u1hf[4], u1hf[5], u1hf[6], u1hf[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u1hh[i].x = u1hf[i].x;
      (&fregs.zmm0)[i]._m512h[0] = u1hh[i].x;
    }
  num_fregs = 8;
  check_union_passing1hh(u1hh[0], u1hh[1], u1hh[2], u1hh[3],
		         u1hh[4], u1hh[5], u1hh[6], u1hh[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u2[i].x = u1[i].x;
      (&fregs.zmm0)[i]._m512[0] = u2[i].x;
    }
  num_fregs = 8;
  check_union_passing2(u2[0], u2[1], u2[2], u2[3],
		       u2[4], u2[5], u2[6], u2[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u2h[i].x = u1hf[i].x;
      (&fregs.zmm0)[i]._m512h[0] = u2h[i].x;
    }
  num_fregs = 8;
  check_union_passing2h(u2h[0], u2h[1], u2h[2], u2h[3],
		        u2h[4], u2h[5], u2h[6], u2h[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u3[i].x = u1[i].x;
      (&fregs.zmm0)[i]._m512[0] = u3[i].x;
    }
  num_fregs = 8;
  check_union_passing3(u3[0], u3[1], u3[2], u3[3],
		       u3[4], u3[5], u3[6], u3[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u3h[i].x = u1hf[i].x;
      (&fregs.zmm0)[i]._m512h[0] = u3h[i].x;
    }
  num_fregs = 8;
  check_union_passing3h(u3h[0], u3h[1], u3h[2], u3h[3],
		        u3h[4], u3h[5], u3h[6], u3h[7]);

  check_union_passing4(u4);
  check_union_passing5(u5);

  check_union_passing4h(u4h);
  check_union_passing5h(u5h);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u6[i].x = u1[i].x;
      (&fregs.zmm0)[i]._m512[0] = u6[i].x;
    }
  num_fregs = 8;
  check_union_passing6(u6[0], u6[1], u6[2], u6[3],
		       u6[4], u6[5], u6[6], u6[7]);

  clear_struct_registers;
  for (i = 0; i < 8; i++)
    {
      u6h[i].x = u1hf[i].x;
      (&fregs.zmm0)[i]._m512h[0] = u6h[i].x;
    }
  num_fregs = 8;
  check_union_passing6h(u6h[0], u6h[1], u6h[2], u6h[3],
		        u6h[4], u6h[5], u6h[6], u6h[7]);
}
