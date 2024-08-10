/* Reproducing this ICE is dependent on line numbering, hence the blank
   lines below.  */
#undef  ASM_GENERATE_INTERNAL_LABEL
#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM)		\
  do								\
    {								\
      __builtin_sprintf (LABEL, "*.%s%u", PREFIX, (unsigned) (NUM));	\
    }								\
  while (0)





void
dbxout_stab_value_internal_label (const char *stem, int counter)
{
  char label[100];
  ASM_GENERATE_INTERNAL_LABEL (label, stem, counter);
}
