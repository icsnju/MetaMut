/* As per inlining-3.c, but testing how the ASCII art version of
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
typedef __SIZE_TYPE__ size_t;

struct input_file_st 
{
  char inpname[1];
};

typedef struct input_file_st input_file;

static inline const char*
get_input_file_name (const input_file *inpf)
{
  if (inpf)
    return inpf->inpname;
  return NULL;
}

size_t
test (const input_file *inpf)
{
  const char *f = get_input_file_name (inpf);
  return __builtin_strlen (f); /* { dg-warning "use of NULL" "warning" } */
}

/* { dg-begin-multiline-output "" }
   return __builtin_strlen (f);
          ^~~~~~~~~~~~~~~~~~~~
  'test': events 1-2 (depth 1)
    |
    | test (const input_file *inpf)
    | ^~~~
    | |
    | (1) entry to 'test'
    |
    |   const char *f = get_input_file_name (inpf);
    |                   ~
    |                   |
    |                   (2) inlined call to 'get_input_file_name' from 'test'
    |
    +--> 'get_input_file_name': event 3 (depth 2)
           |
           |   if (inpf)
           |      ^
           |      |
           |      (3) following 'false' branch (when 'inpf' is NULL)...
           |
    <------+
    |
  'test': events 4-5 (depth 1)
    |
    |   return __builtin_strlen (f);
    |          ^~~~~~~~~~~~~~~~~~~~
    |          |
    |          (4) ...to here
    |          (5) argument 1 ('<unknown>') NULL where non-null expected
    |
   { dg-end-multiline-output "" { target c } } */


/* { dg-begin-multiline-output "" }
   return __builtin_strlen (f);
          ~~~~~~~~~~~~~~~~~^~~
  'size_t test(const input_file*)': events 1-2 (depth 1)
    |
    | test (const input_file *inpf)
    | ^~~~
    | |
    | (1) entry to 'test'
    |
    |   const char *f = get_input_file_name (inpf);
    |                                       ~
    |                                       |
    |                                       (2) inlined call to 'get_input_file_name' from 'test'
    |
    +--> 'const char* get_input_file_name(const input_file*)': event 3 (depth 2)
           |
           |   if (inpf)
           |   ^~
           |   |
           |   (3) following 'false' branch (when 'inpf' is NULL)...
           |
    <------+
    |
  'size_t test(const input_file*)': events 4-5 (depth 1)
    |
    |   return __builtin_strlen (f);
    |          ~~~~~~~~~~~~~~~~~^~~
    |                           |
    |                           (4) ...to here
    |                           (5) argument 1 ('<unknown>') NULL where non-null expected
    |
   { dg-end-multiline-output "" { target c++ } } */