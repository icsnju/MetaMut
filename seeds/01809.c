/* Sanity test for Warray-bounds-7[1-4].c.  Also verify the expected
   inlining stack.
   { dg-do compile }
   { dg-options "-O2 -Wall" } */

/* Verify that -Warray-bounds suppression via #pragma GCC diagnostic
   works at any call site in an inlining stack
   { dg-do compile }
   { dg-options "-O2 -Wall" } */

int a[4];

void f1 (int *p, int i)
{
#pragma GCC diagnostic push
#if IGNORE == '1'
#  pragma GCC diagnostic ignored "-Warray-bounds"
#endif
  p[i + 1] = 0;
#pragma GCC diagnostic pop
}

void f2 (int *p, int i)
{
#pragma GCC diagnostic push
#if IGNORE == '2'
#  pragma GCC diagnostic ignored "-Warray-bounds"
#endif
  f1 (p + 1, i + 1);
#pragma GCC diagnostic pop
}

void f3 (int *p, int i)
{
#pragma GCC diagnostic push
#if IGNORE == '3'
#  pragma GCC diagnostic ignored "-Warray-bounds"
#endif
  f2 (p + 1, i + 1);
#pragma GCC diagnostic pop
}

void f4 (void)
{
#pragma GCC diagnostic push
#if IGNORE == '4'
#  pragma GCC diagnostic ignored "-Warray-bounds"
#endif
  f3 (a, 1);
#pragma GCC diagnostic pop
}

// { dg-regexp "In function 'f1'," "In function f1" { target *-*-* } 0 }
// { dg-regexp "inlined from 'f2' at \[^\\n\\r\]+\[\\n\\r\]" "inlined from f2" { target *-*-* } 0 }
// { dg-regexp "inlined from 'f3' at \[^\\n\\r\]+\[\\n\\r\]" "inlined from f3" { target *-*-* } 0 }
// { dg-regexp "inlined from 'f4' at \[^\\n\\r\]+\[\\n\\r\]" "inlined from f4" { target *-*-* } 0 }
// { dg-message "Warray-bounds-71.h:\\d+:\\d+: warning: array subscript 6 is outside array bounds of 'int\\\[4]'" "warning" { target *-*-* } 0 }
