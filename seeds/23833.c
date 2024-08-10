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

#ifdef __cplusplus
extern "C"
#endif
void abort (void);

int main (void)
{
  struct R1 A1 = My_R1;
  struct R1 B1 = My_R1;

  struct R2 A2 = My_R2;
  struct R2 B2 = My_R2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();
  /* { dg-output "A1 : 78 56 34 12 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  put ("B1 :");
  dump (&B1, sizeof (struct R1));
  new_line ();
  /* { dg-output "B1 : 78 56 34 12 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 12 34 56 78 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  put ("B2 :");
  dump (&B2, sizeof (struct R2));
  new_line ();
  /* { dg-output "B2 : 12 34 56 78 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  if (A1.I != B1.I || A1.A[0] != B1.A[0]) abort();

  if (A2.I != B2.I || A2.A[0] != B2.A[0]) abort ();

  new_line ();
  return 0;
}
