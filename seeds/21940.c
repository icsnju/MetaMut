/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#define TYPE int
#define SIZE 257

void __attribute__ ((weak))
f (TYPE *x, TYPE *y, int n, long m __attribute__((unused)))
{
  for (int i = 0; i < SIZE; ++i)
    x[i * n] += y[i * n];
}

/* { dg-final { scan-assembler {\tld1w\tz[0-9]+} } } */
/* { dg-final { scan-assembler {\tst1w\tz[0-9]+} } } */
/* { dg-final { scan-assembler {\tldr\tw[0-9]+} } } */
/* { dg-final { scan-assembler {\tstr\tw[0-9]+} } } */
/* Should use a WAR check that multiplies by (VF-2)*4 rather than
   an overlap check that multiplies by (257-1)*4.  */
/* { dg-final { scan-assembler {\tcntb\t(x[0-9]+)\n.*\tsub\tx[0-9]+, \1, #8\n.*\tmul\tx[0-9]+,[^\n]*\1} } } */
/* { dg-final { scan-assembler-times {\tcsel\tx[0-9]+[^\n]*xzr} 1 } } */
/* One range check and a check for n being zero.  */
/* { dg-final { scan-assembler-times {\tcmp\t} 1 } } */
/* { dg-final { scan-assembler-times {\tccmp\t} 1 } } */
extern void abort (void) __attribute__ ((noreturn));

#define MARGIN 6

void __attribute__ ((weak, optimize ("no-tree-vectorize")))
test (int n, int m, int offset)
{
  int abs_n = (n < 0 ? -n : n);
  int abs_m = (m < 0 ? -m : m);
  int max_i = (abs_n > abs_m ? abs_n : abs_m);
  int abs_offset = (offset < 0 ? -offset : offset);
  int size = MARGIN * 2 + max_i * SIZE + abs_offset;
  TYPE *array = (TYPE *) __builtin_alloca (size * sizeof (TYPE));
  for (int i = 0; i < size; ++i)
    array[i] = i;
  int base_x = offset < 0 ? MARGIN - offset : MARGIN;
  int base_y = offset < 0 ? MARGIN : MARGIN + offset;
  int start_x = n < 0 ? base_x - n * (SIZE - 1) : base_x;
  int start_y = m < 0 ? base_y - m * (SIZE - 1) : base_y;
  f (&array[start_x], &array[start_y], n, m);
  int j = 0;
  int start = (n < 0 ? size - 1 : 0);
  int end = (n < 0 ? -1 : size);
  int inc = (n < 0 ? -1 : 1);
  for (int i = start; i != end; i += inc)
    {
      if (j == SIZE || i != start_x + j * n)
	{
	  if (array[i] != i)
	    abort ();
	}
      else if (n == 0)
	{
	  TYPE sum = i;
	  for (; j < SIZE; j++)
	    {
	      int next_y = start_y + j * m;
	      if (n >= 0 ? next_y < i : next_y > i)
		sum += array[next_y];
	      else if (next_y == i)
		sum += sum;
	      else
		sum += next_y;
	    }
	  if (array[i] != sum)
	    abort ();
	}
      else
	{
	  int next_y = start_y + j * m;
	  TYPE base = i;
	  if (n >= 0 ? next_y < i : next_y > i)
	    base += array[next_y];
	  else
	    base += next_y;
	  if (array[i] != base)
	    abort ();
	  j += 1;
	}
    }
}

int
main (void)
{
  for (int n = -10; n < 10; ++n)
    for (int offset = -33; offset <= 33; ++offset)
      test (n, n, offset);
  return 0;
}
