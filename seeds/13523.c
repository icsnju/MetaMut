/* { dg-skip-if "" { powerpc*-*-aix* || newlib } } */
/* { dg-skip-if "" { avr-*-* } } */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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

void f (int fd) __attribute__((fd_arg(1))); /* { dg-message "argument 1 of 'f' must be an open file descriptor, due to '__attribute__\\(\\(fd_arg\\(1\\)\\)\\)'" } */

void
test_1 (const char *path)
{
    int fd = open (path, O_RDWR);
    close(fd);
    f(fd); /* { dg-warning "'f' on closed file descriptor 'fd'" } */
      /* { dg-message "\\(3\\) 'f' on closed file descriptor 'fd'; 'close' was at \\(2\\)" "" { target *-*-* } .-1 } */
}

void g (int fd) __attribute__((fd_arg_read(1))); /* { dg-message "argument 1 of 'g' must be a readable file descriptor, due to '__attribute__\\(\\(fd_arg_read\\(1\\)\\)\\)'" } */

void
test_2 (const char *path)
{
  int fd = open (path, O_WRONLY);
  if (fd != -1)
  {
    g (fd); /* { dg-warning "'g' on write-only file descriptor 'fd'" } */
  }
  close (fd);
}

void h (int fd) __attribute__((fd_arg_write(1))); /* { dg-message "argument 1 of 'h' must be a writable file descriptor, due to '__attribute__\\(\\(fd_arg_write\\(1\\)\\)\\)'" } */
void
test_3 (const char *path)
{
  int fd = open (path, O_RDONLY);
  if (fd != -1)
  {
    h (fd); /* { dg-warning "'h' on read-only file descriptor 'fd'" } */
  }
  close(fd);
}

void ff (int fd) __attribute__((fd_arg(1))); /* { dg-message "argument 1 of 'ff' must be an open file descriptor, due to '__attribute__\\(\\(fd_arg\\(1\\)\\)\\)'" } */

void test_4 (const char *path)
{
  int fd = open (path, O_RDWR);
  ff (fd); /* { dg-warning "'ff' on possibly invalid file descriptor 'fd'" } */
  close(fd);
}

void test_sm_fd_constants (void)
{
  __analyzer_dump_named_constant ("O_ACCMODE"); /* { dg-warning "named constant 'O_ACCMODE' has value '\[0-9\]+'" } */
  __analyzer_dump_named_constant ("O_RDONLY"); /* { dg-warning "named constant 'O_RDONLY' has value '\[0-9\]+'" } */
  __analyzer_dump_named_constant ("O_WRONLY"); /* { dg-warning "named constant 'O_WRONLY' has value '\[0-9\]+'" } */
}
