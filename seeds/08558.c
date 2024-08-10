/* Reduced from qemu-7.2.0's hw/intc/omap_intc.c */

#ifndef ANALYZER_DECLS_H
#define ANALYZER_DECLS_H

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

/* Function decls with special meaning to the analyzer.
   None of these are actually implemented.  */

/* Trigger a breakpoint in the analyzer when reached.  */
extern void __analyzer_break (void);

/* Emit a warning describing the 2nd argument (which can be of any
   type), at the given verbosity level.  This is for use when
   debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_describe (int verbosity, ...);

/* Dump copious information about the analyzer’s state when reached.  */
extern void __analyzer_dump (void);

/* Emit a warning describing the size of the base region of (*ptr).  */
extern void __analyzer_dump_capacity (const void *ptr);

/* Dump information about what decls have escaped at this point on the path.  */
extern void __analyzer_dump_escaped (void);

/* Dump information after analysis on all of the exploded nodes at this
   program point.

   __analyzer_dump_exploded_nodes (0);
   will dump just the number of nodes, and their IDs.

   __analyzer_dump_exploded_nodes (1);
   will also dump all of the states within those nodes.  */
extern void __analyzer_dump_exploded_nodes (int);

/* Emit a warning describing what is known about the value of NAME.  */
extern void __analyzer_dump_named_constant (const char *name);

/* Emit a placeholder "note" diagnostic with a path to this call site,
   if the analyzer finds a feasible path to it.  */
extern void __analyzer_dump_path (void);

/* Dump the region_model's state to stderr.  */
extern void __analyzer_dump_region_model (void);

/* Emit a warning describing the state of the 2nd argument
   (which can be of any type) with respect to NAME.
   This is for use when debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_dump_state (const char *name, ...);

/* Emit a warning with text "TRUE", FALSE" or "UNKNOWN" based on the
   truthfulness of the argument.  */
extern void __analyzer_eval (int);

/* Obtain an "unknown" void *.  */
extern void *__analyzer_get_unknown_ptr (void);

/* Complain if PTR doesn't point to a null-terminated string.
   TODO: eventually get the strlen of the buffer (without the
   optimizer touching it).  */
extern __SIZE_TYPE__ __analyzer_get_strlen (const char *ptr);

#endif /* #ifndef ANALYZER_DECLS_H.  */

typedef unsigned char __uint8_t;
typedef unsigned int __uint32_t;
typedef unsigned long int __uint64_t;
typedef __uint8_t uint8_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
typedef uint64_t hwaddr;
typedef struct omap_intr_handler_s omap_intr_handler;

struct omap_intr_handler_bank_s
{
  uint32_t irqs;
  uint32_t inputs;
  uint32_t mask;
  uint32_t fiq;
  uint32_t sens_edge;
  uint32_t swi;
  unsigned char priority[32];
};

struct omap_intr_handler_s
{
  /* [...snip...] */
  unsigned char nbanks;
  /* [...snip...] */
  int sir_intr[2];
  int autoidle;
  uint32_t mask;
  struct omap_intr_handler_bank_s bank[3];
};

uint64_t
omap2_inth_read(struct omap_intr_handler_s* s, int offset)
{
  int bank_no, line_no;
  struct omap_intr_handler_bank_s* bank = NULL;

  if ((offset & 0xf80) == 0x80) {
    bank_no = (offset & 0x60) >> 5;
    if (bank_no < s->nbanks) {
      offset &= ~0x60;
      bank = &s->bank[bank_no];
    } else {
      return 0;
    }
  }

  switch (offset) {
    case 0x10:
      return (s->autoidle >> 2) & 1;

    case 0x14:
      return 1;

    case 0x40:
      return s->sir_intr[0];

    case 0x44:
      return s->sir_intr[1];

    case 0x48:
      return (!s->mask) << 2;

    case 0x4c:
      return 0;

    case 0x50:
      return s->autoidle & 3;

    case 0x80:
      return bank->inputs; /* { dg-bogus "dereference of NULL 'bank'" "PR analyzer/108806" } */

    case 0x84:
      return bank->mask; /* { dg-bogus "dereference of NULL 'bank'" "PR analyzer/108806" } */

    case 0x88:
    case 0x8c:
      return 0;

    case 0x90:
      return bank->swi; /* { dg-bogus "dereference of NULL 'bank'" "PR analyzer/108806" } */

    case 0x94:
      return 0;

    case 0x98:
      return bank->irqs & ~bank->mask & ~bank->fiq; /* { dg-bogus "dereference of NULL 'bank'" "PR analyzer/108806" } */

    case 0x9c:
      return bank->irqs & ~bank->mask & bank->fiq; /* { dg-bogus "dereference of NULL 'bank'" "PR analyzer/108806" } */

    case 0x100 ... 0x300:
      bank_no = (offset - 0x100) >> 7;
      if (bank_no > s->nbanks)
        break;
      bank = &s->bank[bank_no];
      line_no = (offset & 0x7f) >> 2;
      return (bank->priority[line_no] << 2) | ((bank->fiq >> line_no) & 1);
  }
  return 0;
}
