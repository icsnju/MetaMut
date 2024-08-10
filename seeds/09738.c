/* Origin: PR preprocessor/60723

   { dg-do compile }
   { dg-options -no-integrated-cpp }  */

/* Origin: PR preprocessor/60723

   This header file is to be included by the syshdr5.c file.  */

#pragma GCC system_header
#define FOO(A)do {int line = __LINE__ ; A;} while(0)

int
main()
{
  FOO(1/0 /*  { dg-warning "division by zero" }  */
      );
  return 0;
}
