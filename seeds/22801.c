/* As per inlining-4.c, but testing how the ASCII art version of
   the path looks.  */

/* { dg-additional-options "-O2 -fdiagnostics-show-path-depths" } */
/* { dg-additional-options "-fdiagnostics-path-format=inline-events -fdiagnostics-show-caret" } */

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


static inline const char*
inner (int flag)
{
  if (flag)
    return NULL;
  return "foo";
}

static inline const char*
middle (int flag)
{
  return inner (flag);
}

char
outer (int flag)
{
  return *middle (flag); /* { dg-warning "dereference of NULL" "warning" } */
}

/* { dg-begin-multiline-output "" }
   return *middle (flag);
          ^~~~~~~~~~~~~~
  'outer': events 1-2 (depth 1)
    |
    | outer (int flag)
    | ^~~~~
    | |
    | (1) entry to 'outer'
    |
    |   return *middle (flag);
    |           ~
    |           |
    |           (2) inlined call to 'middle' from 'outer'
    |
    +--> 'middle': event 3 (depth 2)
           |
           |   return inner (flag);
           |          ^
           |          |
           |          (3) inlined call to 'inner' from 'middle'
           |
           +--> 'inner': event 4 (depth 3)
                  |
                  |   if (flag)
                  |      ^
                  |      |
                  |      (4) following 'true' branch (when 'flag != 0')...
                  |
    <-------------+
    |
  'outer': event 5 (depth 1)
    |
    |cc1:
    | (5): ...to here
    |
  'outer': event 6 (depth 1)
    |
    |   return *middle (flag);
    |          ^~~~~~~~~~~~~~
    |          |
    |          (6) dereference of NULL '<unknown>'
    |
   { dg-end-multiline-output "" { target c } } */
/* { dg-begin-multiline-output "" }
   return *middle (flag);
                       ^
  'char outer(int)': events 1-2 (depth 1)
    |
    | outer (int flag)
    | ^~~~~
    | |
    | (1) entry to 'outer'
    |
    |   return *middle (flag);
    |                  ~
    |                  |
    |                  (2) inlined call to 'middle' from 'outer'
    |
    +--> 'const char* middle(int)': event 3 (depth 2)
           |
           |   return inner (flag);
           |                ^
           |                |
           |                (3) inlined call to 'inner' from 'middle'
           |
           +--> 'const char* inner(int)': event 4 (depth 3)
                  |
                  |   if (flag)
                  |   ^~
                  |   |
                  |   (4) following 'true' branch (when 'flag != 0')...
                  |
    <-------------+
    |
  'char outer(int)': event 5 (depth 1)
    |
    |cc1plus:
    | (5): ...to here
    |
  'char outer(int)': event 6 (depth 1)
    |
    |   return *middle (flag);
    |                       ^
    |                       |
    |                       (6) dereference of NULL '<unknown>'
    |
   { dg-end-multiline-output "" { target c++ } } */
