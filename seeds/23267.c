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

struct s1
{
  int f1;
};

static struct s1 *p1_glob = NULL;

void test_1 (struct s1 **pp1, struct s1 *p1_parm)
{
  struct s1 *init_p1_glob = p1_glob;

  __analyzer_eval (p1_glob == init_p1_glob); /* { dg-warning "TRUE" } */

  if (!p1_glob)
    return;

  __analyzer_eval (p1_glob == init_p1_glob); /* { dg-warning "TRUE" } */
  __analyzer_eval (p1_glob != NULL); /* { dg-warning "TRUE" } */

  *pp1 = p1_parm;

  /* The write through *pp1 can't have changed p1_glob, because
     we never take a pointer to p1_glob (and it's static to this TU).  */
  __analyzer_eval (p1_glob == init_p1_glob); /* { dg-warning "TRUE" } */
  __analyzer_eval (p1_glob != NULL); /* { dg-warning "TRUE" } */
}

struct s2
{
  int f1;
};

static struct s2 *p2_glob = NULL;

void test_2 (struct s2 **pp2, struct s2 *p2_parm)
{
  /* Ensure that p2_glob is modified.  */
  p2_glob = (struct s2 *) __builtin_malloc (sizeof (struct s2));
  if (!p2_glob)
    return;

  __analyzer_eval (p2_glob != NULL); /* { dg-warning "TRUE" } */

  *pp2 = p2_parm;

  /* The write through *pp2 can't have changed p2_glob, because
     we never take a pointer to p2_glob (and it's static to this TU).  */
  __analyzer_eval (p2_glob != NULL); /* { dg-warning "TRUE" } */
}

struct s3
{
  int f1;
};

struct s3 *p3_glob = NULL;

void test_3 (struct s3 **pp3, struct s3 *p3_parm)
{
  p3_glob = (struct s3 *) __builtin_malloc (sizeof (struct s3));
  if (!p3_glob)
    return;

  __analyzer_eval (p3_glob != NULL); /* { dg-warning "TRUE" } */

  *pp3 = p3_parm;

  /* The write through *pp3 could have changed p3_glob, because
     another TU could take a pointer to p3_glob.  */
  __analyzer_eval (p3_glob != NULL); /* { dg-warning "UNKNOWN" } */
}
