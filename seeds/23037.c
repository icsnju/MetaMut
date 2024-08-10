/* { dg-do run } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-slp-vectorize -save-temps" } */

int p[1000] __attribute__((aligned(8)));
int p2[1000] __attribute__((aligned(8)));

void __attribute__((noinline, noclone))
foo ()
{
  int a, b;

  int i;
  for (i = 0; i < 1000; i += 2)
    {
      a = p[i];
      b = p[i+1];

      p2[i] = a;
      p2[i+1] = b;
    }
}

/* { dg-final { scan-assembler "ld.v2.u32" } } */
/* { dg-final { scan-assembler "st.v2.u32" } } */


int
main(void)
{
  unsigned int i;
  for (i = 0; i < 1000; i += 1)
    {
      p[i] = i;
      p2[i] = 0;
    }

  foo ();

  for (i = 0; i < 1000; i += 1)
    if (p2[i] != i)
      return 1;

  return 0;
}
