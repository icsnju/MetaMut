/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */
/* { dg-skip-if "structure layout assumption not met" { default_packed } } */

#include <string.h>

/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);

typedef unsigned char u8;
typedef unsigned int u32;

struct st
{
  u8 i;  /* { dg-message "padding after field 'i' is uninitialized \\(3 bytes\\)" } */
  u32 j; /* { dg-message "field 'j' is uninitialized \\(4 bytes\\)" } */
};

void test (void __user *dst, u8 a)
{
  struct st s; /* { dg-message "region created on stack here" "where" } */
  /* { dg-message "capacity: 8 bytes" "capacity" { target *-*-* } .-1 } */
  /* { dg-message "suggest forcing zero-initialization by providing a '.0.' initializer" "fix-it hint" { target *-*-* } .-2 } */  
  s.i = a;
  copy_to_user(dst, &s, sizeof (struct st)); /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" "warning" } */
  /* { dg-message "7 bytes are uninitialized" "note how much" { target *-*-* } .-1 } */
}
