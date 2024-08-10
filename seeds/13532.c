/* { dg-do compile } */
/* { dg-options "-mcmse" } */

typedef struct
{
  unsigned short a :11;
} test_st_4;

typedef union
{
  char	      a;
  test_st_4 st4;
}test_un_2;

typedef struct
{
  unsigned char	  a;
  unsigned int	    :0;
  unsigned int	  b :1;
  unsigned short    :0;
  unsigned short  c;
  unsigned int	    :0;
  unsigned int	  d :21;
} test_st_3;

typedef struct
{
  unsigned char	  a :3;
  unsigned int	  b :13;
  test_un_2	  un2;
} test_st_2;

typedef union
{
  test_st_2 st2;
  test_st_3 st3;
}test_un_1;

typedef struct
{
  unsigned char	  a :2;
  unsigned char	    :0;
  unsigned short  b :5;
  unsigned char	    :0;
  unsigned char	  c :4;
  test_un_1	  un1;
} test_st_1;

typedef union
{
  test_st_1 st1;
  struct
    {
      unsigned int v1;
      unsigned int v2;
      unsigned int v3;
      unsigned int v4;
    }values;
} read_st_1;


typedef void __attribute__ ((cmse_nonsecure_call)) (*foo_ns) (test_st_1);

int
main (void)
{
  read_st_1 r;
  foo_ns f;

  f = (foo_ns) 0x200000;
  r.values.v1 = 0xFFFFFFFF;
  r.values.v2 = 0xFFFFFFFF;
  r.values.v3 = 0xFFFFFFFF;
  r.values.v4 = 0xFFFFFFFF;

  f (r.st1);
  return 0;
}

/* { dg-final { scan-assembler "movw\tip, #7939" } } */
/* { dg-final { scan-assembler "movt\tip, 15" } } */
/* { dg-final { scan-assembler "and\tr0, r0, ip" } } */
/* { dg-final { scan-assembler "movw\tip, #65535" } } */
/* { dg-final { scan-assembler "movt\tip, 2047" } } */
/* { dg-final { scan-assembler "and\tr1, r1, ip" } } */
/* { dg-final { scan-assembler "mov\tip, #1" } } */
/* { dg-final { scan-assembler "movt\tip, 65535" } } */
/* { dg-final { scan-assembler "and\tr2, r2, ip" } } */
/* { dg-final { scan-assembler "movw\tip, #65535" } } */
/* { dg-final { scan-assembler "movt\tip, 31" } } */
/* { dg-final { scan-assembler "and\tr3, r3, ip" } } */
/* Shift on the same register as blxns.  */
/* { dg-final { scan-assembler "lsrs\t(r\[4-9\]|r10|fp|ip), \\1, #1.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "lsls\t(r\[4-9\]|r10|fp|ip), \\1, #1.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "push\t\{r4, r5, r6, r7, r8, r9, r10, fp\}" } } */
/* Check the right registers are cleared and none appears twice.  */
/* { dg-final { scan-assembler "clrm\t\{(r4, )?(r5, )?(r6, )?(r7, )?(r8, )?(r9, )?(r10, )?(fp, )?(ip, )?APSR\}" } } */
/* Check that the right number of registers is cleared and thus only one
   register is missing.  */
/* { dg-final { scan-assembler "clrm\t\{((r\[4-9\]|r10|fp|ip), ){8}APSR\}" } } */
/* Check that no cleared register is used for blxns.  */
/* { dg-final { scan-assembler-not "clrm\t\{\[^\}\]\+(r\[4-9\]|r10|fp|ip),\[^\}\]\+\}.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "pop\t\{r4, r5, r6, r7, r8, r9, r10, fp\}" } } */
/* { dg-final { scan-assembler "blxns" } } */
