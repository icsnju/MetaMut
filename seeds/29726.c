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

int test_connect (int sockfd, const struct sockaddr *addr,
		  socklen_t addrlen)
{
  return connect (sockfd, addr, addrlen);
}

void test_null_connect (int fd)
{
  errno = 0;
  int result = connect (fd, NULL, 0);
  __analyzer_eval (result == -1); /* { dg-warning "TRUE" } */
  __analyzer_eval (errno > 0); /* { dg-warning "TRUE" } */
}

int test_uninit_addr (int fd, const char *sockname)
{
  struct sockaddr_un addr;
  return connect (fd, (struct sockaddr *)&addr, sizeof (addr));
  // TODO: complain about uninit addr.
}

void test_connect_after_bind (const char *sockname,
			      const struct sockaddr *baddr, socklen_t baddrlen,
			      const struct sockaddr *caddr, socklen_t caddrlen)
{
  int fd = socket (AF_UNIX, SOCK_STREAM, 0); /* { dg-message "stream socket created here" } */
  if (fd == -1)
    return;

  if (bind (fd, baddr, baddrlen) == -1)
    {
      close (fd);
      return;
    }

  connect (fd, caddr, caddrlen); /* { dg-warning "'connect' on file descriptor 'fd' in wrong phase" "warning" } */
  /* { dg-message "'connect' expects a new socket file descriptor but 'fd' is bound" "final event" { target *-*-* } .-1 } */

  close (fd);      
}

int test_connect_on_constant ()
{
  return connect (0, NULL, 0);
}
