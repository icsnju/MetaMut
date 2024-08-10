/* Allow complex types in system headers even with -std=iso9899:1990
   -pedantic-errors.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

/* Allow complex types in system headers even with -std=iso9899:1990
   -pedantic-errors.  Header file.  */

#pragma GCC system_header

_Complex double x;
