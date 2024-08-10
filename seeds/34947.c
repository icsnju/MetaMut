/* { dg-do run } */

#include <stdio.h>

struct __attribute__((scalar_storage_order("little-endian"), packed)) R1
{
  unsigned S1 : 2;
  unsigned I  : 32;
  unsigned S2 : 2;
  unsigned A1 : 9;
  unsigned A2 : 9;
  unsigned A3 : 9;
  unsigned B  : 1;
};

struct __attribute__((scalar_storage_order("big-endian"), packed)) R2
{
  unsigned S1 : 2;
  unsigned I  : 32;
  unsigned S2 : 2;
  unsigned A1 : 9;
  unsigned A2 : 9;
  unsigned A3 : 9;
  unsigned B  : 1;
};

struct R1 My_R1 = { 2, 0x12345678, 1, 0xAB, 0xCD, 0xEF, 1 };
struct R2 My_R2 = { 2, 0x12345678, 1, 0xAB, 0xCD, 0xEF, 1 };
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
  /* { dg-output "A1 : e2 59 d1 48 b4 aa d9 bb.*\n" } */

  put ("B1 :");
  dump (&B1, sizeof (struct R1));
  new_line ();
  /* { dg-output "B1 : e2 59 d1 48 b4 aa d9 bb.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 84 8d 15 9e 15 5b 35 df.*\n" } */

  put ("B2 :");
  dump (&B2, sizeof (struct R2));
  new_line ();
  /* { dg-output "B2 : 84 8d 15 9e 15 5b 35 df.*\n" } */

  if (A1.S1 != B1.S1) abort ();

  if (A1.S1 != 2) abort ();

  if (A2.S1 != B2.S1) abort ();

  if (A2.S1 != 2) abort ();

  if (A1.I != B1.I || A1.A1 != B1.A1) abort ();

  if (A2.I != B2.I || A2.A1 != B2.A1) abort ();

  new_line ();
  return 0;
}
