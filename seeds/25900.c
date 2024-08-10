/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64 -O2 -fomit-frame-pointer -mno-omit-leaf-frame-pointer -fno-inline" } */

int inc(int n)
{
  return n + 1;
}


int bar(void)
{
  int n = 100;
  n = inc(n);
  n = inc(n) + 100;
  return n;
}

/* { dg-final { scan-assembler-times "sd\tra,\[0-9\]+\\(sp\\)" 1 } } */
/* { dg-final { scan-assembler-not "sd\ts0,\[0-9\]+\\(sp\\)"} } */
