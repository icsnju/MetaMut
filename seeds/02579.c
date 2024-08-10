/* { dg-do run } */
/* { dg-options "-O2" } */

/* { dg-do assemble } */
/* { dg-options "-O2 -save-temps -misa=sm_35" } */

#define MASK 0x1f

unsigned int
rotl (unsigned int val, unsigned int cnt) {
  cnt &= MASK;
  return (val << cnt) | (val >> (-cnt & MASK));
}

unsigned int
rotr (unsigned int val, unsigned int cnt) {
  cnt &= MASK;
  return (val >> cnt) | (val << (-cnt & MASK));
}

/* { dg-final { scan-assembler-times "shf.l.wrap.b32" 1 } } */
/* { dg-final { scan-assembler-times "shf.r.wrap.b32" 1 } } */
/* { dg-final { scan-assembler-not "and.b32" } } */

#define ASSERT(EXPR)				\
  do						\
    {						\
      if (!(EXPR))				\
	__builtin_abort ();			\
    } while (0)

int
main (void)
{
  ASSERT (rotl (0x12345678, 8) == 0x34567812);
  ASSERT (rotl (0x12345678, 8 + 32) == 0x34567812);

  ASSERT (rotr (0x12345678, 8) == 0x78123456);
  ASSERT (rotr (0x12345678, 8 + 32) == 0x78123456);

  return 0;
}
