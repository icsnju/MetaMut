/* { dg-do compile } */
/* { dg-require-effective-target powerpc_pcrel } */
/* { dg-options "-O2 -mdejagnu-cpu=power10" } */

/* Tests whether pc-relative prefixed instructions are generated for the
   short type.  */

#define TYPE short

/* Common tests for prefixed instructions testing whether pc-relative prefixed
   instructions are generated for each type.  */

#ifndef TYPE
#define TYPE unsigned int
#endif

static TYPE a;

/* Make sure a is not optimized away.  */
TYPE *p = &a;

#if !defined(DO_ADD) && !defined(DO_VALUE) && !defined(DO_SET)
#define DO_ADD		1
#define DO_VALUE	1
#define DO_SET		1
#endif

#if DO_ADD
void
add (TYPE b)
{
  a += b;
}
#endif

#if DO_VALUE
TYPE
value (void)
{
  return a;
}
#endif

#if DO_SET
void
set (TYPE b)
{
  a = b;
}
#endif

/* { dg-final { scan-assembler-times {\mplh[az]\M}  2 } } */
/* { dg-final { scan-assembler-times {\mpsth\M}     2 } } */
