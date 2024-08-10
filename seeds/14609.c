/* PR middle-end/109986 */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-dse1 -Wno-psabi" } */

typedef int v4si __attribute__((vector_size(16)));

/* Generic */
__attribute__((noipa)) int 
t1 (int a, int b)
{
  return (~a | b) ^ a;
}

__attribute__((noipa)) unsigned int 
t2 (int a, int b)
{
  return a ^ (~a | (unsigned int) b);
}

__attribute__((noipa)) signed char
t3 (signed char a, signed char b)
{
  return (b | ~a) ^ a;
}

__attribute__((noipa)) unsigned char
t4 (signed char a, signed char b)
{
  return ((unsigned char) a) ^ (b | ~a);
}

__attribute__((noipa)) short
t5 (short a, short b)
{
  return a ^ (b | ~a);
}

__attribute__((noipa)) unsigned short
t6 (short a, short b)
{
  return ((unsigned short) a) ^ (b | ~a);
}

__attribute__((noipa)) long
t7 (long a, long b)
{
  return a ^ (b | ~a);
}

__attribute__((noipa)) unsigned long
t8 (long a, long b)
{
  return ((unsigned long) a) ^ (b | ~a);
}

__attribute__((noipa)) long long
t9 (long long a, long long b)
{
  return a ^ (b | ~a);
}

__attribute__((noipa)) unsigned long long
t10 (long long a, long long b)
{
  return ((unsigned long long) a) ^ (b | ~a);
}

__attribute__((noipa)) v4si
t21 (v4si a, v4si b)
{
  return a ^ (b | ~a);
}

/* Gimple */
__attribute__((noipa)) int 
t11 (int a, int b)
{
  int t1 = ~a;
  int t2 = t1 | b;
  int t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) unsigned int
t12 (int a, unsigned int b)
{
  int t1 = ~a;
  unsigned int t2 = t1 | b;
  unsigned int t3 = a ^ t2;
  return t3;
}

__attribute__((noipa)) signed char
t13 (signed char a, signed char b)
{
  signed char t1 = ~a;
  signed char t2 = b | t1;
  signed char t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) unsigned char
t14 (unsigned char a, signed char b)
{
  unsigned char t1 = ~a;
  signed char t2 = b | t1;
  unsigned char t3 = a ^ t2;
  return t3;
}

__attribute__((noipa)) short 
t15 (short a, short b)
{
  short t1 = ~a;
  short t2 = t1 | b;
  short t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) unsigned short 
t16 (unsigned short a, short b)
{
  short t1 = ~a;
  short t2 = t1 | b;
  unsigned short t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) long 
t17 (long a, long b)
{
  long t1 = ~a;
  long t2 = t1 | b;
  long t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) unsigned long 
t18 (long a, unsigned long b)
{
  long t1 = ~a;
  unsigned long t2 = t1 | b;
  unsigned long t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) long long 
t19 (long long a, long long b)
{
  long long t1 = ~a;
  long long t2 = t1 | b;
  long long t3 = t2 ^ a;
  return t3;
}

__attribute__((noipa)) unsigned long long 
t20 (long long a, long long b)
{
  long long t1 = ~a;
  long long t2 = t1 | b;
  unsigned long long t3 = a ^ t2;
  return t3;
}

__attribute__((noipa)) v4si
t22 (v4si a, v4si b)
{
  v4si t1 = ~a;
  v4si t2 = t1 | b;
  v4si t3 = a ^ t2;
  return t3;
}

/* { dg-final { scan-tree-dump-not " \\\| " "dse1" } } */
/* { dg-final { scan-tree-dump-not " \\\^ " "dse1" } } */
/* { dg-final { scan-tree-dump-times " ~" 22 "dse1" } } */
/* { dg-final { scan-tree-dump-times " & " 22 "dse1" } } */


int 
main ()
{
  if (t1 (29789, 29477) != -28678) __builtin_abort ();
  if (t2 (20196, -18743) != 4294965567) __builtin_abort ();
  if (t3 (127, 99) != -100) __builtin_abort ();
  if (t4 (100, 53) != 219) __builtin_abort ();
  if (t5 (20100, 1283) != -1025) __builtin_abort ();
  if (t6 (20100, 10283) != 63487) __builtin_abort ();
  if (t7 (2136614690L, 1136698390L) != -1128276995L) __builtin_abort ();
  if (t8 (1136698390L, 2136614690L) != -1128276995UL) __builtin_abort ();
  if (t9 (9176690219839792930LL, 3176690219839721234LL) != -3175044472123688707LL)
    __builtin_abort ();
  if (t10 (9176690219839792930LL, 3176690219839721234LL) != 15271699601585862909ULL)
    __builtin_abort ();
  if (t11 (29789, 29477) != -28678) __builtin_abort ();
  if (t12 (20196, -18743) != 4294965567) __builtin_abort ();
  if (t13 (127, 99) != -100) __builtin_abort ();
  if (t14 (100, 53) != 219) __builtin_abort ();
  if (t15 (20100, 1283) != -1025) __builtin_abort ();
  if (t16 (20100, 10283) != 63487) __builtin_abort ();
  if (t17 (2136614690, 1136698390) != -1128276995) __builtin_abort ();
  if (t18 (1136698390L, 2136614690L) != -1128276995UL) __builtin_abort ();
  if (t19 (9176690219839792930LL, 3176690219839721234LL) != -3175044472123688707LL)
    __builtin_abort ();
  if (t20 (9176690219839792930LL, 3176690219839721234LL) != 15271699601585862909ULL)
    __builtin_abort ();
  v4si a1 = {1, 2, 3, 4};
  v4si a2 = {6, 7, 8, 9}; 
  v4si r1 = {-1, -3, -1, -1}; 
  v4si b1 = t21 (a1, a2);
  v4si b2 = t22 (a1, a2);
  if (__builtin_memcmp (&b1,  &r1,  sizeof (b1) != 0)) __builtin_abort();	
  if (__builtin_memcmp (&b2,  &r1,  sizeof (b2) != 0)) __builtin_abort();
  return 0;
}

