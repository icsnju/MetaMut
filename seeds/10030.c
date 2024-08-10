/* Various operations done on sockets in the wrong phase.  */

/* { dg-require-effective-target sockets } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

void test_read_on_new_socket (void *buf)
{
  int fd = socket (AF_UNIX, SOCK_STREAM, 0); /* { dg-message "stream socket created here" } */
  if (fd == -1)
    return;
  read (fd, buf, 1); /* { dg-warning "'read' on file descriptor 'fd' in wrong phase \\\[-Wanalyzer-fd-phase-mismatch\\\]" "warning" } */
  /* { dg-message "'read' expects a stream socket to be connected via 'accept' but 'fd' has not yet been bound" "final event" { target *-*-* } .-1 } */
  close (fd);
}

void test_read_on_bound_socket (int fd, const char *sockname, void *buf)
{
  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  if (bind (fd, (struct sockaddr *)&addr, sizeof (addr)) == -1)
    return;
  /* This could be a datagram socket, so we shouldn't complain here.  */
  read (fd, buf, 1);
}

void test_read_on_listening_socket (int fd, void *buf)
{
  if (listen (fd, 5) == -1) /* { dg-message "stream socket marked as passive here via 'listen'" } */
    return;
  read (fd, buf, 1); /* { dg-message "'read' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'read' expects a stream socket to be connected via the return value of 'accept' but 'fd' is listening; wrong file descriptor\\\?" "final event" { target *-*-* } .-1 } */
}

void test_bind_on_non_socket (const char *filename, const char *sockname)
{
  int fd = open (filename, O_RDONLY);
  if (fd == -1)
    return;

  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  int result = bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "'bind' on non-socket file descriptor 'fd' \\\[-Wanalyzer-fd-type-mismatch\\\]" "warning" } */
  /* { dg-message "'bind' expects a socket file descriptor but 'fd' is not a socket" "final event" { target *-*-* } .-1 } */
  __analyzer_eval (result == -1); /* { dg-warning "TRUE" } */
  
  close (fd);
}

void test_passive_open_read_on_wrong_socket (int sfd)
{
  int cfd = accept (sfd, NULL, NULL);
  write (sfd, "hello", 6); /* { dg-warning "'write' on file descriptor 'sfd' in wrong phase" "warning" } */
  /* { dg-message "'write' expects a stream socket to be connected via the return value of 'accept' but 'sfd' is listening; wrong file descriptor\\\?" "final event" { target *-*-* } .-1 } */
  close (cfd);
}

void test_listen_on_new_stream_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1)
    return;
  listen (fd, 5); /* { dg-message "'listen' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'listen' expects a bound stream socket file descriptor but 'fd' has not yet been bound" "final event" { target *-*-* } .-1 } */
  close (fd);
}

void test_accept_on_new_stream_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1)
    return;
  accept (fd, NULL, NULL); /* { dg-message "'accept' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'accept' expects a listening stream socket file descriptor but 'fd' has not yet been bound" "final event" { target *-*-* } .-1 } */
  close (fd);
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
