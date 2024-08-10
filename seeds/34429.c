/* Verify absence of false positive from -Wanalyzer-mismatching-deallocation
   on realloc(3).
   Based on https://github.com/libguestfs/libguestfs/blob/f19fd566f6387ce7e4d82409528c9dde374d25e0/df/main.c#L404
   which is GPLv2 or later.  */

/* { dg-additional-options "-Wno-analyzer-too-complex" } */

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

extern void free (void *);
extern void *realloc (void *__ptr, size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__warn_unused_result__))
  __attribute__ ((__alloc_size__ (2)));
char *strdup (const char *)
  __attribute__((malloc (free)));

extern void error (int __status, int __errnum, const char *__format, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern int errno;

struct drv
{
  struct drv *next;
};

#define EXIT_FAILURE 1

static char *
single_drive_display_name (struct drv *)
{
  char *result = strdup ("placeholder");
  if (!result)
    __builtin_abort ();
  return result;
}

char *
make_display_name (struct drv *drvs)
{
  char *ret;

  if (drvs->next == NULL)
    ret = single_drive_display_name (drvs);
  else {
    size_t pluses = 0;
    size_t i, len;

    while (drvs->next != NULL) {
      drvs = drvs->next;
      pluses++;
    }

    ret = single_drive_display_name (drvs);
    len = __builtin_strlen (ret);

    ret = (char *) realloc (ret, len + pluses + 1); /* { dg-bogus "'free'" } */
    if (ret == NULL)
      error (EXIT_FAILURE, errno, "realloc");
    for (i = len; i < len + pluses; ++i)
      ret[i] = '+';
    ret[i] = '\0';
  }

  return ret;
}
