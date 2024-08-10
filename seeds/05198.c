/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } } */

#include <stdlib.h>
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

typedef struct boxed_ptr { void *value; } boxed_ptr;

boxed_ptr __attribute__((noinline))
boxed_malloc (size_t sz)
{
  boxed_ptr result;
  result.value = malloc (sz);
  return result;
}

boxed_ptr __attribute__((noinline))
boxed_free (boxed_ptr ptr)
{
  free (ptr.value);
}

const boxed_ptr boxed_null = {NULL};

boxed_ptr test_1 (int flag)
{
  boxed_ptr ptr = boxed_malloc (sizeof (int));

  if (flag) /* { dg-message "following 'false' branch" } */
    if (!ptr.value)
      return boxed_null;

  *((int *)ptr.value) = 42; /* { dg-warning "dereference of possibly-NULL '\[^\n\r\]*'" } */

  return ptr;
}

void test_2 (int flag)
{
  boxed_ptr ptr;

  if (flag)
    ptr = boxed_malloc (4096);
  else
    ptr = boxed_malloc (1024);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  boxed_free (ptr);  

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}

void test_3 (int kind)
{
  boxed_ptr ptr;

  switch (kind)
    {
    default:
      ptr = boxed_malloc (4096);
      break;
    case 0:
      ptr = boxed_malloc (128);
      break;
    case 1:
      ptr = boxed_malloc (1024);
      break;
    case 2:
      ptr = boxed_malloc (65536);
      break;
    }

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "4 processed enodes" } */

  boxed_free (ptr);  

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}
