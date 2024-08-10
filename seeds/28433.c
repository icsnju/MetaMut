/* { dg-do compile } */
/* { dg-options "-O2 -Wall -Wno-array-bounds" } */

#pragma GCC system_header

extern __inline __attribute__ ((__always_inline__,__artificial__,__gnu_inline__)) char *
__attribute__ ((__nothrow__)) strcpy (char *__restrict __dest, __const char *__restrict __src)
{
  return __builtin___strcpy_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}


char buf[6];

int main(int argc, char **argv)
{
  strcpy (buf,"hello ");
  return 0;
}

/* { dg-warning "\\\[-Wstringop-overflow" "warning" { target *-*-* } 0 }
   { dg-message "file included" "included" { target *-*-* } 0 }
   { dg-message "inlined from" "inlined" { target *-*-* } 0 }

   The test might emit two warnings, one for the strcpy call and
   another for the inlined call to __builtin___strcpy_chk() called
   from strcpy().
   { dg-prune-output "writing 7 bytes into a region of size 6" } */
