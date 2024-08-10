/* Copyright (C) 2001-2017 Free Software Foundation, Inc.  */

/* { dg-do preprocess } */
/* { dg-options "-std=gnu99 -pedantic -Wtraditional -ftrack-macro-expansion=0" } */

/* Tests diagnostics are suppressed for some macros defined in system
   headers.  */

/* Source: Neil Booth, 15 Jan 2001.  */

/* Indented to avoid "suggest hiding ..." warnings.   */
 #pragma GCC system_header

#define sys_uint 1U
#define sys_str(x) x
#define sys_foo(x, y...) bar (x, ##y)

#define sys_uint 1U
#define sys_fl 1.0f
#define sys_ld 1.0L

#define uint 1U
#define str(x) x
#define foo(x, y...) bar(x, ##y) /* { dg-warning "named variadic macros" } */

#if uint			/* { dg-warning "traditional C rejects" } */
#endif
#if sys_uint			/* { dg-bogus "traditional C rejects" } */
#endif

(str);				/* { dg-warning "used with arguments" } */
(sys_str);			/* { dg-bogus "used with arguments" } */

foo (one_arg);			/* { dg-bogus "requires at least one" } */
sys_foo (one_arg);		/* { dg-bogus "requires at least one" } */