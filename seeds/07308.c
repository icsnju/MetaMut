/* { dg-do run } */
/* { dg-options "-O1" } */

/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-sccp-details" } */
/* { dg-final { scan-tree-dump-times {final value replacement} 12 "sccp" } } */

unsigned long long
__attribute__((noipa))
foo (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp &= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo1 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp &= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo2 (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp &= (1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo3 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp &= (1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo4 (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp |= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo5 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp |= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo6 (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp |= (1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo7 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp |= (1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo8 (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp ^= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo9 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp ^= ~(1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo10 (unsigned long long tmp)
{
  for (int bit = 0; bit < 64; bit += 3)
    tmp ^= (1ULL << bit);
  return tmp;
}

unsigned long long
__attribute__((noipa))
foo11 (unsigned long long tmp)
{
  for (int bit = 63; bit >= 0; bit -= 3)
    tmp ^= (1ULL << bit);
  return tmp;
}

int main()
{
  unsigned long long tmp = 0x1111111111111111ULL;
  if (foo (tmp) != 0x110110110110110ULL)
    __builtin_abort ();

  if (foo1 (tmp) != 0x110110110110110ULL)
    __builtin_abort ();

  if (foo2 (tmp) != 0x0ULL)
    __builtin_abort ();

  if (foo3 (tmp) != 0x0ULL)
    __builtin_abort ();

  if (foo4 (tmp) != 0xffffffffffffffffULL)
    __builtin_abort ();

  if (foo5 (tmp) != 0xffffffffffffffffULL)
    __builtin_abort ();

  if (foo6 (tmp) != 0x9359359359359359ULL)
    __builtin_abort ();

  if (foo7 (tmp) != 0x9359359359359359ULL)
    __builtin_abort ();

  if (foo8 (tmp) != 0x8358358358358358ULL)
    __builtin_abort ();

  if (foo9 (tmp) != 0x8358358358358358ULL)
    __builtin_abort ();

  if (foo10 (tmp) != 0x8358358358358358ULL)
    __builtin_abort ();

  if (foo11 (tmp) != 0x8358358358358358ULL)
    __builtin_abort ();
}

