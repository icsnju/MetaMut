/* { dg-do compile } */
/* { dg-options "-std=gnu17 -pedantic" } */

/* N1150 6: Constants.
   C99 6.4.4.2: Floating constants.  */

#define EXTERN extern

int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#define FINISH if (failures != 0) __builtin_abort (); return 0;
#else
#define FAILURE __builtin_abort ();
#define FINISH return 0;
#endif

_Decimal32 a = 1.1df;		/* { dg-warning "C2X feature|ISO C" } */
_Decimal32 b = -.003DF;		/* { dg-warning "C2X feature|ISO C" } */
_Decimal64 c = 11e-1dl;		/* { dg-warning "C2X feature|ISO C" } */
_Decimal64 d = -.3DL;		/* { dg-warning "C2X feature|ISO C" } */
_Decimal128 e = 000.3e0dl;	/* { dg-warning "C2X feature|ISO C" } */
_Decimal128 f = 3000300030003e0DL;  /* { dg-warning "C2X feature|ISO C" } */
