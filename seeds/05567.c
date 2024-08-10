/* See e.g. https://en.cppreference.com/w/c/string/byte/strncpy  */

/* { dg-additional-options "-Wno-stringop-overflow" } */
/* { dg-additional-options "-fpermissive" { target c++ } } */

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

extern char *strncpy (char *dst, const char *src, size_t count);

char *
test_passthrough (char *dst, const char *src, size_t count)
{
  char *result = strncpy (dst, src, count);
  __analyzer_eval (result == dst); /* { dg-warning "TRUE" } */
  return result;
}

char *
test_null_dst (const char *src, size_t count)
{
  return strncpy (NULL, src, count); /* { dg-warning "use of NULL where non-null expected" } */
}

char *
test_null_src (char *dst, size_t count)
{
  return strncpy (dst, NULL, count); /* { dg-warning "use of NULL where non-null expected" } */
}

void
test_zero_fill (char *dst)
{
  strncpy (dst, "", 5);
  __analyzer_eval (dst[0] == '\0'); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
  __analyzer_eval (dst[1] == '\0'); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
  __analyzer_eval (dst[2] == '\0'); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
  __analyzer_eval (dst[3] == '\0'); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
  __analyzer_eval (dst[4] == '\0'); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
  __analyzer_eval (__analyzer_get_strlen (dst) == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (__analyzer_get_strlen (dst + 1) == 0); /* { dg-warning "TRUE" "correct" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "status quo" { xfail *-*-* } .-1 } */
}

char *test_unterminated_concrete_a (char *dst)
{
  char buf[3] = "abc"; /* { dg-warning "initializer-string for '\[^\n\]*' is too long" "" { target c++ } } */
  /* Should be OK to copy nothing.  */
  return strncpy (dst, buf, 0); /* { dg-bogus "" } */
}

char *test_unterminated_concrete_b (char *dst)
{
  char buf[3] = "abc"; /* { dg-warning "initializer-string for '\[^\n\]*' is too long" "" { target c++ } } */
  /* Should be OK as the count limits the accesses to valid
     locations within src buf.  */
  return strncpy (dst, buf, 3); /* { dg-bogus "" } */
}

char *test_unterminated_concrete_c (char *dst)
{
  char buf[3] = "abc"; /* { dg-warning "initializer-string for '\[^\n\]*' is too long" "" { target c++ } } */
  /* Should warn: the count is one too high to limit the accesses
     to within src buf.  */
  return strncpy (dst, buf, 4); /* { dg-warning "stack-based buffer over-read" } */
}

char *test_terminated_concrete_d (char *dst)
{
  char buf[6];
  __builtin_memset (buf, 'a', 3);
  __builtin_memset (buf + 3, 'b', 3);

  /* Shouldn't warn.  */
  return strncpy (dst, buf, 6); /* { dg-bogus "" } */
}

char *test_unterminated_concrete_e (char *dst)
{
  char buf[6];
  __builtin_memset (buf, 'a', 3);
  __builtin_memset (buf + 3, 'b', 3);

  /* Should warn.  */
  return strncpy (dst, buf, 7); /* { dg-warning "stack-based buffer over-read" } */
}

char *test_unterminated_symbolic (char *dst, size_t count)
{
  char buf[3] = "abc"; /* { dg-warning "initializer-string for '\[^\n\]*' is too long" "" { target c++ } } */
  return strncpy (dst, buf, count);
}

char *test_terminated_symbolic (char *dst, size_t count)
{
  const char *src = "abc";
  return strncpy (dst, src, count); /* { dg-bogus "" } */
}

char *test_uninitialized_concrete_a (char *dst)
{
  char buf[16];
  return strncpy (dst, buf, 0); /* { dg-bogus "" } */
}

char *test_uninitialized_concrete_b (char *dst)
{
  char buf[16];
  return strncpy (dst, buf, 1); /* { dg-warning "use of uninitialized value" } */
}

char *test_initialized_concrete_c (char *dst)
{
  char buf[16];
  buf[0] = 'a';
  return strncpy (dst, buf, 1);  /* { dg-bogus "" } */
}

char *test_uninitialized_symbolic (char *dst, size_t count)
{
  char buf[16];
  return strncpy (dst, buf, count); /* { dg-warning "use of uninitialized value" } */
}

void test_truncation_1 (const char *src)
{
  char buf[16];
  strncpy (buf, src, 16);
  /* buf might not be terminated (when strlen(src) > 16).  */
  __analyzer_get_strlen (buf); /* { dg-warning "stack-based buffer over-read" "" { xfail *-*-* } } */
}

void test_truncation_2 (size_t count)
{
  char buf[16];
  strncpy (buf, "abc", count);
  /* buf might not be terminated (when count <= 3).  */
  __analyzer_get_strlen (buf); /* { dg-warning "stack-based buffer over-read" "" { xfail *-*-* } } */
}

void test_too_big_concrete (void)
{
  char buf[10];
  strncpy (buf, "abc", 128); /* { dg-warning "stack-based buffer overflow" } */
}

void test_too_big_symbolic (const char *src)
{
  char buf[10];
  strncpy (buf, src, 128); /* { dg-warning "stack-based buffer overflow" } */
}
