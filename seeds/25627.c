/* 
   Origin: Dodji Seketeli <dodji@redhat.com>
   { dg-options "-std=iso9899:1999 -pedantic" } 
   { dg-do compile } 
 */

#pragma GCC system_header

#define BINARY_INT_CONSTANT_IN_SYSTEM_HEADER 0b1101

int
foo (void)
{
#if BINARY_INT_CONSTANT_IN_SYSTEM_HEADER /* A binary constant defined
					    in system header.  No
					    warning.  */
  return 23;
#endif
  return 0b1101; /* { dg-warning "binary constants are a C2X feature or GCC extension" } */
}
