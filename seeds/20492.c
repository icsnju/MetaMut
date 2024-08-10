/* { dg-do run } */
/* { dg-options "-DEXPENSIVE" { target run_expensive_tests } } */
/* { dg-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

typedef unsigned char V __attribute__((vector_size(32)));
typedef V VI;

#define N 32
#define TESTS \
T (0,	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31) \
T (1,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) \
T (2,	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) \
T (3,	26, 60, 14, 40, 44, 51, 12, 45, 9, 37, 4, 47, 35, 63, 54, 30, 28, 10, 48, 32, 5, 59, 7, 52, 11, 6, 62, 49, 25, 25, 20, 55) \
T (4,	32, 16, 54, 20, 62, 60, 56, 9, 38, 26, 42, 12, 17, 51, 4, 58, 59, 25, 50, 27, 41, 37, 8, 30, 18, 24, 40, 36, 25, 7, 37, 63) \
T (5,	0, 4, 60, 9, 23, 19, 45, 6, 5, 12, 38, 60, 34, 36, 26, 11, 47, 52, 63, 39, 7, 56, 17, 54, 53, 39, 18, 2, 37, 55, 22, 21) \
T (6,	2, 44, 46, 13, 40, 38, 52, 10, 50, 55, 11, 15, 9, 0, 12, 42, 45, 19, 11, 34, 26, 24, 62, 25, 5, 17, 1, 47, 21, 7, 60, 41) \
T (7,	50, 13, 37, 62, 9, 49, 14, 53, 15, 42, 36, 1, 33, 27, 63, 41, 57, 12, 7, 2, 44, 48, 16, 39, 25, 31, 20, 10, 3, 60, 43, 21) \
T (8,	1, 37, 22, 61, 5, 14, 46, 47, 6, 35, 10, 48, 30, 26, 33, 13, 18, 18, 11, 21, 44, 29, 60, 45, 19, 37, 7, 43, 36, 1, 28, 59) \
T (9,	30, 44, 42, 51, 6, 7, 38, 13, 43, 31, 58, 0, 56, 17, 27, 32, 10, 53, 3, 29, 21, 12, 15, 50, 28, 24, 16, 61, 9, 34, 59, 19) \
T (10,	20, 49, 50, 14, 54, 53, 33, 40, 46, 15, 0, 28, 25, 30, 10, 17, 51, 59, 26, 13, 45, 27, 2, 61, 4, 36, 8, 0, 18, 33, 22, 63) \
T (11,	13, 40, 7, 33, 51, 21, 59, 46, 47, 32, 9, 34, 63, 35, 41, 17, 24, 1, 8, 26, 52, 62, 11, 22, 37, 28, 5, 39, 10, 53, 60, 2) \
T (12,	39, 43, 54, 27, 53, 39, 27, 30, 2, 17, 13, 33, 7, 52, 40, 15, 36, 57, 10, 28, 22, 23, 25, 24, 41, 47, 8, 20, 5, 3, 4, 0) \
T (13,	7, 51, 13, 61, 25, 4, 19, 58, 35, 33, 29, 15, 40, 2, 39, 16, 38, 3, 54, 63, 15, 6, 48, 21, 14, 52, 17, 50, 34, 55, 57, 50) \
T (14,	22, 53, 28, 42, 45, 38, 49, 13, 54, 61, 21, 52, 7, 16, 34, 9, 1, 43, 62, 43, 35, 50, 47, 58, 20, 3, 30, 15, 37, 53, 43, 36) \
T (15,	2, 43, 49, 34, 28, 35, 29, 36, 51, 9, 17, 48, 10, 37, 45, 21, 52, 19, 25, 33, 60, 31, 30, 42, 12, 26, 27, 46, 5, 40, 14, 36) \
T (16,	0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62) \
T (17,	1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63) \
T (18,	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3) \
T (19,	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0) \
T (20,	0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47) \
T (21,	16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63) \
T (22,	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0) \
T (23,	30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 31) \
T (24,	0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 2, 5, 8, 11, 14, 17, 20, 23, 26, 29) \
T (25,	11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42) \
T (26,	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52) \
T (27,	22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53) \
T (28,	38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 0, 1, 2, 3, 4, 5, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 16, 17, 18, 19, 20, 21) \
T (29,	0, 43, 2, 3, 57, 5, 6, 7, 8, 53, 40, 11, 12, 13, 42, 15, 16, 40, 18, 19, 20, 21, 22, 23, 24, 25, 36, 58, 36, 29, 30, 31)
#define EXPTESTS \
T (116,	13, 38, 47, 3, 17, 8, 38, 20, 59, 61, 39, 26, 7, 49, 63, 43, 57, 16, 40, 19, 4, 32, 27, 7, 52, 19, 46, 55, 36, 41, 48, 6) \
T (117,	39, 35, 59, 20, 56, 18, 58, 63, 57, 14, 2, 16, 5, 61, 35, 4, 53, 9, 52, 51, 27, 33, 61, 12, 3, 35, 36, 40, 37, 7, 45, 42) \
T (118,	4, 5, 8, 14, 35, 12, 55, 34, 6, 53, 60, 5, 48, 56, 22, 18, 62, 44, 38, 31, 28, 16, 33, 36, 43, 26, 16, 23, 37, 47, 51, 3) \
T (119,	26, 50, 53, 27, 6, 29, 14, 19, 23, 48, 39, 8, 58, 0, 12, 15, 17, 41, 33, 18, 52, 31, 10, 9, 49, 13, 51, 45, 47, 60, 35, 57) \
T (120,	37, 60, 13, 55, 52, 42, 5, 22, 17, 59, 0, 21, 59, 11, 25, 56, 62, 56, 31, 54, 9, 47, 28, 6, 44, 33, 61, 12, 19, 16, 8, 40) \
T (121,	47, 45, 44, 40, 22, 17, 4, 31, 8, 52, 34, 32, 41, 21, 62, 37, 5, 13, 58, 27, 0, 17, 33, 57, 42, 57, 18, 43, 61, 36, 38, 14) \
T (122,	31, 16, 42, 6, 34, 2, 12, 20, 57, 29, 23, 60, 24, 10, 17, 0, 21, 32, 45, 5, 19, 30, 59, 4, 9, 1, 46, 14, 26, 63, 3, 22) \
T (123,	46, 61, 50, 1, 38, 37, 62, 40, 10, 49, 44, 26, 16, 24, 5, 2, 39, 20, 15, 30, 34, 29, 57, 27, 47, 55, 8, 44, 36, 4, 9, 25) \
T (124,	45, 53, 7, 48, 23, 21, 20, 7, 13, 4, 22, 24, 39, 0, 63, 60, 18, 37, 34, 36, 54, 49, 1, 1, 38, 27, 17, 57, 27, 14, 59, 62) \
T (125,	30, 57, 46, 21, 51, 23, 25, 62, 29, 12, 51, 15, 41, 33, 16, 56, 19, 61, 27, 49, 41, 40, 55, 54, 44, 43, 2, 26, 34, 22, 11, 20) \
T (126,	26, 29, 15, 57, 6, 15, 30, 10, 53, 24, 19, 52, 23, 43, 20, 1, 18, 29, 56, 51, 38, 8, 51, 2, 4, 41, 62, 12, 7, 63, 3, 33) \
T (127,	42, 10, 36, 2, 41, 34, 19, 2, 46, 27, 5, 55, 15, 22, 36, 26, 30, 58, 51, 61, 18, 48, 24, 21, 56, 7, 14, 47, 17, 6, 0, 8) \
T (128,	34, 23, 16, 20, 15, 13, 3, 30, 59, 46, 54, 24, 26, 45, 17, 48, 4, 27, 7, 31, 2, 41, 1, 28, 51, 12, 63, 8, 32, 19, 18, 33) \
T (129,	3, 41, 62, 28, 30, 63, 24, 5, 33, 60, 55, 32, 37, 45, 56, 52, 50, 35, 26, 38, 10, 40, 15, 14, 17, 58, 46, 41, 11, 34, 48, 2) \
T (130,	59, 60, 24, 20, 61, 39, 43, 37, 53, 32, 8, 44, 28, 13, 36, 58, 0, 42, 60, 55, 31, 57, 56, 10, 18, 3, 5, 12, 18, 34, 49, 54) \
T (131,	3, 62, 16, 40, 38, 32, 5, 54, 53, 28, 63, 20, 12, 14, 50, 4, 13, 34, 32, 29, 26, 18, 25, 52, 2, 42, 30, 59, 43, 41, 27, 10) \
T (132,	35, 31, 57, 52, 4, 0, 61, 21, 15, 3, 9, 54, 38, 13, 24, 30, 37, 58, 41, 6, 49, 32, 12, 28, 19, 59, 14, 42, 16, 48, 56, 23) \
T (133,	62, 12, 13, 50, 26, 32, 45, 10, 48, 55, 49, 8, 24, 42, 52, 57, 53, 3, 54, 38, 33, 43, 7, 26, 23, 6, 21, 61, 18, 7, 51, 44) \
T (134,	31, 15, 38, 39, 60, 14, 5, 21, 53, 6, 9, 43, 12, 36, 32, 40, 30, 10, 56, 51, 50, 35, 38, 3, 37, 13, 2, 24, 63, 34, 55, 34) \
T (135,	29, 36, 22, 7, 63, 23, 19, 20, 12, 58, 13, 50, 41, 1, 59, 33, 46, 39, 52, 11, 8, 51, 62, 42, 31, 10, 58, 38, 45, 4, 32, 53) \
T (136,	19, 20, 47, 13, 22, 10, 14, 37, 35, 31, 8, 15, 32, 54, 43, 40, 4, 9, 1, 32, 12, 29, 23, 60, 52, 51, 39, 52, 56, 6, 26, 11) \
T (137,	39, 11, 49, 34, 37, 51, 32, 63, 26, 62, 16, 49, 14, 5, 7, 3, 59, 33, 54, 58, 61, 9, 47, 36, 13, 23, 28, 2, 40, 4, 44, 1) \
T (138,	53, 24, 33, 35, 19, 49, 55, 34, 44, 27, 40, 47, 38, 17, 25, 39, 60, 4, 26, 15, 7, 36, 55, 51, 39, 20, 45, 62, 57, 50, 57, 21) \
T (139,	24, 58, 16, 63, 7, 20, 18, 9, 35, 48, 12, 54, 0, 62, 41, 42, 25, 50, 17, 61, 51, 37, 4, 55, 33, 22, 12, 40, 53, 26, 47, 29) \
T (140,	55, 53, 40, 61, 63, 57, 46, 17, 8, 56, 4, 29, 27, 32, 24, 52, 5, 13, 12, 5, 44, 30, 26, 3, 22, 0, 47, 19, 39, 50, 38, 15) \
T (141,	15, 46, 3, 18, 3, 55, 22, 39, 10, 62, 61, 38, 48, 16, 6, 45, 11, 60, 5, 26, 4, 50, 24, 59, 47, 8, 29, 53, 12, 21, 43, 58) \
T (142,	22, 8, 21, 6, 55, 10, 55, 20, 59, 19, 55, 23, 52, 39, 46, 49, 20, 1, 40, 5, 12, 14, 57, 50, 29, 2, 37, 31, 62, 18, 34, 54) \
T (143,	54, 28, 50, 0, 62, 18, 10, 8, 13, 29, 52, 8, 5, 36, 35, 4, 58, 19, 56, 29, 38, 30, 55, 51, 40, 34, 53, 26, 33, 59, 12, 27) \
T (144,	42, 4, 35, 20, 52, 3, 10, 21, 17, 63, 62, 40, 15, 41, 54, 39, 8, 57, 5, 45, 37, 38, 1, 43, 32, 58, 6, 50, 33, 3, 55, 9) \
T (145,	27, 62, 49, 14, 19, 39, 61, 40, 22, 20, 10, 22, 34, 43, 9, 46, 31, 42, 54, 23, 35, 50, 4, 3, 41, 24, 8, 30, 9, 45, 48, 17) \
T (146,	4, 40, 22, 45, 8, 35, 9, 24, 9, 41, 7, 47, 49, 58, 51, 43, 54, 37, 18, 39, 56, 1, 46, 0, 33, 38, 15, 13, 17, 31, 54, 2) \
T (147,	55, 19, 20, 3, 41, 8, 49, 36, 28, 38, 1, 11, 44, 55, 32, 15, 59, 0, 63, 50, 2, 16, 61, 21, 14, 23, 30, 22, 33, 46, 48, 62) \
T (148,	30, 40, 59, 11, 9, 8, 50, 42, 10, 6, 58, 22, 45, 60, 63, 4, 49, 31, 5, 7, 25, 18, 52, 13, 53, 0, 0, 39, 24, 48, 3, 20) \
T (149,	41, 1, 63, 11, 49, 24, 26, 51, 31, 57, 7, 36, 46, 4, 53, 19, 29, 21, 30, 14, 10, 48, 38, 13, 59, 25, 44, 43, 35, 61, 33, 16) \
T (150,	38, 44, 5, 26, 25, 42, 16, 15, 43, 28, 41, 3, 52, 62, 50, 11, 6, 35, 13, 12, 57, 24, 32, 31, 39, 55, 46, 33, 0, 18, 4, 47) \
T (151,	45, 29, 60, 15, 6, 36, 53, 18, 54, 47, 61, 13, 3, 50, 21, 27, 28, 21, 7, 63, 26, 34, 9, 24, 52, 46, 38, 56, 48, 59, 31, 2) \
T (152,	32, 23, 3, 13, 35, 55, 18, 19, 42, 29, 48, 54, 53, 30, 17, 62, 37, 38, 31, 32, 6, 61, 7, 10, 52, 8, 27, 12, 59, 50, 36, 28) \
T (153,	37, 63, 52, 6, 46, 43, 28, 51, 7, 25, 59, 26, 42, 40, 22, 5, 4, 58, 54, 44, 41, 32, 53, 14, 39, 17, 30, 61, 24, 45, 31, 47) \
T (154,	33, 38, 42, 58, 12, 10, 8, 25, 3, 63, 55, 37, 31, 30, 2, 0, 59, 34, 43, 11, 36, 27, 49, 28, 22, 60, 48, 20, 1, 6, 35, 47) \
T (155,	57, 2, 6, 50, 16, 12, 36, 59, 1, 61, 39, 35, 21, 20, 17, 51, 48, 32, 19, 53, 29, 23, 18, 31, 24, 4, 52, 8, 27, 62, 60, 9) \
T (156,	60, 54, 13, 2, 32, 8, 58, 42, 23, 24, 12, 63, 43, 59, 22, 44, 15, 20, 4, 57, 61, 27, 47, 9, 49, 35, 28, 21, 48, 52, 19, 48) \
T (157,	61, 38, 50, 28, 35, 15, 29, 51, 30, 20, 1, 54, 10, 0, 24, 46, 59, 3, 16, 32, 45, 12, 7, 11, 48, 37, 26, 19, 62, 49, 13, 5) \
T (158,	10, 15, 16, 60, 24, 52, 19, 0, 25, 41, 46, 11, 29, 5, 62, 22, 43, 36, 17, 7, 18, 34, 49, 45, 59, 9, 42, 51, 55, 44, 21, 3) \
T (159,	30, 18, 51, 36, 21, 15, 6, 61, 13, 58, 44, 25, 57, 22, 11, 54, 33, 39, 43, 0, 26, 19, 29, 63, 8, 4, 41, 23, 31, 58, 5, 47) \
T (160,	43, 57, 36, 22, 21, 46, 53, 50, 63, 10, 20, 52, 8, 34, 18, 4, 56, 38, 41, 48, 35, 5, 39, 23, 54, 30, 4, 13, 40, 45, 2, 12) \
T (161,	24, 31, 61, 52, 55, 36, 32, 10, 33, 3, 58, 35, 62, 50, 51, 54, 63, 30, 28, 4, 26, 47, 14, 56, 23, 59, 39, 38, 16, 25, 17, 37) \
T (162,	60, 17, 42, 30, 56, 12, 14, 47, 6, 20, 5, 19, 55, 54, 33, 39, 58, 30, 22, 32, 0, 7, 18, 4, 63, 48, 36, 10, 3, 21, 28, 35) \
T (163,	11, 12, 21, 42, 0, 50, 48, 35, 20, 48, 38, 46, 52, 36, 43, 31, 28, 52, 48, 27, 1, 13, 60, 58, 39, 16, 32, 37, 57, 53, 24, 41) \
T (164,	10, 17, 45, 30, 36, 24, 55, 59, 5, 50, 15, 56, 13, 38, 25, 29, 53, 20, 6, 1, 26, 51, 36, 32, 23, 35, 62, 3, 34, 10, 12, 52) \
T (165,	15, 0, 34, 40, 12, 13, 28, 39, 39, 20, 48, 14, 22, 27, 51, 34, 11, 18, 16, 38, 62, 58, 6, 41, 44, 42, 36, 7, 33, 37, 37, 5) \
T (166,	35, 44, 35, 56, 39, 57, 11, 35, 1, 0, 61, 31, 52, 32, 33, 24, 37, 49, 18, 29, 24, 12, 5, 43, 55, 25, 60, 40, 4, 30, 38, 3) \
T (167,	25, 2, 53, 19, 62, 59, 49, 43, 15, 12, 15, 28, 22, 35, 40, 31, 47, 11, 16, 10, 41, 5, 7, 23, 48, 24, 60, 52, 30, 50, 55, 7) \
T (168,	49, 27, 41, 40, 42, 20, 39, 50, 17, 58, 9, 54, 2, 38, 48, 16, 62, 43, 18, 59, 22, 63, 29, 61, 60, 6, 33, 24, 14, 57, 4, 28) \
T (169,	13, 41, 9, 55, 51, 0, 46, 18, 42, 53, 37, 5, 7, 39, 15, 36, 30, 50, 11, 20, 52, 47, 1, 44, 63, 43, 6, 60, 2, 34, 31, 54) \
T (170,	49, 35, 62, 17, 55, 14, 30, 11, 8, 29, 2, 54, 31, 44, 36, 46, 37, 21, 15, 28, 27, 23, 57, 53, 51, 37, 34, 19, 47, 16, 43, 59) \
T (171,	50, 37, 40, 26, 46, 58, 54, 59, 17, 30, 23, 11, 43, 61, 31, 8, 60, 34, 25, 63, 4, 41, 39, 48, 51, 13, 49, 22, 52, 57, 29, 13) \
T (172,	42, 47, 27, 36, 47, 41, 45, 24, 8, 20, 35, 22, 38, 50, 57, 52, 28, 46, 53, 54, 37, 2, 48, 33, 32, 5, 60, 17, 19, 34, 55, 43) \
T (173,	19, 12, 42, 37, 36, 13, 0, 1, 62, 53, 54, 50, 44, 0, 24, 59, 2, 6, 10, 29, 3, 63, 34, 28, 13, 61, 46, 33, 7, 26, 27, 5) \
T (174,	32, 5, 30, 54, 15, 26, 25, 28, 1, 51, 11, 39, 2, 30, 27, 13, 43, 6, 7, 25, 31, 8, 3, 16, 41, 29, 26, 5, 19, 58, 42, 59) \
T (175,	48, 5, 7, 28, 40, 56, 49, 42, 47, 61, 24, 13, 44, 18, 38, 53, 30, 10, 21, 6, 29, 2, 33, 22, 14, 59, 42, 21, 45, 31, 27, 54) \
T (176,	56, 39, 62, 32, 46, 48, 36, 27, 3, 53, 22, 42, 31, 9, 40, 0, 19, 58, 16, 26, 1, 10, 16, 21, 63, 54, 23, 2, 15, 6, 7, 47) \
T (177,	31, 18, 57, 52, 5, 32, 56, 7, 11, 6, 9, 20, 30, 56, 46, 23, 42, 38, 3, 34, 19, 45, 21, 55, 0, 8, 53, 24, 29, 60, 25, 49) \
T (178,	32, 2, 48, 34, 7, 57, 1, 58, 39, 16, 53, 33, 31, 12, 21, 6, 4, 62, 50, 46, 61, 23, 5, 40, 44, 9, 28, 11, 11, 37, 25, 13) \
T (179,	47, 40, 12, 47, 26, 3, 33, 30, 46, 61, 33, 10, 19, 1, 20, 5, 37, 9, 28, 27, 25, 63, 15, 23, 43, 54, 11, 30, 6, 24, 57, 18) \
T (180,	16, 27, 2, 62, 63, 52, 26, 21, 37, 18, 41, 45, 15, 9, 51, 46, 28, 38, 60, 30, 35, 20, 7, 52, 53, 24, 42, 56, 33, 22, 48, 31) \
T (181,	0, 40, 54, 27, 2, 3, 21, 10, 32, 31, 55, 31, 34, 53, 36, 22, 59, 41, 1, 20, 13, 42, 46, 17, 52, 49, 28, 41, 37, 40, 4, 43) \
T (182,	35, 7, 38, 12, 27, 41, 8, 17, 1, 23, 46, 3, 28, 24, 61, 60, 21, 19, 48, 33, 26, 29, 45, 51, 48, 56, 34, 22, 10, 62, 25, 39) \
T (183,	61, 14, 56, 62, 44, 59, 54, 26, 61, 27, 52, 24, 3, 33, 12, 16, 48, 47, 28, 9, 21, 38, 23, 0, 55, 5, 22, 39, 9, 35, 59, 63) \
T (184,	41, 27, 35, 53, 49, 26, 12, 14, 3, 39, 19, 2, 16, 46, 47, 52, 24, 59, 6, 51, 52, 34, 4, 50, 32, 13, 63, 20, 45, 10, 28, 40) \
T (185,	14, 7, 4, 53, 23, 11, 9, 41, 26, 20, 46, 51, 3, 8, 18, 56, 31, 48, 38, 15, 12, 46, 42, 59, 40, 57, 16, 2, 21, 33, 37, 11) \
T (186,	46, 10, 6, 33, 7, 2, 37, 4, 11, 32, 59, 60, 2, 4, 12, 47, 26, 39, 27, 52, 5, 58, 30, 24, 36, 55, 63, 28, 62, 25, 43, 25) \
T (187,	6, 60, 15, 13, 18, 56, 39, 43, 61, 20, 63, 26, 37, 53, 23, 33, 44, 10, 14, 58, 35, 2, 17, 37, 41, 12, 1, 22, 10, 16, 5, 55) \
T (188,	31, 62, 49, 57, 22, 42, 61, 6, 34, 35, 3, 13, 16, 56, 2, 28, 38, 49, 22, 49, 0, 29, 17, 57, 59, 27, 5, 22, 26, 24, 40, 30) \
T (189,	3, 24, 42, 30, 19, 13, 9, 57, 6, 46, 36, 52, 16, 55, 60, 4, 2, 50, 32, 33, 58, 15, 22, 33, 21, 39, 20, 37, 27, 0, 1, 53) \
T (190,	19, 62, 32, 12, 18, 61, 31, 60, 14, 6, 15, 17, 22, 39, 30, 56, 49, 48, 16, 63, 42, 43, 1, 11, 20, 59, 27, 25, 63, 29, 37, 2) \
T (191,	0, 15, 16, 7, 56, 54, 8, 63, 39, 18, 32, 46, 12, 35, 17, 38, 41, 34, 36, 40, 51, 21, 61, 26, 43, 57, 53, 4, 45, 6, 57, 2) \
T (192,	52, 12, 62, 8, 60, 56, 29, 5, 6, 7, 9, 21, 44, 47, 48, 53, 58, 54, 45, 40, 15, 17, 1, 28, 4, 55, 22, 57, 32, 19, 33, 10) \
T (193,	14, 48, 58, 51, 60, 30, 42, 4, 3, 17, 44, 36, 34, 9, 8, 16, 46, 43, 32, 24, 47, 21, 2, 49, 59, 55, 10, 62, 11, 25, 29, 57) \
T (194,	35, 0, 38, 57, 14, 16, 12, 7, 10, 40, 20, 44, 42, 40, 22, 47, 31, 5, 54, 8, 13, 26, 62, 28, 32, 37, 48, 34, 41, 39, 60, 4) \
T (195,	30, 15, 1, 38, 55, 58, 7, 14, 13, 12, 11, 35, 17, 22, 39, 32, 50, 13, 5, 62, 51, 63, 46, 10, 8, 61, 9, 49, 59, 4, 0, 34) \
T (196,	37, 31, 6, 15, 23, 0, 30, 35, 4, 54, 56, 63, 3, 20, 44, 27, 43, 25, 4, 41, 5, 1, 38, 51, 59, 26, 42, 48, 58, 50, 5, 2) \
T (197,	3, 21, 28, 5, 22, 62, 16, 17, 56, 45, 46, 33, 41, 24, 25, 31, 10, 54, 19, 14, 0, 59, 42, 34, 12, 23, 53, 35, 55, 30, 18, 8) \
T (198,	3, 56, 16, 31, 45, 6, 37, 26, 34, 41, 36, 52, 21, 48, 13, 27, 22, 51, 62, 60, 59, 0, 25, 62, 44, 9, 23, 42, 11, 33, 39, 57) \
T (199,	59, 20, 31, 41, 24, 27, 26, 54, 7, 42, 30, 5, 25, 37, 53, 11, 21, 42, 39, 3, 2, 28, 47, 59, 32, 10, 57, 61, 52, 22, 19, 15) \
T (200,	3, 57, 34, 47, 8, 31, 63, 39, 34, 19, 52, 42, 51, 28, 50, 44, 59, 27, 21, 24, 30, 61, 28, 20, 56, 20, 37, 32, 12, 22, 14, 4) \
T (201,	1, 61, 23, 43, 21, 38, 13, 42, 45, 40, 24, 16, 59, 4, 46, 11, 43, 10, 30, 55, 20, 5, 49, 62, 22, 31, 54, 21, 53, 36, 17, 8) \
T (202,	49, 56, 60, 30, 24, 23, 40, 15, 32, 61, 37, 13, 56, 5, 57, 0, 28, 39, 27, 59, 60, 42, 3, 6, 51, 36, 8, 53, 34, 22, 35, 10) \
T (203,	27, 13, 16, 12, 30, 31, 15, 47, 23, 22, 37, 29, 40, 55, 6, 44, 11, 4, 49, 9, 50, 1, 25, 38, 59, 61, 2, 18, 28, 8, 17, 57) \
T (204,	5, 62, 5, 37, 27, 2, 31, 25, 9, 40, 58, 34, 60, 17, 14, 0, 8, 47, 44, 43, 21, 54, 15, 20, 57, 35, 30, 61, 36, 7, 16, 32) \
T (205,	55, 22, 9, 29, 54, 52, 0, 13, 39, 36, 63, 12, 43, 56, 58, 0, 10, 18, 48, 1, 20, 60, 4, 62, 2, 51, 41, 61, 1, 26, 24, 21) \
T (206,	29, 14, 52, 20, 49, 59, 31, 55, 61, 15, 20, 11, 1, 16, 39, 43, 53, 13, 35, 7, 38, 28, 41, 10, 63, 56, 2, 23, 0, 57, 30, 26) \
T (207,	31, 45, 56, 4, 19, 1, 32, 44, 20, 50, 8, 39, 58, 33, 59, 29, 62, 3, 55, 11, 28, 6, 32, 29, 43, 41, 7, 5, 18, 15, 21, 14) \
T (208,	29, 1, 31, 17, 10, 58, 27, 5, 63, 27, 23, 49, 60, 4, 33, 3, 9, 51, 28, 37, 35, 61, 42, 19, 18, 39, 34, 15, 21, 14, 7, 36) \
T (209,	47, 2, 8, 51, 56, 61, 12, 0, 37, 49, 24, 29, 44, 60, 30, 63, 33, 55, 5, 4, 27, 45, 50, 13, 59, 14, 39, 46, 40, 28, 19, 11) \
T (210,	50, 2, 52, 36, 3, 59, 4, 24, 20, 41, 8, 49, 48, 5, 28, 37, 22, 18, 26, 16, 61, 38, 43, 33, 30, 16, 6, 42, 55, 54, 14, 32) \
T (211,	56, 57, 33, 0, 47, 43, 50, 22, 15, 53, 1, 17, 60, 20, 63, 2, 27, 18, 51, 45, 23, 7, 31, 47, 4, 38, 34, 35, 49, 10, 9, 32) \
T (212,	42, 54, 63, 4, 20, 3, 48, 34, 28, 10, 6, 60, 33, 29, 37, 14, 35, 13, 11, 57, 43, 44, 24, 39, 7, 56, 37, 22, 41, 31, 49, 32) \
T (213,	40, 25, 61, 33, 60, 4, 1, 28, 29, 63, 47, 56, 9, 6, 22, 39, 7, 16, 38, 55, 18, 10, 34, 3, 24, 59, 9, 51, 44, 35, 14, 47) \
T (214,	14, 11, 0, 4, 25, 6, 13, 60, 2, 26, 11, 27, 31, 30, 41, 58, 48, 24, 35, 44, 32, 49, 17, 8, 3, 1, 54, 16, 61, 15, 10, 18) \
T (215,	7, 30, 38, 10, 26, 20, 54, 1, 28, 41, 0, 0, 5, 32, 56, 61, 48, 42, 14, 4, 34, 55, 39, 53, 46, 13, 62, 31, 53, 37, 24, 11) \
T (216,	43, 36, 2, 21, 0, 28, 16, 53, 23, 7, 47, 30, 37, 28, 6, 35, 40, 60, 50, 32, 60, 9, 32, 46, 55, 49, 29, 13, 22, 14, 8, 62) \
T (217,	15, 53, 9, 63, 0, 6, 29, 26, 35, 55, 38, 23, 50, 45, 2, 25, 16, 7, 53, 48, 47, 4, 18, 34, 28, 22, 43, 8, 60, 61, 21, 1) \
T (218,	60, 32, 56, 46, 28, 61, 35, 57, 2, 11, 44, 34, 13, 23, 39, 50, 18, 5, 1, 40, 59, 3, 51, 49, 43, 14, 12, 27, 43, 31, 1, 25) \
T (219,	4, 10, 19, 21, 46, 37, 30, 61, 45, 45, 40, 59, 39, 18, 42, 50, 2, 3, 23, 15, 32, 26, 52, 31, 35, 38, 48, 53, 20, 6, 12, 57) \
T (220,	37, 43, 11, 32, 25, 41, 42, 54, 19, 27, 46, 12, 35, 16, 58, 48, 38, 21, 52, 56, 13, 22, 24, 1, 30, 24, 51, 50, 59, 20, 15, 31) \
T (221,	31, 52, 23, 52, 42, 57, 3, 59, 56, 34, 35, 50, 25, 2, 45, 8, 11, 16, 62, 21, 25, 0, 15, 28, 18, 7, 24, 33, 1, 4, 20, 48) \
T (222,	2, 37, 10, 34, 16, 28, 56, 57, 42, 11, 0, 27, 60, 25, 23, 61, 44, 36, 55, 53, 62, 41, 13, 27, 24, 20, 14, 39, 19, 8, 50, 56) \
T (223,	47, 23, 30, 12, 48, 20, 63, 38, 60, 46, 43, 54, 1, 53, 6, 62, 2, 0, 37, 55, 39, 33, 19, 28, 26, 5, 41, 35, 44, 31, 9, 33) \
T (224,	8, 3, 25, 48, 57, 1, 24, 23, 32, 22, 12, 10, 5, 43, 46, 39, 56, 19, 33, 27, 0, 56, 13, 2, 38, 51, 60, 35, 15, 59, 23, 28) \
T (225,	15, 40, 34, 53, 58, 22, 7, 47, 54, 5, 33, 13, 60, 17, 52, 45, 24, 20, 36, 16, 61, 21, 46, 56, 23, 2, 42, 63, 43, 59, 57, 9) \
T (226,	43, 21, 31, 3, 24, 2, 53, 41, 25, 12, 47, 19, 48, 28, 36, 45, 29, 23, 9, 50, 63, 20, 1, 61, 7, 6, 37, 26, 10, 55, 60, 58) \
T (227,	59, 13, 48, 58, 52, 5, 16, 39, 7, 36, 37, 41, 47, 26, 40, 42, 25, 54, 6, 51, 63, 27, 12, 56, 28, 19, 49, 62, 38, 44, 61, 9)
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
