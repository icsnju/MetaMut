/* { dg-do run } */
/* { dg-options "-O2" } */

/* { dg-do compile } */
/* { dg-options "-O2 -mno-movbe" } */
/* { dg-final { scan-assembler-times "bswap\[ \t\]+" 6 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "bswap\[ \t\]+" 9 { target ia32 } } } */

#include<stdint.h>

uint64_t
__attribute__((noipa))
swap_rotate_64 (uint64_t x)
{
  return ((((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 0) |
	  (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 48) |
	  (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 32) |
	  (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  16) |
	  (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  0) |
	  (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 16) |
	  (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 32) |
	  (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 48));
}

uint64_t
__attribute__((noipa))
swap_rotate_64_mask_1 (uint64_t x)
{
  return ((((uint64_t)(0) & (uint64_t)0x00000000000000ffULL) << 0) |
	  (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 48) |
	  (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 32) |
	  (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  16) |
	  (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  0) |
	  (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 16) |
	  (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 32) |
	  (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 48));
}

uint64_t
__attribute__((noipa))
swap_rotate_64_mask_2 (uint64_t x)
{
  return ((((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 0) |
	  (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 48) |
	  (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 32) |
	  (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  16) |
	  (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  0) |
	  (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 16) |
	  (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 32) |
	  (((uint64_t)(0) & (uint64_t)0xff00000000000000ULL) >> 48));
}


uint32_t
__attribute__((noipa))
swap_rotate_32 (uint32_t x)
{
  return ((((uint32_t)(x) & (uint32_t)0x00000000000000ffULL) << 8) |
	  (((uint32_t)(x) & (uint32_t)0x000000000000ff00ULL) >> 8) |
	  (((uint32_t)(x) & (uint32_t)0x0000000000ff0000ULL) << 8) |
	  (((uint32_t)(x) & (uint32_t)0x00000000ff000000ULL) >> 8));
}

uint32_t
__attribute__((noipa))
swap_rotate_32_mask_1 (uint32_t x)
{
  return ((((uint32_t)(0) & (uint32_t)0x00000000000000ffULL) << 8) |
	  (((uint32_t)(x) & (uint32_t)0x000000000000ff00ULL) >> 8) |
	  (((uint32_t)(x) & (uint32_t)0x0000000000ff0000ULL) << 8) |
	  (((uint32_t)(x) & (uint32_t)0x00000000ff000000ULL) >> 8));
}

uint32_t
__attribute__((noipa))
swap_rotate_32_mask_2 (uint32_t x)
{
  return ((((uint32_t)(x) & (uint32_t)0x00000000000000ffULL) << 8) |
	  (((uint32_t)(0) & (uint32_t)0x000000000000ff00ULL) >> 8) |
	  (((uint32_t)(x) & (uint32_t)0x0000000000ff0000ULL) << 8) |
	  (((uint32_t)(x) & (uint32_t)0x00000000ff000000ULL) >> 8));
}

int main ()
{
  uint64_t a = 0x0807060504030201ULL;
  uint64_t res = swap_rotate_64 (a);
  if (res != 0x0203040506070801ULL)
    __builtin_abort ();

  res = swap_rotate_64_mask_1 (a);
  if (res != 0x0203040506070800ULL)
    __builtin_abort ();

  res = swap_rotate_64_mask_2 (a);
  if (res != 0x0203040506070001ULL)
    __builtin_abort ();

  uint32_t b = 0x04030201;
  uint32_t res2 = swap_rotate_32 (b);
  if (res2 != 0x03040102)
    __builtin_abort ();

  res2 = swap_rotate_32_mask_1 (b);
  if (res2 != 0x03040002)
    __builtin_abort ();

  res2 = swap_rotate_32_mask_2 (b);
  if (res2 != 0x03040100)
    __builtin_abort ();

  return 0;
}
