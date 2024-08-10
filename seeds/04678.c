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

void test_leak_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "datagram socket created here" } */
} /* { dg-warning "leak of file descriptor 'fd'" } */

void test_leak_socket_no_lhs (void)
{
  socket (AF_UNIX, SOCK_DGRAM, 0);  /* { dg-warning "leak of file descriptor" } */
}

void test_close_unchecked_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0);
  close (fd);
}

void test_close_checked_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0);
  if (fd == -1)
    return;
  __analyzer_dump_state ("file-descriptor", fd); /* { dg-warning "state: 'fd-new-datagram-socket'" } */
  close (fd);
}

void test_leak_checked_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "datagram socket created here" } */
  if (fd == -1) /* { dg-warning "leak of file descriptor 'fd'" } */
    return;
  // TODO: strange location for leak message
}

void test_bind (const char *sockname)
{
  struct sockaddr_un addr;
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0);
  if (fd == -1)
    return;
  __analyzer_dump_state ("file-descriptor", fd); /* { dg-warning "state: 'fd-new-datagram-socket'" } */
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr));
  close (fd);
}

void test_bind_on_unchecked_socket (const char *sockname)
{
  struct sockaddr_un addr;
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "when 'socket' fails" } */
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "'bind' on possibly invalid file descriptor 'fd'" } */
  close (fd);
}

void test_leak_of_bound_socket (const char *sockname)
{
  struct sockaddr_un addr;
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "datagram socket created here" } */
  if (fd == -1)
    return;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  bind (fd, (struct sockaddr *)&addr, sizeof (addr)); /* { dg-warning "leak of file descriptor 'fd'" } */
}

void test_listen_on_datagram_socket_without_bind (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "datagram socket created here" }  */
  if (fd == -1)
    return;
  __analyzer_dump_state ("file-descriptor", fd); /* { dg-warning "state: 'fd-new-datagram-socket'" } */
  listen (fd, 5); /* { dg-warning "'listen' on datagram socket file descriptor 'fd' \\\[-Wanalyzer-fd-type-mismatch\\\]" "warning" } */
  /* { dg-message "'listen' expects a stream socket file descriptor but 'fd' is a datagram socket" "final event" { target *-*-* } .-1 } */
  close (fd);
}

void test_listen_on_datagram_socket_with_bind (const char *sockname)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0); /* { dg-message "datagram socket created here" }  */
  if (fd == -1)
    return;

  __analyzer_dump_state ("file-descriptor", fd); /* { dg-warning "state: 'fd-new-datagram-socket'" } */

  struct sockaddr_un addr;
  memset (&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
  if (bind (fd, (struct sockaddr *)&addr, sizeof (addr)) == -1) /* { dg message "datagram socket bound here" } */
    {
      close (fd);
      return;
    }
  listen (fd, 5); /* { dg-warning "'listen' on datagram socket file descriptor 'fd' \\\[-Wanalyzer-fd-type-mismatch\\\]" "warning" } */
  /* { dg-message "'listen' expects a stream socket file descriptor but 'fd' is a datagram socket" "final event" { target *-*-* } .-1 } */
  close (fd);
}
