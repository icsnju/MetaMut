/* { dg-do compile } */
/* { dg-skip-if "" { ! { clmcpu } } } */
/* { dg-options "-mcpu=nps400 -mcmem" } */

#define CMEM_SECTION_ATTR __attribute__ ((section (".cmem")));


struct foo_type
{
  unsigned int a;
  unsigned short b;
  unsigned char c;
};

struct foo_type foo __attribute__ ((section (".cmem")));

unsigned int
f ()
{
  unsigned int tmp = foo.a + foo.b + foo.c;
  return tmp;
}

/* { dg-final { scan-assembler "xld\\s" } } */
/* { dg-final { scan-assembler "xldw\\s" } } */
/* { dg-final { scan-assembler "xldb\\s" } } */
