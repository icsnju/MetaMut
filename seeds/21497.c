/* PR sanitizer/65400 */
/* { dg-shouldfail "tsan" } */
/* { dg-additional-options "-fno-omit-frame-pointer -ldl" } */
/* { dg-additional-sources pr65400-2.c } */

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
int v __attribute__((aligned(8)));
int q __attribute__((aligned(8)));
int o __attribute__((aligned(8)));
extern void baz4 (int *);

__attribute__((noinline, noclone)) int
bar (int x)
{
  q += x;
  return x;
}

void
foo (int *x)
{
  if (__builtin_expect (x == 0, 1))
    return;
  bar (bar (bar (bar (*x))));
}

__attribute__((noinline, noclone)) void
baz1 (int *x)
{
  foo (x);
}

__attribute__((noinline, noclone)) void
baz2 (int **x)
{
  foo (*x);
}

__attribute__((noinline, noclone)) void
baz3 (void)
{
  barrier_wait (&barrier);
  v++;
}

__attribute__((noinline, noclone)) void
baz5 (void)
{
  int i;
  o = 1;
  baz1 (&o);
  int *p = &o;
  baz2 (&p);
  for (i = 0; i < 128; i++)
    baz4 (&o);
  if (q != 130 * 4)
    __builtin_abort ();
  baz3 ();
}

__attribute__((noinline, noclone)) void *
tf (void *arg)
{
  (void) arg;
  baz5 ();
  return NULL;
}

int
main ()
{
  pthread_t th;
  barrier_init (&barrier, 2);
  if (pthread_create (&th, NULL, tf, NULL))
    return 0;
  v++;
  barrier_wait (&barrier);
  pthread_join (th, NULL);
  return 0;
}

/* { dg-output "WARNING: ThreadSanitizer: data race.*#2 _?tf" } */
