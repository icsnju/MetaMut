#include <stdlib.h>
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

struct str {
  size_t len;
  char data[];
};

struct str *
test_const_size (void)
{
  struct str *str = (struct str *) malloc(sizeof(str) + 10);
  if (str) {
    str->len = 10;
    memset(str->data, 'x', 10);
    return str;
  }
  return NULL;
}

struct str *
test_const_size_oob_1 (void)
{
  /* Forgetting to add space for the trailing array.  */
  struct str *str = (struct str *) malloc(sizeof(str));
  if (str) {
    str->len = 10;
    memset(str->data, 'x', 10); /* { dg-warning "heap-based buffer overflow" "Wanalyzer-out-of-bounds" } */
    /* { dg-warning "'memset' writing 10 bytes into a region of size 0 overflows the destination" "Wstringop-overflow" { target c } .-1 } */
    /* { dg-warning "'void\\* memset\\(void\\*, int, size_t\\)' writing 10 bytes into a region of size 0 overflows the destination" "Wstringop-overflow" { target c++ } .-2 } */
    return str;
  }
  return NULL;
}

struct str *
test_const_size_oob_2 (void)
{
  struct str *str = (struct str *) malloc(sizeof(str) + 10);
  if (str) {
    str->len = 10;
    /* Using the wrong size here.  */
    memset(str->data, 'x', 11); /* { dg-warning "heap-based buffer overflow" "Wanalyzer-out-of-bounds" } */
    /* { dg-warning "'memset' writing 11 bytes into a region of size 10 overflows the destination" "Wstringop-overflow" { target c } .-1 } */
    /* { dg-warning "'void\\* memset\\(void\\*, int, size_t\\)' writing 11 bytes into a region of size 10 overflows the destination" "Wstringop-overflow" { target c++ } .-2 } */

    return str;
  }
  return NULL;
}

struct str *
test_symbolic_size (size_t len)
{
  struct str *str = (struct str *) malloc(sizeof(str) + len);
  if (str) {
    str->len = len;
    memset(str->data, 'x', len);
    return str;
  }
  return NULL;
}

struct str *
test_symbolic_size_oob (size_t len)
{
  /* Forgetting to add space for the trailing array.  */
  struct str *str = (struct str *) malloc(sizeof(str));
  if (str) {
    str->len = len;
    memset(str->data, 'x', len); /* { dg-warning "heap-based buffer overflow" "PR analyzer/98247" { xfail *-*-* } } */
    // TODO(xfail): we don't yet complain about this case, which occurs when len > 0
    return str;
  }
  return NULL;
}

struct str *
test_symbolic_size_with_terminator (size_t len)
{
  struct str *str = (struct str *) malloc(sizeof(str) + len + 1);
  if (str) {
    str->len = len;
    memset(str->data, 'x', len);
    str->data[len] = '\0';
    return str;
  }
  return NULL;
}

struct str *
test_symbolic_size_with_terminator_oob (size_t len)
{
  /* Forgetting to add 1 for the terminator.  */
  struct str *str = (struct str *) malloc(sizeof(str) + len);
  if (str) {
    str->len = len;
    memset(str->data, 'x', len);
    str->data[len] = '\0'; /* { dg-warning "heap-based buffer overflow" } */
    return str;
  }
  return NULL;
}
