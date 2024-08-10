
/* { dg-do run } */
/* { dg-options "-O3" } */

extern void abort (void);


typedef float F32;
typedef double F64;
typedef float *__restrict__ pRF32;
typedef double *__restrict__ pRF64;

extern float fabsf (float);
extern double fabs (double);

#define DEF3a(fname, type, op) \
			 void  fname##_##type (pR##type a,   \
					       pR##type b,   \
					       pR##type c)   \
			 {				     \
			   int i;			     \
			   for (i = 0; i < 16; i++)	     \
			     a[i] = op (b[i] - c[i]);	     \
			 }

#define DEF3(fname, type, op) \
			void  fname##_##type (pR##type a,   \
					      pR##type b,   \
					      pR##type c)   \
			{				    \
			  int i; 			    \
			  for (i = 0; i < 16; i++)	    \
			    a[i] = b[i] op c[i];	    \
			}

#define DEF2(fname, type, op) \
			void fname##_##type (pR##type a, \
					     pR##type b) \
			{				  \
			  int i; 			  \
			  for (i = 0; i < 16; i++)	  \
			    a[i] = op(b[i]);		  \
			}


#define DEFN3a(fname, op) \
		 DEF3a (fname, F32, op) \
		 DEF3a (fname, F64, op)

#define DEFN3(fname, op) \
		DEF3 (fname, F32, op) \
		DEF3 (fname, F64, op)

#define DEFN2(fname, op) \
		DEF2 (fname, F32, op) \
		DEF2 (fname, F64, op)

DEFN3 (add, +)
DEFN3 (sub, -)
DEFN3 (mul, *)
DEFN3 (div, /)
DEFN2 (neg, -)
DEF2 (abs, F32, fabsf)
DEF2 (abs, F64, fabs)
DEF3a (fabd, F32, fabsf)
DEF3a (fabd, F64, fabs)


#define DEFN_SETV(type) \
		void set_vector_##type (pR##type a, type n)   \
		{					      \
		  int i;				      \
		  for (i=0; i<16; i++)			      \
		    a[i] = n;				      \
		}

#define DEFN_CHECKV(type) \
		void check_vector_##type (pR##type a, pR##type vec) \
		{						    \
		  int i;					    \
		  for (i=0; i<16; i++)				    \
		    if (a[i] != vec[i])				    \
		      abort ();					    \
		}

#define TEST2(fname, type) \
			set_vector_##type (a##type, 0.0);              \
			fname##_##type (a##type, b##type);             \
			check_vector_##type (a##type, fname##_##type##_vector);

#define TEST3(fname, type) \
			set_vector_##type (a##type, 0.0);              \
			fname##_##type (a##type, b##type, c##type);    \
			check_vector_##type (a##type, fname##_##type##_vector);

#define TEST(fname, N) \
		TEST##N(fname, F32); \
		TEST##N(fname, F64);

DEFN_SETV (F32)
DEFN_SETV (F64)

DEFN_CHECKV (F32)
DEFN_CHECKV (F64)

