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

extern int foo (int) __attribute__ ((__pure__));
static jmp_buf buf;

void inner (int x)
{
  foo (x);
  longjmp (buf, 1);
  foo (x);
}

void outer (int y)
{
  foo (y);
  inner (y);
  foo (y);
}

int main (void)
{   
  if (!SETJMP(buf))
    outer (42);
  else
    __analyzer_dump_path (); /* { dg-message "path" } */
  return 0;
}

/* { dg-begin-multiline-output "" }
   NN |     __analyzer_dump_path ();
      |     ^~~~~~~~~~~~~~~~~~~~~~~
  'main': event 1
    |
    |   NN | int main (void)
    |      |     ^~~~
    |      |     |
    |      |     (1) entry to 'main'
    |
  'main': event 2
    |
    |   NN |   if (!SETJMP(buf))
    |      |        ^~~~~~
    |      |        |
    |      |        (2) 'setjmp' called here
    |
  'main': events 3-5
    |
    |   NN |   if (!SETJMP(buf))
    |      |      ^
    |      |      |
    |      |      (3) following 'true' branch...
    |   NN |     outer (42);
    |      |     ~~~~~~~~~~
    |      |     |
    |      |     (4) ...to here
    |      |     (5) calling 'outer' from 'main'
    |
    +--> 'outer': events 6-7
           |
           |   NN | void outer (int y)
           |      |      ^~~~~
           |      |      |
           |      |      (6) entry to 'outer'
           |......
           |   NN |   inner (y);
           |      |   ~~~~~~~~~
           |      |   |
           |      |   (7) calling 'inner' from 'outer'
           |
           +--> 'inner': events 8-9
                  |
                  |   NN | void inner (int x)
                  |      |      ^~~~~
                  |      |      |
                  |      |      (8) entry to 'inner'
                  |......
                  |   NN |   longjmp (buf, 1);
                  |      |   ~~~~~~~~~~~~~~~~
                  |      |   |
                  |      |   (9) rewinding from 'longjmp' in 'inner'...
                  |
    <-------------+
    |
  'main': event 10
    |
    |   NN |   if (!SETJMP(buf))
    |      |        ^~~~~~
    |      |        |
    |      |        (10) ...to 'setjmp' in 'main' (saved at (2))
    |
  'main': events 11-13
    |
    |   NN |   if (!SETJMP(buf))
    |      |      ^
    |      |      |
    |      |      (11) following 'false' branch...
    |......
    |   NN |     __analyzer_dump_path ();
    |      |     ~~~~~~~~~~~~~~~~~~~~~~~
    |      |     |
    |      |     (12) ...to here
    |      |     (13) here
    |
    { dg-end-multiline-output "" } */
