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

#ifdef __cplusplus
  #define CONST_CAST(type) const_cast<type>
#else
  #define CONST_CAST(type)
#endif

extern int execl(const char *pathname, const char *arg, ...);
extern int execlp(const char *file, const char *arg, ...);
extern int execle(const char *pathname, const char *arg, ...);
extern int execv(const char *pathname, char *const argv[]);
extern int execvp(const char *file, char *const argv[]);
extern int execvpe(const char *file, char *const argv[], char *const envp[]);

int test_execl_ls_al ()
{
  return execl ("/usr/bin/ls", "ls", "-al", NULL);
}

int test_execlpl_ls_al ()
{
  return execlp ("ls", "ls", "-al", NULL);
}

int test_execle_ls_al ()
{
  char * env[3] = {CONST_CAST(char *)("FOO=BAR"), CONST_CAST(char *)("BAZ"), NULL};
  return execl ("/usr/bin/ls", "ls", "-al", NULL, env);
}

int test_execv_ls_al ()
{
  char * argv[3] = {CONST_CAST(char *)("ls"), CONST_CAST(char *)("-al"), NULL};
  return execv ("/usr/bin/ls", argv);
}

int test_execvp_ls_al ()
{
  char *argv[3] = {CONST_CAST(char *)("ls"), CONST_CAST(char *)("-al"), NULL};
  return execvp ("ls", argv);
}

int test_execvpe_ls_al ()
{
  char *env[3] = {CONST_CAST(char *)("FOO=BAR"), CONST_CAST(char *)("BAZ"), NULL};
  char *argv[3] = {CONST_CAST(char *)("ls"), CONST_CAST(char *)("-al"), NULL};
  return execvpe ("ls", argv, env);
}
