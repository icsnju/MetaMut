/* { dg-require-effective-target sockets } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */

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

int test_accept (int fd, struct sockaddr *addr, socklen_t *addrlen)
{
  return accept (fd, addr, addrlen);
}

void test_accept_leak_no_lhs (int fd, struct sockaddr *addr, socklen_t *addrlen)
{
  accept (fd, addr, addrlen); /* { dg-warning "leak of file descriptor" } */
}

void test_accept_leak_with_lhs (int fd, struct sockaddr *addr, socklen_t *addrlen)
{
  int newfd = accept (fd, addr, addrlen); /* { dg-message "socket created here" } */
} /* { dg-warning "leak of file descriptor 'newfd'" } */

int test_accept_null_addr (int fd)
{
  return accept (fd, NULL, 0);
}

int test_accept_uninit_addrlen (int fd)
{
  struct sockaddr_storage addr;
  socklen_t addr_len;
  return accept (fd, (struct sockaddr *)&addr, &addr_len); /* { dg-warning "use of uninitialized value 'addr_len'" } */
}

int test_accept_writes_to_addr_and_len (int fd)
{
  struct sockaddr_storage addr;
  socklen_t addr_len = sizeof (addr);
  __analyzer_eval (addr_len == sizeof (addr)); /* { dg-warning "TRUE" } */
  int newfd = accept (fd, (struct sockaddr *)&addr, &addr_len);
  if (newfd == -1)
    return newfd;
  /* Check that the analyzer considers addr and addr_len to
     have been written to.  */
  __analyzer_eval (((char *)&addr)[0]); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (addr_len == sizeof (addr)); /* { dg-warning "UNKNOWN" } */
  return newfd;
}

void test_accept_on_new_datagram_socket (void)
{
  int fd = socket (AF_UNIX, SOCK_DGRAM, 0);
  if (fd == -1)
    return;
  accept (fd, NULL, NULL); /* { dg-message "'accept' on datagram socket file descriptor 'fd' \\\[-Wanalyzer-fd-type-mismatch\\\]" "warning" } */
  /* { dg-message "'accept' expects a stream socket file descriptor but 'fd' is a datagram socket" "final event" { target *-*-* } .-1 } */
  close (fd);
}

int test_accept_on_accept (int fd_a)
{
  int fd_b = accept (fd_a, NULL, 0);
  if (fd_b == -1)
    return -1;

  int fd_c = accept (fd_b, NULL, 0);  /* { dg-warning "'accept' on file descriptor 'fd_b' in wrong phase \\\[-Wanalyzer-fd-phase-mismatch\\\]" "warning" } */
  /* { dg-message "'accept' expects a listening stream socket file descriptor but 'fd_b' is connected" "final event" { target *-*-* } .-1 } */

  return fd_b;
}

int test_accept_on_constant ()
{
  return accept (0, NULL, 0);
}
