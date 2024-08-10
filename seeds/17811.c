/* See e.g. https://en.cppreference.com/w/c/string/byte/strcat */

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

char *strcat (char *dest, const char *src);
#define NULL ((void *)0)

char *
test_passthrough (char *dest, const char *src)
{
  return strcat (dest, src);
}

char *
test_null_dest (const char *src)
{
  return strcat (NULL, src); /* { dg-warning "use of NULL where non-null expected" } */
}

char *
test_null_src (char *dest)
{
  return strcat (dest, NULL); /* { dg-warning "use of NULL where non-null expected" } */
}

char *
test_uninit_dest (const char *src)
{
  char dest[10];
  return strcat (dest, src); /* { dg-warning "use of uninitialized value 'dest\\\[0\\\]'" } */
}

char *
test_uninit_src (char *dest)
{
  const char src[10];
  return strcat (dest, src); /* { dg-warning "use of uninitialized value 'src\\\[0\\\]'" } */
}

char *
test_dest_not_terminated (char *src)
{
  char dest[3] = "foo";
  return strcat (dest, src); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 1 \\('&dest'\\) of 'strcat'" "" { target *-*-* } .-1 } */
}

char *
test_src_not_terminated (char *dest)
{
  const char src[3] = "foo";
  return strcat (dest, src); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 2 \\('&src'\\) of 'strcat'" "" { target *-*-* } .-1 } */
}

char * __attribute__((noinline))
call_strcat (char *dest, const char *src)
{
  return strcat (dest, src);
}

void
test_concrete_valid_static_size (void)
{
  char buf[16];
  char *p1 = __builtin_strcpy (buf, "abc");
  char *p2 = call_strcat (buf, "def");
  __analyzer_eval (p1 == buf); /* { dg-warning "TRUE" } */
  __analyzer_eval (p2 == buf); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[0] == 'a'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[1] == 'b'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[2] == 'c'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[3] == 'd'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[4] == 'e'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[5] == 'f'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[6] == '\0'); /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf) == 6);  /* { dg-warning "TRUE" } */
}

void
test_concrete_valid_static_size_2 (void)
{
  char buf[16];
  char *p1 = __builtin_strcpy (buf, "abc");
  char *p2 = call_strcat (buf, "def");
  char *p3 = call_strcat (buf, "ghi");
  __analyzer_eval (p1 == buf); /* { dg-warning "TRUE" } */
  __analyzer_eval (p2 == buf); /* { dg-warning "TRUE" } */
  __analyzer_eval (p3 == buf); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[0] == 'a'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[1] == 'b'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[2] == 'c'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[3] == 'd'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[4] == 'e'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[5] == 'f'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[6] == 'g'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[7] == 'h'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[8] == 'i'); /* { dg-warning "TRUE" } */
  __analyzer_eval (buf[9] == '\0'); /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf) == 9);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 1) == 8);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 2) == 7);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 3) == 6);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 4) == 5);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 5) == 4);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 6) == 3);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 7) == 2);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 8) == 1);  /* { dg-warning "TRUE" } */
  __analyzer_eval (__builtin_strlen (buf + 9) == 0);  /* { dg-warning "TRUE" } */
}

char * __attribute__((noinline))
call_strcat_invalid (char *dest, const char *src)
{
  return strcat (dest, src); /* { dg-warning "stack-based buffer overflow" } */
}

void
test_concrete_invalid_static_size (void)
{
  char buf[3];
  buf[0] = '\0';
  call_strcat_invalid (buf, "abc");
}

void
test_concrete_symbolic (const char *suffix)
{
  char buf[10];
  buf[0] = '\0';
  call_strcat (buf, suffix);
}

/* TODO:
     - "The behavior is undefined if the strings overlap."
*/
