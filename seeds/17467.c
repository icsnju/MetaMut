/* { dg-do run } */
/* { dg-require-effective-target hwaddress_exec } */
/* { dg-shouldfail "hwasan" } */

/* Basic tests for stack tagging.

   0) Accesses outside of a variable crash.
   1) Valid accesses work.
*/

#define ARG 17
/* { dg-do run } */
/* { dg-require-effective-target hwaddress_exec } */

/* Basic tests for stack tagging.

   0) Valid accesses work.
   1) Accesses outside of a variable crash.
*/
int __attribute__ ((noinline))
accessing_pointers (int *left, int *right)
{
  int x = right[2];
  left[3] = right[1];
  return right[1] + left[2];
}

int __attribute__ ((noinline))
using_stack (int num)
{
  int big_array[10];
  int other_array[20];
  accessing_pointers(other_array, big_array);
  return big_array[num];
}

#ifndef ARG
#define ARG 0
#endif

int global;

int __attribute__ ((noinline))
main ()
{
  global += using_stack (ARG);
  return 0;
}
#undef ARG

/* { dg-output "HWAddressSanitizer: tag-mismatch on address 0x\[0-9a-f\]*.*" } */
/* { dg-output "READ of size 4 at 0x\[0-9a-f\]* tags: \[\[:xdigit:\]\]\[\[:xdigit:\]\]/\[\[:xdigit:\]\]\[\[:xdigit:\]\].* \\(ptr/mem\\) in thread T0.*" } */
/* { dg-output "Address 0x\[0-9a-f\]* is located in stack of thread T0.*" } */
/* { dg-output "SUMMARY: HWAddressSanitizer: tag-mismatch \[^\n\]*.*" } */
