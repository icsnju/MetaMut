/* { dg-do compile } */
/* { dg-options "-mcmse" } */

typedef struct
{
  unsigned char	  a :2;
  unsigned char	    :0;
  unsigned short  b :5;
  unsigned char	    :0;
  unsigned short  c :3;
  unsigned char	    :0;
  unsigned int	  d :9;
} test_st_1;

typedef struct
{
  unsigned short  a :7;
  unsigned char	    :0;
  unsigned char	  b :1;
  unsigned char	    :0;
  unsigned short  c :6;
} test_st_2;

typedef union
{
  test_st_1 st_1;
  test_st_2 st_2;
}test_un;

typedef union
{
  test_un un;
  struct
    {
      unsigned int v1;
      unsigned int v2;
      unsigned int v3;
      unsigned int v4;
    }values;
} read_un;


typedef void __attribute__ ((cmse_nonsecure_call)) (*foo_ns) (test_un);

int
main (void)
{
  read_un r;
  foo_ns f;

  f = (foo_ns) 0x200000;
  r.values.v1 = 0xFFFFFFFF;
  r.values.v2 = 0xFFFFFFFF;

  f (r.un);
  return 0;
}

/* { dg-final { scan-assembler "movw\tip, #8063" } } */
/* { dg-final { scan-assembler "movt\tip, 63" } } */
/* { dg-final { scan-assembler "and\tr0, r0, ip" } } */
/* { dg-final { scan-assembler "movw\tip, #511" } } */
/* { dg-final { scan-assembler "and\tr1, r1, ip" } } */
/* Shift on the same register as blxns.  */
/* { dg-final { scan-assembler "lsrs\t(r\[2-9\]|r10|fp|ip), \\1, #1.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "lsls\t(r\[2-9\]|r10|fp|ip), \\1, #1.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "push\t\{r4, r5, r6, r7, r8, r9, r10, fp\}" } } */
/* Check the right registers are cleared and none appears twice.  */
/* { dg-final { scan-assembler "clrm\t\{(r2, )?(r3, )?(r4, )?(r5, )?(r6, )?(r7, )?(r8, )?(r9, )?(r10, )?(fp, )?(ip, )?APSR\}" } } */
/* Check that the right number of registers is cleared and thus only one
   register is missing.  */
/* { dg-final { scan-assembler "clrm\t\{((r\[2-9\]|r10|fp|ip), ){10}APSR\}" } } */
/* Check that no cleared register is used for blxns.  */
/* { dg-final { scan-assembler-not "clrm\t\{\[^\}\]\+(r\[2-9\]|r10|fp|ip),\[^\}\]\+\}.*blxns\t\\1" } } */
/* { dg-final { scan-assembler "pop\t\{r4, r5, r6, r7, r8, r9, r10, fp\}" } } */
/* { dg-final { scan-assembler "blxns" } } */
