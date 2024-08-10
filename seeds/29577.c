/* PR c/67730 */
/* { dg-do compile } */
/* { dg-options "" } */

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

extern void bar (int);

int
fn1 (void)
{
  int a = NULL; /* { dg-warning "initialization of 'int' from 'void \\*' makes integer from pointer" } */
  a = NULL; /* { dg-warning "assignment to 'int' from 'void \\*' makes integer from pointer" } */
  bar (NULL); /* { dg-warning "passing argument 1" } */
  return NULL; /* { dg-warning "returning 'void \\*' from a function with return type 'int' makes integer from pointer" } */
}

int
fn2 (void)
{
  RETURN; /* { dg-warning "returning 'void \\*' from a function with return type 'int' makes integer from pointer" } */
}
