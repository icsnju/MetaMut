/* { dg-do preprocess } */
/* { dg-options "-pedantic -std=c89 -Wall" } */

/* A system header may contain C++ comments irrespective of mode.  */
#pragma GCC system_header
// C++ comment is not in C89  { dg-bogus "style comment" "bad warning" }


/* { dg-final { scan-file-not cxxcom2.i "is not in C89" } } */

