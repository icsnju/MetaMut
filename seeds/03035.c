/* { dg-additional-options "-fanalyzer-transitivity" } */

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

void test (int i)
{
  switch (i)
    {
    case 0:
      __analyzer_eval (i == 0); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != -1); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 0); /* { dg-warning "FALSE" } */
      __analyzer_eval (i != 1); /* { dg-warning "TRUE" } */
      break;

    case 3 ... 5:
      __analyzer_eval (i != 0); /* { dg-warning "TRUE" } */
      __analyzer_eval (i > 1); /* { dg-warning "TRUE" } */
      __analyzer_eval (i > 2); /* { dg-warning "TRUE" } */
      __analyzer_eval (i >= 2); /* { dg-warning "TRUE" } */
      __analyzer_eval (i >= 3); /* { dg-warning "TRUE" } */
      __analyzer_eval (i <= 5); /* { dg-warning "TRUE" } */
      __analyzer_eval (i < 6); /* { dg-warning "TRUE" } */
      __analyzer_eval (i <= 6); /* { dg-warning "TRUE" } */
      __analyzer_eval (i < 7); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 6); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 3); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 4); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 5); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i >= 4); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i >= 5); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i <= 3); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i <= 4); /* { dg-warning "UNKNOWN" } */
      break;

    default:
      __analyzer_eval (i == -1); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i == 0); /* { dg-warning "FALSE" } */
      __analyzer_eval (i == 2); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i == 3); /* { dg-warning "FALSE" } */
      __analyzer_eval (i == 4); /* { dg-warning "FALSE" } */
      __analyzer_eval (i == 5); /* { dg-warning "FALSE" } */
      __analyzer_eval (i == 6); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 0); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 1); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 3); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 4); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 5); /* { dg-warning "TRUE" } */
      __analyzer_eval (i != 6); /* { dg-warning "UNKNOWN" } */
      break;
    }
}

/* Verify that the analyzer follows the correct paths on a
   switch statement guarded by an if, using noinline to defeat
   optimizations.  */

static void __attribute__((noinline))
__analyzer_called_by_test_2 (int y)
{
  switch (y)
    {
    case 0:
      __analyzer_dump_path (); /* { dg-bogus "path" } */
      break;
    case 1:
      __analyzer_dump_path (); /* { dg-message "path" } */
      break;
    case 2:
      __analyzer_dump_path (); /* { dg-bogus "path" } */
      break;
    default:
      __analyzer_dump_path (); /* { dg-bogus "path" } */
      break;
    }
}

void test_2 (int x)
{
  if (x == 1)
    __analyzer_called_by_test_2 (x);
}

void test_3 (int x, int y)
{
  if (y == 3)
    switch (x)
      {
      case 0 ... 9:
      case 20 ... 29:
	if (x == y)
	  __analyzer_dump_path (); /* { dg-message "path" } */
	else
	  __analyzer_dump_path (); /* { dg-message "path" } */
      }
}

struct s4
{
  unsigned char level:3;
  unsigned char key_id_mode:2;
  unsigned char reserved:3;
};

void test_4 (struct s4 *p)
{
  switch (p->key_id_mode)
    {
    case 0:
      __analyzer_dump_path (); /* { dg-message "path" } */
      break;
    case 1:
      __analyzer_dump_path (); /* { dg-message "path" } */
      break;
    case 2:
      __analyzer_dump_path (); /* { dg-message "path" } */
      break;
    case 3:
      __analyzer_dump_path (); /* { dg-message "path" } */
      break;
    }
  __analyzer_dump_path (); /* { dg-message "path" } */
}

int test_5 (unsigned v)
{
  switch (v)
    {
    case 0:
      return 7;
      break;
    case 1:
      return 23;
      break;
    default:
      return v * 2;
    }
}

int test_6 (unsigned v)
{
  switch (v)
    {
    case 0:
      return 3;
    case -1:
      return 22;
    }
  return -3;
}

int g7 = -1;
int test_7 ()
{
	switch (g7++) {
	case 0:
	  return 32;

	case 100:
	  return 42;
	}
	return 0;
}

int test_bitmask_1 (int x)
{
  int flag = 0;
  if (x & 0x80)
    flag = 1;

  switch (x)
    {
    case 0:
      if (flag)
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      else
	__analyzer_dump_path ();  /* { dg-message "path" } */
      break;

    case 0x80:
      if (flag)
	__analyzer_dump_path ();  /* { dg-message "path" } */
      else
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      break;

    case 0x81:
      if (flag)
	__analyzer_dump_path ();  /* { dg-message "path" } */
      else
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      break;
    }
}

int test_bitmask_2 (int x)
{
  int flag = 0;
  if ((x & 0xf80) == 0x80)
    flag = 1;

  switch (x)
    {
    case 0:
      if (flag)
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      else
	__analyzer_dump_path ();  /* { dg-message "path" } */
      break;

    case 0x80:
      if (flag)
	__analyzer_dump_path ();  /* { dg-message "path" } */
      else
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      break;

    case 0x81:
      if (flag)
	__analyzer_dump_path ();  /* { dg-message "path" } */
      else
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      break;

    case 0x180:
      if (flag)
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      else
	__analyzer_dump_path ();  /* { dg-message "path" } */
      break;

    case 0xf80:
      if (flag)
	__analyzer_dump_path ();  /* { dg-bogus "path" } */
      else
	__analyzer_dump_path ();  /* { dg-message "path" } */
      break;
    }
}
