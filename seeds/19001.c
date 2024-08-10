/* Test for cross x86_64<->w64 abi va_list calls.
*/
/* Origin: Kai Tietz <kai.tietz@onevision.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */
/* First the default target definition.  */
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
  typedef __builtin_va_list __gnuc_va_list;
#endif

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
  typedef __gnuc_va_list va_list;
#endif

#define __va_copy(d,s)	__builtin_va_copy(d,s)
#define __va_start(v,l)	__builtin_va_start(v,l)
#define __va_arg(v,l)	__builtin_va_arg(v,l)
#define __va_end(v)	__builtin_va_end(v)

#define __ms_va_copy(d,s) __builtin_ms_va_copy(d,s)
#define __ms_va_start(v,l) __builtin_ms_va_start(v,l)
#define __ms_va_arg(v,l)	__builtin_va_arg(v,l)
#define __ms_va_end(v) __builtin_ms_va_end(v)

#define __sysv_va_copy(d,s) __builtin_sysv_va_copy(d,s)
#define __sysv_va_start(v,l) __builtin_sysv_va_start(v,l)
#define __sysv_va_arg(v,l)	__builtin_va_arg(v,l)
#define __sysv_va_end(v) __builtin_sysv_va_end(v)

#define CALLABI_NATIVE

#ifdef _WIN64
#define CALLABI_CROSS __attribute__ ((sysv_abi))

#define CROSS_VA_LIST	__builtin_sysv_va_list

#define CROSS_VA_COPY(d,s) __sysv_va_copy(d,s)
#define CROSS_VA_START(v,l) __sysv_va_start(v,l)
#define CROSS_VA_ARG(v,l) __sysv_va_arg(v,l)
#define CROSS_VA_END(v)  __sysv_va_end(v)

#else

#define CALLABI_CROSS __attribute__ ((ms_abi))

#define CROSS_VA_LIST	__builtin_ms_va_list

#define CROSS_VA_COPY(d,s) __ms_va_copy(d,s)
#define CROSS_VA_START(v,l) __ms_va_start(v,l)
#define CROSS_VA_ARG(v,l) __ms_va_arg(v,l)
#define CROSS_VA_END(v)  __ms_va_end(v)

#endif
extern __SIZE_TYPE__ strlen (const char *);
extern int sprintf (char *,const char *, ...);
extern void abort (void);

static
void CALLABI_CROSS vdo_cpy (char *s, CROSS_VA_LIST argp)
{
  __SIZE_TYPE__ len;
  char *r = s;
  char *e;
  *r = 0;
  for (;;) {
    e = CROSS_VA_ARG (argp,char *);
    if (*e == 0) break;
    sprintf (r,"%s", e);
    r += strlen (r);
  }
}

static
void CALLABI_CROSS do_cpy (char *s, ...)
{
  CROSS_VA_LIST argp;
  CROSS_VA_START (argp, s);
  vdo_cpy (s, argp);
  CROSS_VA_END (argp);
}

int main ()
{
  char s[256];

  do_cpy (s, "1","2","3","4", "5", "6", "7", "");

  if (s[0] != '1' || s[1] !='2' || s[2] != '3' || s[3] != '4'
      || s[4] != '5' || s[5] != '6' || s[6] != '7' || s[7] != 0)
    abort ();

  return 0;
}
