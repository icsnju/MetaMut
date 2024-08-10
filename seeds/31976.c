/* { dg-do run } */

#include <stdio.h>

struct __attribute__((scalar_storage_order("little-endian"))) R1
{
  int I;
  int A[3];
};

struct __attribute__((scalar_storage_order("big-endian"))) R2
{
  int I;
  int A[3];
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
  struct R1 Local_R1;
  struct R2 Local_R2;
  int C1;
  int C2;

  Local_R1.I    = 1;
  Local_R1.A[0] = 0xAB0012;
  Local_R1.A[1] = 0xCD0034;
  Local_R1.A[2] = 0xEF0056;
  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 01 00 00 00 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  Local_R2.I    = 1;
  Local_R2.A[0] = 0xAB0012;
  Local_R2.A[1] = 0xCD0034;
  Local_R2.A[2] = 0xEF0056;
  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 00 00 00 01 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  C1 = Local_R1.A[Local_R1.I];
  printf ("C1 : %d\n", C1);
  /* { dg-output "C1 : 13434932.*\n" } */

  Local_R1.I++;
  C1 = Local_R1.A[Local_R1.I];
  printf ("C1 : %d\n", C1);
  /* { dg-output "C1 : 15663190.*\n" } */

  C2 = Local_R2.A[Local_R2.I];
  printf ("C2 : %d\n", C2);
  /* { dg-output "C2 : 13434932.*\n" } */

  Local_R2.I++;
  C2 = Local_R2.A[Local_R2.I];
  printf ("C2 : %d\n", C2);
  /* { dg-output "C2 : 15663190.*\n" } */

  new_line ();
  return 0;
}
