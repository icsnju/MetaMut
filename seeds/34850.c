/* Check that the appropriate atomic insns are used if the result values
   are unused.  */
/* { dg-do compile { target { atomic_model_soft_tcb_available } } }  */
/* { dg-options "-dp -O2 -matomic-model=soft-tcb,gbr-offset=0,strict" }  */
/* { dg-final { scan-assembler-times "atomic_add" 12 } }  */
/* { dg-final { scan-assembler-times "atomic_and" 6 } }  */
/* { dg-final { scan-assembler-times "atomic_or" 6 } }  */
/* { dg-final { scan-assembler-times "atomic_xor" 6 } }  */
/* { dg-final { scan-assembler-times "atomic_nand" 6 } }  */
/* { dg-final { scan-assembler-times "atomic_not" 12 } }  */
/* { dg-final { scan-assembler-not "fetch" } }  */

/* Check that the appropriate atomic insns are used if the result values
   are unused.  */

#define concat_1(x, y) x ## y
#define concat(x, y) concat_1 (x, y)
#define makefuncname(name) concat (concat (test_, __LINE__), name)

#define emitfuncs(name,val)\
  void makefuncname (_0) (char* mem)\
  {\
    name (mem, val, __ATOMIC_ACQ_REL);\
  }\
  void makefuncname (_1) (short* mem)\
  {\
    name (mem, val, __ATOMIC_ACQ_REL);\
  }\
  void makefuncname (_2) (int* mem)\
  {\
    name (mem, val, __ATOMIC_ACQ_REL);\
  }\

emitfuncs (__atomic_add_fetch, 1)
emitfuncs (__atomic_fetch_add, 1)

emitfuncs (__atomic_sub_fetch, 1)
emitfuncs (__atomic_fetch_sub, 1)

emitfuncs (__atomic_and_fetch, 1)
emitfuncs (__atomic_fetch_and, 1)

emitfuncs (__atomic_or_fetch, 1)
emitfuncs (__atomic_fetch_or, 1)

emitfuncs (__atomic_xor_fetch, 1)
emitfuncs (__atomic_fetch_xor, 1)

emitfuncs (__atomic_nand_fetch, 1)
emitfuncs (__atomic_fetch_nand, 1)

emitfuncs (__atomic_xor_fetch, -1)
emitfuncs (__atomic_fetch_xor, -1)

emitfuncs (__atomic_nand_fetch, -1)
emitfuncs (__atomic_fetch_nand, -1)
