/* { dg-do compile } */
/* { dg-options "-march=rv32gc_zbb -mabi=ilp32 -fno-lto -O2" } */
/* { dg-skip-if "" { *-*-* } { "-g" } } */
/* { dg-final { check-function-bodies "**" "" } } */
/* { dg-final { scan-assembler-not "and" } } */

/*
**foo1:
**	rori	a0,a0,16
**	ret
*/
unsigned int foo1(unsigned int rs1)
{
    return (rs1 << 16) | (rs1 >> 16);
}
