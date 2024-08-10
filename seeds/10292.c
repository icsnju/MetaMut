/* { dg-do compile } */
/* { dg-options "-save-temps -Wint-conversion" } */
#pragma GCC system_header

/* N.B. the semicolon in the macro definition is important, since it produces a
   second token from this system header on the same line as the __LINE__ token.  */
#define X(a, b) __LINE__;
int main ()
{
    X(a, 
      b);
    char *should_warn = 1; /* { dg-warning {-Wint-conversion} } */
}
