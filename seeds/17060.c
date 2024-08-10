// TODO: remove need for this option
/* { dg-additional-options "-fanalyzer-checker=taint" } */
/* { dg-require-effective-target alloca } */

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

/* malloc with tainted size.  */

void *test_1 (FILE *f)
{
  struct foo tmp;
  if (1 == fread(&tmp, sizeof(tmp), 1, f)) { /* { dg-message "\\(\[0-9\]+\\) 'tmp' gets an unchecked value here" "event: tmp gets unchecked value" { xfail *-*-* } } */
                                             /* { dg-message "\\(\[0-9\]+\\) following 'true' branch\\.\\.\\." "event: following true branch" { target *-*-* } .-1 } */
    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'tainted'" } */
    /* { dg-message "\\(\[0-9\]+\\) \\.\\.\\.to here" "event: to here" { target *-*-* } .-1 } */
    
    return malloc (tmp.sz); /* { dg-warning "use of attacker-controlled value 'tmp\\.sz' as allocation size without upper-bounds checking" "warning" } */
    /* { dg-message "23: \\(\[0-9\]+\\) 'tmp.i' has an unchecked value here \\(from 'tmp'\\)" "event: tmp.i has an unchecked value" { xfail *-*-* } .-1 } */
    /* { dg-message "\\(\[0-9\]+\\) use of attacker-controlled value 'tmp\\.sz' as allocation size without upper-bounds checking" "final event" { target *-*-* } .-2 } */
    /* { dg-message "heap-based allocation" "memory space" { target *-*-* } .-3 } */
    
    // TOOD: better messages for state changes
  }
  return 0;
}

/* VLA with tainted size.  */

void *test_2 (FILE *f)
{
  struct foo tmp;
  if (1 == fread(&tmp, sizeof(tmp), 1, f)) { /* { dg-message "\\(\[0-9\]+\\) 'tmp' gets an unchecked value here" "event: tmp gets unchecked value" { xfail *-*-* } } */
                                             /* { dg-message "\\(\[0-9\]+\\) following 'true' branch\\.\\.\\." "event: following true branch" { target *-*-* } .-1 } */
    __analyzer_dump_state ("taint", tmp.sz); /* { dg-warning "state: 'tainted'" } */
    /* { dg-message "\\(\[0-9\]+\\) \\.\\.\\.to here" "event: to here" { target *-*-* } .-1 } */

    /* VLA with tainted size.  */
    {
      char buf[tmp.sz]; /* { dg-warning "use of attacker-controlled value 'tmp\\.sz' as allocation size without upper-bounds checking" "warning" } */
      /* { dg-message "\\(\[0-9\]+\\) 'tmp.i' has an unchecked value here \\(from 'tmp'\\)" "event: tmp.i has an unchecked value" { xfail *-*-* } .-1 } */
      /* { dg-message "\\(\[0-9\]+\\) use of attacker-controlled value 'tmp\\.sz' as allocation size without upper-bounds checking" "final event" { target *-*-* } .-2 } */
      /* { dg-message "stack-based allocation" "memory space" { target *-*-* } .-3 } */
      fread (buf, tmp.sz, 1, f);
    }
    
    // TOOD: better messages for state changes
  }
  return 0;
}

void *test_3 (FILE *f)
{
  int num;
  fread (&num, sizeof (int), 1, f);
  __analyzer_dump_state ("taint", num); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", num * 16); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", (size_t)(num * 16)); /* { dg-warning "state: 'tainted'" } */
  return malloc (num * 16); /* { dg-warning "use of attacker-controlled value 'num \\* 16' as allocation size without upper-bounds checking" } */
}
