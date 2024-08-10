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

int main (void)
{
  struct R1 Local_R1;
  struct R2 Local_R2;

  put ("My_R1    :");
  dump (&My_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "My_R1    : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  put ("My_R2    :");
  dump (&My_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "My_R2    : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  Local_R1 = My_R1;
  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  Local_R2 = My_R2;
  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  Local_R1.S1   = 2;
  Local_R1.I    = 0x78ABCDEF;
  Local_R1.S2   = 1;
  Local_R1.N.C1 = 0x12;
  Local_R1.N.C2 = 0x34;
  Local_R1.N.C3 = 0x56;
  Local_R1.N.B  = 4;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  Local_R2.S1   = 2;
  Local_R2.I    = 0x78ABCDEF;
  Local_R2.S2   = 1;
  Local_R2.N.C1 = 0x12;
  Local_R2.N.C2 = 0x34;
  Local_R2.N.C3 = 0x56;
  Local_R2.N.B  = 4;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  Local_R1.S1   = Local_R2.S1;
  Local_R1.I    = Local_R2.I;
  Local_R1.S2   = Local_R2.S2;
  Local_R1.N.C1 = Local_R2.N.C1;
  Local_R1.N.C2 = Local_R2.N.C2;
  Local_R1.N.C3 = Local_R2.N.C3;
  Local_R1.N.B  = Local_R2.N.B;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : c2 7b f3 2a 5e 12 9a 95.*\n" } */

  Local_R2.S1   = Local_R1.S1;
  Local_R2.I    = Local_R1.I;
  Local_R2.S2   = Local_R1.S2;
  Local_R2.N.C1 = Local_R1.N.C1;
  Local_R2.N.C2 = Local_R1.N.C2;
  Local_R2.N.C3 = Local_R1.N.C3;
  Local_R2.N.B  = Local_R1.N.B;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 09 e2 af 37 bd 24 d2 b4.*\n" } */

  new_line ();
  return 0;
}
