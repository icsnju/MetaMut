/* { dg-do run } */

#include <stdio.h>

struct __attribute__((scalar_storage_order("big-endian"))) Nested1
{
  int A[3];
};

struct __attribute__((scalar_storage_order("little-endian"))) R1
{
  int I;
  struct Nested1 N;
};

struct __attribute__((scalar_storage_order("little-endian"))) Nested2
{
  int A[3];
};

struct __attribute__((scalar_storage_order("big-endian"))) R2
{
  int I;
  struct Nested2 N;
};

struct R1 My_R1 = { 0x12345678, { { 0xAB0012, 0xCD0034, 0xEF0056 } } };
struct R2 My_R2 = { 0x12345678, { { 0xAB0012, 0xCD0034, 0xEF0056 } } };
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
  struct R1 Local_R1;
  struct R2 Local_R2;

  put ("My_R1    :");
  dump (&My_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "My_R1    : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  put ("My_R2    :");
  dump (&My_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "My_R2    : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  Local_R1 = My_R1;
  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  Local_R2 = My_R2;
  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  Local_R1.I      = 0x12345678;
  Local_R1.N.A[0] = 0xAB0012;
  Local_R1.N.A[1] = 0xCD0034;
  Local_R1.N.A[2] = 0xEF0056;
  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  Local_R2.I      = 0x12345678;
  Local_R2.N.A[0] = 0xAB0012;
  Local_R2.N.A[1] = 0xCD0034;
  Local_R2.N.A[2] = 0xEF0056;
  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  Local_R1.I      = Local_R2.I;
  Local_R1.N.A[0] = Local_R2.N.A[0];
  Local_R1.N.A[1] = Local_R2.N.A[1];
  Local_R1.N.A[2] = Local_R2.N.A[2];

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  Local_R2.I      = Local_R1.I;
  Local_R2.N.A[0] = Local_R1.N.A[0];
  Local_R2.N.A[1] = Local_R1.N.A[1];
  Local_R2.N.A[2] = Local_R1.N.A[2];

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  new_line ();
  return 0;
}
