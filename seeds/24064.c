/* { dg-do compile { target powerpc*-*-darwin* } } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-Wno-long-long" } */

#pragma pack(push, 2)

typedef struct _cd {
 char c;
 double d;
} cd;

typedef struct _dc {
 double d;
 char c;
} dc;

typedef struct _cL {
 char c;
 long long L;
} cL;

typedef struct _Lc {
 long long L;
 char c;
} Lc;

typedef struct _cD {
 char c;
 long double D;
} cD;

typedef struct _Dc {
 long double D;
 char c;
} Dc;

int tcd[sizeof(cd) != 10 ? -1 : 1];
int acd[__alignof__(cd) != 2 ? -1 : 1];

int sdc[sizeof(dc) != 10 ? -1 : 1];
int adc[__alignof__(dc) != 2 ? -1 : 1];

int scL[sizeof(cL) != 10 ? -1 : 1];
int acL[__alignof__(cL) != 2 ? -1 : 1];

int sLc[sizeof(Lc) != 10 ? -1 : 1];
int aLc[__alignof__(Lc) != 2 ? -1 : 1];

int scD[sizeof(cD) != 18 ? -1 : 1];
int acD[__alignof__(cD) != 2 ? -1 : 1];

int sDc[sizeof(Dc) != 18 ? -1 : 1];
int aDc[__alignof__(Dc) != 2 ? -1 : 1];

#pragma pack(pop)
