/* Check that atomic ops utilize insns with immediate values.  */
/* { dg-do compile { target { atomic_model_hard_llcs_available } } }  */
/* { dg-options "-O2 -matomic-model=hard-llcs,strict" }  */
/* { dg-final { scan-assembler-times "add\t#1" 6 } }  */
/* { dg-final { scan-assembler-times "add\t#-1" 6 } }  */
/* { dg-final { scan-assembler-times "and\t#1" 12 } }  */
/* { dg-final { scan-assembler-times "\tor\t#1" 6 } }  */
/* { dg-final { scan-assembler-times "xor\t#1" 6 } }  */
/* { dg-final { scan-assembler-times "cmp/eq\t#1" 1 } }  */

/* Check that atomic ops utilize insns with immediate values.  */

#define emitfuncs(name)\
  void test_ ## name ## _0 (char* mem)\
  {\
    name (mem, 1, __ATOMIC_ACQ_REL);\
  }\
  void test_ ## name ## _1 (short* mem)\
  {\
    name (mem, 1, __ATOMIC_ACQ_REL);\
  }\
  void test_ ## name ##_2 (int* mem)\
  {\
    name (mem, 1, __ATOMIC_ACQ_REL);\
  }\

emitfuncs (__atomic_add_fetch)
emitfuncs (__atomic_fetch_add)

emitfuncs (__atomic_sub_fetch)
emitfuncs (__atomic_fetch_sub)

emitfuncs (__atomic_and_fetch)
emitfuncs (__atomic_fetch_and)

emitfuncs (__atomic_or_fetch)
emitfuncs (__atomic_fetch_or)

emitfuncs (__atomic_xor_fetch)
emitfuncs (__atomic_fetch_xor)

emitfuncs (__atomic_nand_fetch)
emitfuncs (__atomic_fetch_nand)

void
test___atomic_compare_exchange_0 (char* mem)
{
  char expected = 1;
  char desired = 5;
  __atomic_compare_exchange (mem, &expected, &desired, 0, __ATOMIC_ACQ_REL,
			     __ATOMIC_RELAXED);
}

void
test___atomic_compare_exchange_1 (short* mem)
{
  short expected = 1;
  short desired = 5;
  __atomic_compare_exchange (mem, &expected, &desired, 0, __ATOMIC_ACQ_REL,
			     __ATOMIC_RELAXED);
}

void
test___atomic_compare_exchange_2 (int* mem)
{
  int expected = 1;
  int desired = 5;
  __atomic_compare_exchange (mem, &expected, &desired, 0, __ATOMIC_ACQ_REL,
			     __ATOMIC_RELAXED);
}