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

int main (void)
{
  struct R1 Local_R1;
  struct R2 Local_R2;

  Local_R1.S1 = My_R1.S1 - 1;
  Local_R1.I  = My_R1.I + 1;
  Local_R1.S2 = My_R1.S2 - 1;
  Local_R1.A1 = My_R1.A1 % 16;
  Local_R1.A2 = My_R1.A2 % 16;
  Local_R1.A3 = My_R1.A3 % 16;
  Local_R1.B  = !My_R1.B;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : e5 59 d1 48 b0 a0 c1 03.*\n" } */

  Local_R2.S1 = My_R2.S1 - 1;
  Local_R2.I  = My_R2.I + 1;
  Local_R2.S2 = My_R2.S2 - 1;
  Local_R2.A1 = My_R2.A1 % 16;
  Local_R2.A2 = My_R2.A2 % 16;
  Local_R2.A3 = My_R2.A3 % 16;
  Local_R2.B  = !My_R2.B;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 44 8d 15 9e 40 58 34 1e.*\n" } */

  Local_R1.S1 = 2;
  Local_R1.I  = 0x12345678;
  Local_R1.S2 = 1;
  Local_R1.A1 = 0xAB;
  Local_R1.A2 = 0xCD;
  Local_R1.A3 = 0xEF;
  Local_R1.B  = 1;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : e2 59 d1 48 b4 aa d9 bb.*\n" } */

  Local_R2.S1 = 2;
  Local_R2.I  = 0x12345678;
  Local_R2.S2 = 1;
  Local_R2.A1 = 0xAB;
  Local_R2.A2 = 0xCD;
  Local_R2.A3 = 0xEF;
  Local_R2.B  = 1;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 84 8d 15 9e 15 5b 35 df.*\n" } */

  Local_R1.S1 = Local_R1.S1 - 1;
  Local_R1.I  = Local_R1.I + 1;
  Local_R1.S2 = Local_R1.S2 - 1;
  Local_R1.A1 = Local_R1.A1 % 16;
  Local_R1.A2 = Local_R1.A2 % 16;
  Local_R1.A3 = Local_R1.A3 % 16;
  Local_R1.B  = !Local_R1.B;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : e5 59 d1 48 b0 a0 c1 03.*\n" } */

  Local_R2.S1 = Local_R2.S1 - 1;
  Local_R2.I  = Local_R2.I + 1;
  Local_R2.S2 = Local_R2.S2 - 1;
  Local_R2.A1 = Local_R2.A1 % 16;
  Local_R2.A2 = Local_R2.A2 % 16;
  Local_R2.A3 = Local_R2.A3 % 16;
  Local_R2.B  = !Local_R2.B;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 44 8d 15 9e 40 58 34 1e.*\n" } */

  new_line ();
  return 0;
}
