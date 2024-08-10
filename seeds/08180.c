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

#ifdef __cplusplus
extern "C"
#endif
void abort (void);

int Get_Elem1 (struct R1 R)
{
  struct R1 Tmp = R;
  return Tmp.N.C1;
}

void Set_Elem1 (struct R1 *R, int I)
{
  struct R1 Tmp = *R;
  Tmp.N.C1 = I;
  *R = Tmp;
}

int Get_Elem2 (struct R2 R)
{
  struct R2 Tmp = R;
  return Tmp.N.C1;
}

void Set_Elem2 (struct R2 *R, int I)
{
  struct R2 Tmp = *R;
  Tmp.N.C1 = I;
  *R = Tmp;
}

int main (void)
{
  struct R1 A1 = My_R1;
  struct R2 A2 = My_R2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();
  /* { dg-output "A1 : 78 56 34 12 00 ab 00 12 00 cd 00 34 00 ef 00 56.*\n" } */

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();
  /* { dg-output "A2 : 12 34 56 78 12 00 ab 00 34 00 cd 00 56 00 ef 00.*\n" } */

  if (Get_Elem1 (A1) != 0xAB0012) abort ();

  Set_Elem1 (&A1, 0xCD0034);
  if (Get_Elem1 (A1) != 0xCD0034) abort ();

  if (Get_Elem2 (A2) != 0xAB0012) abort ();

  Set_Elem2 (&A2, 0xCD0034);
  if (Get_Elem2 (A2) != 0xCD0034) abort ();

  new_line ();
  return 0;
}
