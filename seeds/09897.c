/* This file was miscompiled by an earlier version of the object size
   checking patch.  Object size in one of the memcpy calls was
   incorrectly determined to be 0 while it should be (size_t) -1
   (== unknown).  */
/* { dg-do compile } */
/* { dg-options "-O2 -ftrack-macro-expansion=0" } */

#ifndef os
# define os(ptr) __builtin_object_size (ptr, 0)
#endif

/* This is one of the alternatives for object size checking.
   If dst has side-effects, size checking will never be done.  */
#undef memcpy
#define memcpy(dst, src, len) \
  __builtin___memcpy_chk (dst, src, len, os (dst))
#undef mempcpy
#define mempcpy(dst, src, len) \
  __builtin___mempcpy_chk (dst, src, len, os (dst))
#undef memmove
#define memmove(dst, src, len) \
  __builtin___memmove_chk (dst, src, len, os (dst))
#undef memset
#define memset(dst, val, len) \
  __builtin___memset_chk (dst, val, len, os (dst))
#undef strcpy
#define strcpy(dst, src) \
  __builtin___strcpy_chk (dst, src, os (dst))
#undef stpcpy
#define stpcpy(dst, src) \
  __builtin___stpcpy_chk (dst, src, os (dst))
#undef strcat
#define strcat(dst, src) \
  __builtin___strcat_chk (dst, src, os (dst))
#undef strncpy
#define strncpy(dst, src, len) \
  __builtin___strncpy_chk (dst, src, len, os (dst))
#undef stpncpy
#define stpncpy(dst, src, len) \
  __builtin___stpncpy_chk (dst, src, len, os (dst))
#undef strncat
#define strncat(dst, src, len) \
  __builtin___strncat_chk (dst, src, len, os (dst))
#undef sprintf
#define sprintf(dst, ...) \
  __builtin___sprintf_chk (dst, 0, os (dst), __VA_ARGS__)
#undef vsprintf
#define vsprintf(dst, fmt, ap) \
  __builtin___vsprintf_chk (dst, 0, os (dst), fmt, ap)
#undef snprintf
#define snprintf(dst, len, ...) \
  __builtin___snprintf_chk (dst, len, 0, os (dst), __VA_ARGS__)
#undef vsnprintf
#define vsnprintf(dst, len, fmt, ap) \
  __builtin___vsnprintf_chk (dst, len, 0, os (dst), fmt, ap)

/* Now "redefine" even builtins for the purpose of testing.  */
#undef __builtin_memcpy
#define __builtin_memcpy(dst, src, len) memcpy (dst, src, len)
#undef __builtin_mempcpy
#define __builtin_mempcpy(dst, src, len) mempcpy (dst, src, len)
#undef __builtin_memmove
#define __builtin_memmove(dst, src, len) memmove (dst, src, len)
#undef __builtin_memset
#define __builtin_memset(dst, val, len) memset (dst, val, len)
#undef __builtin_strcpy
#define __builtin_strcpy(dst, src) strcpy (dst, src)
#undef __builtin_stpcpy
#define __builtin_stpcpy(dst, src) stpcpy (dst, src)
#undef __builtin_strcat
#define __builtin_strcat(dst, src) strcat (dst, src)
#undef __builtin_strncpy
#define __builtin_strncpy(dst, src, len) strncpy (dst, src, len)
#undef __builtin_strncat
#define __builtin_strncat(dst, src, len) strncat (dst, src, len)
#undef __builtin_sprintf
#define __builtin_sprintf(dst, ...) sprintf (dst, __VA_ARGS__)
#undef __builtin_vsprintf
#define __builtin_vsprintf(dst, fmt, ap) vsprintf (dst, fmt, ap)
#undef __builtin_snprintf
#define __builtin_snprintf(dst, len, ...) snprintf (dst, len, __VA_ARGS__)
#undef __builtin_vsnprintf
#define __builtin_vsnprintf(dst, len, fmt, ap) vsnprintf (dst, len, fmt, ap)

extern void *chk_fail_buf[];
extern volatile int chk_fail_allowed, chk_calls;
extern volatile int memcpy_disallowed, mempcpy_disallowed, memmove_disallowed;
extern volatile int memset_disallowed, strcpy_disallowed, stpcpy_disallowed;
extern volatile int strncpy_disallowed, stpncpy_disallowed, strcat_disallowed;
extern volatile int strncat_disallowed, sprintf_disallowed, vsprintf_disallowed;
extern volatile int snprintf_disallowed, vsnprintf_disallowed;

/* A storage class that ensures that declarations bind locally.  We want
   to test non-static declarations where we know it is safe to do so.  */
#if __PIC__ && !__PIE__
#define LOCAL static
#else
#define LOCAL
#endif

void *bar (int);
extern void *malloc (__SIZE_TYPE__);

struct A
{
  int i, j, k;
};

/* Here all object sizes are not known at compile time.  There
   should be no warning, nor any checker functions called.  */

void
foo (const struct A *x, int y, const unsigned char *z)
{
  unsigned int b;
  unsigned char *c = 0;

  b = (x->i & 0xff) == 1 ? 3 : 4;
  if (y)
    c = bar (x->j * x->k);

  const unsigned char *d = z;
  unsigned char *e = c;
  unsigned char *f = c + x->j * x->k;
  int g = 0;

  while (e < f)
    {
      unsigned int h = *d++;

      if (h & 128)
	{
	  h = h - 128;
	  g = e + h * b > f;
	  if (g)
	    h = (f - e) / b;
	  if (b < 4)
	    do
	      {
		memcpy (e, d, 3);
		e += 3;
	      }
	    while (--h);
	  else
	    do
	      {
		memcpy (e, d, 4);
		e += 4;
	      }
	    while (--h);
	  d += b;
	}
      else
	{
	  h *= b;
	  g = e + h > f;
	  if (g)
	    h = f - e;
	  memcpy (e, d, h);
	  e += h;
	  d += h;
	}
    }
}

/* The same routine, slightly modified:
   1) c has known size at compile time
   2) e += h was changed into e += 16.
      GCC could actually through VRP determine that
      in e += h is (h >= 0 && h <= 127), thus know
      it is pointer addition and not subtraction and
      know e's __builtin_object_size (e, 0) is at 512,
      but we are not there yet.  */

unsigned char *
baz (const struct A *x, const unsigned char *z)
{
  unsigned int b;
  unsigned char *c = 0;

  b = (x->i & 0xff) == 1 ? 3 : 4;
  c = malloc (512);

  const unsigned char *d = z;
  unsigned char *e = c;
  unsigned char *f = c + x->j * x->k;
  int g = 0;

  while (e < f)
    {
      unsigned int h = *d++;

      if (h & 128)
	{
	  h = h - 128;
	  g = e + h * b > f;
	  if (g)
	    h = (f - e) / b;
	  if (b < 4)
	    do
	      {
		memcpy (e, d, 3);
		e += 3;
	      }
	    while (--h);
	  else
	    do
	      {
		memcpy (e, d, 513); /* { dg-warning "writing" "memcpy" } */
		e += 4;
	      }
	    while (--h);
	  d += b;
	}
      else
	{
	  h *= b;
	  g = e + h > f;
	  if (g)
	    h = f - e;
	  memcpy (e, d, h);
	  /* e += h; */
	  e += 16;
	  d += h;
	}
    }
  return c;
}
