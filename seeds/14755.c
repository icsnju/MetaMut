/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-options "-O3" } */
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void test_finite ();
void test_infinite ();

void do_exit (int i)
{
  exit (0);
}

int main(void)
{
  test_finite ();
  struct sigaction s;
  sigemptyset (&s.sa_mask);
  s.sa_handler = do_exit;
  s.sa_flags = 0;
  sigaction (SIGALRM, &s, NULL);
  alarm (1);

  test_infinite ();

  __builtin_abort ();
  return 1;
}


__attribute__ ((noinline))
unsigned foo(unsigned val, unsigned start)
{
  unsigned cnt = 0;
  for (unsigned i = start; i < val; i-=16)
    cnt++;
  return cnt;
}

void test_finite ()
{
  foo (UINT_MAX - 15, 32);
}

void test_infinite ()
{
  foo (UINT_MAX - 14, 32);
}
