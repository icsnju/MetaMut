/* { dg-do run } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-options "-fsanitize=float-cast-overflow -fno-sanitize-recover=float-cast-overflow" } */
/* FIXME: When _DecimalXX <-> {signed, unsigned} __int128 conversions are
   supported, -DBROKEN_DECIMAL_INT128 can be removed.  */
/* { dg-additional-options "-DUSE_DFP -DBROKEN_DECIMAL_INT128" { target dfp } } */

#define USE_FLT_DBL_LDBL
#ifdef __SIZEOF_INT128__
#define USE_INT128
#endif
#ifdef __SIZEOF_FLOAT80__
#define USE_FLOAT80
#endif
#ifdef __SIZEOF_FLOAT128__
#define USE_FLOAT128
#endif

#define CVTFN(type1, type2) \
__attribute__((noinline)) type1	\
cvt_##type1##_##type2 (type2 x)	\
{				\
  return x;			\
}

typedef signed char sc;
#define sc_MIN (-__SCHAR_MAX__ - 1)
#define sc_MAX __SCHAR_MAX__
typedef unsigned char uc;
#define uc_MIN 0
#define uc_MAX (2U * __SCHAR_MAX__ + 1U)
typedef char c;
#define c_MIN ((((char) -1) > (char) 0) ? uc_MIN : sc_MIN)
#define c_MAX ((((char) -1) > (char) 0) ? uc_MAX : sc_MAX)
typedef signed short ss;
#define ss_MIN (-__SHRT_MAX__ - 1)
#define ss_MAX __SHRT_MAX__
typedef unsigned short us;
#define us_MIN 0
#define us_MAX (2U * __SHRT_MAX__ + 1U)
typedef signed int si;
#define si_MIN (-__INT_MAX__ - 1)
#define si_MAX __INT_MAX__
typedef unsigned int ui;
#define ui_MIN 0
#define ui_MAX (2U * __INT_MAX__ + 1U)
typedef signed long sl;
#define sl_MIN (-__LONG_MAX__ - 1L)
#define sl_MAX __LONG_MAX__
typedef unsigned long ul;
#define ul_MIN 0L
#define ul_MAX (2UL * __LONG_MAX__ + 1UL)
typedef signed long long sll;
#define sll_MIN (-__LONG_LONG_MAX__ - 1LL)
#define sll_MAX __LONG_LONG_MAX__
typedef unsigned long long ull;
#define ull_MIN 0LL
#define ull_MAX (2ULL * __LONG_LONG_MAX__ + 1ULL)
#ifdef USE_INT128
typedef signed __int128 si128;
# define si128_MAX \
  ((signed __int128) ((((unsigned __int128) 1) \
		       << (__CHAR_BIT__ * __SIZEOF_INT128__ - 1)) - 1))
# define si128_MIN (-si128_MAX - 1)
typedef unsigned __int128 ui128;
#define ui128_MIN ((unsigned __int128) 0)
#define ui128_MAX (((unsigned __int128) 2) * si128_MAX + 1)
#endif

#ifdef si128_MAX
# define CVTS128(type2) CVTFN (si128, type2) CVTFN (ui128, type2)
#else
# define CVTS128(type2)
#endif

#define CVTS(type2) \
  CVTFN (sc, type2) CVTFN (c, type2) CVTFN (uc, type2)	\
  CVTFN (ss, type2) CVTFN (us, type2)			\
  CVTFN (si, type2) CVTFN (ui, type2)			\
  CVTFN (sl, type2) CVTFN (ul, type2)			\
  CVTFN (sll, type2) CVTFN (ull, type2)			\
  CVTS128 (type2)

#ifdef __SIZEOF_INT128__
# define TWO ((unsigned __int128) 2)
# define M1U ((unsigned __int128) -1)
# define MAXS (__CHAR_BIT__ * __SIZEOF_INT128__)
# define MAXT unsigned __int128
#else
# define TWO 2ULL
# define M1U -1ULL
# define MAXS (__CHAR_BIT__ * __SIZEOF_LONG_LONG__)
# define MAXT unsigned long long
#endif

