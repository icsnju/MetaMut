/* { dg-do compile } */
/* { dg-require-effective-target powerpc_prefixed_addr } */
/* { dg-options "-O2 -mdejagnu-cpu=power10" } */

/* Tests whether prefixed instructions with large numeric offsets are generated
   for the unsigned short type.  */

#define TYPE unsigned short

/* Common tests for prefixed instructions testing whether we can generate a
   34-bit offset using 1 instruction.  */

#ifndef TYPE
#define TYPE unsigned int
#endif

#if !defined(DO_ADD) && !defined(DO_VALUE) && !defined(DO_SET)
#define DO_ADD		1
#define DO_VALUE	1
#define DO_SET		1
#endif

#ifndef CONSTANT
#define CONSTANT	0x12480UL
#endif

#if DO_ADD
void
add (TYPE *p, TYPE a)
{
  p[CONSTANT] += a;
}
#endif

#if DO_VALUE
TYPE
value (TYPE *p)
{
  return p[CONSTANT];
}
#endif

#if DO_SET
void
set (TYPE *p, TYPE a)
{
  p[CONSTANT] = a;
}
#endif

/* { dg-final { scan-assembler-times {\mplhz\M}  2 } } */
/* { dg-final { scan-assembler-times {\mpsth\M}  2 } } */