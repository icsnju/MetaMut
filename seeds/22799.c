/* PR tree-optimization/88676 */
/* { dg-do run } */
/* { dg-options "-O2" } */

/* PR tree-optimization/88676 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-not " = PHI <" "optimized" } } */

void bar (int, int, int);

__attribute__((noipa)) int
f1 (unsigned x)
{
  int r;
  if (x >= 2)
    __builtin_unreachable ();
  switch (x)
    {
    case 0:
      r = 1;
      break;
    case 1:
      r = 2;
      break;
    default:
      r = 0;
      break;
    }
  return r;
}

__attribute__((noipa)) void
f2 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x != 98)
    y = 115;
  else
    y = 116;
  bar (x, y, 116);
}

__attribute__((noipa)) void
f3 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x == 98)
    y = 115;
  else
    y = 116;
  bar (x, y, 115);
}

__attribute__((noipa)) void
f4 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x != 99)
    y = 115;
  else
    y = 116;
  bar (x, y, 115);
}

__attribute__((noipa)) void
f5 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x == 99)
    y = 115;
  else
    y = 116;
  bar (x, y, 116);
}

__attribute__((noipa)) void
f6 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x != 98)
    y = 116;
  else
    y = 115;
  bar (x, y, 115);
}

__attribute__((noipa)) void
f7 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x == 98)
    y = 116;
  else
    y = 115;
  bar (x, y, 116);
}

__attribute__((noipa)) void
f8 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x != 99)
    y = 116;
  else
    y = 115;
  bar (x, y, 116);
}

__attribute__((noipa)) void
f9 (int x)
{
  int y;
  x = (x & 1) + 98;
  if (x == 99)
    y = 116;
  else
    y = 115;
  bar (x, y, 115);
}

__attribute__((noipa)) int
f10 (int x)
{
  x = (x & 1) + 36;
  if (x == 36)
    return 85;
  else
    return 84;
}

__attribute__((noipa)) void
bar (int x, int y, int z)
{
  if (z != 115 && z != 116)
    __builtin_abort ();
  if (x == 98)
    {
      if (y != z)
	__builtin_abort ();
    }
  else if (x != 99)
    __builtin_abort ();
  else if (z == 115)
    {
      if (y != 116)
	__builtin_abort ();
    }
  else if (y != 115)
    __builtin_abort ();
}

int
main ()
{
  if (f1 (0) != 1 || f1 (1) != 2)
    __builtin_abort ();
  int i;
  for (i = -12; i < 12; i++)
    {
      f2 (i);
      f3 (i);
      f4 (i);
      f5 (i);
      f6 (i);
      f7 (i);
      f8 (i);
      f9 (i);
      if (f10 (i) != ((i & 1) ? 84 : 85))
	__builtin_abort ();
    }
  return 0;
}
