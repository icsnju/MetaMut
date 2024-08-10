/* { dg-do run } */

#include <stdio.h>

#define Pi 3.1415927f

struct __attribute__((scalar_storage_order("little-endian"))) R1
{
  float F;
};

struct __attribute__((scalar_storage_order("big-endian"))) R2
{
  float F;
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
  /* { dg-output "A1 : db 0f 49 40.*\n" } */

  put ("B1 :");
  dump (&B1, sizeof (struct R1));
  new_line ();
  /* { dg-output "B1 : db 0f 49 40.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 40 49 0f db.*\n" } */

  put ("B2 :");
  dump (&B2, sizeof (struct R2));
  new_line ();
  /* { dg-output "B2 : 40 49 0f db.*\n" } */

  if (A1.F != B1.F) abort ();

  if (A1.F != Pi) abort ();

  if (A2.F != B2.F) abort ();

  if (A2.F != Pi) abort ();

  new_line ();
  return 0;
}
