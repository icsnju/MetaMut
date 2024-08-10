/* { dg-additional-options "-fdiagnostics-show-line-numbers -fdiagnostics-path-format=inline-events -fdiagnostics-show-caret" } */
/* { dg-enable-nn-line-numbers "" } */
/* { dg-require-effective-target indirect_jumps } */

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
#ifndef ANALYZER_DECLS_H
#define ANALYZER_DECLS_H

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

/* Function decls with special meaning to the analyzer.
   None of these are actually implemented.  */

/* Trigger a breakpoint in the analyzer when reached.  */
extern void __analyzer_break (void);

/* Emit a warning describing the 2nd argument (which can be of any
   type), at the given verbosity level.  This is for use when
   debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_describe (int verbosity, ...);

/* Dump copious information about the analyzer’s state when reached.  */
extern void __analyzer_dump (void);

/* Emit a warning describing the size of the base region of (*ptr).  */
extern void __analyzer_dump_capacity (const void *ptr);

/* Dump information about what decls have escaped at this point on the path.  */
extern void __analyzer_dump_escaped (void);

/* Dump information after analysis on all of the exploded nodes at this
   program point.

   __analyzer_dump_exploded_nodes (0);
   will dump just the number of nodes, and their IDs.

   __analyzer_dump_exploded_nodes (1);
   will also dump all of the states within those nodes.  */
extern void __analyzer_dump_exploded_nodes (int);

/* Emit a warning describing what is known about the value of NAME.  */
extern void __analyzer_dump_named_constant (const char *name);

/* Emit a placeholder "note" diagnostic with a path to this call site,
   if the analyzer finds a feasible path to it.  */
extern void __analyzer_dump_path (void);

/* Dump the region_model's state to stderr.  */
extern void __analyzer_dump_region_model (void);

/* Emit a warning describing the state of the 2nd argument
   (which can be of any type) with respect to NAME.
   This is for use when debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_dump_state (const char *name, ...);

/* Emit a warning with text "TRUE", FALSE" or "UNKNOWN" based on the
   truthfulness of the argument.  */
extern void __analyzer_eval (int);

/* Obtain an "unknown" void *.  */
extern void *__analyzer_get_unknown_ptr (void);

/* Complain if PTR doesn't point to a null-terminated string.
   TODO: eventually get the strlen of the buffer (without the
   optimizer touching it).  */
extern __SIZE_TYPE__ __analyzer_get_strlen (const char *ptr);

#endif /* #ifndef ANALYZER_DECLS_H.  */

extern void foo (int);

void test_1 (void)
{
  SETJMP (NULL);
}

void test_2 (void)
{
  jmp_buf env;
  int i;

  foo (0);

  i = SETJMP(env);

  foo (1);

  if (i != 0)
    {
      foo (2);
      __analyzer_dump_path (); /* { dg-message "path" } */
    }
  else
    longjmp (env, 1);

  foo (3);
}

/* { dg-begin-multiline-output "" }
   NN |       __analyzer_dump_path ();
      |       ^~~~~~~~~~~~~~~~~~~~~~~
  'test_2': event 1
    |
    |   NN |   i = SETJMP(env);
    |      |       ^~~~~~
    |      |       |
    |      |       (1) 'setjmp' called here
    |
  'test_2': events 2-4
    |
    |   NN |   if (i != 0)
    |      |      ^
    |      |      |
    |      |      (2) following 'false' branch (when 'i == 0')...
    |......
    |   NN |     longjmp (env, 1);
    |      |     ~~~~~~~~~~~~~~~~
    |      |     |
    |      |     (3) ...to here
    |      |     (4) rewinding within 'test_2' from 'longjmp'...
    |
  'test_2': event 5
    |
    |   NN |   i = SETJMP(env);
    |      |       ^~~~~~
    |      |       |
    |      |       (5) ...to 'setjmp' (saved at (1))
    |
  'test_2': events 6-8
    |
    |   NN |   if (i != 0)
    |      |      ^
    |      |      |
    |      |      (6) following 'true' branch (when 'i != 0')...
    |   NN |     {
    |   NN |       foo (2);
    |      |       ~~~~~~~
    |      |       |
    |      |       (7) ...to here
    |   NN |       __analyzer_dump_path ();
    |      |       ~~~~~~~~~~~~~~~~~~~~~~~
    |      |       |
    |      |       (8) here
    |
    { dg-end-multiline-output "" { target c } } */
/* { dg-begin-multiline-output "" }
   NN |       __analyzer_dump_path ();
      |       ~~~~~~~~~~~~~~~~~~~~~^~
  'void test_2()': event 1
    |
    |   NN |   i = SETJMP(env);
    |      |       ^~~~~~
    |      |       |
    |      |       (1) 'setjmp' called here
    |
  'void test_2()': events 2-4
    |
    |   NN |   if (i != 0)
    |      |   ^~
    |      |   |
    |      |   (2) following 'false' branch (when 'i == 0')...
    |......
    |   NN |     longjmp (env, 1);
    |      |     ~~~~~~~~~~~~~~~~
    |      |             |
    |      |             (3) ...to here
    |      |             (4) rewinding within 'test_2' from 'longjmp'...
    |
  'void test_2()': event 5
    |
    |   NN |   i = SETJMP(env);
    |      |       ^~~~~~
    |      |       |
    |      |       (5) ...to 'setjmp' (saved at (1))
    |
  'void test_2()': events 6-8
    |
    |   NN |   if (i != 0)
    |      |   ^~
    |      |   |
    |      |   (6) following 'true' branch (when 'i != 0')...
    |   NN |     {
    |   NN |       foo (2);
    |      |       ~~~~~~~
    |      |           |
    |      |           (7) ...to here
    |   NN |       __analyzer_dump_path ();
    |      |       ~~~~~~~~~~~~~~~~~~~~~~~
    |      |                            |
    |      |                            (8) here
    |
    { dg-end-multiline-output "" { target c++ } } */

void test_3 (void)
{
  longjmp (NULL, 0);
}

void test_4 (void)
{
  longjmp (NULL, 1);
}

void test_5 (void)
{
  jmp_buf env;
  longjmp (env, 1);
}
