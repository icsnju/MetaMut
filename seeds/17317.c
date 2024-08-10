/* PR c/67730 */
/* { dg-do compile } */
/* { dg-options "-Wc++-compat" } */

#pragma GCC system_header
#define NULL (void *) 0
#define ONEP (void *) 1
#define RETURN return NULL

extern void sysbar (unsigned char *);

unsigned char *
sysfn1 (void *p)
{
   unsigned char *uc = ONEP;
   uc = ONEP;
   sysbar (ONEP);
   return ONEP;
}

extern void sysbar2 (int);

int
sysfn2 (void)
{
  int a = NULL;
  a = NULL;
  sysbar2 (NULL);
  return NULL;
}

int
sysfn3 (void)
{
  RETURN;
}

extern void bar (unsigned char *);

unsigned char *
f (void *p)
{
   unsigned char *uc = ONEP; /* { dg-warning "request for implicit conversion" } */
   uc = ONEP; /* { dg-warning "request for implicit conversion" } */
   bar (ONEP); /* { dg-warning "request for implicit conversion" } */
   return ONEP; /* { dg-warning "request for implicit conversion" } */
}
