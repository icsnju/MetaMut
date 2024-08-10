/* { dg-shouldfail "tsan" } */
/* { dg-additional-options "-ldl" } */

#include <pthread.h>
/* TSAN-invisible barriers.  Link with -ldl.  */
#include <pthread.h>
#include <dlfcn.h>

static __typeof(pthread_barrier_wait) *barrier_wait;

static
void barrier_init (pthread_barrier_t *barrier, unsigned count)
{
  void *h = dlopen ("libpthread.so.0", RTLD_LAZY);
  barrier_wait = (__typeof (pthread_barrier_wait) *)
	 	 dlsym (h, "pthread_barrier_wait");
  pthread_barrier_init (barrier, NULL, count);
}

static pthread_barrier_t barrier;
int Global;

void *Thread1(void *x) {
  barrier_wait(&barrier);
  __atomic_fetch_add(&Global, 1, __ATOMIC_RELAXED);
  return NULL;
}

void *Thread2(void *x) {
  Global++;
  barrier_wait(&barrier);
  return NULL;
}

int main() {
  barrier_init(&barrier, 2);
  pthread_t t[2];
  pthread_create(&t[0], NULL, Thread1, NULL);
  pthread_create(&t[1], NULL, Thread2, NULL);
  pthread_join(t[0], NULL);
  pthread_join(t[1], NULL);
  return 0;
}

/* { dg-output "WARNING: ThreadSanitizer: data race.*(\n|\r\n|\r)" } */
/* { dg-output "  Atomic write of size 4.*" } */
/* { dg-output "    #0 Thread1.*" } */
