/* { dg-require-effective-target sigsetjmp } */

/* Various integration tests for setjmp-handling expect a precise
   multiline output.
   
   The outputs from -fdiagnostics-path-format=inline-events for such
   setjmp tests are dependent on whether setjmp is a macro or a function
   (and whether that macro is defined in a system header).

   setjmp is a function on some systems and a macro on others.
   This header provides a SETJMP macro in a (fake) system header,
   along with precanned decls of setjmp, for consistency of output across
   different systems.  */

#pragma GCC system_header

struct __jmp_buf_tag {
  char buf[1];
};
typedef struct __jmp_buf_tag jmp_buf[1];
typedef struct __jmp_buf_tag sigjmp_buf[1];

extern int setjmp(jmp_buf env);
extern int sigsetjmp(sigjmp_buf env, int savesigs);

extern void longjmp(jmp_buf env, int val);
extern void siglongjmp(sigjmp_buf env, int val);

#define SETJMP(E) setjmp(E)
#include <stddef.h>
#include <stdlib.h>

extern int foo (int) __attribute__ ((__pure__));

static sigjmp_buf env;

static void inner (void)
{
  void *ptr = malloc (1024); /* { dg-message "allocated here" }  */

  siglongjmp (env, 1); /* { dg-warning "leak of 'ptr'" "warning" } */
  /* { dg-message "rewinding from 'siglongjmp' in 'inner'" " event: rewind from" { target *-*-* } .-1 } */

  free (ptr);
}

void outer (void)
{
  int i;

  foo (0);

  i = sigsetjmp(env, 0); /* { dg-message "'sigsetjmp' called here" "event: sigsetjmp call" } */
  /* { dg-message "to 'sigsetjmp' in 'outer'" "event: rewind to"  { target *-*-* } .-1 } */

  if (i == 0)
    {
      foo (1);
      inner ();
    }

  foo (3);
}
