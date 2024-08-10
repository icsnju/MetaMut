
/* { dg-do run } */
/* { dg-options "-O3 -std=c99" } */

#include "limits.h"

extern void abort (void);

#define N 16


extern int abs (int);
extern long labs (long);

typedef signed char *__restrict__ pRINT8;
typedef short *__restrict__ pRINT16;
typedef int *__restrict__ pRINT32;
typedef long *__restrict__ pRLONG;
typedef long long *__restrict__ pRINT64;

#define DEF_ABS(size)  void absolute_s##size (pRINT##size a, pRINT##size b) \
		       { \
			 int i;  \
			 for (i=0; i<N; i++)     \
			    a[i] = (b[i] > 0 ? b[i] : -b[i]); \
		       }

DEF_ABS (8);
DEF_ABS (16);
DEF_ABS (32);
DEF_ABS (64);

/* Test abs () vectorization.  */
void absolute_s32_lib (pRINT32 a, pRINT32 b)
{
  int i;
  for (i=0; i<N; i++)
    a[i] = abs (b[i]);
}

void absolute_l32_lib (pRLONG a, pRLONG b)
{
  int i;
  for (i=0; i<N; i++)
    a[i] = labs (b[i]);
}

#define SET_VEC(size, type) void set_vector_##size (pRINT##size a) \
		      	    {				        \
			      int i;			        \
			      for (i=0; i<N; i++)	        \
			        a[i] = (type##_MIN) + (i + 1);  \
		      	    }

#define SET_RVEC(size, type) void set_rvector_##size (pRINT##size a) \
		      	     {				      \
			       int i;			      \
			       for (i=0; i<N; i++)	      \
			         a[i] = type##_MAX - i;       \
		      	     }

#define CHECK_VEC(size) void check_vector_##size (pRINT##size a, \
						  pRINT##size b) \
			{				       \
			  int i;			       \
			  for (i=0; i<N; i++)		       \
			    if (a[i] != b[i])  		       \
			      abort (); 		       \
			}


SET_RVEC (8, SCHAR)
SET_RVEC (16, SHRT)
SET_RVEC (32, INT)
SET_RVEC (64, LLONG)

void
set_rvector_long (pRLONG a)
{
  int i;
  for (i=0; i<N; i++)
    a[i] = (LONG_MAX) - i;
}

SET_VEC (8, SCHAR)
SET_VEC (16, SHRT)
SET_VEC (32, INT)
SET_VEC (64, LLONG)

void
set_vector_long (long *__restrict__ a)
{
  long i;
  for (i=0; i<N; i++)
    a[i] = (LONG_MIN) + i + 1;
}

CHECK_VEC (8)
CHECK_VEC (16)
CHECK_VEC (32)
CHECK_VEC (64)

void
check_vector_long (long *__restrict__ a, long *__restrict__ b)
{
  long i;
  for (i=0; i<N; i++)
    if (a[i] != b[i])
      abort ();
}

int main (void)
{

  signed char a8[N];
  short a16[N];
  int a32[N];
  long long a64[N];
  /* abs () from stdlib.  */
  int alib32[N];
  long alibl[N];


  signed char b8[N];
  short b16[N];
  int b32[N];
  long long b64[N];
  /* abs () from stdlib.  */
  long blibl[N];

  signed char abs_vector_8[N];
  short abs_vector_16[N];
  int abs_vector_32[N];
  long long abs_vector_64[N];
  long abs_vector_long[N];

  /* Set up result vectors.  */
  set_rvector_8 (abs_vector_8);
  set_rvector_16 (abs_vector_16);
  set_rvector_32 (abs_vector_32);
  set_rvector_long (abs_vector_long);
  set_rvector_64 (abs_vector_64);

  /* Set up inputs.  */
  set_vector_8 (b8);
  set_vector_16 (b16);
  set_vector_32 (b32);
  set_vector_64 (b64);
  set_vector_long (blibl);

  /* Calculate their absolute values.  */
  absolute_s8 (a8, b8);
  absolute_s16 (a16, b16);
  absolute_s32 (a32, b32);
  absolute_s64 (a64, b64);
  /* abs () from stdlib.  */
  absolute_s32_lib (alib32, b32);
  absolute_l32_lib (alibl, blibl);

  /* Check.  */
  check_vector_8 (a8, abs_vector_8);
  check_vector_16 (a16, abs_vector_16);
  check_vector_32 (a32, abs_vector_32);
  check_vector_64 (a64, abs_vector_64);
  check_vector_32 (alib32, abs_vector_32);
  check_vector_long (alibl, abs_vector_long);

  return 0;
}
