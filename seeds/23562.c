/* { dg-do run } */
/* { dg-options "-DEXPENSIVE" { target run_expensive_tests } } */
/* { dg-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __SIZEOF_FLOAT__ == 4
typedef float V __attribute__((vector_size(32)));
# if __SIZEOF_INT__ == 4
typedef unsigned int VI __attribute__((vector_size(32)));
# elif __SIZEOF_LONG__ == 4
typedef unsigned long VI __attribute__((vector_size(32)));
# else
#  define UNSUPPORTED
# endif
#else
# define UNSUPPORTED
#endif

#define N 8
#define TESTS \
T (0,	0, 1, 2, 3, 4, 5, 6, 7) \
T (1,	0, 0, 0, 0, 0, 0, 0, 0) \
T (2,	0, 1, 2, 3, 0, 1, 2, 3) \
T (3,	0, 10, 3, 8, 1, 11, 10, 2) \
T (4,	0, 1, 2, 3, 3, 0, 2, 1) \
T (5,	12, 5, 14, 9, 8, 15, 1, 7) \
T (6,	9, 10, 11, 8, 4, 15, 14, 12) \
T (7,	4, 10, 14, 9, 11, 1, 12, 11) \
T (8,	15, 3, 3, 8, 5, 11, 2, 10) \
T (9,	13, 11, 10, 15, 8, 5, 14, 8) \
T (10,	9, 13, 12, 14, 10, 1, 5, 14) \
T (11,	5, 11, 12, 6, 3, 2, 4, 15) \
T (12,	5, 13, 14, 8, 4, 10, 4, 12) \
T (13,	14, 8, 12, 3, 13, 9, 5, 4) \
T (14,	15, 3, 13, 6, 14, 12, 10, 0) \
T (15,	0, 5, 11, 7, 4, 6, 14, 1) \
T (16,	0, 2, 4, 6, 8, 10, 12, 14) \
T (17,	1, 3, 5, 7, 9, 11, 13, 15) \
T (18,	3, 3, 3, 3, 3, 3, 3, 3) \
T (19,	7, 6, 5, 4, 3, 2, 1, 0) \
T (20,	0, 8, 1, 9, 2, 10, 3, 11) \
T (21,	4, 12, 5, 13, 6, 14, 7, 15) \
T (22,	1, 2, 3, 4, 5, 6, 7, 0) \
T (23,	6, 5, 4, 3, 2, 1, 0, 7) \
T (24,	0, 1, 2, 3, 8, 9, 10, 11) \
T (25,	0, 1, 2, 3, 12, 13, 14, 15) \
T (26,	0, 1, 8, 9, 10, 11, 12, 13) \
T (27,	0, 8, 9, 10, 11, 12, 13, 14) \
T (28,	1, 9, 3, 11, 5, 13, 7, 15) \
T (29,	0, 9, 2, 11, 4, 13, 6, 15)
#define EXPTESTS \
T (116,	9, 3, 9, 4, 7, 0, 0, 6) \
T (117,	4, 14, 12, 8, 9, 6, 0, 10) \
T (118,	10, 12, 1, 3, 4, 11, 9, 2) \
T (119,	4, 11, 9, 5, 8, 14, 0, 2) \
T (120,	0, 10, 8, 6, 4, 9, 7, 5) \
T (121,	10, 15, 0, 4, 12, 9, 7, 3) \
T (122,	13, 6, 0, 7, 5, 12, 12, 2) \
T (123,	4, 10, 11, 15, 12, 7, 3, 8) \
T (124,	1, 13, 15, 9, 6, 5, 7, 4) \
T (125,	7, 13, 2, 5, 2, 8, 1, 6) \
T (126,	4, 5, 2, 12, 14, 13, 8, 6) \
T (127,	4, 10, 2, 7, 11, 15, 9, 0) \
T (128,	14, 8, 12, 10, 13, 3, 11, 2) \
T (129,	10, 8, 14, 9, 5, 1, 15, 7) \
T (130,	12, 2, 9, 13, 5, 14, 1, 15) \
T (131,	10, 4, 1, 14, 11, 15, 9, 2) \
T (132,	9, 12, 7, 4, 2, 1, 0, 9) \
T (133,	11, 15, 4, 10, 3, 12, 13, 5) \
T (134,	1, 5, 6, 13, 14, 15, 3, 10) \
T (135,	0, 14, 6, 10, 1, 13, 3, 2) \
T (136,	2, 11, 1, 5, 12, 13, 3, 5) \
T (137,	7, 11, 4, 1, 12, 10, 14, 10) \
T (138,	3, 6, 7, 13, 11, 15, 10, 0) \
T (139,	10, 8, 7, 14, 1, 11, 0, 13) \
T (140,	5, 3, 13, 11, 8, 5, 12, 6) \
T (141,	15, 2, 12, 13, 10, 8, 5, 0) \
T (142,	13, 9, 10, 2, 11, 3, 4, 1) \
T (143,	2, 14, 6, 5, 1, 8, 15, 0) \
T (144,	12, 9, 14, 10, 1, 3, 11, 13) \
T (145,	12, 13, 11, 2, 5, 6, 4, 8) \
T (146,	5, 0, 2, 1, 6, 8, 15, 13) \
T (147,	8, 7, 1, 3, 5, 11, 14, 15) \
T (148,	0, 9, 2, 15, 3, 1, 8, 10) \
T (149,	4, 14, 11, 6, 8, 5, 12, 7) \
T (150,	5, 9, 10, 12, 14, 15, 2, 7) \
T (151,	11, 6, 5, 7, 11, 14, 2, 1) \
T (152,	13, 1, 7, 4, 6, 8, 15, 9) \
T (153,	10, 12, 9, 1, 6, 7, 8, 15) \
T (154,	8, 4, 5, 1, 3, 0, 7, 13) \
T (155,	13, 9, 3, 4, 10, 1, 15, 7) \
T (156,	13, 8, 2, 7, 0, 6, 3, 6) \
T (157,	15, 15, 13, 6, 0, 5, 14, 4) \
T (158,	13, 1, 2, 2, 7, 9, 2, 6) \
T (159,	5, 12, 10, 13, 6, 1, 4, 7) \
T (160,	0, 2, 9, 1, 5, 11, 14, 11) \
T (161,	14, 6, 5, 10, 3, 2, 15, 4) \
T (162,	3, 10, 0, 1, 13, 14, 11, 15) \
T (163,	13, 7, 5, 9, 5, 0, 11, 4) \
T (164,	2, 11, 1, 12, 3, 13, 4, 9) \
T (165,	1, 0, 10, 11, 5, 13, 4, 3) \
T (166,	3, 9, 1, 12, 15, 14, 10, 5) \
T (167,	3, 10, 11, 14, 5, 1, 8, 12) \
T (168,	10, 15, 5, 14, 4, 13, 6, 3) \
T (169,	1, 8, 6, 4, 11, 13, 7, 10) \
T (170,	8, 7, 1, 15, 11, 9, 0, 3) \
T (171,	4, 0, 11, 7, 1, 15, 3, 13) \
T (172,	14, 7, 3, 4, 9, 11, 0, 6) \
T (173,	7, 3, 11, 4, 8, 2, 10, 15) \
T (174,	7, 9, 14, 2, 0, 5, 13, 3) \
T (175,	4, 8, 5, 9, 3, 11, 1, 14) \
T (176,	13, 12, 3, 3, 1, 4, 8, 5) \
T (177,	7, 12, 9, 13, 10, 4, 5, 8) \
T (178,	14, 3, 12, 7, 2, 6, 5, 5) \
T (179,	7, 0, 8, 6, 2, 14, 12, 15) \
T (180,	2, 12, 0, 4, 1, 15, 11, 10) \
T (181,	0, 7, 5, 12, 15, 10, 14, 3) \
T (182,	3, 10, 2, 5, 11, 6, 13, 14) \
T (183,	3, 2, 7, 11, 0, 13, 8, 10) \
T (184,	0, 12, 15, 1, 9, 2, 11, 4) \
T (185,	11, 15, 8, 10, 9, 1, 13, 3) \
T (186,	12, 13, 15, 12, 3, 9, 5, 7) \
T (187,	4, 10, 5, 6, 1, 11, 0, 11) \
T (188,	11, 6, 7, 9, 0, 8, 14, 8) \
T (189,	1, 15, 6, 9, 12, 6, 7, 14) \
T (190,	1, 5, 6, 11, 12, 13, 3, 0) \
T (191,	0, 8, 15, 13, 12, 6, 1, 4) \
T (192,	12, 15, 8, 4, 2, 0, 9, 5) \
T (193,	14, 5, 13, 10, 12, 11, 0, 1) \
T (194,	12, 1, 9, 8, 10, 9, 0, 2) \
T (195,	11, 0, 13, 4, 6, 2, 14, 15) \
T (196,	0, 10, 6, 2, 12, 4, 9, 13) \
T (197,	7, 12, 8, 10, 1, 0, 5, 0) \
T (198,	12, 13, 0, 5, 3, 14, 11, 4) \
T (199,	9, 1, 4, 14, 10, 12, 15, 6) \
T (200,	3, 12, 13, 6, 14, 2, 1, 6) \
T (201,	5, 14, 8, 10, 1, 12, 2, 0) \
T (202,	5, 8, 2, 7, 4, 15, 14, 2) \
T (203,	14, 13, 10, 9, 11, 15, 7, 8) \
T (204,	12, 13, 14, 2, 4, 9, 5, 7) \
T (205,	0, 7, 5, 4, 7, 13, 6, 8) \
T (206,	7, 0, 15, 6, 12, 2, 5, 4) \
T (207,	8, 6, 0, 1, 1, 11, 1, 9) \
T (208,	11, 6, 14, 9, 5, 3, 7, 13) \
T (209,	12, 3, 15, 9, 1, 0, 8, 13) \
T (210,	11, 1, 9, 8, 7, 6, 12, 2) \
T (211,	10, 9, 2, 6, 8, 11, 0, 4) \
T (212,	10, 13, 15, 9, 6, 15, 14, 10) \
T (213,	9, 5, 8, 3, 4, 7, 11, 4) \
T (214,	1, 2, 13, 5, 8, 4, 3, 6) \
T (215,	8, 3, 2, 4, 9, 14, 12, 13) \
T (216,	5, 7, 8, 15, 3, 1, 10, 4) \
T (217,	2, 9, 3, 2, 14, 11, 5, 7) \
T (218,	15, 6, 4, 10, 14, 3, 13, 2) \
T (219,	0, 8, 14, 5, 15, 7, 10, 1) \
T (220,	14, 0, 6, 10, 8, 2, 7, 4) \
T (221,	15, 13, 3, 14, 11, 2, 14, 6) \
T (222,	8, 2, 10, 13, 1, 0, 4, 11) \
T (223,	7, 15, 2, 9, 1, 12, 11, 3) \
T (224,	13, 15, 3, 12, 15, 7, 0, 8) \
T (225,	0, 2, 1, 11, 14, 3, 9, 14) \
T (226,	12, 14, 3, 15, 8, 5, 1, 7) \
T (227,	0, 5, 13, 8, 4, 2, 1, 3)
/* Driver fragment for __builtin_shuffle of any vector shape.  */

extern void abort (void);

#ifndef UNSUPPORTED
V a, b, c, d;

#define T(num, msk...) \
__attribute__((noinline, noclone)) void		\
test_##num (void)				\
{						\
  VI mask = { msk };				\
  int i;					\
  c = __builtin_shuffle (a, mask);		\
  d = __builtin_shuffle (a, b, mask);		\
  __asm ("" : : "r" (&c), "r" (&d) : "memory");	\
  for (i = 0; i < N; ++i)			\
    if (c[i] != a[mask[i] & (N - 1)])		\
      abort ();					\
    else if ((mask[i] & N))			\
      {						\
	if (d[i] != b[mask[i] & (N - 1)])	\
	  abort ();				\
      }						\
    else if (d[i] != a[mask[i] & (N - 1)])	\
      abort ();					\
}
TESTS
#ifdef EXPENSIVE
EXPTESTS
#endif
#endif

int
main ()
{
#ifndef UNSUPPORTED
  int i;
  for (i = 0; i < N; ++i)
    {
      a[i] = i + 2;
      b[i] = N + i + 2;
    }

#undef T
#define T(num, msk...) \
  test_##num ();
  TESTS
#ifdef EXPENSIVE
  EXPTESTS
#endif
#endif

  return 0;
}
