/* { dg-do compile } */
/* Verify that store mappings match the Ztso suggested mapping.  */
/* { dg-options "-march=rv64id_ztso -mabi=lp64d -O3" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** foo:
**	lw\ta[0-9]+,0\(a1\)
**	sw\ta[0-9]+,0\(a0\)
**	fence\trw,rw
**	ret
*/
void foo (int* bar, int* baz)
{
  __atomic_store(bar, baz, __ATOMIC_SEQ_CST);
}
