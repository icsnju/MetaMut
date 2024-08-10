/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } } */

unsigned
foo1 (unsigned x, unsigned y, unsigned z)
{
  return x & (y - z);
}

int
foo2 (int x, int y, int z)
{
  return x & (y - z);
}

unsigned
foo3 (unsigned x, unsigned y, unsigned z)
{
  return x & (y * z);
}

int
foo4 (int x, int y, int z)
{
  return x & (y * z);
}

unsigned
foo5 (unsigned x, unsigned y)
{
  return x & (y / x);
}

int
foo6 (int x, int y)
{
  return x & (y / x);
}

unsigned
foo7 (unsigned x, unsigned y)
{
  return x & (y % x);
}

int
foo8 (int x, int y)
{
  return x & (y % x);
}

int
foo9 (int x)
{
  return x & (-x);
}

unsigned
foo10 (unsigned x, unsigned y)
{
  return x & (y + x);
}


unsigned
foo11 (unsigned x)
{
  return x & (15 + x);
}

/* { dg-final { scan-assembler-times "subw" 2 } } */
/* { dg-final { scan-assembler-times "addw" 1 } } */
/* { dg-final { scan-assembler-times "addiw" 1 } } */
/* { dg-final { scan-assembler-times "mulw" 2 } } */
/* { dg-final { scan-assembler-times "divw" 1 } } */
/* { dg-final { scan-assembler-times "divuw" 1 } } */
/* { dg-final { scan-assembler-times "remw" 1 } } */
/* { dg-final { scan-assembler-times "remuw" 1 } } */
/* { dg-final { scan-assembler-times "negw" 1 } } */
/* { dg-final { scan-assembler-not "sext.w" } } */
