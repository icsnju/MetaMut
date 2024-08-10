/* { dg-do run } */
/* { dg-options "-DEXPENSIVE" { target run_expensive_tests } } */
/* { dg-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

typedef unsigned char V __attribute__((vector_size(16)));
typedef V VI;

#define N 16
#define TESTS \
T (0,	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) \
T (1,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) \
T (2,	0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7) \
T (3,	0, 18, 7, 16, 23, 5, 4, 22, 1, 6, 21, 20, 19, 3, 17, 2) \
T (4,	0, 1, 2, 3, 4, 5, 6, 7, 7, 1, 2, 0, 4, 5, 6, 3) \
T (5,	0, 19, 16, 3, 2, 17, 18, 1, 24, 8, 11, 10, 27, 26, 25, 9) \
T (6,	4, 23, 20, 7, 6, 21, 22, 5, 28, 12, 15, 14, 31, 30, 29, 13) \
T (7,	7, 1, 2, 0, 4, 5, 6, 3, 7, 1, 2, 0, 4, 5, 6, 3) \
T (8,	7, 3, 0, 5, 4, 1, 6, 2, 31, 27, 24, 29, 28, 25, 30, 26) \
T (9,	7, 1, 2, 0, 4, 5, 6, 3, 15, 9, 14, 10, 8, 12, 13, 11) \
T (10,	15, 0, 3, 12, 14, 2, 1, 13, 4, 11, 9, 6, 5, 10, 7, 8) \
T (11,	15, 9, 14, 10, 8, 12, 13, 11, 7, 1, 2, 0, 4, 5, 6, 3) \
T (12,	2, 5, 24, 23, 17, 22, 20, 21, 12, 14, 13, 8, 6, 20, 10, 18) \
T (13,	23, 11, 15, 9, 0, 14, 8, 12, 10, 13, 19, 11, 2, 26, 24, 30) \
T (14,	25, 5, 17, 1, 9, 15, 21, 7, 28, 2, 18, 13, 30, 14, 10, 4) \
T (15,	1, 30, 27, 31, 9, 18, 25, 12, 7, 4, 2, 16, 25, 20, 10, 3) \
T (16,	0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30) \
T (17,	1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31) \
T (18,	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3) \
T (19,	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0) \
T (20,	0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23) \
T (21,	8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31) \
T (22,	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0) \
T (23,	14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 15) \
T (24,	22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4, 5) \
T (25,	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21)
#define EXPTESTS \
T (116,	28, 13, 27, 11, 21, 1, 5, 22, 29, 14, 15, 6, 3, 10, 16, 30) \
T (117,	22, 26, 1, 13, 29, 3, 18, 18, 11, 21, 12, 28, 19, 5, 7, 4) \
T (118,	1, 28, 26, 11, 4, 27, 30, 12, 10, 19, 6, 7, 13, 31, 0, 24) \
T (119,	7, 30, 17, 27, 0, 10, 13, 21, 3, 29, 24, 21, 12, 6, 15, 18) \
T (120,	28, 13, 26, 24, 21, 16, 29, 9, 10, 2, 27, 19, 20, 17, 18, 14) \
T (121,	22, 21, 1, 8, 14, 15, 0, 28, 25, 30, 10, 17, 19, 27, 26, 13) \
T (122,	28, 13, 27, 2, 21, 29, 22, 4, 8, 0, 18, 1, 31, 7, 19, 14) \
T (123,	15, 0, 9, 2, 31, 3, 17, 24, 1, 8, 26, 20, 30, 11, 22, 5) \
T (124,	28, 14, 5, 7, 9, 10, 21, 31, 2, 11, 22, 27, 7, 27, 30, 2) \
T (125,	17, 13, 1, 7, 20, 22, 8, 15, 4, 25, 10, 28, 23, 24, 9, 21) \
T (126,	17, 5, 3, 4, 0, 7, 19, 13, 29, 25, 26, 1, 15, 8, 2, 7) \
T (127,	16, 6, 3, 22, 31, 15, 13, 21, 14, 20, 29, 17, 18, 2, 23, 25) \
T (128,	18, 22, 21, 28, 10, 13, 6, 17, 29, 20, 7, 16, 2, 9, 1, 11) \
T (129,	30, 11, 11, 22, 21, 10, 3, 18, 15, 26, 20, 16, 17, 13, 29, 7) \
T (130,	5, 9, 21, 16, 29, 11, 4, 2, 17, 28, 3, 19, 9, 10, 13, 20) \
T (131,	13, 3, 19, 25, 1, 28, 31, 30, 10, 12, 21, 26, 27, 14, 5, 17) \
T (132,	8, 28, 26, 31, 5, 14, 4, 13, 6, 15, 3, 17, 6, 20, 27, 29) \
T (133,	7, 26, 8, 17, 31, 11, 25, 16, 19, 4, 0, 27, 7, 20, 19, 13) \
T (134,	14, 23, 19, 4, 9, 11, 16, 6, 7, 3, 27, 20, 24, 18, 26, 31) \
T (135,	7, 25, 14, 2, 16, 5, 29, 3, 4, 24, 21, 9, 20, 19, 27, 1) \
T (136,	5, 14, 29, 12, 3, 3, 1, 4, 8, 7, 9, 13, 10, 4, 12, 23) \
T (137,	2, 6, 21, 28, 5, 23, 16, 24, 22, 18, 30, 12, 31, 16, 4, 1) \
T (138,	4, 31, 11, 26, 16, 23, 5, 12, 15, 14, 19, 3, 18, 21, 27, 6) \
T (139,	29, 14, 19, 18, 23, 27, 16, 7, 8, 26, 0, 12, 15, 17, 9, 1) \
T (140,	18, 27, 20, 31, 8, 10, 9, 17, 13, 19, 12, 15, 28, 3, 25, 5) \
T (141,	28, 13, 23, 20, 10, 5, 22, 17, 27, 0, 21, 27, 11, 25, 24, 30) \
T (142,	24, 17, 31, 22, 9, 15, 28, 6, 12, 23, 30, 1, 21, 5, 27, 29) \
T (143,	12, 19, 16, 0, 8, 15, 13, 22, 17, 4, 31, 20, 2, 9, 21, 30) \
T (144,	5, 13, 26, 12, 27, 0, 17, 1, 25, 8, 10, 25, 18, 11, 29, 4) \
T (145,	6, 18, 13, 29, 14, 31, 16, 10, 2, 12, 20, 25, 23, 28, 24, 10) \
T (146,	17, 0, 21, 0, 12, 13, 16, 5, 19, 29, 30, 27, 4, 9, 1, 20) \
T (147,	14, 26, 28, 17, 31, 10, 6, 3, 29, 22, 18, 1, 6, 5, 30, 8) \
T (148,	10, 17, 12, 18, 26, 16, 5, 24, 2, 7, 20, 15, 30, 2, 29, 25) \
T (149,	30, 27, 15, 23, 8, 12, 29, 2, 4, 9, 25, 13, 21, 7, 16, 20) \
T (150,	7, 13, 4, 22, 24, 0, 31, 28, 18, 5, 2, 16, 17, 1, 1, 6) \
T (151,	24, 27, 17, 25, 27, 6, 14, 30, 21, 19, 23, 29, 12, 19, 15, 9) \
T (152,	1, 16, 24, 19, 29, 27, 17, 9, 8, 23, 22, 12, 11, 2, 26, 25) \
T (153,	2, 25, 22, 8, 11, 16, 20, 26, 29, 15, 6, 15, 30, 10, 21, 24) \
T (154,	19, 20, 23, 11, 20, 1, 18, 29, 21, 24, 6, 8, 19, 2, 4, 9) \
T (155,	30, 12, 29, 21, 7, 8, 31, 15, 3, 1, 10, 4, 2, 9, 19, 2) \
T (156,	14, 27, 5, 23, 15, 22, 4, 26, 30, 19, 29, 18, 16, 24, 21, 7) \
T (157,	14, 15, 26, 17, 30, 16, 6, 0, 10, 8, 2, 23, 20, 13, 3, 27) \
T (158,	2, 14, 13, 22, 24, 26, 17, 16, 4, 27, 7, 31, 9, 1, 28, 3) \
T (159,	13, 15, 19, 12, 31, 23, 16, 8, 0, 2, 17, 27, 30, 14, 18, 1) \
T (160,	3, 9, 30, 28, 31, 24, 5, 1, 23, 0, 13, 24, 20, 18, 3, 26) \
T (161,	6, 10, 28, 8, 15, 14, 17, 26, 9, 0, 11, 2, 16, 2, 27, 24) \
T (162,	20, 29, 7, 11, 5, 21, 0, 8, 12, 28, 13, 4, 26, 10, 28, 23) \
T (163,	31, 25, 24, 10, 18, 3, 5, 12, 18, 2, 17, 22, 30, 16, 8, 6) \
T (164,	0, 5, 22, 3, 21, 28, 31, 20, 12, 14, 18, 4, 13, 2, 0, 29) \
T (165,	26, 18, 25, 20, 2, 0, 10, 30, 27, 11, 9, 3, 31, 4, 29, 21) \
T (166,	15, 21, 3, 9, 22, 6, 29, 13, 24, 30, 31, 5, 26, 17, 0, 12) \
T (167,	3, 28, 19, 27, 26, 14, 5, 10, 16, 24, 12, 30, 23, 13, 18, 26) \
T (168,	0, 13, 10, 16, 23, 17, 8, 24, 20, 25, 21, 3, 12, 22, 6, 1) \
T (169,	11, 18, 7, 26, 23, 6, 24, 21, 29, 18, 7, 19, 12, 31, 15, 28) \
T (170,	14, 5, 21, 6, 9, 11, 12, 4, 0, 8, 30, 10, 24, 19, 18, 3) \
T (171,	6, 3, 5, 13, 10, 2, 24, 31, 23, 2, 29, 4, 22, 7, 19, 20) \
T (172,	12, 26, 13, 18, 19, 9, 1, 27, 14, 7, 20, 31, 11, 8, 30, 4) \
T (173,	1, 23, 10, 31, 12, 26, 6, 13, 7, 30, 18, 4, 0, 27, 21, 19) \
T (174,	20, 15, 13, 22, 10, 14, 5, 3, 31, 8, 0, 11, 4, 9, 1, 0) \
T (175,	12, 29, 23, 28, 8, 20, 19, 7, 20, 24, 6, 26, 11, 17, 2, 5) \
T (176,	19, 0, 31, 26, 30, 16, 17, 14, 5, 7, 3, 27, 1, 22, 29, 9) \
T (177,	17, 15, 4, 1, 7, 13, 23, 28, 2, 8, 26, 12, 21, 24, 3, 19) \
T (178,	17, 23, 21, 2, 12, 27, 8, 15, 6, 25, 7, 28, 4, 26, 1, 23) \
T (179,	17, 25, 19, 4, 7, 20, 13, 30, 18, 3, 25, 21, 24, 26, 16, 31) \
T (180,	7, 20, 18, 9, 3, 16, 12, 22, 0, 30, 10, 25, 17, 29, 19, 5) \
T (181,	4, 17, 23, 1, 16, 22, 12, 31, 25, 9, 8, 21, 26, 15, 29, 29) \
T (182,	31, 25, 14, 17, 8, 24, 4, 29, 27, 0, 20, 5, 13, 12, 5, 30) \
T (183,	29, 26, 3, 22, 24, 30, 0, 15, 19, 7, 18, 6, 25, 14, 3, 3) \
T (184,	23, 22, 7, 18, 10, 30, 29, 6, 16, 13, 11, 28, 5, 26, 4, 24) \
T (185,	27, 15, 8, 29, 21, 23, 12, 11, 26, 22, 21, 6, 10, 23, 20, 27) \
T (186,	19, 23, 20, 7, 8, 14, 6, 17, 20, 1, 5, 12, 25, 18, 10, 29) \
T (187,	12, 14, 2, 5, 31, 21, 30, 18, 2, 6, 17, 22, 22, 28, 0, 18) \
T (188,	10, 8, 13, 29, 20, 18, 8, 5, 4, 3, 26, 19, 24, 29, 6, 30) \
T (189,	23, 19, 8, 2, 21, 18, 26, 1, 27, 12, 10, 4, 3, 20, 10, 17) \
T (190,	31, 30, 21, 8, 15, 9, 3, 22, 7, 17, 25, 5, 13, 6, 1, 11) \
T (191,	0, 5, 26, 6, 18, 1, 9, 31, 11, 3, 10, 23, 30, 27, 17, 14) \
T (192,	19, 7, 29, 27, 8, 17, 22, 20, 10, 22, 2, 11, 9, 14, 31, 23) \
T (193,	3, 23, 10, 18, 14, 9, 4, 24, 8, 30, 9, 13, 16, 17, 4, 22) \
T (194,	13, 22, 8, 3, 9, 24, 9, 7, 15, 17, 26, 19, 11, 5, 18, 24) \
T (195,	1, 18, 17, 13, 14, 0, 6, 15, 31, 22, 9, 2, 23, 19, 20, 3) \
T (196,	9, 8, 17, 4, 28, 20, 6, 1, 11, 12, 23, 0, 15, 27, 31, 18) \
T (197,	11, 2, 16, 1, 29, 21, 14, 9, 23, 30, 22, 8, 27, 9, 18, 10) \
T (198,	10, 6, 26, 22, 13, 28, 18, 31, 4, 17, 27, 5, 7, 25, 20, 21) \
T (199,	9, 8, 0, 0, 10, 7, 4, 24, 16, 3, 20, 1, 31, 11, 17, 26) \
T (200,	19, 24, 31, 25, 7, 4, 14, 4, 9, 21, 29, 30, 10, 16, 6, 13) \
T (201,	27, 4, 25, 12, 11, 3, 29, 1, 16, 6, 12, 5, 26, 25, 10, 15) \
T (202,	11, 28, 9, 3, 20, 30, 18, 6, 13, 12, 25, 24, 5, 26, 0, 31) \
T (203,	7, 23, 14, 16, 1, 31, 3, 0, 18, 6, 4, 10, 11, 15, 13, 29) \
T (204,	28, 15, 6, 4, 21, 18, 22, 29, 13, 3, 18, 27, 21, 7, 31, 26) \
T (205,	2, 13, 7, 22, 21, 9, 24, 15, 6, 20, 14, 29, 16, 27, 31, 0) \
T (206,	23, 3, 13, 0, 18, 19, 10, 29, 16, 22, 21, 30, 17, 5, 6, 31) \
T (207,	0, 3, 6, 29, 7, 10, 20, 17, 5, 8, 27, 18, 12, 4, 30, 28) \
T (208,	5, 31, 20, 6, 14, 11, 28, 19, 7, 25, 27, 26, 10, 8, 22, 4) \
T (209,	26, 22, 12, 9, 25, 0, 21, 14, 4, 7, 17, 30, 29, 24, 13, 31) \
T (210,	15, 1, 6, 10, 26, 12, 8, 25, 3, 31, 23, 5, 30, 2, 0, 27) \
T (211,	2, 11, 1, 27, 4, 17, 28, 22, 5, 28, 16, 20, 6, 3, 0, 15) \
T (212,	25, 2, 6, 18, 16, 12, 4, 27, 1, 29, 7, 3, 21, 20, 17, 19) \
T (213,	16, 0, 19, 21, 29, 23, 18, 31, 24, 12, 4, 20, 2, 8, 27, 30) \
T (214,	20, 28, 9, 28, 22, 13, 2, 0, 8, 26, 10, 23, 24, 12, 31, 11) \
T (215,	27, 22, 12, 15, 10, 20, 4, 25, 29, 9, 17, 3, 28, 21, 31, 23) \
T (216,	16, 29, 20, 19, 16, 6, 18, 28, 3, 15, 29, 30, 1, 22, 10, 0) \
T (217,	24, 14, 27, 3, 16, 30, 6, 0, 13, 12, 19, 7, 11, 20, 5, 26) \
T (218,	19, 30, 26, 17, 13, 5, 10, 15, 16, 28, 24, 20, 0, 25, 9, 14) \
T (219,	11, 0, 29, 5, 16, 30, 22, 19, 25, 4, 17, 7, 18, 2, 14, 13) \
T (220,	27, 9, 16, 10, 19, 23, 12, 21, 3, 30, 18, 4, 15, 6, 29, 13) \
T (221,	26, 12, 25, 22, 11, 18, 1, 7, 0, 19, 29, 31, 8, 4, 9, 15) \
T (222,	18, 23, 31, 7, 26, 5, 15, 11, 25, 4, 22, 21, 14, 10, 20, 8) \
T (223,	2, 18, 4, 24, 6, 20, 9, 25, 16, 3, 5, 22, 10, 7, 23, 30) \
T (224,	4, 13, 6, 8, 22, 2, 12, 24, 31, 29, 20, 23, 0, 10, 1, 3) \
T (225,	26, 3, 30, 18, 4, 19, 22, 31, 28, 24, 15, 14, 1, 23, 27, 7) \
T (226,	18, 6, 23, 16, 25, 26, 17, 5, 28, 10, 30, 24, 12, 14, 15, 20) \
T (227,	5, 19, 23, 22, 1, 7, 26, 12, 30, 17, 0, 0, 10, 6, 18, 4)
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
