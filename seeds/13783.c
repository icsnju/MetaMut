/* Ensure we get clean warning results when using the pthread
 * initialization macros.
 *
 * Origin: Kaveh Ghazi (ghazi@caip.rutgers.edu) 9/27/2006.
 */

/* { dg-do compile } */
/* { dg-require-effective-target pthread_h } */
/* { dg-options "-Wextra -Wall" } */
/* The definition of PTHREAD_MUTEX_INITIALIZER is missing an initializer for
   mutexAttr.mutexAttrType in kernel mode for various VxWorks versions.  */
/* { dg-xfail-if "missing initializer" { vxworks_kernel } } */

/* 
 * Common code for the pthread-init-*.c tests.
 *
 * Origin: Kaveh Ghazi (ghazi@caip.rutgers.edu) 9/27/2006.
 */

#define _GNU_SOURCE

#include <pthread.h>
#define UNUSED __attribute__ ((__unused__))

void foo(void)
{
#ifdef PTHREAD_MUTEX_INITIALIZER
  pthread_mutex_t pmutex UNUSED = PTHREAD_MUTEX_INITIALIZER;
#endif
#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
  pthread_mutex_t pmutex_rc UNUSED = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif
#ifdef PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP
  pthread_mutex_t pmutex_ec UNUSED = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
#endif
#ifdef PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP
  pthread_mutex_t pmutex_ad UNUSED = PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;
#endif
#ifdef PTHREAD_COND_INITIALIZER
  pthread_cond_t pcond UNUSED = PTHREAD_COND_INITIALIZER;
#endif
#ifdef PTHREAD_RWLOCK_INITIALIZER
  pthread_rwlock_t prwlock UNUSED = PTHREAD_RWLOCK_INITIALIZER;
#endif
#ifdef PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER_NP
  pthread_rwlock_t prwlock_nrc UNUSED
    = PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER_NP;
#endif
#ifdef PTHREAD_ONCE_INIT
  pthread_once_t ponce UNUSED = PTHREAD_ONCE_INIT;
#endif
}

