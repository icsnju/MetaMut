/* { dg-do compile } */
/* { dg-options "-O3" } */

/* Common code for testing the TLS code generation.  */

__thread int
foo;

int
setfoo (int x)
{
  int result = foo;
  foo = x;
  return result;
}

static __thread int
foostatic;

int
setfoostatic (int x)
{
  int result = foostatic;
  foostatic = x;
  return result;
}

/* foo must use the initial-exec model, foostatic must use the local-exec
   model.  */

/* { dg-final { scan-assembler-times {\tear} 4 { target lp64 } } } */
/* { dg-final { scan-assembler-times {\tear} 2 { target { ! lp64 } } } } */
