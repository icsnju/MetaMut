extern void abort (void);
extern int inside_main;

__attribute__ ((__noinline__))
char *
strchr (const char *s, int c)
{
#ifdef __OPTIMIZE__
  if (inside_main)
    abort ();
#endif

  for (;;)
    {
      if (*s == c)
	return (char *) s;
      if (*s == 0)
	return 0;
      s++;
    }
}

__attribute__ ((__noinline__))
char *
index (const char *s, int c)
{
  return strchr (s, c);
}
#ifdef __vxworks
/* The RTP C library uses bzero, bfill and bcopy, all of which are defined
   in the same file as index.  */
extern int inside_main;

__attribute__ ((__noinline__))
void
bzero (void *s, __SIZE_TYPE__ n)
{
  char *p;

  for (p = s; n-- > 0; p++)
    *p = 0;

#ifdef __OPTIMIZE__
  if (inside_main)
    abort ();
#endif
}
extern int inside_main;

__attribute__ ((__noinline__))
void
bfill (void *s, __SIZE_TYPE__ n, int ch)
{
  char *p;

  for (p = s; n-- > 0; p++)
    *p = ch;

#ifdef __OPTIMIZE__
  if (inside_main)
    abort ();
#endif
}
extern void abort (void);
extern int inside_main;

__attribute__ ((__noinline__))
void *
memmove (void *dst, const void *src, __SIZE_TYPE__ n)
{
  char *dstp;
  const char *srcp;

#ifdef __OPTIMIZE__
  if (inside_main)
    abort ();
#endif

  srcp = src;
  dstp = dst;
  if (srcp < dstp)
    while (n-- != 0)
      dstp[n] = srcp[n];
  else
    while (n-- != 0)
      *dstp++ = *srcp++;

  return dst;
}

void
bcopy (const void *src, void *dst, __SIZE_TYPE__ n)
{
  memmove (dst, src, n);
}
#endif
