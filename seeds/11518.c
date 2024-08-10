/* { dg-do compile } */
/* { dg-options "-O2" } */

#define GPF float
#define SUFFIX(x) x##f

extern GPF SUFFIX(trunc) (GPF);
extern GPF SUFFIX(ceil) (GPF);
extern GPF SUFFIX(floor) (GPF);
extern GPF SUFFIX(nearbyint) (GPF);
extern GPF SUFFIX(rint) (GPF);
extern GPF SUFFIX(round) (GPF);
extern GPF SUFFIX(roundeven) (GPF);

GPF test1a (GPF x)
{
  return SUFFIX(__builtin_trunc)(x);
}

GPF test1b (GPF x)
{
  return SUFFIX(trunc)(x);
}

GPF test2a (GPF x)
{
  return SUFFIX(__builtin_ceil)(x);
}

GPF test2b (GPF x)
{
  return SUFFIX(ceil)(x);
}

GPF test3a (GPF x)
{
  return SUFFIX(__builtin_floor)(x);
}

GPF test3b (GPF x)
{
  return SUFFIX(floor)(x);
}

GPF test4a (GPF x)
{
  return SUFFIX(__builtin_nearbyint)(x);
}

GPF test4b (GPF x)
{
  return SUFFIX(nearbyint)(x);
}

GPF test5a (GPF x)
{
  return SUFFIX(__builtin_rint)(x);
}

GPF test5b (GPF x)
{
  return SUFFIX(rint)(x);
}

GPF test6a (GPF x)
{
  return SUFFIX(__builtin_round)(x);
}

GPF test6b (GPF x)
{
  return SUFFIX(round)(x);
}

GPF test7a (GPF x)
{
  return SUFFIX(__builtin_roundeven)(x);
}

GPF test7b (GPF x)
{
  return SUFFIX(roundeven)(x);
}


/* { dg-final { scan-assembler-times "frintz\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frintp\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frintm\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frinti\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frintx\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frinta\ts\[0-9\]" 2 } } */
/* { dg-final { scan-assembler-times "frintn\ts\[0-9\]" 2 } } */
