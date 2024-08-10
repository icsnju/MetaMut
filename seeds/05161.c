/* Passing tainted sizes to external functions with attribute ((access)) with
   a size-index.  */

// TODO: remove need for the explicit taint option:
/* { dg-additional-options "-fanalyzer-checker=taint -fanalyzer-show-duplicate-count" } */

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct foo
{
  size_t sz;
};

char buf[100];

extern void extern_fn_read_only (void *p, size_t sz) /* { dg-message "parameter 2 of 'extern_fn_read_only' marked as a size via attribute 'access \\(read_only, 1, 2\\)'" } */
  __attribute__ ((access (read_only, 1, 2)));

void test_fn_read_only (FILE *f, void *p)
{
  struct foo tmp;
  if (1 == fread(&tmp, sizeof(tmp), 1, f)) { /* { dg-message "\\(\[0-9\]+\\) 'tmp' gets an unchecked value here" "event: tmp gets unchecked value" { xfail *-*-* } } */
                                             /* { dg-message "\\(\[0-9\]+\\) following 'true' branch\\.\\.\\." "event: following true branch" { target *-*-* } .-1 } */
    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'tainted'" } */
    /* { dg-message "\\(\[0-9\]+\\) \\.\\.\\.to here" "event: to here" { target *-*-* } .-1 } */

    extern_fn_read_only (p, tmp.sz); /* { dg-warning "use of attacker-controlled value 'tmp.sz' as size without upper-bounds checking" "warning" } */
    /* { dg-bogus "duplicate" "duplicate" { target *-*-* } .-1 } */
  }
}

/* We shouldn't complain if the value has been sanitized.  */

void test_fn_sanitized (FILE *f, void *p)
{
  struct foo tmp;
  if (1 == fread(&tmp, sizeof(tmp), 1, f)) {
    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'tainted'" } */

    if (tmp.sz > 100)
      return;

    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'has_ub'" } */
    
    extern_fn_read_only (p, tmp.sz); /* { dg-bogus "use of attacker-controlled value" } */
  }
}

/* We shouldn't complain if there was no size annotation.  */

extern void extern_fn_no_size (void *p)
  __attribute__ ((access (read_only, 1)));

void test_fn_no_size (FILE *f, void *p)
{
  struct foo tmp;
  if (1 == fread(&tmp, sizeof(tmp), 1, f)) {
    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'tainted'" } */
    extern_fn_no_size (p);
  }
}