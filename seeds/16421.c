/* { dg-do compile } */
/* { dg-options "-march=armv8-a+nolse -O2" } */

int v;

void
sync_lock_release (void)
{
  __sync_lock_release (&v);
}

/* { dg-final { scan-assembler-times "stlr" 1 } } */
