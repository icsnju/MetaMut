/* { dg-do run } */

#include <stdio.h>

struct __attribute__((scalar_storage_order("big-endian"))) Nested1
{
  int C1;
  int C2;
  int C3;
};

struct __attribute__((scalar_storage_order("little-endian"))) R1
{
  int I;
  struct Nested1 N;
};

struct __attribute__((scalar_storage_order("little-endian"))) Nested2
{
  int C1;
  int C2;
  int C3;
};

struct __attribute__((scalar_storage_order("big-endian"))) R2
{
  int I;
  struct Nested2 N;
};

struct R1 My_R1 = { 0x12345678, { 0xAB0012, 0xCD0034, 0xEF0056 } };
struct R2 My_R2 = { 0x12345678, { 0xAB0012, 0xCD0034, 0xEF0056 } };
#ifdef __nvptx__
/* Force function call.  NVPTX's IO is extremely limited.  */
#undef putchar
#endif

void dump (void *p, unsigned int len)
{
  const char digits[17] = "0123456789abcdef";
  unsigned char *a = (unsigned char *)p;
  int i;

  for (i = 0; i < len; i++)
    {
      putchar (' ');
      putchar (digits[a[i] / 16]);
      putchar (digits[a[i] % 16]);
    }
}

void put (const char s[])
{
  int i;
  for (i = 0; s[i]; i++)
    putchar (s[i]);
}

void new_line (void)
{
  putchar ('\n');
}

int main (void)
{
  struct R1 A1 = My_R1;
  struct R2 A2 = My_R2;

  struct Nested1 N1;
  struct Nested2 N2;

  int C1;
  int C2;
  int C3;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();
  /* { dg-output "A1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  N1 = A1.N;
  C1 = N1.C1;
  C2 = N1.C2;
  C3 = N1.C3;

  printf ("C1 : %d\n", C1);
  /* { dg-output "C1 : 11206674.*\n" } */

  printf ("C2 : %d\n", C2);
  /* { dg-output "C2 : 13434932.*\n" } */

  printf ("C3 : %d\n", C3);
  /* { dg-output "C3 : 15663190.*\n" } */

  N1.C1 = C1;
  N1.C2 = C2;
  N1.C3 = C3;
  A1.N = N1;

  N2 = A2.N;
  C1 = N2.C1;
  C2 = N2.C2;
  C3 = N2.C3;

  printf ("C1 : %d\n", C1);
  /* { dg-output "C1 : 11206674.*\n" } */

  printf ("C2 : %d\n", C2);
  /* { dg-output "C2 : 13434932.*\n" } */

  printf ("C3 : %d\n", C3);
  /* { dg-output "C3 : 15663190.*\n" } */

  N2.C1 = C1;
  N2.C2 = C2;
  N2.C3 = C3;
  A2.N = N2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();
  /* { dg-output "A1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  new_line ();
  return 0;
}