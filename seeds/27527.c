/* { dg-require-effective-target sockets } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */

#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
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

void test_bind (int fd, const char *sockname)
{
  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  if (bind (fd, (struct sockaddr *)&addr, sizeof (addr)) == -1)
    __analyzer_dump_path (); /* { dg-message "path" } */
  else
    __analyzer_dump_path (); /* { dg-message "path" } */
}

void test_null_bind (int fd)
{
  errno = 0;
  int result = bind (fd, NULL, 0);
  __analyzer_eval (result == -1); /* { dg-warning "TRUE" } */
  __analyzer_eval (errno > 0); /* { dg-warning "TRUE" } */
}

void test_double_bind (int fd, const char *sockname)
{
  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr));
  bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "'bind' on file descriptor 'fd' in wrong phase \\\[-Wanalyzer-fd-phase-mismatch\\\]" "warning" } */
  /* { dg-message "'bind' expects a new socket file descriptor but 'fd' has already been bound" "final event" { target *-*-* } .-1 } */
}

int test_uninit_addr (int fd, const char *sockname)
{
  struct sockaddr_un addr;
  return bind (fd, (struct sockaddr *)&addr, sizeof (addr));
  // TODO: complain about uninit addr.
}

void test_bind_after_connect (int fd, const char *sockname,
				const struct sockaddr *caddr, socklen_t caddrlen)
{
  if (connect (fd, caddr, caddrlen) == -1)
    return;

  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr));
  /* TODO: we don't warn for this; after the plain "connect" we're
     in the stop state.  */
}

void test_bind_after_accept (int fd, const char *sockname)
{
  int afd = accept (fd, NULL, NULL);
  if (afd == -1)
    return;

  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (afd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "'bind' on file descriptor 'afd' in wrong phase \\\[-Wanalyzer-fd-phase-mismatch\\\]" "warning" } */
  /* { dg-message "'bind' expects a new socket file descriptor but 'afd' is already connected" "final event" { target *-*-* } .-1 } */

  close (afd);
}

int test_bind_on_constant ()
{
  return bind (0, NULL, 0);
}
