/* PR 23497 */
/* { dg-do compile } */
/* { dg-options "-O -Wuninitialized -ftrivial-auto-var-init=zero" } */
/* PR 23497 */
/* { dg-do compile } */
/* { dg-options "-O -Wuninitialized" } */

typedef _Complex float C;
C foo()
{
  C f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}