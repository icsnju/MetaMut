/* { dg-do compile } */
/* Verify that fence mappings match Table A.6's recommended mapping.  */
/* { dg-options "-O3" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** foo:
**	ret
*/
void foo()
{
  __atomic_thread_fence(__ATOMIC_RELAXED);
}