int main (void)
{
  F32 aF32[16];
  F32 bF32[16];
  F32 cF32[16];

  F64 aF64[16];
  F64 bF64[16];
  F64 cF64[16];
  int i;

  F32  add_F32_vector[] = { 3.0f, 5.0f, 7.0f, 9.0f, 11.0f,
			    13.0f, 15.0f, 17.0f, 19.0f,
			    21.0f, 23.0f, 25.0f, 27.0f,
			    29.0f, 31.0f, 33.0f };

  F64  add_F64_vector[] = { 3.0, 5.0, 7.0, 9.0, 11.0,
			    13.0, 15.0, 17.0, 19.0,
			    21.0, 23.0, 25.0, 27.0,
			    29.0, 31.0, 33.0 };

  F32  sub_F32_vector[] = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
			    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
			    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
			    -1.0f };

  F64  sub_F64_vector[] = { -1.0, -1.0, -1.0, -1.0, -1.0,
			    -1.0, -1.0, -1.0, -1.0, -1.0,
			    -1.0, -1.0, -1.0, -1.0, -1.0,
			    -1.0 };

  F32  mul_F32_vector[] = { 2.0f, 6.0f, 12.0f, 20.0f, 30.0f,
			    42.0f, 56.0f, 72.0f, 90.0f,
			    110.0f, 132.0f, 156.0f, 182.0f,
			    210.0f, 240.0f, 272.0f };

  F64  mul_F64_vector[] = { 2.0, 6.0, 12.0, 20.0, 30.0,
			    42.0, 56.0, 72.0, 90.0,
			    110.0, 132.0, 156.0, 182.0,
			    210.0, 240.0, 272.0 };

  F32  div_F32_vector[] = { 0.5f, (float)(2.0/3.0), 0.75f, 0.8f,
			    (float)(5.0/6.0), (float)(6.0/7.0), 0.875000f,
			    (float)(8.0/9.0), 0.900000f, (float)(10.0/11.0),
			    (float)(11.0/12.0), (float)(12.0/13.0),
			    (float)(13.0/14.0), (float)(14.0/15.0), 0.937500f,
			    (float)(16.0/17.0) };

  F64  div_F64_vector[] = { 0.5, (2.0/3.0), 0.75, 0.8, (5.0/6.0),
			    (6.0/7.0), 0.875000, (8.0/9.0), 0.900000,
			    (10.0/11.0), (11.0/12.0), (12.0/13.0), (13.0/14.0),
			    (14.0/15.0), 0.937500, (16.0/17.0) };

  F32  neg_F32_vector[] = { -1.0f, -2.0f, -3.0f, -4.0f,
			    -5.0f, -6.0f, -7.0f, -8.0f,
			    -9.0f, -10.0f, -11.0f, -12.0f,
			    -13.0f, -14.0f, -15.0f, -16.0f };

  F64  neg_F64_vector[] = { -1.0, -2.0, -3.0, -4.0,
			    -5.0, -6.0, -7.0, -8.0,
			    -9.0, -10.0, -11.0, -12.0,
			    -13.0, -14.0, -15.0, -16.0 };

  F32  abs_F32_vector[] = { 1.0f, 2.0f, 3.0f, 4.0f,
			    5.0f, 6.0f, 7.0f, 8.0f,
			    9.0f, 10.0f, 11.0f, 12.0f,
			    13.0f, 14.0f, 15.0f, 16.0f };

  F64  abs_F64_vector[] = { 1.0, 2.0, 3.0, 4.0,
			    5.0, 6.0, 7.0, 8.0,
			    9.0, 10.0, 11.0, 12.0,
			    13.0, 14.0, 15.0, 16.0 };

  F32  fabd_F32_vector[] = { 1.0f, 1.0f, 1.0f, 1.0f,
			     1.0f, 1.0f, 1.0f, 1.0f,
			     1.0f, 1.0f, 1.0f, 1.0f,
			     1.0f, 1.0f, 1.0f, 1.0f };

  F64  fabd_F64_vector[] = { 1.0, 1.0, 1.0, 1.0,
			     1.0, 1.0, 1.0, 1.0,
			     1.0, 1.0, 1.0, 1.0,
			     1.0, 1.0, 1.0, 1.0 };

  /* Setup input vectors.  */
  for (i=1; i<=16; i++)
    {
      bF32[i-1] = (float)i;
      cF32[i-1] = (float)(i+1);
      bF64[i-1] = (double)i;
      cF64[i-1] = (double)(i+1);
    }

  TEST (add, 3);
  TEST (sub, 3);
  TEST (mul, 3);
  TEST (div, 3);
  TEST (neg, 2);
  TEST (abs, 2);
  TEST (fabd, 3);

  return 0;
}
