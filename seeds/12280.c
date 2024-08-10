/* { dg-do compile } */
/* { dg-skip-if "" { ! { clmcpu } } } */
/* { dg-options "-mcpu=nps400 -mcmem" } */

#define CMEM_SECTION_ATTR __attribute__ ((section (".cmem_shared")));


struct foo_type
{
  unsigned int a;
  unsigned short b;
  unsigned char c;
};

struct foo_type foo CMEM_SECTION_ATTR

int
f ()
{
  foo.a = 3;
  foo.b = 2;
  foo.c = 1;
  return 0;
}

/* { dg-final { scan-assembler "xst\\s" } } */
/* { dg-final { scan-assembler "xstw\\s" } } */
/* { dg-final { scan-assembler "xstb\\s" } } */
