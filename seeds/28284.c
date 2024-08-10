/* { dg-do run } */
/* { dg-options "-O2" } */

#define __builtin_object_size __builtin_dynamic_object_size
/* { dg-do run } */
/* { dg-options "-O2" } */

typedef __SIZE_TYPE__ size_t;
#ifdef __cplusplus
extern "C" {
#endif
  extern void exit (int);
  extern void *malloc (size_t);
  extern void free (void *);
  extern void *calloc (size_t, size_t);
  extern void *alloca (size_t);
  extern void *memcpy (void *, const void *, size_t);
  extern void *memset (void *, int, size_t);
  extern char *strcpy (char *, const char *);
  extern char *strdup (const char *);
  extern char *strndup (const char *, size_t);
#ifdef __cplusplus
}
#endif

unsigned nfails = 0;

#define FAIL() \
  do { \
    __builtin_printf ("Failure at line: %d\n", __LINE__);		      \
    nfails++;								      \
  } while (0)

#define DONE() \
  do {									      \
    if (nfails > 0)							      \
      __builtin_abort ();						      \
    return 0;								      \
  } while (0)

int
main ()
{
  struct A { char buf1[9]; char buf2[1]; } a;

  if (__builtin_object_size (a.buf1 + (0 + 4), 1) != 5)
    FAIL ();
  char *p = a.buf1;
  p += 1;
  p += 3;
  if (__builtin_object_size (p, 1) != 5)
    FAIL ();
  p = (char *) &a;
  char *q = p + 1;
  char *r = q + 3;
  char *t = r;
  if (r != (char *) &a + 4)
    t = (char *) &a + 1;
  if (__builtin_object_size (t, 1) != 6)
    FAIL ();

  DONE ();
}
