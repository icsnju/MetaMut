#include <stdarg.h>

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

#ifdef SKIP_VA
const int test_va = 0;
#else
const int test_va = 1;
#endif

/* Type definitions that are used by multiple tests.  */

typedef struct { float f; double d; } Sfd;
typedef struct { float f; long double l; } Sfl;
typedef struct { double d; float f; } Sdf;
typedef struct { double d; long double l; } Sdl;
typedef struct { long double l; float f; } Slf;
typedef struct { long double l; double d; } Sld;

typedef struct { float f; double d; long double l; } Sfdl;
typedef struct { float f; long double l; double d; } Sfld;
typedef struct { double d; float f; long double l; } Sdfl;
typedef struct { double d; long double l; float f; } Sdlf;
typedef struct { long double l; float f; double d; } Slfd;
typedef struct { long double l; double d; float f; } Sldf;
/* Function definitions that are used by multiple tests.  */

void initSfd (Sfd *p, double y)
{ p->f = y; p->d = y+1; }
void initSfl (Sfl *p, double y)
{ p->f = y; p->l = y+1; }
void initSdf (Sdf *p, double y)
{ p->d = y; p->f = y+1; }
void initSdl (Sdl *p, double y)
{ p->d = y; p->l = y+1; }
void initSlf (Slf *p, double y)
{ p->l = y; p->f = y+1; }
void initSld (Sld *p, double y)
{ p->l = y; p->d = y+1; }

void initSfdl (Sfdl *p, double y)
{ p->f = y; p->d = y+1; p->l = y+2; }
void initSfld (Sfld *p, double y)
{ p->f = y; p->l = y+1; p->d = y+2; }
void initSdfl (Sdfl *p, double y)
{ p->d = y; p->f = y+1; p->l = y+2; }
void initSdlf (Sdlf *p, double y)
{ p->d = y; p->l = y+1; p->f = y+2; }
void initSlfd (Slfd *p, double y)
{ p->l = y; p->f = y+1; p->d = y+2; }
void initSldf (Sldf *p, double y)
{ p->l = y; p->d = y+1; p->f = y+2; }

#define TEST(TYPE)						\
extern TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
extern TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
extern TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;	\
extern TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;	\
								\
extern void check##TYPE (TYPE x, double y);			\
								\
void								\
checkg##TYPE (void)						\
{								\
  check##TYPE (  g1s##TYPE,  (double)1);			\
  check##TYPE (  g2s##TYPE,  (double)2);			\
  check##TYPE (  g3s##TYPE,  (double)3);			\
  check##TYPE (  g4s##TYPE,  (double)4);			\
  check##TYPE (  g5s##TYPE,  (double)5);			\
  check##TYPE (  g6s##TYPE,  (double)6);			\
  check##TYPE (  g7s##TYPE,  (double)7);			\
  check##TYPE (  g8s##TYPE,  (double)8);			\
  check##TYPE (  g9s##TYPE,  (double)9);			\
  check##TYPE ( g10s##TYPE, (double)10);			\
  check##TYPE ( g11s##TYPE, (double)11);			\
  check##TYPE ( g12s##TYPE, (double)12);			\
  check##TYPE ( g13s##TYPE, (double)13);			\
  check##TYPE ( g14s##TYPE, (double)14);			\
  check##TYPE ( g15s##TYPE, (double)15);			\
  check##TYPE ( g16s##TYPE, (double)16);			\
}								\
								\
void								\
test##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,			\
	    TYPE s5, TYPE s6, TYPE s7, TYPE s8,			\
	    TYPE s9, TYPE s10, TYPE s11, TYPE s12,		\
	    TYPE s13, TYPE s14, TYPE s15, TYPE s16)		\
{								\
  check##TYPE (s1, (double)1);					\
  check##TYPE (s2, (double)2);					\
  check##TYPE (s3, (double)3);					\
  check##TYPE (s4, (double)4);					\
  check##TYPE (s5, (double)5);					\
  check##TYPE (s6, (double)6);					\
  check##TYPE (s7, (double)7);					\
  check##TYPE (s8, (double)8);					\
  check##TYPE (s9, (double)9);					\
  check##TYPE (s10, (double)10);				\
  check##TYPE (s11, (double)11);				\
  check##TYPE (s12, (double)12);				\
  check##TYPE (s13, (double)13);				\
  check##TYPE (s14, (double)14);				\
  check##TYPE (s15, (double)15);				\
  check##TYPE (s16, (double)16);				\
}								\
								\
void								\
testva##TYPE (int n, ...)					\
{								\
  int i;							\
  va_list ap;							\
  if (test_va)							\
    {								\
      va_start (ap, n);						\
      for (i = 0; i < n; i++)					\
	{							\
	  TYPE t = va_arg (ap, TYPE);				\
	  check##TYPE (t, (double)i+1);				\
	}							\
      va_end (ap);						\
    }								\
}

TEST(Sfd)
TEST(Sfl)
TEST(Sdf)
TEST(Sdl)
TEST(Slf)
TEST(Sld)
TEST(Sfdl)
TEST(Sfld)
TEST(Sdfl)
TEST(Sdlf)
TEST(Slfd)
TEST(Sldf)
