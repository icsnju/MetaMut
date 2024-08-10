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

extern int errno;

extern void error (int __status, int __errnum, const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern void error_at_line (int __status, int __errnum, const char *__fname,
			   unsigned int __lineno, const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 5, 6)));

/* When status is an unknown param.  */

void test_1 (int st)
{
  error (st, errno, "test");
  __analyzer_eval (st == 0); /* { dg-warning "TRUE" } */
}

/* When status is known zero.  */

void test_2 (int st)
{
  error (0, errno, "test");
  __analyzer_dump_path (); /* { dg-message "here" } */
}

/* When status is a non-zero known constant.  */

void test_3 (int st)
{
  error (1, errno, "test");
  __analyzer_dump_path (); /* { dg-bogus "here" } */
}

/* When status has been tested against zero.  */

void test_4 (int st)
{
  if (st)
    {
      error (st, errno, "nonzero branch");
      __analyzer_dump_path (); /* { dg-bogus "here" } */
    }
  else
    {
      error (st, errno, "zero branch");
      __analyzer_dump_path (); /* { dg-message "here" } */
    }
}

/* Similarly for error_at_line.  */

void test_5 (int st)
{
  error_at_line (st, errno, __FILE__, __LINE__, "test");
  __analyzer_eval (st == 0); /* { dg-warning "TRUE" } */
}

/* Non-trivial format string.  */

void test_6 (int st, const char *str)
{
  error (st, errno, "test: %s", str);
  __analyzer_eval (st == 0); /* { dg-warning "TRUE" } */
}

char *test_error_unterminated (int st)
{
  char fmt[3] = "abc";
  error (st, errno, fmt); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 3 \\('&fmt'\\) of 'error'..." "event" { target *-*-* } .-1 } */
}

char *test_error_at_line_unterminated (int st, int errno)
{
  char fmt[3] = "abc";
  error_at_line (st, errno, __FILE__, __LINE__, fmt); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 5 \\('&fmt'\\) of 'error_at_line'..." "event" { target *-*-* } .-1 } */
}

char *test_error_uninitialized (int st, int errno)
{
  char fmt[16];
  error (st, errno, fmt); /* { dg-warning "use of uninitialized value 'fmt\\\[0\\\]'" } */
  /* { dg-message "while looking for null terminator for argument 3 \\('&fmt'\\) of 'error'..." "event" { target *-*-* } .-1 } */
}

char *test_error_at_line_uninitialized (int st, int errno)
{
  char fmt[16];
  error_at_line (st, errno, __FILE__, __LINE__, fmt); /* { dg-warning "use of uninitialized value 'fmt\\\[0\\\]'" } */
  /* { dg-message "while looking for null terminator for argument 5 \\('&fmt'\\) of 'error_at_line'..." "event" { target *-*-* } .-1 } */
}
