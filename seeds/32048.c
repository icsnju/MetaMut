/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */

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

/* Basic example of known fn behavior.  */

extern int returns_42 (void);

void test_1 (void)
{
  int val = returns_42 ();
  __analyzer_eval (val == 42); /* { dg-warning "TRUE" } */
}

/* Example of bifurcation, with a copy that can fail.  */

extern int
attempt_to_copy (void *to, const void *from, int sz);

void test_copy_success (void *to, const void *from, int sz)
{
  if (!attempt_to_copy (to, from, sz))
    {
      /* Success */
    }
}

void test_copy_failure (void *to, const void *from, int sz)
{
  if (attempt_to_copy (to, from, sz)) /* { dg-message "when 'attempt_to_copy' fails" } */
    __analyzer_dump_path (); /* { dg-message "path" } */
}

struct coord
{
  int x;
  int y;
  int z;
};

void test_copy_2 (void)
{
  struct coord to = {1, 2, 3};
  struct coord from = {4, 5, 6};
  if (attempt_to_copy (&to, &from, sizeof (struct coord)))
    {
      /* Failure.  */
      __analyzer_eval (to.x == 1); /* { dg-warning "TRUE" } */
      __analyzer_eval (to.y == 2); /* { dg-warning "TRUE" } */
      __analyzer_eval (to.z == 3); /* { dg-warning "TRUE" } */
    }
  else
    {
      /* Success.  */
      __analyzer_eval (to.x == 4); /* { dg-warning "TRUE" } */
      __analyzer_eval (to.y == 5); /* { dg-warning "TRUE" } */
      __analyzer_eval (to.z == 6); /* { dg-warning "TRUE" } */
    }
}
