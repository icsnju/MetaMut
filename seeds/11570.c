// TODO: remove need for this option
/* { dg-additional-options "-fanalyzer-checker=taint" } */

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

struct arg_buf
{
  int i;
  int j;
};

/* Example of marking a function as tainted.  */

void __attribute__((tainted_args))
test_1 (int i, void *p, char *q)
{
  /* There should be a single enode,
     for the "tainted" entry to the function.  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */

  __analyzer_dump_state ("taint", i); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", p); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", q); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", *q); /* { dg-warning "state: 'tainted'" } */

  struct arg_buf *args = (struct arg_buf *) p;
  __analyzer_dump_state ("taint", args->i); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", args->j); /* { dg-warning "state: 'tainted'" } */  
}

/* Example of marking a callback field as tainted.  */

struct s2
{
  void (*cb) (int, void *, char *)
    __attribute__((tainted_args));
};

/* Function not marked as tainted.  */

void
test_2a (int i, void *p, char *q)
{
  /* There should be a single enode,
     for the normal entry to the function.  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */

  __analyzer_dump_state ("taint", i); /* { dg-warning "state: 'start'" } */
  __analyzer_dump_state ("taint", p); /* { dg-warning "state: 'start'" } */
  __analyzer_dump_state ("taint", q); /* { dg-warning "state: 'start'" } */

  struct arg_buf *args = (struct arg_buf *) p;
  __analyzer_dump_state ("taint", args->i); /* { dg-warning "state: 'start'" } */
  __analyzer_dump_state ("taint", args->j); /* { dg-warning "state: 'start'" } */  
}

/* Function referenced via t2b.cb, marked as "tainted".  */

void
test_2b (int i, void *p, char *q)
{
  /* There should be two enodes
     for the direct call, and the "tainted" entry to the function.  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */
}

/* Callback used via t2c.cb, marked as "tainted".  */
void
__analyzer_test_2c (int i, void *p, char *q)
{
  /* There should be a single enode,
     for the "tainted" entry to the function.  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */

  __analyzer_dump_state ("taint", i); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", p); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", q); /* { dg-warning "state: 'tainted'" } */
}

struct s2 t2b =
{
  .cb = test_2b
};

struct s2 t2c =
{
  .cb = __analyzer_test_2c
};
