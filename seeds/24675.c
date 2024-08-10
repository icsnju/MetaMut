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

int test_listen (int fd, int backlog)
{
  return listen (fd, backlog);
}

/* Some systems seem to allow repeated calls to listen.  */

void test_double_listen (int fd, int backlog)
{
  listen (fd, backlog);
  listen (fd, backlog);
}

void test_listen_before_bind (int fd, const char *sockname)
{
  if (listen (fd, 5) == -1) /* { dg-message "stream socket marked as passive here via 'listen'" } */
    return;

  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "'bind' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'bind' expects a new socket file descriptor but 'fd' is already listening" "final event" { target *-*-* } .-1 } */
}

void test_listen_on_unchecked_bind (int fd, const char *sockname)
{
  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-message "when 'bind' fails" } */
  listen (fd, 5); /* { dg-warning "'listen' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'listen' expects a bound stream socket file descriptor but 'fd' has not yet been bound" "final event" { target *-*-* } .-1 } */
  close (fd);
}

void test_listen_on_new_datagram_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0);
  if (fd == -1)
    return;
  listen (fd, 5); /* { dg-message "'listen' on datagram socket file descriptor 'fd' \\\[-Wanalyzer-fd-type-mismatch\\\]" "warning" } */
  /* { dg-message "'listen' expects a stream socket file descriptor but 'fd' is a datagram socket" "final event" { target *-*-* } .-1 } */
  close (fd);
}

void test_listen_on_connected_socket (int fd)
{
  int afd = accept (fd, NULL, 0);
  if (afd == -1)
    return;
  listen (afd, 5); /* { dg-warning "'listen' on file descriptor 'afd' in wrong phase" "warning" } */
  /* { dg-message "'listen' expects a bound stream socket file descriptor but 'afd' is connected" "final event" { target *-*-* } .-1 } */
  close (afd);
}

int test_listen_on_constant ()
{
  return listen (0, 10);
}
