/* PR tree-optimization/88444 */
/* { dg-do compile } */
/* { dg-options "-O1 -finline-functions -finline-small-functions -fdump-tree-fixup_cfg3" } */
/* { dg-final { scan-tree-dump-not " = \\(long int\\) 0;" "fixup_cfg3" } } */

/* PR tree-optimization/88444 */
/* { dg-do compile } */
/* { dg-options "-O1 -ftree-vrp -fno-tree-ccp -fno-tree-forwprop -fno-tree-fre" } */

int v;

int
foo (int, int);

static inline int
bar (long int x)
{
  return !!x ? x : 1;
}

static inline void
baz (int x)
{
  v += foo (0, 0) + bar (x);
}

void
qux (void)
{
  int a = 0;
  v = v || foo (0, 0);
  v = v || foo (0, 0);
  v = v || foo (0, 0);
  baz (a);
}
