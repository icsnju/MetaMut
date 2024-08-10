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

struct A
{
  int i, j, k;
  char buf[255];
  int l, m, n, o;
};

int
main (void)
{
  const size_t s = sizeof (struct A);
  const size_t o = __builtin_offsetof (struct A, buf);
  struct A *a = malloc (s);
  struct A *b = malloc (o + 212);
  if (__builtin_object_size (a->buf, 0) != s - o)
    FAIL ();
  if (__builtin_object_size (a->buf, 1) != sizeof (a->buf))
    FAIL ();
  if (__builtin_object_size (a->buf, 2) != s - o)
    FAIL ();
  if (__builtin_object_size (a->buf, 3) != sizeof (a->buf))
    FAIL ();
  if (__builtin_object_size (&a->buf[0], 0) != s - o)
    FAIL ();
  if (__builtin_object_size (&a->buf[0], 1) != sizeof (a->buf))
    FAIL ();
  if (__builtin_object_size (&a->buf[0], 2) != s - o)
    FAIL ();
  if (__builtin_object_size (&a->buf[0], 3) != sizeof (a->buf))
    FAIL ();
  if (__builtin_object_size (&a->buf[6], 0) != s - o - 6)
    FAIL ();
  if (__builtin_object_size (&a->buf[6], 1) != sizeof (a->buf) - 6)
    FAIL ();
  if (__builtin_object_size (&a->buf[6], 2) != s - o - 6)
    FAIL ();
  if (__builtin_object_size (&a->buf[6], 3) != sizeof (a->buf) - 6)
    FAIL ();
  if (__builtin_object_size (b->buf, 0) != 212)
    FAIL ();
  if (__builtin_object_size (b->buf, 1) != 212)
    FAIL ();
  if (__builtin_object_size (b->buf, 2) != 212)
    FAIL ();
  if (__builtin_object_size (b->buf, 3) != 212)
    FAIL ();
  if (__builtin_object_size (&b->buf[0], 0) != 212)
    FAIL ();
  if (__builtin_object_size (&b->buf[0], 1) != 212)
    FAIL ();
  if (__builtin_object_size (&b->buf[0], 2) != 212)
    FAIL ();
  if (__builtin_object_size (&b->buf[0], 3) != 212)
    FAIL ();
  if (__builtin_object_size (&b->buf[28], 0) != 212 - 28)
    FAIL ();
  if (__builtin_object_size (&b->buf[28], 1) != 212 - 28)
    FAIL ();
  if (__builtin_object_size (&b->buf[28], 2) != 212 - 28)
    FAIL ();
  if (__builtin_object_size (&b->buf[28], 3) != 212 - 28)
    FAIL ();
  DONE ();
}
