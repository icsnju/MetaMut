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

struct S {
    int len;
    char s[0];
};
int main()
{
  char buf[sizeof (struct S) + 32];
  if (__builtin_object_size (((struct S *)&buf[0])->s, 1) != 32)
    FAIL ();
  if (__builtin_object_size (((struct S *)&buf[1])->s, 1) != 31)
    FAIL ();
  if (__builtin_object_size (((struct S *)&buf[64])->s, 0) != 0)
    FAIL ();

  DONE ();
}
