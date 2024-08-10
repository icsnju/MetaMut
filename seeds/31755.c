/* { dg-do run } */
/* { dg-options "-O2" } */

/* { dg-do compile } */
/* { dg-options "-O2 -mno-movbe" } */
/* { dg-final { scan-assembler-times "bswap\[ \t\]+" 5 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "bswap\[ \t\]+" 8 { target ia32 } } } */

#include<stdint.h>

uint64_t
__attribute__((noipa))
swap_rotate_64 (unsigned char* x)
{
  return ((uint64_t)(x[0]) << 0 |
	  (uint64_t)(x[1]) << 56 |
	  (uint64_t)(x[2]) << 48 |
	  (uint64_t)(x[3]) << 40 |
	  (uint64_t)(x[4]) << 32 |
	  (uint64_t)(x[5]) << 24 |
	  (uint64_t)(x[6]) << 16 |
	  (uint64_t)(x[7]) << 8);
}

uint64_t
__attribute__((noipa))
swap_rotate_64_mask_1 (unsigned char* x)
{
  return ((uint64_t)(x[0]) << 24 |
	  (uint64_t)(x[1]) << 16 |
	  (uint64_t)(x[2]) << 8 |
	  (uint64_t)(0) << 0 |
	  (uint64_t)(x[4]) << 56 |
	  (uint64_t)(x[5]) << 48 |
	  (uint64_t)(x[6]) << 40 |
	  (uint64_t)(x[7]) << 32);
}

uint64_t
__attribute__((noipa))
swap_rotate_64_mask_2 (unsigned char* x)
{
  return ((uint64_t)(x[0]) << 0 |
	  (uint64_t)(x[1]) << 56 |
	  (uint64_t)(x[2]) << 48 |
	  (uint64_t)(0) << 40 |
	  (uint64_t)(x[4]) << 32 |
	  (uint64_t)(x[5]) << 24 |
	  (uint64_t)(x[6]) << 16 |
	  (uint64_t)(x[7]) << 8);
}


uint32_t
__attribute__((noipa))
swap_rotate_32 (unsigned char* x)
{
  return ((uint64_t)(x[0]) << 8 |
	  (uint64_t)(x[1]) << 0 |
	  (uint64_t)(x[2]) << 24 |
	  (uint64_t)(x[3]) << 16);
}

uint32_t
__attribute__((noipa))
swap_rotate_32_mask_1 (unsigned char* x)
{
  return ((uint64_t)(x[0]) << 8 |
	  (uint64_t)(0) << 0 |
	  (uint64_t)(x[2]) << 24 |
	  (uint64_t)(x[3]) << 16);
}

int main ()
{
  unsigned char a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  uint64_t res = swap_rotate_64 (a);
  if (res != 0x0203040506070801ULL)
    __builtin_abort ();

  res = swap_rotate_64_mask_1 (a);
  if (res != 0x0506070801020300ULL)
    __builtin_abort ();

  res = swap_rotate_64_mask_2 (a);
  if (res != 0x0203000506070801ULL)
    __builtin_abort ();

  uint32_t res2 = swap_rotate_32 (a);
  if (res2 != 0x03040102)
    __builtin_abort ();

  res2 = swap_rotate_32_mask_1 (a);
  if (res2 != 0x03040100)
    __builtin_abort ();

  return 0;
}
