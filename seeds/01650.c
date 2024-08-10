/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-require-effective-target powerpc_altivec_ok } */
/* { dg-options "-O2 -mdejagnu-cpu=power5 -mabi=altivec -mno-altivec -mno-vsx" } */
/* { dg-final { scan-assembler	   "\[ \t\]and "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]or "      } } */
/* { dg-final { scan-assembler	   "\[ \t\]xor "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]nor "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]andc "    } } */
/* { dg-final { scan-assembler	   "\[ \t\]eqv "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]orc "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]nand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vandc "   } } */
/* { dg-final { scan-assembler-not "\[ \t\]vor "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]vxor "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vnor "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxland "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlor "   } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlxor "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlnor "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlandc " } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxleqv "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlorc "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlnand " } } */

#ifndef TYPE
typedef int v4si __attribute__ ((vector_size (16)));
#define TYPE v4si
#endif

/* Test various logical operations.  */

TYPE arg1 (TYPE p, TYPE q) { return p & q; }		/* AND  */
TYPE arg2 (TYPE p, TYPE q) { return p | q; }		/* OR   */
TYPE arg3 (TYPE p, TYPE q) { return p ^ q; }		/* XOR  */
TYPE arg4 (TYPE p)	   { return ~ p; }		/* NOR  */
TYPE arg5 (TYPE p, TYPE q) { return ~(p & q); }		/* NAND */
TYPE arg6 (TYPE p, TYPE q) { return ~(p | q); }		/* NOR  */
TYPE arg7 (TYPE p, TYPE q) { return ~(p ^ q); }		/* EQV  */
TYPE arg8 (TYPE p, TYPE q) { return (~p) & q; }		/* ANDC */
TYPE arg9 (TYPE p, TYPE q) { return (~p) | q; }		/* ORC  */
TYPE arg10(TYPE p, TYPE q) { return (~p) ^ q; }		/* EQV  */
TYPE arg11(TYPE p, TYPE q) { return p & (~q); }		/* ANDC */
TYPE arg12(TYPE p, TYPE q) { return p | (~q); }		/* ORC  */
TYPE arg13(TYPE p, TYPE q) { return p ^ (~q); }		/* EQV  */

void ptr1 (TYPE *p) { p[0] = p[1] & p[2]; }		/* AND  */
void ptr2 (TYPE *p) { p[0] = p[1] | p[2]; }		/* OR   */
void ptr3 (TYPE *p) { p[0] = p[1] ^ p[2]; }		/* XOR  */
void ptr4 (TYPE *p) { p[0] = ~p[1]; }			/* NOR  */
void ptr5 (TYPE *p) { p[0] = ~(p[1] & p[2]); }		/* NAND */
void ptr6 (TYPE *p) { p[0] = ~(p[1] | p[2]); }		/* NOR  */
void ptr7 (TYPE *p) { p[0] = ~(p[1] ^ p[2]); }		/* EQV  */
void ptr8 (TYPE *p) { p[0] = ~(p[1]) & p[2]; }		/* ANDC */
void ptr9 (TYPE *p) { p[0] = (~p[1]) | p[2]; }		/* ORC  */
void ptr10(TYPE *p) { p[0] = (~p[1]) ^ p[2]; }		/* EQV  */
void ptr11(TYPE *p) { p[0] = p[1] & (~p[2]); }		/* ANDC */
void ptr12(TYPE *p) { p[0] = p[1] | (~p[2]); }		/* ORC  */
void ptr13(TYPE *p) { p[0] = p[1] ^ (~p[2]); }		/* EQV  */