#ifdef USE_FLT_DBL_LDBL
typedef float f;
#define f_RADIX 2
#define f_MANT_DIG __FLT_MANT_DIG__
#define f_MAX ((TWO << (f_MANT_DIG - 1)) - 1)
typedef double d;
#define d_RADIX 2
#define d_MANT_DIG __DBL_MANT_DIG__
#define d_MAX ((TWO << (d_MANT_DIG - 1)) - 1)
typedef long double ld;
#define ld_RADIX 2
#define ld_MANT_DIG __LDBL_MANT_DIG__
#define ld_MAX \
  (ld_MANT_DIG > MAXS ? M1U : (TWO << (ld_MANT_DIG > MAXS \
				       ? 0 : ld_MANT_DIG - 1)) - 1)
CVTS (f)
CVTS (d)
CVTS (ld)
#endif
#ifdef USE_FLOAT80
typedef __float80 f80;
# define f80_RADIX 2
# define f80_MANT_DIG 64
# define f80_MAX ((TWO << (f80_MANT_DIG - 1)) - 1)
CVTS (f80)
#endif
#ifdef USE_FLOAT128
typedef __float128 f128;
# define f128_RADIX 2
# define f128_MANT_DIG 113
# define f128_MAX \
  (f128_MANT_DIG > MAXS ? M1U : (TWO << (f128_MANT_DIG > MAXS \
					 ? 0 : f128_MANT_DIG - 1)) - 1)
CVTS (f128)
#endif
#ifdef USE_DFP
# ifdef __cplusplus
typedef float _Decimal32 __attribute__((mode(SD)));
typedef float _Decimal64 __attribute__((mode(DD)));
typedef float _Decimal128 __attribute__((mode(TD)));
# endif
typedef _Decimal32 d32;
# define d32_RADIX 10
# define d32_MANT_DIG __DEC32_MANT_DIG__
# if d32_MANT_DIG == 7
#  define d32_MAX 9999999ULL
# endif
typedef _Decimal64 d64;
# define d64_RADIX 10
# define d64_MANT_DIG __DEC64_MANT_DIG__
# if d64_MANT_DIG == 16
#  define d64_MAX 9999999999999999ULL
# endif
typedef _Decimal128 d128;
# define d128_RADIX 10
# define d128_MANT_DIG __DEC128_MANT_DIG__
# if d128_MANT_DIG == 34
#  ifdef __SIZEOF_INT128__
/* #define d128_MAX 0x1ed09bead87c0378d8e63ffffffff */
#   define d128_MAX \
  ((((unsigned __int128) 0x1ed09bead87c0) << 64) + 0x378d8e63ffffffffULL)
#  else
#   define d128_MAX M1U
#  endif
# endif
# ifdef BROKEN_DECIMAL_INT128
#  undef CVTS128
#  define CVTS128(type2)
# endif
CVTS (d32)
CVTS (d64)
CVTS (d128)
#endif

extern
#ifdef __cplusplus
"C"
#endif
void abort ();

