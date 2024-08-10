/* { dg-additional-options "-Wno-analyzer-too-complex" } */

/* C only: C++ exceptions cause a Wanalyzer-va-list-leak warning to be emitted
   at the end of commandrf. Therefore this test has been duplicated as
   c-c++-common/analyzer/pr99193-1-noexcept.c  */

/* Verify absence of false positive from -Wanalyzer-mismatching-deallocation
   on realloc(3).
   Based on https://github.com/libguestfs/libguestfs/blob/f19fd566f6387ce7e4d82409528c9dde374d25e0/daemon/command.c#L115
   which is GPLv2 or later.  */

typedef __SIZE_TYPE__ size_t;
typedef __builtin_va_list va_list;

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

extern void *malloc (size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__alloc_size__ (1)));
extern void perror (const char *__s);
extern void *realloc (void *__ptr, size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__warn_unused_result__))
  __attribute__ ((__alloc_size__ (2)));

extern void guestfs_int_cleanup_free (void *ptr);
extern int commandrvf (char **stdoutput, char **stderror, unsigned flags,
                       char const* const *argv);
#define CLEANUP_FREE __attribute__((cleanup(guestfs_int_cleanup_free))) 

int
commandrf (char **stdoutput, char **stderror, unsigned flags,
           const char *name, ...)
{
  va_list args;
  CLEANUP_FREE const char **argv = NULL;
  char *s;
  int i, r;

  /* Collect the command line arguments into an array. */
  i = 2;
  argv = (const char **) malloc (sizeof (char *) * i);

 if (argv == NULL) {
    perror ("malloc");
    return -1;
  }
  argv[0] = (char *) name;
  argv[1] = NULL;

  __builtin_va_start (args, name);

  while ((s = __builtin_va_arg (args, char *)) != NULL) {
    const char **p = (const char **) realloc (argv, sizeof (char *) * (++i)); /* { dg-bogus "'free'" } */
    if (p == NULL) {
      perror ("realloc");
      __builtin_va_end (args);
      return -1;
    }
    argv = p;
    argv[i-2] = s;
    argv[i-1] = NULL;
  }

  __builtin_va_end (args);

  r = commandrvf (stdoutput, stderror, flags, argv);

  return r;
}
