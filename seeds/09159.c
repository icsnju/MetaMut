
/* { dg-do run } */
/* { dg-options "-O3" } */

#include "limits.h"

extern void abort (void);

#define N 16


typedef signed char *__restrict__ pRSINT8;
typedef signed short *__restrict__ pRSINT16;
typedef signed int *__restrict__ pRSINT32;
typedef signed long long *__restrict__ pRSINT64;

typedef unsigned char *__restrict__ pRUINT8;
typedef unsigned short *__restrict__ pRUINT16;
typedef unsigned int *__restrict__ pRUINT32;
typedef unsigned long long *__restrict__ pRUINT64;

#undef SH /* Including limits.h on vxworks defines this.  */
typedef signed short SH;
typedef unsigned short UH;
typedef signed int SS;
typedef unsigned int US;
typedef signed long long SLL;
typedef unsigned long long ULL;

#define DEF_MULLB(sign)   \
		   void widen_mult_##sign##b  (pR##sign##INT##16 a, \
					       pR##sign##INT##8 b, \
					       pR##sign##INT##8 c) \
		   { \
			int i;  \
			for (i=0; i<N; i++)     \
			  a[i] = (sign##H)b[i] * c[i];   \
		   }

#define DEF_MULLH(sign)   \
		   void widen_mult_##sign##h (pR##sign##INT##32 a, \
					      pR##sign##INT##16 b, \
					      pR##sign##INT##16 c) \
		   { \
			int i;  \
			for (i=0; i<N; i++)     \
			  a[i] = (sign##S)b[i] * c[i];   \
		   }
#define DEF_MULLS(sign)   \
		   void widen_mult_##sign##s (pR##sign##INT##64 a, \
					      pR##sign##INT##32 b, \
					      pR##sign##INT##32 c) \
		   { \
			int i;  \
			for (i=0; i<N; i++)     \
			  a[i] = (sign##LL)b[i] * c[i];   \
		   }

#define DEF_MULL2(x) x (S) \
		     x (U)

#define SET_VEC(size, type, sign)  \
			    void set_vector_##sign##size \
						   (pR##sign##INT##size b, \
						    pR##sign##INT##size c) \
		      	    {				        \
			      int i;			        \
			      for (i=0; i<N; i++)	        \
			      { \
			        b[i] = (type)((INT_MAX >> (32 - size)) - i); \
			        c[i] = (type)((INT_MAX >> (32 - size)) - i * 2); \
			      } \
		      	    }

#define CHECK_VEC(size, sign) void check_vector_##sign##size (pR##sign##INT##size a, \
						  pR##sign##INT##size b) \
			{				       \
			  int i;			       \
			  for (i=0; i<N; i++)		       \
			    if (a[i] != b[i])  		       \
			      abort (); 		       \
			}

SET_VEC (8,  signed char,  S)
SET_VEC (16, signed short, S)
SET_VEC (32, signed int,   S)

SET_VEC (8,  unsigned char,  U)
SET_VEC (16, unsigned short, U)
SET_VEC (32, unsigned int,   U)

DEF_MULL2 (DEF_MULLB)
DEF_MULL2 (DEF_MULLH)
DEF_MULL2 (DEF_MULLS)

CHECK_VEC (8, S)
CHECK_VEC (8, U)
CHECK_VEC (16, S)
CHECK_VEC (16, U)
CHECK_VEC (32, S)
CHECK_VEC (32, U)
CHECK_VEC (64, S)
CHECK_VEC (64, U)

int main (void)
{

#define DECL_VAR(name) signed char name##_S8[N]; \
		       signed short name##_S16[N]; \
		       signed int name##_S32[N]; \
		       unsigned char name##_U8[N]; \
		       unsigned short name##_U16[N]; \
		       unsigned int name##_U32[N];

  DECL_VAR (output);
  signed long long output_S64[N];
  unsigned long long output_U64[N];

  DECL_VAR (input1);
  DECL_VAR (input2);

  signed short expected_S16[] =
		    { 16129, 15750, 15375, 15004, 14637, 14274, 13915, 13560,
		      13209, 12862, 12519, 12180, 11845, 11514, 11187, 10864 };

  signed int expected_S32[] =
		    { 1073676289, 1073577990, 1073479695, 1073381404, 1073283117,
		      1073184834, 1073086555, 1072988280, 1072890009, 1072791742,
		      1072693479, 1072595220, 1072496965, 1072398714, 1072300467,
		      1072202224 };

  signed long long expected_S64[] =
		    { 4611686014132420609LL, 4611686007689969670LL,
		      4611686001247518735LL, 4611685994805067804LL,
		      4611685988362616877LL, 4611685981920165954LL,
		      4611685975477715035LL, 4611685969035264120LL,
		      4611685962592813209LL, 4611685956150362302LL,
		      4611685949707911399LL, 4611685943265460500LL,
		      4611685936823009605LL, 4611685930380558714LL,
		      4611685923938107827LL, 4611685917495656944LL };

  unsigned short expected_U16[] =
		    { 16129, 15750, 15375, 15004, 14637, 14274, 13915, 13560,
		      13209, 12862, 12519, 12180, 11845, 11514, 11187, 10864 };

  unsigned int expected_U32[] =
		    { 1073676289, 1073577990, 1073479695, 1073381404, 1073283117,
		      1073184834, 1073086555, 1072988280, 1072890009, 1072791742,
		      1072693479, 1072595220, 1072496965, 1072398714, 1072300467,
		      1072202224 };

  unsigned long long expected_U64[] =
		    { 4611686014132420609ULL, 4611686007689969670ULL,
		      4611686001247518735ULL, 4611685994805067804ULL,
		      4611685988362616877ULL, 4611685981920165954ULL,
		      4611685975477715035ULL, 4611685969035264120ULL,
		      4611685962592813209ULL, 4611685956150362302ULL,
		      4611685949707911399ULL, 4611685943265460500ULL,
		      4611685936823009605ULL, 4611685930380558714ULL,
		      4611685923938107827ULL, 4611685917495656944ULL };

  /* Set up input.  */
  set_vector_S8 (input1_S8, input2_S8);
  set_vector_S16 (input1_S16, input2_S16);
  set_vector_S32 (input1_S32, input2_S32);
  set_vector_U8 (input1_U8, input2_U8);
  set_vector_U16 (input1_U16, input2_U16);
  set_vector_U32 (input1_U32, input2_U32);

  /* Calculate actual results.  */
  widen_mult_Sb (output_S16, input1_S8, input2_S8);
  widen_mult_Sh (output_S32, input1_S16, input2_S16);
  widen_mult_Ss (output_S64, input1_S32, input2_S32);
  widen_mult_Ub (output_U16, input1_U8, input2_U8);
  widen_mult_Uh (output_U32, input1_U16, input2_U16);
  widen_mult_Us (output_U64, input1_U32, input2_U32);

  /* Check actual vs. expected.  */
  check_vector_S16 (expected_S16, output_S16);
  check_vector_S32 (expected_S32, output_S32);
  check_vector_S64 (expected_S64, output_S64);
  check_vector_U16 (expected_U16, output_U16);
  check_vector_U32 (expected_U32, output_U32);
  check_vector_U64 (expected_U64, output_U64);

  return 0;
}
