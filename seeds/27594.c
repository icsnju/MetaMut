/* { dg-do run } */
/* { dg-options "-O1" } */

/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-sccp-details" } */
/* { dg-final { scan-tree-dump-times {final value replacement} 12 "sccp" } } */

unsigned short
__attribute__((noipa))
foo (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp &= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo1 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp &= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo2 (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp &= (1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo3 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp &= (1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo4 (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp |= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo5 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp |= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo6 (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp |= (1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo7 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp |= (1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo8 (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp ^= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo9 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp ^= ~(1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo10 (unsigned short tmp)
{
  for (int bit = 0; bit < 16; bit += 3)
    tmp ^= (1U << bit);
  return tmp;
}

unsigned short
__attribute__((noipa))
foo11 (unsigned short tmp)
{
  for (int bit = 15; bit >= 0; bit -= 3)
    tmp ^= (1U << bit);
  return tmp;
}

int main()
{
  unsigned short tmp = 0x1111U;

  if (foo (tmp) != 0x110)
    __builtin_abort ();

  if (foo1 (tmp) != 0x110)
    __builtin_abort ();

  if (foo2 (tmp) != 0x0)
    __builtin_abort ();

  if (foo3 (tmp) != 0x0)
    __builtin_abort ();

  if (foo4 (tmp) != 0xffff)
    __builtin_abort ();

  if (foo5 (tmp) != 0xffff)
    __builtin_abort ();

  if (foo6 (tmp) != 0x9359)
    __builtin_abort ();

  if (foo7 (tmp) != 0x9359)
    __builtin_abort ();

  if (foo8 (tmp) != 0x8358)
    __builtin_abort ();

  if (foo9 (tmp) != 0x8358)
    __builtin_abort ();

  if (foo10 (tmp) != 0x8358)
    __builtin_abort ();

  if (foo11 (tmp) != 0x8358)
    __builtin_abort ();
}

