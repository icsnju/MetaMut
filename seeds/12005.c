/* { dg-do run } */
/* { dg-options "-DEXPENSIVE" { target run_expensive_tests } } */
/* { dg-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __SIZEOF_LONG_LONG__ == 8
typedef unsigned long long V __attribute__((vector_size(32)));
typedef V VI;
#else
#define UNSUPPORTED
#endif

#define N 4
#define TESTS \
T (0,	0, 1, 2, 3) \
T (1,	0, 0, 0, 0) \
T (2,	6, 3, 1, 5) \
T (3,	1, 3, 5, 0) \
T (4,	6, 3, 5, 4) \
T (5,	6, 4, 1, 0) \
T (6,	6, 1, 4, 2) \
T (7,	3, 7, 4, 4) \
T (8,	3, 2, 4, 4) \
T (9,	3, 2, 6, 1) \
T (10,	5, 4, 5, 6) \
T (11,	1, 4, 0, 7) \
T (12,	1, 5, 7, 2) \
T (13,	2, 3, 0, 4) \
T (14,	7, 6, 4, 2) \
T (15,	6, 1, 3, 4) \
T (16,	0, 2, 4, 6) \
T (17,	1, 3, 5, 7) \
T (18,	3, 3, 3, 3) \
T (19,	3, 2, 1, 0) \
T (20,	0, 4, 1, 5) \
T (21,	2, 6, 3, 7) \
T (22,	1, 2, 3, 0) \
T (23,	2, 1, 0, 3) \
T (24,	2, 5, 6, 3) \
T (25,	0, 1, 4, 5) \
T (26,	1, 5, 3, 7) \
T (27,	0, 5, 2, 7)
#define EXPTESTS \
T (116,	1, 2, 4, 3) \
T (117,	7, 3, 3, 0) \
T (118,	5, 3, 2, 7) \
T (119,	0, 3, 5, 6) \
T (120,	0, 0, 1, 5) \
T (121,	4, 6, 2, 1) \
T (123,	4, 6, 3, 2) \
T (124,	4, 7, 5, 6) \
T (125,	0, 4, 2, 4) \
T (126,	2, 4, 6, 0) \
T (127,	4, 3, 5, 1) \
T (128,	5, 4, 7, 3) \
T (129,	7, 5, 6, 4) \
T (130,	2, 0, 5, 3) \
T (131,	7, 4, 3, 0) \
T (132,	6, 1, 3, 1) \
T (133,	3, 4, 1, 7) \
T (134,	0, 0, 6, 4) \
T (135,	6, 4, 0, 1) \
T (136,	6, 0, 2, 4) \
T (137,	1, 3, 4, 2) \
T (138,	3, 1, 2, 4) \
T (139,	3, 1, 5, 0) \
T (140,	1, 6, 0, 2) \
T (141,	0, 2, 6, 4) \
T (142,	1, 7, 5, 2) \
T (143,	7, 0, 4, 1) \
T (144,	7, 3, 5, 6) \
T (145,	0, 7, 5, 4) \
T (146,	6, 4, 2, 3) \
T (147,	1, 5, 7, 6) \
T (148,	5, 7, 4, 7) \
T (149,	5, 2, 2, 0) \
T (150,	7, 1, 6, 4) \
T (151,	5, 2, 4, 6) \
T (152,	5, 0, 4, 6) \
T (153,	4, 2, 7, 3) \
T (154,	7, 1, 0, 6) \
T (155,	0, 4, 2, 5) \
T (156,	3, 4, 3, 2) \
T (157,	2, 0, 6, 1) \
T (158,	5, 1, 7, 4) \
T (159,	2, 1, 5, 6) \
T (160,	1, 6, 5, 7) \
T (161,	2, 4, 1, 6) \
T (162,	3, 7, 1, 6) \
T (163,	2, 1, 4, 7) \
T (164,	4, 2, 1, 0) \
T (165,	0, 7, 1, 3) \
T (166,	7, 4, 2, 3) \
T (167,	4, 5, 3, 5) \
T (168,	1, 5, 6, 7) \
T (169,	6, 3, 2, 0) \
T (170,	6, 2, 1, 5) \
T (171,	5, 6, 1, 3) \
T (172,	2, 2, 3, 1) \
T (173,	5, 4, 3, 5) \
T (174,	7, 3, 4, 1) \
T (175,	4, 2, 3, 6) \
T (176,	7, 6, 5, 3) \
T (177,	7, 2, 0, 6) \
T (178,	1, 3, 0, 2) \
T (179,	5, 3, 0, 5) \
T (180,	4, 6, 7, 2) \
T (181,	4, 5, 2, 0) \
T (182,	5, 0, 1, 2) \
T (183,	2, 3, 4, 1) \
T (184,	2, 6, 5, 1) \
T (185,	0, 6, 7, 4) \
T (186,	4, 1, 6, 2) \
T (187,	1, 3, 2, 3) \
T (188,	2, 5, 4, 3) \
T (189,	2, 5, 6, 4) \
T (190,	4, 0, 5, 0) \
T (191,	2, 1, 6, 0) \
T (192,	7, 5, 0, 1) \
T (193,	3, 5, 6, 7) \
T (194,	0, 1, 2, 7) \
T (195,	3, 1, 0, 2) \
T (196,	2, 4, 6, 3) \
T (197,	6, 0, 5, 4) \
T (198,	6, 5, 7, 1) \
T (199,	2, 5, 4, 6) \
T (200,	7, 2, 3, 6) \
T (201,	3, 5, 7, 3) \
T (202,	1, 7, 4, 6) \
T (203,	4, 0, 7, 1) \
T (204,	7, 1, 0, 4) \
T (205,	5, 1, 3, 4) \
T (206,	0, 7, 3, 5) \
T (207,	3, 2, 1, 5) \
T (208,	7, 5, 0, 2) \
T (209,	7, 0, 6, 3) \
T (210,	6, 6, 7, 7) \
T (211,	5, 6, 0, 4) \
T (212,	5, 1, 2, 2) \
T (213,	7, 1, 2, 6) \
T (214,	5, 4, 2, 6) \
T (215,	1, 5, 6, 4) \
T (216,	7, 0, 2, 1) \
T (217,	1, 5, 3, 6) \
T (218,	3, 3, 6, 5) \
T (219,	2, 3, 5, 7) \
T (220,	2, 4, 3, 0) \
T (221,	1, 5, 6, 3) \
T (222,	7, 5, 1, 5) \
T (223,	0, 5, 3, 4) \
T (224,	2, 3, 1, 4) \
T (225,	2, 3, 5, 1) \
T (226,	4, 3, 1, 0) \
T (227,	2, 3, 5, 5)
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
