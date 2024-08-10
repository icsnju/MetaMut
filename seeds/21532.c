#include <string.h>
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

const char* test_literal (int x)
{
  char *p = __builtin_strchr ("123", x); /* { dg-message "when '__builtin_strchr' returns non-NULL" } */
  if (p)
    {
      __analyzer_eval (*p == x); /* { dg-message "UNKNOWN" } */
      /* TODO: this ought to be TRUE, but it's unclear that it's
	 worth stashing this constraint.  */
      *p = 'A'; /* { dg-warning "write to string literal" } */
    }
  return p;
}

void test_2 (const char *s, int c)
{
  char *p = __builtin_strchr (s, c); /* { dg-message "when '__builtin_strchr' returns NULL"} */
  *p = 'A'; /* { dg-warning "dereference of NULL 'p'" "null deref" } */
}

void test_3 (const char *s, int c)
{
  char *p = strchr (s, c); /* { dg-message "when 'strchr' returns NULL"} */
  *p = 'A'; /* { dg-warning "dereference of NULL 'p'" "null deref" } */
}

void test_unterminated (int c)
{
  char buf[3] = "abc";
  strchr (buf, c); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 1 \\('&buf'\\) of 'strchr'..." "event" { target *-*-* } .-1 } */
}

void test_uninitialized (int c)
{
  char buf[16];
  strchr (buf, c); /* { dg-warning "use of uninitialized value 'buf\\\[0\\\]'" } */
  /* { dg-message "while looking for null terminator for argument 1 \\('&buf'\\) of 'strchr'..." "event" { target *-*-* } .-1 } */
}