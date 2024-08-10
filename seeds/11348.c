typedef __SIZE_TYPE__ size_t;
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

/* Duplicating a string.  */

/* Correct but poor implementation with repeated __builtin_strlen calls.  */

char *
alloc_dup_1_correct (const char *x)
{
  size_t sz = __builtin_strlen (x) + 1;
  char *result = (char *) __builtin_malloc (sz);
  if (!result)
    return NULL;
  __builtin_memcpy (result, x, __builtin_strlen (x));
  result[__builtin_strlen(x)] = '\0';
  return result;
}

/* Incorrect version: forgetting to add space for terminator.  */

char *
alloc_dup_1_incorrect (const char *x, const char *y)
{
  /* Forgetting to add space for the terminator here.  */
  size_t sz = __builtin_strlen (x) + 1;
  char *result = (char *) __builtin_malloc (sz);
  if (!result)
    return NULL;
  __builtin_memcpy (result, x, __builtin_strlen (x));
  result[__builtin_strlen(x)] = '\0'; /* { dg-warning "heap-based buffer overflow" "PR analyzer/105899" { xfail *-*-* } } */
  return result;
}

/* As above, but only calling __builtin_strlen once.  */

char *
alloc_dup_2_correct (const char *x)
{
  size_t len_x = __builtin_strlen (x);
  size_t sz = len_x + 1;
  char *result = (char *) __builtin_malloc (sz);
  if (!result)
    return NULL;
  __builtin_memcpy (result, x, len_x);
  result[len_x] = '\0';
  return result;
}

char *
alloc_dup_of_concatenated_pair_2_incorrect (const char *x, const char *y)
{
  size_t len_x = __builtin_strlen (x);
  size_t sz = len_x; /* Forgetting to add space for the terminator.  */
  char *result = (char *) __builtin_malloc (sz); /* { dg-message "capacity: 'len_x' bytes" } */
  if (!result)
    return NULL;
  __builtin_memcpy (result, x, len_x);
  result[len_x] = '\0'; /* { dg-warning "heap-based buffer overflow" } */
  return result;
}
