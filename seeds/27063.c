/* PR tree-opt/109938 */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-dse2 -Wno-psabi" } */

typedef int v4si __attribute__((vector_size(4 * sizeof(int))));

/* Generic */
__attribute__((noipa)) int
t1 (int a, int b, int c)
{
  return ((a ^ c) & b) | a;
}

__attribute__((noipa)) unsigned int
t2 (int a, unsigned int b, int c)
{
  return ((a ^ c) & b) | a;
}

__attribute__((noipa)) unsigned long
t3 (unsigned long a, long b, unsigned long c)
{
  return ((a ^ c) & b) | a;
}

__attribute__((noipa)) unsigned short
t4 (short a, unsigned short b, unsigned short c)
{
  return (unsigned short) ((a ^ c) & b) | a;
}

__attribute__((noipa)) unsigned char
t5 (unsigned char a, signed char b, signed char c)
{
  return ((a ^ c) & b) | a;
}

__attribute__((noipa)) long long
t6 (long long a, long long b, long long c)
{
  return ((a ^ c) & (unsigned long long) b) | a;
}

/* Gimple */
__attribute__((noipa)) int
t7 (int a, int b, int c)
{
  int t1 = a ^ c;
  int t2 = t1 & b;
  int t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) int
t8 (int a, unsigned int b, unsigned int c)
{
  unsigned int t1 = a ^ c;
  int t2 = t1 & b;
  int t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) unsigned int
t9 (unsigned int a, unsigned int b, int c)
{
  unsigned int t1 = a ^ c;
  unsigned int t2 = t1 & b;
  unsigned int t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) unsigned long
t10 (unsigned long a, long b, unsigned long c)
{
  unsigned long t1 = a ^ c;
  unsigned long t2 = t1 & b;
  unsigned long t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) unsigned short
t11 (short a, unsigned short b, short c)
{
  short t1 = a ^ c;
  unsigned short t2 = t1 & b;
  unsigned short t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) unsigned char
t12 (signed char a, unsigned char b, signed char c)
{
  unsigned char t1 = a ^ c;
  unsigned char t2 = t1 & b;
  unsigned char t3 = t2 | a;
  return t3;
}

__attribute__((noipa)) unsigned long long
t13 (unsigned long long a, long long b, unsigned long long c)
{
  long long t1 = a ^ c;
  long long t2 = t1 & b;
  unsigned long long t3 = t2 | a;
  return t3;
}

/* Vectors */
__attribute__((noipa)) v4si
t14 (v4si a, v4si b, v4si c)
{
  return ((a ^ c) & b) | a;
}

__attribute__((noipa)) v4si
t15 (v4si a, v4si b, v4si c)
{
  v4si t1 = a ^ c;
  v4si t2 = t1 & b;
  v4si t3 = t2 | a;
  return t3;
}

/* { dg-final { scan-tree-dump-not " \\\^ " "dse2" } } */
/* { dg-final { scan-tree-dump-times " \\\| " 15 "dse2" } } */
/* { dg-final { scan-tree-dump-times " & " 15 "dse2" } } */

int 
main ()
{
  if (t1 (29789, 29477, 23942) != 30045) __builtin_abort ();
  if (t2 (-20196, 18743, -32901) != -1729) __builtin_abort ();
  if (t3 (2136614690L, 1136698390L, 2123767997L) != 2145003318UL) __builtin_abort ();
  if (t4 (-4878, 9977, 23313) != 61171) __builtin_abort ();
  if (t5 (127, 99, 43) != 127) __builtin_abort ();
  if (t6 (9176690219839792930LL, 3176690219839721234LL, 5671738468274920831LL)
      != 9177833729112616754LL) __builtin_abort ();
  if (t7 (29789, 29477, 23942) != 30045) __builtin_abort ();
  if (t8 (23489, 99477, 87942) != 90053) __builtin_abort ();
  if (t9 (10489, 66477, -73313) != 10749) __builtin_abort ();
  if (t10 (2136614690L, -1136614690L, 4136614690UL) != 4284131106UL)
    __builtin_abort ();
  if (t11 (29789, 29477, 12345) != 29821) __builtin_abort ();
  if (t12 (-120, 98, -73) != 170) __builtin_abort ();
  if (t13 (9176690219839792930ULL, -3176690219839721234LL, 5671738468274920831ULL)
      != 9221726284835125102ULL) __builtin_abort ();
  v4si a1 = {29789, -20196, 23489, 10489};
  v4si a2 = {29477, 18743, 99477, 66477}; 
  v4si a3 = {23942, -32901, 87942, -73313};
  v4si r1 = {30045, 63807, 90053, 10749}; 
  v4si b1 = t14 (a1, a2, a3);
  v4si b2 = t15 (a1, a2, a3);
  if (__builtin_memcmp (&b1,  &r1,  sizeof (b1) != 0)) __builtin_abort();	
  if (__builtin_memcmp (&b2,  &r1,  sizeof (b2) != 0)) __builtin_abort();
  return 0;
}
