/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-options "-O2 -mdejagnu-cpu=power8" } */
/* { dg-final { scan-assembler-not "\[ \t\]and "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]or "      } } */
/* { dg-final { scan-assembler-not "\[ \t\]xor "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]nor "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]eqv "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]andc "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]orc "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]nand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vandc "   } } */
/* { dg-final { scan-assembler-not "\[ \t\]vor "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]vxor "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vnor "    } } */
/* { dg-final { scan-assembler     "\[ \t\]xxland "  } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlor "   } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlxor "  } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlnor "  } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlandc " } } */
/* { dg-final { scan-assembler     "\[ \t\]xxleqv "  } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlorc "  } } */
/* { dg-final { scan-assembler     "\[ \t\]xxlnand " } } */

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
