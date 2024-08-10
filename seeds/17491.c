/* PR target/93078 */
/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -mavx512f -mprefer-vector-width=512 -masm=att" } */
/* { dg-final { scan-assembler "vrndscaleps\[ \t]\+\\\$12,\[^\n\r]*%z" } } */
/* { dg-final { scan-assembler "vrndscaleps\[ \t]\+\\\$4,\[^\n\r]*%z" } } */
/* { dg-final { scan-assembler "vrndscalepd\[ \t]\+\\\$12,\[^\n\r]*%z" } } */
/* { dg-final { scan-assembler "vrndscalepd\[ \t]\+\\\$4,\[^\n\r]*%z" } } */

/* PR target/93078 */
/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -msse4.1 -mno-sse4.2 -masm=att" } */
/* { dg-final { scan-assembler "roundps\[ \t]\+\\\$12," } } */
/* { dg-final { scan-assembler "roundps\[ \t]\+\\\$4," } } */
/* { dg-final { scan-assembler "roundpd\[ \t]\+\\\$12," } } */
/* { dg-final { scan-assembler "roundpd\[ \t]\+\\\$4," } } */

float a[16], b[16];
double c[8], d[8];

void
foo (void)
{
  int i;
  for (i = 0; i < 16; ++i)
    b[i] = __builtin_nearbyintf (a[i]);
}

void
bar (void)
{
  int i;
  for (i = 0; i < 16; ++i)
    b[i] = __builtin_rintf (a[i]);
}

void
baz (void)
{
  int i;
  for (i = 0; i < 8; ++i)
    d[i] = __builtin_nearbyint (c[i]);
}

void
qux (void)
{
  int i;
  for (i = 0; i < 8; ++i)
    d[i] = __builtin_rint (c[i]);
}
