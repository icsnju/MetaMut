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

static jmp_buf env;

static void inner (void)
{
  SETJMP (env);
}

void outer (void)
{
  int i;

  inner ();

  longjmp (env, 42); /* { dg-warning "'longjmp' called after enclosing function of 'setjmp' has returned" } */
}

/* { dg-begin-multiline-output "" }
   NN |   longjmp (env, 42);
      |   ^~~~~~~~~~~~~~~~~
  'outer': events 1-2
    |
    |   NN | void outer (void)
    |      |      ^~~~~
    |      |      |
    |      |      (1) entry to 'outer'
    |......
    |   NN |   inner ();
    |      |   ~~~~~~~~
    |      |   |
    |      |   (2) calling 'inner' from 'outer'
    |
    +--> 'inner': event 3
           |
           |   NN | static void inner (void)
           |      |             ^~~~~
           |      |             |
           |      |             (3) entry to 'inner'
           |
         'inner': event 4
           |
           |   NN |   SETJMP (env);
           |      |   ^~~~~~
           |      |   |
           |      |   (4) 'setjmp' called here
           |
         'inner': event 5
           |
           |   NN | }
           |      | ^
           |      | |
           |      | (5) stack frame is popped here, invalidating saved environment
           |
    <------+
    |
  'outer': events 6-7
    |
    |   NN |   inner ();
    |      |   ^~~~~~~~
    |      |   |
    |      |   (6) returning to 'outer' from 'inner'
    |   NN | 
    |   NN |   longjmp (env, 42);
    |      |   ~~~~~~~~~~~~~~~~~
    |      |   |
    |      |   (7) 'longjmp' called after enclosing function of 'setjmp' returned at (5)
    |
    { dg-end-multiline-output "" { target c } } */
/* { dg-begin-multiline-output "" }
   NN |   longjmp (env, 42);
      |   ~~~~~~~~^~~~~~~~~
  'void outer()': events 1-2
    |
    |   NN | void outer (void)
    |      |      ^~~~~
    |      |      |
    |      |      (1) entry to 'outer'
    |......
    |   NN |   inner ();
    |      |   ~~~~~~~~
    |      |         |
    |      |         (2) calling 'inner' from 'outer'
    |
    +--> 'void inner()': event 3
           |
           |   NN | static void inner (void)
           |      |             ^~~~~
           |      |             |
           |      |             (3) entry to 'inner'
           |
         'void inner()': event 4
           |
           |   NN |   SETJMP (env);
           |      |   ^~~~~~
           |      |   |
           |      |   (4) 'setjmp' called here
           |
         'void inner()': event 5
           |
           |   NN | }
           |      | ^
           |      | |
           |      | (5) stack frame is popped here, invalidating saved environment
           |
    <------+
    |
  'void outer()': events 6-7
    |
    |   NN |   inner ();
    |      |   ~~~~~~^~
    |      |         |
    |      |         (6) returning to 'outer' from 'inner'
    |   NN | 
    |   NN |   longjmp (env, 42);
    |      |   ~~~~~~~~~~~~~~~~~
    |      |           |
    |      |           (7) 'longjmp' called after enclosing function of 'setjmp' returned at (5)
    |
    { dg-end-multiline-output "" { target c++ } } */
