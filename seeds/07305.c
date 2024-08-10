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

#define T(TYPE)							\
extern TYPE g01##TYPE, g02##TYPE, g03##TYPE, g04##TYPE;		\
extern TYPE g05##TYPE, g06##TYPE, g07##TYPE, g08##TYPE;		\
extern TYPE g09##TYPE, g10##TYPE, g11##TYPE, g12##TYPE;		\
extern TYPE g13##TYPE, g14##TYPE, g15##TYPE, g16##TYPE;		\
								\
extern void check##TYPE (TYPE x, double y);			\
								\
void								\
checkg##TYPE (void)						\
{								\
  check##TYPE (g01##TYPE,  1.0);					\
  check##TYPE (g02##TYPE,  2.0);					\
  check##TYPE (g03##TYPE,  3.0);					\
  check##TYPE (g04##TYPE,  4.0);					\
  check##TYPE (g05##TYPE,  5.0);					\
  check##TYPE (g06##TYPE,  6.0);					\
  check##TYPE (g07##TYPE,  7.0);					\
  check##TYPE (g08##TYPE,  8.0);					\
  check##TYPE (g09##TYPE,  9.0);					\
  check##TYPE (g10##TYPE, 10.0);					\
  check##TYPE (g11##TYPE, 11.0);					\
  check##TYPE (g12##TYPE, 12.0);					\
  check##TYPE (g13##TYPE, 13.0);					\
  check##TYPE (g14##TYPE, 14.0);					\
  check##TYPE (g15##TYPE, 15.0);					\
  check##TYPE (g16##TYPE, 16.0);					\
}								\
								\
TYPE								\
test0##TYPE (void)						\
{								\
  return g01##TYPE;						\
}								\
								\
TYPE								\
test1##TYPE (TYPE x01)						\
{								\
  return x01;							\
}								\
								\
TYPE								\
testva##TYPE (int n, ...)					\
{								\
  int i;							\
  TYPE rslt;							\
  va_list ap;							\
  if (1)							\
    {								\
      va_start (ap, n);						\
      for (i = 0; i < n; i++)					\
	{							\
	  rslt = va_arg (ap, TYPE);				\
	}							\
      va_end (ap);						\
    }								\
  return rslt;							\
}

T(Sfd)
T(Sfl)
T(Sdf)
T(Sdl)
T(Slf)
T(Sld)
T(Sfdl)
T(Sfld)
T(Sdfl)
T(Sdlf)
T(Slfd)
T(Sldf)
