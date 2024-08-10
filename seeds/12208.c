/* Test for cross x86_64<->w64 abi standard calls via variable.
*/
/* Origin: Kai Tietz <kai.tietz@onevision.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99 -ffast-math" } */
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

typedef int (CALLABI_CROSS *func)(void *, char *, char *, short, long long);

int CALLABI_CROSS
callback(void * ptr, char *string1, char *string2, short number, long long rand)
{
    return (rand != 0x1234567890abcdefLL);
}

int main()
{
    volatile func callme = callback;
    if(callme(0, 0, 0, 0, 0x1234567890abcdefLL))
     abort();
    return 0;
}
