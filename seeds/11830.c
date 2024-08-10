/* { dg-do run } */
/* { dg-options "-O -fdump-tree-phiopt" } */

#include <stdint.h>

__attribute__ ((noinline, noipa))
uint8_t three_minmax3 (uint8_t xc, uint8_t xm, uint8_t xy) {
        uint8_t  xk;
    xc=~xc;
    xm=~xm;
    xy=~xy;
    if (xc > xm) {
        xk = (uint8_t) (xy < xc ? xc : xy);
    } else {
        xk = (uint8_t) (xm < xy ? xm : xy);
    }
    return xk;
}

int
main (void)
{
  volatile uint8_t xy = 255;
  volatile uint8_t xm = 0;
  volatile uint8_t xc = 127;
  if (three_minmax3 (xc, xm, xy) != 0)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "MIN_EXPR" 1 "phiopt1" } } */
/* { dg-final { scan-tree-dump-times "MAX_EXPR" 1 "phiopt1" } } */
