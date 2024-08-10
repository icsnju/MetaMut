
/* { dg-do run } */
/* { dg-options "-O3" } */

typedef int *__restrict__ pRINT;
typedef unsigned int *__restrict__ pRUINT;
typedef long long *__restrict__ pRINT64;
typedef unsigned long long *__restrict__ pRUINT64;
extern int abs (int j);

void test_orn (pRUINT a, pRUINT b, pRUINT c)
{
  int i;
  for (i = 0; i < 16; i++)
     c[i] = a[i] | (~b[i]);
}

void test_bic (pRUINT a, pRUINT b, pRUINT c)
{
  int i;
  for (i = 0; i < 16; i++)
     c[i] = a[i] & (~b[i]);
}

void mla (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] += a[i] * b[i]; 
}

void mls (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] -= a[i] * b[i];
}

void smax (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] = (a[i] > b[i] ? a[i] : b[i]);
}

void smin (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] = (a[i] < b[i] ? a[i] : b[i]);
}

void umax (pRUINT a, pRUINT b, pRUINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] = (a[i] > b[i] ? a[i] : b[i]);
}

void umin (pRUINT a, pRUINT b, pRUINT c)
{
  int i;
  for (i=0;i<16;i++)
    c[i] = (a[i] < b[i] ? a[i] : b[i]);
}

unsigned int reduce_umax (pRUINT a)
{
  int i;
  unsigned int s = a[0];
  for (i = 1; i < 16; i++)
    s = (s > a[i] ? s : a[i]);

  return s;
}

unsigned int reduce_umin (pRUINT a)
{
  int i;
  unsigned int s = a[0];
  for (i = 1; i < 16; i++)
    s = (s < a[i] ? s : a[i]);

  return s;
}

int reduce_smax (pRINT a)
{
  int i;
  int s = a[0];
  for (i = 1; i < 16; i++)
    s = (s > a[i] ? s : a[i]);

  return s;
}

int reduce_smin (pRINT a)
{
  int i;
  int s = a[0];
  for (i = 1; i < 16; i++)
    s = (s < a[i] ? s : a[i]);

  return s;
}

unsigned int reduce_add_u32 (pRINT a)
{
  int i;
  unsigned int s = 0;
  for (i = 0; i < 16; i++)
    s += a[i];

  return s;
}

int reduce_add_s32 (pRINT a)
{
  int i;
  int s = 0;
  for (i = 0; i < 16; i++)
    s += a[i];

  return s;
}

unsigned long long reduce_add_u64 (pRUINT64 a)
{
  int i;
  unsigned long long s = 0;
  for (i = 0; i < 16; i++)
    s += a[i];

  return s;
}

long long reduce_add_s64 (pRINT64 a)
{
  int i;
  long long s = 0;
  for (i = 0; i < 16; i++)
    s += a[i];

  return s;
}

void sabd (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i = 0; i < 16; i++)
    c[i] = abs (a[i] - b[i]);
}

void saba (pRINT a, pRINT b, pRINT c)
{
  int i;
  for (i = 0; i < 16; i++)
    c[i] += abs (a[i] - b[i]);
}

extern void abort (void);

void set_vector (int *a, int n)
{
  int i;
  for (i=0; i<16; i++)
    a[i] = n;
}

void check_vector (pRINT c, pRINT result, char *str)
{
  int i;
  for (i=0; i<16 ; i++)
    if (c[i] != result[i])
      abort ();
}

#define TEST(func, sign) set_vector (sign##c, 0); \
		         func (sign##a, sign##b, sign##c);    \
		         check_vector (sign##c, func##_vector, #func);


#define TESTV(func, sign)  \
                    if (func (sign##a) != func##_value) \
		      abort ();

#define TESTVLL(func, sign)  \
                    if (func (ll##sign##a) != func##_value) \
		      abort ();
		          
int main (void)
{
  int sa[16];
  int sb[16];
  int sc[16];
  unsigned int ua[16];
  unsigned int ub[16];
  unsigned int uc[16];
  long long llsa[16];
  unsigned long long llua[16];
  int i;

  /* Table of standard values to compare against.  */
  unsigned int test_bic_vector[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned int test_orn_vector[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int mla_vector[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225};
  int mls_vector[] = {0, -1, -4, -9, -16, -25, -36, -49, -64, -81, -100, -121, -144, -169, -196, -225};
  int smax_vector[] = {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15};
  int smin_vector[] = {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15};
  unsigned int umax_vector[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  unsigned int umin_vector[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  int sabd_vector[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int saba_vector[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int reduce_smax_value = 0;
  int reduce_smin_value = -15;
  unsigned int reduce_umax_value = 15;
  unsigned int reduce_umin_value = 0;
  unsigned int reduce_add_u32_value = 120;
  int reduce_add_s32_value = -120;
  long long reduce_add_s64_value = -120;
  unsigned long long reduce_add_u64_value = 120;

  /* Set up input vectors.  */
  for (i=0; i < 16; i++)
    {
      sa[i] = sb[i] = -i;
      llsa[i] = (long long)-i;
      ua[i] = ub[i] = i;
      llua[i] = (unsigned long long)i;
    }

  TEST (test_bic, s);
  TEST (test_orn, s);
  TEST (mla, s);
  TEST (mls, s);
  TEST (smax, s);
  TEST (smin, s);
  TEST (umax, u);
  TEST (umin, u);
  TEST (sabd, s);
  TEST (saba, s);
  TESTV (reduce_smax, s);
  TESTV (reduce_smin, s);
  TESTV (reduce_umax, u);
  TESTV (reduce_umin, u);
  TESTV (reduce_add_u32, u);
  TESTV (reduce_add_s32, s);
  TESTVLL (reduce_add_u64, u);
  TESTVLL (reduce_add_s64, s);
  return 0;
}
