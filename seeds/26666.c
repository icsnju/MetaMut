/* { dg-do compile } */
/* { dg-options "-O3 -fPIC" } */

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

/* foo must use the global dynamic model.
   __tls_get_offset must be referenced through PLT.  */

/* { dg-final { scan-assembler-times {\tbrasl\t%r14,__tls_get_offset@PLT:tls_gdcall:foo\n} 1 } } */

/* foostatic must use the local dynamic model.
   __tls_get_offset must be referenced through PLT.  */

/* { dg-final { scan-assembler-times {\tbrasl\t%r14,__tls_get_offset@PLT:tls_ldcall} 1 } } */
