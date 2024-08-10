/* { dg-do compile } */
/* { dg-options "-std=c2x -Wc11-c2x-compat" } */
/* { dg-add-options float64x } */
/* { dg-require-effective-target float64x } */

_Float64x a		/* { dg-warning "ISO C does not support the '_Float64x' type before C2X" } */
  = 1.0F64x;		/* { dg-warning "non-standard suffix on floating constant before C2X" } */
__extension__ _Float64x b
  = 2.0F64x;
