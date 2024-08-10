/* PR rtl-optimization/38245 */
/* { dg-do run } */
/* { dg-additional-sources "pr38245-4.c" } */
/* { dg-options "-O2" } */

/* PR rtl-optimization/38245 */

struct B { long a, b; char p[32]; };
extern int globv;

extern int b1 (long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long)
     __attribute__((pure, noinline));
extern int b2 (long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long,
	       long long, long, long, long, long, long, long, long)
     __attribute__((const, noinline));
extern int b3 (long long, long, long, long, long, long, long, struct B,
	       long long, struct B, long, struct B, long, long, long, long,
	       long long, struct B, long, struct B, long, long, long, long,
	       long long, struct B, long, struct B, long, long, long, long)
     __attribute__((pure, noinline));
extern int b4 (long long, long, long, long, long, long, long, struct B,
	       long long, struct B, long, struct B, long, long, long, long,
	       long long, struct B, long, struct B, long, long, long, long,
	       long long, struct B, long, struct B, long, long, long, long)
     __attribute__((const, noinline));
extern int b5 () __attribute__((pure, noinline));
extern int b6 () __attribute__((const, noinline));
extern int b7 (int, int)
     __attribute__((pure, noinline));
extern int b8 (int, int)
     __attribute__((const, noinline));
extern int b9 (int, int, int, int, int, int, int)
     __attribute__((pure, noinline));
extern int b10 (int, int, int, int, int, int, int)
     __attribute__((const, noinline));

extern void abort (void);

struct A { int i, j; union { short s[4]; long long l; }; char pad[512]; } a;
int globv = 6;

void __attribute__((noinline))
f1 (void)
{
  a.s[2] = b1 (6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	       6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21);
  a.l = 6;
}

void __attribute__((noinline))
f2 (void)
{
  a.s[2] = b2 (6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	       6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21);
  a.l = 6;
}

void __attribute__((noinline))
f3 (void)
{
  struct B b = { 30, 31, { 32, 33 } };
  a.s[2] = b3 (6, 7, 8, 9, 10, 11, 12, b, 14, b, 16, b, 18, 19, 20, 21,
	       6, b, 8, b, 10, 11, 12, 13, 14, b, 16, b, 18, 19, 20, 21);
  a.l = 6;
}

void __attribute__((noinline))
f4 (void)
{
  struct B b = { 30, 31, { 32, 33 } };
  a.s[2] = b4 (6, 7, 8, 9, 10, 11, 12, b, 14, b, 16, b, 18, 19, 20, 21,
	       6, b, 8, b, 10, 11, 12, 13, 14, b, 16, b, 18, 19, 20, 21);
  a.l = 6;
}

void __attribute__((noinline))
f5 (void)
{
  a.s[2] = b5 (6.0, 7, 8, 9, 10, 11, 21.0, 22.0, 23.0);
  a.l = 6;
}

void __attribute__((noinline))
f6 (void)
{
  a.s[2] = b6 (6.0, 7, 8, 9, 10, 11, 21.0, 22.0, 23.0);
  a.l = 6;
}

void __attribute__((noinline))
f7 (void)
{
  a.s[2] = b7 (6, 7);
  a.l = 6;
}

void __attribute__((noinline))
f8 (void)
{
  a.s[2] = b8 (6, 7);
  a.l = 6;
}

void __attribute__((noinline))
f9 (void)
{
  a.s[2] = b9 (6, 7, 8, 9, 10, 11, 12);
  a.l = 6;
}

void __attribute__((noinline))
f10 (void)
{
  a.s[2] = b10 (6, 7, 8, 9, 10, 11, 12);
  a.l = 6;
}

int
main (void)
{
  char buf[256];
  int i;
  for (i = 0; i < (int) sizeof buf; i++)
    buf[i] = i;
  asm volatile ("" : : "r" (buf) : "memory");
  f1 ();
  f2 ();
  f3 ();
  f4 ();
  f5 ();
  f6 ();
  f7 ();
  f8 ();
  f9 ();
  f10 ();
  asm volatile ("" : : "r" (buf) : "memory");
  for (i = 0; i < (int) sizeof buf; i++)
    if (buf[i] != (char) i)
      abort ();
  return 0;
}
