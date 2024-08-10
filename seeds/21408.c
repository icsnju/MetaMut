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


extern void *calloc (size_t __nmemb, size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__alloc_size__ (1, 2))) ;
extern void *malloc (size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__alloc_size__ (1)));
extern void *realloc (void *__ptr, size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__warn_unused_result__))
  __attribute__ ((__alloc_size__ (2)));
extern void free (void *__ptr)
  __attribute__ ((__nothrow__ , __leaf__));

/* realloc where we don't know the original size of the region.  */

char *test_8 (char *p, size_t sz)
{
  char *q;

  __analyzer_dump_capacity (p); /* { dg-warning "capacity: 'UNKNOWN\\(\[^\n\r\]*\\)'" } */

  p[0] = 'a';
  p[1] = 'b';
  p[2] = 'c';
  __analyzer_eval (p[0] == 'a'); /* { dg-warning "TRUE" } */
  __analyzer_eval (p[1] == 'b'); /* { dg-warning "TRUE" } */
  __analyzer_eval (p[2] == 'c'); /* { dg-warning "TRUE" } */

  q = (char *) realloc (p, 6);

  /* We should have 3 nodes, corresponding to "failure",
     "success without moving", and "success with moving".  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "3 processed enodes" } */
  
  if (q)
    {
      q[3] = 'd';
      q[4] = 'e';
      q[5] = 'f';
      if (q == p)
	{
	  /* "realloc" success, growing the buffer in-place.  */
	  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
	  __analyzer_dump_capacity (q); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)6'" } */
	  __analyzer_eval (q[0] == 'a'); /* { dg-warning "TRUE" } */
	  __analyzer_eval (q[1] == 'b'); /* { dg-warning "TRUE" } */
	  __analyzer_eval (q[2] == 'c'); /* { dg-warning "TRUE" } */
	}
      else
	{
	  /* "realloc" success, moving the buffer (and thus freeing "p").  */
	  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
	  __analyzer_dump_capacity (q); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)6'" } */
	  /* We don't know how much of the buffer is copied.  */
	  __analyzer_eval (q[0] == 'a'); /* { dg-warning "UNKNOWN" } */
	  __analyzer_eval (q[1] == 'b'); /* { dg-warning "UNKNOWN" } */
	  __analyzer_eval (q[2] == 'c'); /* { dg-warning "UNKNOWN" } */
	  __analyzer_eval (p[0] == 'a'); /* { dg-warning "UNKNOWN" "unknown" } */
	  /* { dg-warning "use after 'free' of 'p'" "use after free" { target *-*-* } .-1 } */
	}
      __analyzer_eval (q[3] == 'd'); /* { dg-warning "TRUE" } */
      __analyzer_eval (q[4] == 'e'); /* { dg-warning "TRUE" } */
      __analyzer_eval (q[5] == 'f'); /* { dg-warning "TRUE" } */
    }
  else
    {
      /* "realloc" failure.  p should be unchanged.  */
      __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
      __analyzer_dump_capacity (q); /* { dg-warning "capacity: 'UNKNOWN\\(\[^\n\r\]*\\)'" } */
      __analyzer_eval (p[0] == 'a'); /* { dg-warning "TRUE" } */
      __analyzer_eval (p[1] == 'b'); /* { dg-warning "TRUE" } */
      __analyzer_eval (p[2] == 'c'); /* { dg-warning "TRUE" } */
      return p;
    }

  return q;
}
