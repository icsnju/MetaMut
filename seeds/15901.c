/* { dg-do compile } */
/* { dg-options "-O2 -march=armv7-a" } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-add-options arm_neon } */


#define PRE_STORE(T, ATTR)		\
  ATTR T *				\
  T ## _pre_store (T *p, T v)		\
  {					\
    *++p = v;				\
    return p;				\
  }					\

#define POST_STORE(T, ATTR)		\
  ATTR T *				\
  T ## _post_store (T *p, T v)		\
  {					\
    *p++ = v;				\
    return p;				\
  }

#define POST_STORE_VEC(T, VT, OP, ATTR)	\
  ATTR T *				\
  VT ## _post_store (T * p, VT v)	\
  {					\
    OP (p, v);				\
    p += sizeof (VT) / sizeof (T);	\
    return p;				\
  }

#define PRE_LOAD(T, ATTR)		\
  ATTR void				\
  T ## _pre_load (T *p)			\
  {					\
    ATTR extern void f ## T (T*,T);	\
    T x = *++p;				\
    f ## T (p, x);			\
  }

#define POST_LOAD(T, ATTR)		\
  ATTR void				\
  T ## _post_load (T *p)		\
  {					\
    ATTR extern void f ## T (T*,T);	\
    T x = *p++;				\
    f ## T (p, x);			\
  }

#define POST_LOAD_VEC(T, VT, OP, ATTR)	\
  ATTR void				\
  VT ## _post_load (T * p)		\
  {					\
    ATTR extern void f ## T (T*,T);	\
    VT x = OP (p, v);			\
    p += sizeof (VT) / sizeof (T);	\
    f ## T (p, x);			\
  }

typedef long long ll;

/* no special function attribute required */
#define ATTR /* */

PRE_STORE(char, ATTR)
/* { dg-final { scan-assembler "strb.*#1]!" } } */
PRE_STORE(short, ATTR)
/* { dg-final { scan-assembler "strh.*#2]!" } } */
PRE_STORE(int, ATTR)
/* { dg-final { scan-assembler "str.*#4]!" } } */
PRE_STORE(ll, ATTR)
/* { dg-final { scan-assembler "strd.*#8]!" } } */

POST_STORE(char, ATTR)
/* { dg-final { scan-assembler "strb.*], #1" } } */
POST_STORE(short, ATTR)
/* { dg-final { scan-assembler "strh.*], #2" } } */
POST_STORE(int, ATTR)
/* { dg-final { scan-assembler "str.*], #4" } } */
POST_STORE(ll, ATTR)
/* { dg-final { scan-assembler "strd.*], #8" } } */

PRE_LOAD(char, ATTR)
/* { dg-final { scan-assembler "ldrb.*#1]!" } } */
PRE_LOAD(short, ATTR)
/* { dg-final { scan-assembler "ldrsh.*#2]!" } } */
PRE_LOAD(int, ATTR)
/* { dg-final { scan-assembler "ldr.*#4]!" } } */
PRE_LOAD(ll, ATTR)
/* { dg-final { scan-assembler "ldrd.*#8]!" } } */

POST_LOAD(char, ATTR)
/* { dg-final { scan-assembler "ldrb.*], #1" } } */
POST_LOAD(short, ATTR)
/* { dg-final { scan-assembler "ldrsh.*], #2" } } */
POST_LOAD(int, ATTR)
/* { dg-final { scan-assembler "ldr.*], #4" } } */
POST_LOAD(ll, ATTR)
/* { dg-final { scan-assembler "ldrd.*], #8" } } */

/* { dg-final { scan-assembler-not "\tadd" } } */
