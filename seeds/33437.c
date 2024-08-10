/* PR tree-optimization/91183 - strlen of a strcpy result with a conditional
   source not folded
   Runtime test to verify that multibyte stores are handled correctly.
   { dg-do run }
   { dg-options "-O2 -Wall" }  */

/* This is a replacement of needed parts from <stdlib.h> and <string.h>
   for -foptimize-strlen testing, to ensure we are testing the builtins
   rather than whatever the OS has in its headers.  */

#define NULL ((void *) 0)
typedef __SIZE_TYPE__ size_t;
extern void abort (void);
void *calloc (size_t, size_t);
void *malloc (size_t);
void free (void *);
char *strdup (const char *);
size_t strlen (const char *);
size_t strnlen (const char *, size_t);
void *memcpy (void *__restrict, const void *__restrict, size_t);
void *memmove (void *, const void *, size_t);
char *strcpy (char *__restrict, const char *__restrict);
char *strcat (char *__restrict, const char *__restrict);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
int strcmp (const char *, const char *);
#ifdef USE_GNU
void *mempcpy (void *__restrict, const void *__restrict, size_t);
char *stpcpy (char *__restrict, const char *__restrict);
#endif

int sprintf (char * __restrict, const char *__restrict, ...);
int snprintf (char * __restrict, size_t, const char *__restrict, ...);

#if defined(FORTIFY_SOURCE) && FORTIFY_SOURCE > 0 && __OPTIMIZE__
# define bos(ptr) __builtin_object_size (ptr, FORTIFY_SOURCE > 0)
# define bos0(ptr) __builtin_object_size (ptr, 0)

extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
memcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  return __builtin___memcpy_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
memmove (void *dest, const void *src, size_t len)
{
  return __builtin___memmove_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
strcpy (char *__restrict dest, const char *__restrict src)
{
  return __builtin___strcpy_chk (dest, src, bos (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
strcat (char *__restrict dest, const char *__restrict src)
{
  return __builtin___strcat_chk (dest, src, bos (dest));
}

# ifdef USE_GNU
extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
mempcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  return __builtin___mempcpy_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
stpcpy (char *__restrict dest, const char *__restrict src)
{
  return __builtin___stpcpy_chk (dest, src, bos (dest));
}
# endif
#endif

#define CHAR_BIT __CHAR_BIT__

typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;

#define NOIPA __attribute__ ((noclone, noinline, noipa))

/* Prevent the optimizer from detemining invariants from prior tests.  */
NOIPA void terminate (void)
{
  __builtin_abort ();
}

#define VERIFY(expr, str)						\
  do {									\
    const unsigned expect = strlen (str);				\
    const unsigned len = strlen (expr);					\
    if (len != expect)							\
      {									\
	__builtin_printf ("line %i: strlen(%s) == %u failed: "		\
			  "got %u with a = \"%.*s\"\n",			\
			  __LINE__, #expr, expect, len,			\
			  (int)sizeof a, a);				\
	terminate ();							\
      }									\
    if (memcmp (a, str, expect + 1))					\
      {									\
	__builtin_printf ("line %i: expected string \"%s\", "		\
			  "got a = \"%.*s\"\n",				\
			  __LINE__, str, (int)sizeof a, a);		\
	terminate ();							\
      }									\
  } while (0)


#define ELT(s, i)   ((s "\0\0\0\0")[i])

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define I16(s) (((uint16_t)ELT (s, 0) << 8) + (uint16_t)ELT (s, 1))
#  define I32(s)				\
  (((uint32_t)ELT (s, 0) << 24)			\
   + ((uint32_t)ELT (s, 1) << 16)		\
   + ((uint32_t)ELT (s, 2) << 8)		\
   + (uint32_t)ELT (s, 3))
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define I16(s) (((uint16_t)ELT (s, 1) << 8) + (uint16_t)ELT (s, 0))
#  define I32(s)				\
  (((uint32_t)ELT (s, 3) << 24)			\
   + ((uint32_t)ELT (s, 2) << 16)		\
   + ((uint32_t)ELT (s, 1) << 8)		\
   + (uint32_t)ELT (s, 0))
#endif

char a[32];

NOIPA void
i16_1 (void)
{
  *(uint16_t*)a = I16 ("12");
  *(uint16_t*)(a + 2) = I16 ("3");
  VERIFY (a, "123");

  *(uint16_t*)(a + 1) = I16 ("23");
  VERIFY (a, "123");

  *(uint16_t*)(a) = I16 ("12");
  VERIFY (a, "123");

  *(uint16_t*)(a + 1) = I16 ("2");
  VERIFY (a, "12");

  *(uint16_t*)(a + 3) = I16 ("45");
  *(uint16_t*)(a + 2) = I16 ("34");
  VERIFY (a, "12345");
}

NOIPA void
i16_2 (void)
{
  strcpy (a, "12");
  strcat (a, "34");

  *(uint16_t*)a = I16 ("12");
  VERIFY (a, "1234");

  *(uint16_t*)(a + 1) = I16 ("12");
  VERIFY (a, "1124");

  *(uint16_t*)(a + 2) = I16 ("12");
  VERIFY (a, "1112");

  *(uint16_t*)(a + 3) = I16 ("12");
  VERIFY (a, "11112");

  *(uint16_t*)(a + 4) = I16 ("12");
  VERIFY (a, "111112");
}


NOIPA void
i32_1 (void)
{
  *(uint32_t*)a = I32 ("1234");
  VERIFY (a, "1234");

  *(uint32_t*)(a + 1) = I32 ("2345");
  VERIFY (a, "12345");
}

NOIPA void
i32_2 (void)
{
  strcpy (a, "12");
  strcat (a, "34");

  *(uint32_t*)a = I32 ("1234");
  VERIFY (a, "1234");

  *(uint32_t*)(a + 4) = I32 ("567");
  VERIFY (a, "1234567");

  *(uint32_t*)(a + 7) = I32 ("89\0");
  VERIFY (a, "123456789");

  *(uint32_t*)(a + 3) = I32 ("4567");
  VERIFY (a, "123456789");

  *(uint32_t*)(a + 2) = I32 ("3456");
  VERIFY (a, "123456789");

  *(uint32_t*)(a + 1) = I32 ("2345");
  VERIFY (a, "123456789");
}


NOIPA void
i32_3 (void)
{
  strcpy (a, "1234");
  strcat (a, "5678");

  *(uint32_t*)a = I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)(a + 1) = I32 ("234");
  VERIFY (a, "1234");

  *(uint32_t*)(a + 2) = I32 ("3456");
  VERIFY (a, "12345678");

  *(uint32_t*)(a + 3) = I32 ("4567");
  VERIFY (a, "12345678");

  *(uint32_t*)(a + 4) = I32 ("5678");
  VERIFY (a, "12345678");

  *(uint32_t*)(a + 5) = I32 ("6789");
  VERIFY (a, "123456789");

  *(uint32_t*)(a + 6) = I32 ("789A");
  VERIFY (a, "123456789A");
}

volatile int vzero = 0;

NOIPA void
i32_4 (void)
{
  strcpy (a, "1234");
  strcat (a, "5678");

  *(uint32_t*)a = vzero ? I32 ("1\0\0\0") : I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)a = vzero ? I32 ("12\0\0") : I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)a = vzero ? I32 ("123\0") : I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)a = vzero ? I32 ("1234") : I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)a = vzero ? I32 ("1235") : I32 ("1234");
  VERIFY (a, "12345678");

  *(uint32_t*)a = vzero ? I32 ("1234") : I32 ("123\0");
  VERIFY (a, "123");

  *(uint32_t*)(a + 3) = vzero ? I32 ("456\0") : I32 ("4567");
  VERIFY (a, "12345678");
}


int main ()
{
  memset (a, 0, sizeof a);
  i16_1 ();

  memset (a, 0, sizeof a);
  i16_2 ();


  memset (a, 0, sizeof a);
  i32_1 ();

  memset (a, 0, sizeof a);
  i32_2 ();

  memset (a, 0, sizeof a);
  i32_3 ();

  memset (a, 0, sizeof a);
  i32_4 ();
}
