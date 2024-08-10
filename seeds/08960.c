/* Check that atomic not ops are generated.  */
/* { dg-do compile { target { atomic_model_soft_imask_available } } }  */
/* { dg-options "-O2 -matomic-model=soft-imask,strict -mno-usermode" }  */
/* { dg-final { scan-assembler-times "not\t" 12 } }  */

/* Check that atomic not ops are generated.  */

#define emitfuncs(name)\
  void test_ ## name ## _0 (char* mem)\
  {\
    name (mem, -1, __ATOMIC_ACQ_REL);\
  }\
  void test_ ## name ## _1 (short* mem)\
  {\
    name (mem, -1, __ATOMIC_ACQ_REL);\
  }\
  void test_ ## name ##_2 (int* mem)\
  {\
    name (mem, -1, __ATOMIC_ACQ_REL);\
  }\

emitfuncs (__atomic_xor_fetch)
emitfuncs (__atomic_fetch_xor)

emitfuncs (__atomic_nand_fetch)
emitfuncs (__atomic_fetch_nand)
