/* { dg-do run } */

#include <stdio.h>

struct __attribute__((scalar_storage_order("little-endian"), packed)) Nested1
{
  unsigned C1 : 7;
  unsigned C2 : 7;
  unsigned C3 : 7;
  unsigned B  : 3;
};

struct __attribute__((scalar_storage_order("little-endian"), packed)) R1
{
  unsigned S1 : 6;
  unsigned I  : 32;
  unsigned S2 : 2;
  struct Nested1 N;
};

struct __attribute__((scalar_storage_order("big-endian"), packed)) Nested2
{
  unsigned C1 : 7;
  unsigned C2 : 7;
  unsigned C3 : 7;
  unsigned B  : 3;
};

struct __attribute__((scalar_storage_order("big-endian"), packed)) R2
{
  unsigned S1 : 6;
  unsigned I  : 32;
  unsigned S2 : 2;
  struct Nested2 N;
};

struct R1 My_R1 = { 2, 0x78ABCDEF, 1, { 0x12, 0x34, 0x56, 4 } };
struct R2 My_R2 = { 2, 0x78ABCDEF, 1, { 0x12, 0x34, 0x56, 4 } };
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
  /* { dg-output "A1 : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  put ("B1 :");
  dump (&B1, sizeof (struct R1));
  new_line ();
  /* { dg-output "B1 : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  put ("B2 :");
  dump (&B2, sizeof (struct R2));
  new_line ();
  /* { dg-output "B2 : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  if (A1.S1 != B1.S1) abort ();

  if (A1.S1 != 2) abort ();

  if (A2.S1 != B2.S1) abort ();

  if (A2.S1 != 2) abort ();

  if (A1.I != B1.I || A1.N.C1 != B1.N.C1) abort ();

  if (A2.I != B2.I || A2.N.C1 != B2.N.C1) abort ();

  new_line ();
  return 0;
}
