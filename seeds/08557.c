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

extern void unknown_fn (void *, void *);

static int a;
static int b = 42;
int c;
int d = 17;
struct { int x; int y; char rgb[3]; } s = {5, 10, {0x80, 0x40, 0x20}};
void *e = &d;

extern struct _IO_FILE *stderr;

/* If we're not on a direct path from "main", we know nothing about
   the values of globals.  */

void test (void)
{
  __analyzer_eval (a == 0); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (b == 42); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (c == 0); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (d == 17); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (s.rgb[2] == 0x20); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (e == &d); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (stderr == 0); /* { dg-warning "UNKNOWN" } */
}

static void __attribute__((noinline))
__analyzer_called_from_main (void)
{
  /* When accessed from main, the vars still have their initializer values.  */
  __analyzer_eval (a == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (b == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (c == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (d == 17); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.rgb[2] == 0x20); /* { dg-warning "TRUE" } */
  __analyzer_eval (e == &d); /* { dg-warning "TRUE" } */
  /* ...apart from those defined in a different TU (or that were inited
     before "main").  */
  __analyzer_eval (stderr == 0); /* { dg-warning "UNKNOWN" } */
}

int main (void)
{
  /* When accessed from main, the vars still have their initializer values.  */
  __analyzer_eval (a == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (b == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (c == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (d == 17); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.rgb[2] == 0x20); /* { dg-warning "TRUE" } */
  __analyzer_eval (e == &d); /* { dg-warning "TRUE" } */
  /* ...apart from those defined in a different TU (or that were inited
     before "main").  */
  __analyzer_eval (stderr == 0); /* { dg-warning "UNKNOWN" } */

  __analyzer_called_from_main ();

  unknown_fn (&a, &c);

  /* "a" escaped above and so could have been written to.  */
  __analyzer_eval (a == 0); /* { dg-warning "UNKNOWN" } */
  /* "b" doesn't escape and is static, and so must still have its
     initial value.  */
  __analyzer_eval (b == 42); /* { dg-warning "TRUE" } */
  /* The other globals are non-static and so have implicitly escaped,
     and so could have been written to.  */
  __analyzer_eval (c == 0); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (d == 17); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (s.rgb[2] == 0x20); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (e == &d); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (stderr == 0); /* { dg-warning "UNKNOWN" } */
}
