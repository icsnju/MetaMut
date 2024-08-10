/* PR 7263:  __extension__ keyword doesn't suppress warning on LL or ULL constants.  */
/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors -ftrack-macro-expansion=0" } */
#define _Complex_I_ext      (__extension__ 1.0iF)

#define _Complex_I      (1.0iF)
__complex__  bar () /* { dg-error "ISO C does not support plain .complex. meaning .double complex." } */
{
  return _Complex_I_ext;
}

__extension__ __complex__ 
bar2 ()
{
  return _Complex_I;
}

__complex__ bar3 () /* { dg-error "ISO C does not support plain .complex. meaning .double complex." } */
{
  return _Complex_I; /* { dg-error "imaginary constants are a GCC extension" } */
}