#define TEST(type1, type2) \
  if (cvt_##type1##_##type2 (-0.5f) != 0) abort ();		\
  if (cvt_##type1##_##type2 (0.5f) != 0) abort ();		\
  if (cvt_##type1##_##type2 (-0.75f) != 0) abort ();		\
  if (cvt_##type1##_##type2 (0.75f) != 0) abort ();		\
  if (type1##_MIN)						\
    {								\
      /* For RADIX 2 type1##_MIN should be always */		\
      /* exactly representable in type2.  */			\
      if (type2##_RADIX == 2					\
	  || type1##_MAX <= type2##_MAX)			\
	{							\
	  if (cvt_##type1##_##type2 (type1##_MIN)		\
	      != type1##_MIN) abort ();				\
	  volatile type2 tem = ((type2) -0.75f) + type1##_MIN;	\
	  volatile type2 tem2 = ((type2) -1.0f) + type1##_MIN;	\
	  if (tem != tem2					\
	      && cvt_##type1##_##type2 ((type2) -0.75f		\
					+ type1##_MIN)		\
		 != type1##_MIN) abort ();			\
	}							\
      else							\
	{							\
	  type2 min = type1##_MIN;				\
	  /* tem could be below minimum here due to */		\
	  /* rounding.  */					\
	  MAXT add = 1;						\
	  while (add)						\
	    {							\
	      volatile type2 tem = type1##_MIN + (type1) add;	\
	      if (tem != min)					\
		break;						\
	      MAXT newadd = add * type2##_RADIX;		\
	      if (newadd < add || newadd > type1##_MAX)		\
		add = 0;					\
	      else						\
		add = newadd;					\
	    }							\
	  if (add)						\
	    {							\
	      MAXT newadd					\
		= (-(type1##_MIN + (type1) add)) % add;		\
	      volatile type2 tem = type1##_MIN + (type1) newadd;\
	      volatile type2 tem2 = type1##_MIN + (type1) add;	\
	      if (tem == tem2)					\
		add = newadd;					\
	      else						\
		{						\
		  newadd += add;				\
		  if (newadd < add || newadd > type1##_MAX)	\
		    add = 0;					\
		  else						\
		    {						\
		      tem = type1##_MIN + (type1) newadd;	\
		      if (tem == tem2)				\
			add = newadd;				\
		      else					\
			add = 0;				\
		    }						\
		}						\
	    }							\
	  if (add						\
	      && cvt_##type1##_##type2 (type1##_MIN		\
					+ (type1) add)		\
		 != type1##_MIN + (type1) add) abort ();	\
	}							\
    }								\
  if (type1##_MAX <= type2##_MAX)				\
    {								\
      if (cvt_##type1##_##type2 (type1##_MAX) != type1##_MAX)	\
	abort ();						\
      volatile type2 tem = ((type2) 0.75f) + type1##_MAX;	\
      volatile type2 tem2 = ((type2) 1.0f) + type1##_MAX;	\
      if (tem < tem2						\
	  && cvt_##type1##_##type2 ((type2) 0.75f + type1##_MAX)\
	     != type1##_MAX) abort ();				\
    }								\
  else								\
    {								\
      type2 max = type1##_MAX;					\
      /* tem could be above maximum here due to rounding.  */	\
      MAXT sub = 1;						\
      while (sub)						\
	{							\
	  volatile type2 tem = type1##_MAX - sub;		\
	  if (tem != max)					\
	    break;						\
	  MAXT newsub = sub * type2##_RADIX;			\
	  if (newsub < sub || newsub > type1##_MAX)		\
	    sub = 0;						\
	  else							\
	    sub = newsub;					\
	}							\
      if (sub)							\
	{							\
	  MAXT newsub = ((type1##_MAX - sub) % sub);		\
	  volatile type2 tem = type1##_MAX - newsub;		\
	  volatile type2 tem2 = type1##_MAX - sub;		\
	  if (tem == tem2)					\
	    sub = newsub;					\
	  else							\
	    {							\
	      newsub += sub;					\
	      if (newsub < sub || newsub > type1##_MAX)		\
		sub = 0;					\
	      else						\
		{						\
		  tem = type1##_MAX - newsub;			\
		  if (tem == tem2)				\
		    sub = newsub;				\
		  else						\
		    sub = 0;					\
		}						\
	    }							\
	}							\
      if (sub							\
	  && cvt_##type1##_##type2 (type1##_MAX - sub)		\
	     != type1##_MAX - sub) abort ();			\
    }


#ifdef si128_MAX
# define TESTS128(type2) TEST (si128, type2) TEST (ui128, type2)
#else
# define TESTS128(type2)
#endif

#define TESTS(type2) \
  TEST (sc, type2) TEST (c, type2) TEST (uc, type2)	\
  TEST (ss, type2) TEST (us, type2)			\
  TEST (si, type2) TEST (ui, type2)			\
  TEST (sl, type2) TEST (ul, type2)			\
  TEST (sll, type2) TEST (ull, type2)			\
  TESTS128 (type2)

int
main ()
{
#ifdef f_MAX
  TESTS (f)
#endif
#ifdef d_MAX
  TESTS (d)
#endif
#ifdef ld_MAX
  TESTS (ld)
#endif
#ifdef f80_MAX
  TESTS (f80)
#endif
#ifdef f128_MAX
  TESTS (f128)
#endif
#ifdef BROKEN_DECIMAL_INT128
# undef TESTS128
# define TESTS128(type2)
# undef TWO
# undef M1U
# undef MAXS
# undef MAXT
# define TWO 2ULL
# define M1U -1ULL
# define MAXS (__CHAR_BIT__ * __SIZEOF_LONG_LONG__)
# define MAXT unsigned long long
#endif
#ifdef d32_MAX
  TESTS (d32)
#endif
#ifdef d64_MAX
  TESTS (d64)
#endif
#ifdef d128_MAX
  TESTS (d128)
#endif
  return 0;
}
