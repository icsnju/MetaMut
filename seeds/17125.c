/* { dg-do run } */
/* { dg-require-effective-target hwaddress_exec } */
/* { dg-shouldfail "hwasan" } */

#define ARG 12
/* { dg-do run } */
/* { dg-require-effective-target hwaddress_exec } */

/* Handling large aligned variables.
   Large aligned variables take a different code-path through expand_stack_vars
   in cfgexpand.c.  This testcase is just to exercise that code-path.

   The alternate code-path produces a second base-pointer through some
   instructions emitted in the prologue.
   
   Test cases are:
   0) Valid access works without complaint.
   1) Invalid access is caught.  */
int __attribute__ ((noinline))
handle_large_alignment (int num)
{
  int other_array[10];
  int big_array[100] __attribute__ ((aligned (32)));
  return big_array[num] + other_array[num];
}

#ifndef ARG
#define ARG 1
#endif

int global;

int __attribute__ ((noinline))
main ()
{
  global += handle_large_alignment (ARG);
  return 0;
}
#undef ARG

/* { dg-output "HWAddressSanitizer: tag-mismatch on address 0x\[0-9a-f\]*.*" } */
/* NOTE: This assumes the current tagging mechanism (one at a time from the
   base and large aligned variables being handled first).  */
/* { dg-output "READ of size 4 at 0x\[0-9a-f\]* tags: \[\[:xdigit:\]\]\[\[:xdigit:\]\]/\[\[:xdigit:\]\]\[\[:xdigit:\]\].* \\(ptr/mem\\) in thread T0.*" } */
/* { dg-output "Address 0x\[0-9a-f\]* is located in stack of thread T0.*" } */
/* { dg-output "SUMMARY: HWAddressSanitizer: tag-mismatch \[^\n\]*.*" } */
