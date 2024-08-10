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

void *Thread(void *x) {
  pthread_mutex_lock((pthread_mutex_t*)x);
  pthread_mutex_unlock((pthread_mutex_t*)x);
  barrier_wait(&barrier);
  return 0;
}

int main() {
  barrier_init(&barrier, 2);
  pthread_mutex_t Mtx;
  pthread_mutex_init(&Mtx, 0);
  pthread_t t;
  pthread_create(&t, 0, Thread, &Mtx);
  barrier_wait(&barrier);
  pthread_mutex_destroy(&Mtx);
  pthread_join(t, 0);
  return 0;
}

/* { dg-output "WARNING: ThreadSanitizer: data race.*(\n|\r\n|\r)" } */
