/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } } */

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

/* va_arg in loop, with no caller to function containing va_start.  */

int test_1a (const char *fmt, ...)
{
  __builtin_va_list args;
  int sum = 0;
  char ch;

  __builtin_va_start(args, fmt);

  while (ch = *fmt++)
    if (ch == '%')
      sum += __builtin_va_arg(args, int);

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with no caller to function containing va_start.  */

static int test_1b_callee (const char *fmt, __builtin_va_list args)
{
  int sum = 0;
  char ch;

  while (ch = *fmt++)
    if (ch == '%')
      sum += __builtin_va_arg(args, int);

  return sum;
}

int test_1b_caller (const char *fmt, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, fmt);

  sum = test_1b_callee (fmt, args);

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with a caller to the function containing va_start,
   with specific args.  */

static int
test_1c_inner (const char *fmt, __builtin_va_list args)
{
  int sum = 0;
  char ch;

  while (ch = *fmt++)
    if (ch == '%')
      sum += __builtin_va_arg(args, int);

  return sum;
}

static int
test_1c_middle (const char *fmt, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, fmt);

  sum = test_1c_inner (fmt, args);

  __builtin_va_end(args);

  return sum;
}

void test_1c_outer (void)
{
  int sum = test_1c_middle ("%%", 42, 17);

  __analyzer_describe (0, sum); /* { dg-message "'\\(int\\)59'" } */
}

/* va_arg in loop, with no caller to function containing va_start.  */

int test_2a (int count, ...)
{
  __builtin_va_list args;
  int sum = 0;
  char ch;

  __builtin_va_start(args, count);

  while (count-- > 0)
    sum += __builtin_va_arg(args, int);

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with no caller to function containing va_start.  */

static int test_2b_callee (int count, __builtin_va_list args)
{
  int sum = 0;

  while (count-- > 0)
    sum += __builtin_va_arg(args, int);

  return sum;
}

int test_2b_caller (int count, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, count);

  sum = test_2b_callee (count, args);

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with a caller to the function containing va_start,
   with specific args.  */

static int test_2c_inner (int count, __builtin_va_list args)
{
  int sum = 0;

  while (count-- > 0)
    sum += __builtin_va_arg(args, int);

  return sum;
}

int test_2c_middle (int count, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, count);

  sum = test_2c_inner (count, args);

  __builtin_va_end(args);

  return sum;
}

void test_2c_outer (void)
{
  int sum = test_2c_middle (2, 50, 42);

  __analyzer_describe (0, sum); /* { dg-message "'\\(int\\)92'" } */
}

/* va_arg in loop, with no caller to function containing va_start.  */

int test_3a (int placeholder, ...)
{
  __builtin_va_list args;
  int sum = 0;
  int val;

  __builtin_va_start(args, placeholder);

  while (val = __builtin_va_arg(args, int))
    sum += val;

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with no caller to function containing va_start.  */

static int test_3b_callee (__builtin_va_list args)
{
  int sum = 0;
  int val;
  while (val = __builtin_va_arg(args, int))
    sum += val;
  return sum;
}

int test_3b_caller (int placeholder, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, placeholder);

  sum = test_3b_callee (args);

  __builtin_va_end(args);

  return sum;
}

/* va_arg in loop, with a caller to the function containing va_start,
   with specific args.  */

static int test_3c_inner (__builtin_va_list args)
{
  int sum = 0;
  int val;
  while (val = __builtin_va_arg(args, int))
    sum += val;
  return sum;
}

int test_3c_middle (int placeholder, ...)
{
  __builtin_va_list args;
  int sum = 0;

  __builtin_va_start(args, placeholder);

  sum = test_3c_inner (args);

  __builtin_va_end(args);

  return sum;
}

void test_3c_outer (void)
{
  int sum = test_3c_middle (0, 5, 12, 0);
  __analyzer_describe (0, sum); /* { dg-message "'\\(int\\)17'" } */
}

/* va_arg in loop, with no caller to function containing va_start,
   with a va_copy.  */

static int test_3d_callee (__builtin_va_list args)
{
  int sum = 0;
  int val;
  while (val = __builtin_va_arg(args, int))
    sum += val;
  return sum;
}

int test_3d_caller (int placeholder, ...)
{
  __builtin_va_list args1, args2;
  int sum = 0;

  __builtin_va_start(args1, placeholder);
  __builtin_va_copy (args2, args1);

  sum = test_3d_callee (args1);
  __builtin_va_end(args1);

  sum += test_3d_callee (args2);
  __builtin_va_end(args2);

  return sum;
}

/* va_arg in loop, with a caller to the function containing va_start,
   with specific args, with a va_copy.  */

static int test_3e_inner (__builtin_va_list args)
{
  int sum = 0;
  int val;
  while (val = __builtin_va_arg(args, int))
    sum += val;
  return sum;
}

int test_3e_middle (int placeholder, ...)
{
  __builtin_va_list args1, args2;
  int sum = 0;

  __builtin_va_start(args1, placeholder);
  __builtin_va_copy (args2, args1);

  sum = test_3e_inner (args1);
  __builtin_va_end(args1);

  sum += test_3e_inner (args2);
  __builtin_va_end(args2);

  return sum;
}

void test_3e_outer (void)
{
  int sum = test_3e_middle (0, 5, 6, 0);
  __analyzer_describe (0, sum); /* { dg-message "'\\(int\\)22'" } */
}

/* va_arg in loop, with specific symbolic args.  */

static int test_3f_callee (int placeholder, ...)
{
  __builtin_va_list args;
  int sum = 0;
  int val;

  __builtin_va_start(args, placeholder);

  while (val = __builtin_va_arg(args, int))
    sum += val;

  __builtin_va_end(args);

  return sum;
}

void test_3f_caller (int x, int y, int z)
{
  int sum = test_3f_callee (0, x, y, z, 0);
  __analyzer_describe (0, sum); /* { dg-message "'UNKNOWN\\(int\\)'" } */
}
