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
extern void abort (void);

#define SZ_ARGS	1ll,2ll,3ll,4ll,5ll,6ll,7ll,0ll

static
int fct1 (CROSS_VA_LIST argp, ...)
{
  long long p1,p2;
  int ret = 1;
  va_list argp_2;

    __va_start (argp_2,argp);
  do {
    p1 = __va_arg (argp_2, long long);
    p2 = CROSS_VA_ARG (argp, long long);
    if (p1 != p2)
      ret = 0;
  } while (ret && p1 != 0);
  __va_end (argp_2);
  return ret;
}

static
int CALLABI_CROSS fct2 (int dummy, ...)
{
  CROSS_VA_LIST argp;
  int ret = dummy;

  CROSS_VA_START (argp, dummy);
  ret += fct1 (argp, SZ_ARGS);
  CROSS_VA_END (argp);
  return ret;
}

int main()
{
  if (fct2 (-1, SZ_ARGS) != 0)
    abort ();
  return 0;
}
