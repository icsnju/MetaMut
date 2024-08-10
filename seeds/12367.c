/* { dg-options "-Wno-psabi" { target { { i?86-*-* x86_64-*-* } && ilp32 } } } */

/* Disable this test for 16-bit targets.  */

#include <limits.h>

#if !(defined __GNUC__) || (INT_MAX > 32767)

/*  Several of the binary compatibility tests use these macros to
    allow debugging the test or tracking down a failure by getting an
    indication of whether each individual check passed or failed.
    When DBG is defined, each check is shown by a dot (pass) or 'F'
    (fail) rather than aborting as soon as a failure is detected.  */
 
#ifdef DBG
#include <stdio.h>
#define DEBUG_INIT setbuf (stdout, NULL);
#define DEBUG_FPUTS(x) fputs (x, stdout)
#define DEBUG_DOT putc ('.', stdout)
#define DEBUG_NL putc ('\n', stdout)
#define DEBUG_FAIL putc ('F', stdout); fails++
#define DEBUG_CHECK { DEBUG_FAIL; } else { DEBUG_DOT; }
#define DEBUG_FINI if (fails) DEBUG_FPUTS ("failed\n"); \
		   else DEBUG_FPUTS ("passed\n");
#else
#define DEBUG_INIT
#define DEBUG_FPUTS(x)
#define DEBUG_DOT
#define DEBUG_NL
#define DEBUG_FAIL abort ()
#define DEBUG_CHECK abort ();
#define DEBUG_FINI
#endif

#ifdef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
#define SKIP_COMPLEX_INT
#endif
#endif

#ifndef SKIP_COMPLEX
#ifdef __GNUC__
#define CINT(x, y) (x + y * __extension__ 1i)
#define CDBL(x, y) (x + y * __extension__ 1.0i)
#else
#ifdef __SUNPRO_C
/* ??? Complex support without <complex.h>.  */
#else
#include <complex.h>
#endif
#ifndef SKIP_COMPLEX_INT
#define CINT(x, y) ((_Complex int) (x + y * _Complex_I))
#endif
#define CDBL(x, y) (x + y * _Complex_I)
#endif
#endif

#ifdef __cplusplus
extern "C" void abort (void);
#else
extern void abort (void);
#endif
extern int fails;
/* Define several variants of struct epoll_event from the Linux kernel,
   specifying various attributes that affect alignment and size.
 
   This test was developed for systems for which int is 32 bits and
   long long is 64 bits; it might need to be disabled for systems where
   either of those is not true.  */

#define DESC_orig "original"
struct epoll_event_orig {
  unsigned int events;
  unsigned long long data;
};

#ifndef SKIP_ATTRIBUTE
#define DESC_structmax "maximum useful struct alignment"
struct epoll_event_structmax {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((aligned));


#define DESC_struct4 "4-byte struct alignment"
struct epoll_event_struct4 {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((aligned(4)));

#define DESC_struct8 "8-byte struct alignment"
struct epoll_event_struct8 {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((aligned(8)));

#define DESC_data4 "4-byte alignment for data"
struct epoll_event_data4 {
  unsigned int events;
  unsigned long long data __attribute__ ((aligned(4)));
};

#define DESC_data8 "8-byte alignment for data"
struct epoll_event_data8 {
  unsigned int events;
  unsigned long long data __attribute__ ((aligned(8)));
};

#define DESC_p "packed attribute"
struct epoll_event_p {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((packed));

#define DESC_pstruct4 "packed attribute, 4-byte struct alignment"
struct epoll_event_pstruct4 {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((packed)) __attribute__ ((aligned(4)));

#define DESC_pstruct8 "packed attribute, 8-byte struct alignment"
struct epoll_event_pstruct8 {
  unsigned int events;
  unsigned long long data;
} __attribute__ ((packed)) __attribute__ ((aligned(8)));

#define DESC_pdata4 "packed attribute, 4-byte alignment for data"
struct epoll_event_pdata4 {
  unsigned int events;
  unsigned long long data __attribute__ ((aligned(4)));
} __attribute__ ((packed));

#define DESC_pdata8 "packed attribute, 8-byte alignment for data"
struct epoll_event_pdata8 {
  unsigned int events;
  unsigned long long data __attribute__ ((aligned(8)));
} __attribute__ ((packed));
#endif

#define TEST(NAME)						\
struct outer_##NAME {						\
  int i;							\
  struct epoll_event_##NAME ee;					\
};								\
								\
extern unsigned int v1_##NAME;					\
extern unsigned int v2_##NAME;					\
extern unsigned long long v3_##NAME;				\
								\
extern struct outer_##NAME s_##NAME[2];				\
								\
extern void pass_##NAME (struct outer_##NAME);			\
extern struct outer_##NAME return_##NAME (void);		\
								\
void								\
checkp_##NAME (struct outer_##NAME *p)				\
{								\
  if (p->i != v1_##NAME)					\
    DEBUG_CHECK;						\
  if (p->ee.events != v2_##NAME)				\
    DEBUG_CHECK;						\
  if (p->ee.data != v3_##NAME)					\
    DEBUG_CHECK;						\
}								\
								\
void								\
test_##NAME (void)						\
{								\
  struct outer_##NAME s;					\
  DEBUG_FPUTS (DESC_##NAME);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  global array");				\
  checkp_##NAME (&s_##NAME[0]);					\
  checkp_##NAME (&s_##NAME[1]);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  argument");					\
  pass_##NAME (s_##NAME[0]);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  function result");				\
  s = return_##NAME ();						\
  checkp_##NAME (&s);						\
  DEBUG_NL;							\
}

TEST (orig)
#ifndef SKIP_ATTRIBUTE
TEST (structmax)
TEST (struct4)
TEST (struct8)
TEST (data4)
TEST (data8)
TEST (p)
TEST (pstruct4)
TEST (pstruct8)
TEST (pdata4)
TEST (pdata8)
#endif

#else

int i;  /* prevent compiling an empty file */

#endif  /* INT_MAX */
