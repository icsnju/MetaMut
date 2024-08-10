/* { dg-do run } */

#include <stdio.h>

#define Pi 3.14159265358979323846

struct __attribute__((scalar_storage_order("little-endian"))) R1
{
  double F;
};

struct __attribute__((scalar_storage_order("big-endian"))) R2
{
  double F;
};

struct R1 My_R1 = { Pi };
struct R2 My_R2 = { Pi };
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

  Local_R1.F = My_R1.F + 1.0;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 8c 16 22 aa fd 90 10 40.*\n" } */

  Local_R2.F = My_R2.F + 1.0;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 40 10 90 fd aa 22 16 8c.*\n" } */

  Local_R1.F = Pi;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 18 2d 44 54 fb 21 09 40.*\n" } */

  Local_R2.F = Pi;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 40 09 21 fb 54 44 2d 18.*\n" } */

  Local_R1.F = Local_R1.F + 1.0;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();
  /* { dg-output "Local_R1 : 8c 16 22 aa fd 90 10 40.*\n" } */

  Local_R2.F = Local_R2.F + 1.0;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();
  /* { dg-output "Local_R2 : 40 10 90 fd aa 22 16 8c.*\n" } */

  new_line ();
  return 0;
}
