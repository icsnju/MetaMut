/* { dg-do compile } */
/* { dg-options "-O2" } */

#define GPF double
#define SUFFIX(x) x
#define GPI long

extern GPF SUFFIX(trunc) (GPF);
extern GPF SUFFIX(ceil) (GPF);
extern GPF SUFFIX(floor) (GPF);
extern GPF SUFFIX(round) (GPF);

GPI test1a (GPF x) {
  return SUFFIX(__builtin_trunc)(x);
}

GPI test1b (GPF x)
{
  return SUFFIX(trunc)(x);
}

GPI test2a (GPF x)
{
  return SUFFIX(__builtin_lceil)(x);
}

GPI test2b (GPF x)
{
  return SUFFIX(ceil)(x);
}

GPI test2c (GPF x)
{
  return SUFFIX(__builtin_ceil)(x);
}

GPI test3a (GPF x)
{
  return SUFFIX(__builtin_lfloor)(x);
}

GPI test3b (GPF x)
{
  return SUFFIX(floor)(x);
}

GPI test3c (GPF x)
{
  return SUFFIX(__builtin_floor)(x);
}

GPI test4a (GPF x)
{
  return SUFFIX(__builtin_round)(x);
}

GPI test4b (GPF x)
{
  return SUFFIX(round)(x);
}



/* { dg-final { scan-assembler-times "fcvtzs\tx\[0-9\]+, *d\[0-9\]" 2 {target lp64} } } */
/* { dg-final { scan-assembler-times "fcvtzs\tw\[0-9\]+, *d\[0-9\]" 2 {target ilp32} } } */
/* { dg-final { scan-assembler-times "fcvtps\tx\[0-9\]+, *d\[0-9\]" 3 {target lp64} } } */
/* { dg-final { scan-assembler-times "fcvtps\tw\[0-9\]+, *d\[0-9\]" 3 {target ilp32} } } */
/* { dg-final { scan-assembler-times "fcvtms\tx\[0-9\]+, *d\[0-9\]" 3 {target lp64} } } */
/* { dg-final { scan-assembler-times "fcvtms\tw\[0-9\]+, *d\[0-9\]" 3 {target ilp32} } } */
/* { dg-final { scan-assembler-times "fcvtas\tx\[0-9\]+, *d\[0-9\]" 2 {target lp64} } } */
/* { dg-final { scan-assembler-times "fcvtas\tw\[0-9\]+, *d\[0-9\]" 2 {target ilp32} } } */
