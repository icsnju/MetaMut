/* "The osf_getsysinfo function in arch/alpha/kernel/osf_sys.c in the
   Linux kernel before 2.6.39.4 on the Alpha platform does not properly
   restrict the data size for GSI_GET_HWRPB operations, which allows
   local users to obtain sensitive information from kernel memory via
   a crafted call."

   Fixed in 3d0475119d8722798db5e88f26493f6547a4bb5b on linux-2.6.39.y
   in linux-stable.  */

// TODO: remove need for this option:
/* { dg-additional-options "-fanalyzer-checker=taint" } */

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
/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern int copy_from_user(void *to, const void __user *from, long n)
  __attribute__((access (write_only, 1, 3),
		 access (read_only, 2, 3)));

extern long copy_to_user(void __user *to, const void *from, unsigned long n)
  __attribute__((access (write_only, 1, 3),
		 access (read_only, 2, 3)));

/* Adapted from include/linux/linkage.h.  */

#define asmlinkage

/* Adapted from include/linux/syscalls.h.  */

#define __SC_DECL1(t1, a1)	t1 a1
#define __SC_DECL2(t2, a2, ...) t2 a2, __SC_DECL1(__VA_ARGS__)
#define __SC_DECL3(t3, a3, ...) t3 a3, __SC_DECL2(__VA_ARGS__)
#define __SC_DECL4(t4, a4, ...) t4 a4, __SC_DECL3(__VA_ARGS__)
#define __SC_DECL5(t5, a5, ...) t5 a5, __SC_DECL4(__VA_ARGS__)
#define __SC_DECL6(t6, a6, ...) t6 a6, __SC_DECL5(__VA_ARGS__)

#define SYSCALL_DEFINEx(x, sname, ...)				\
	__SYSCALL_DEFINEx(x, sname, __VA_ARGS__)

#define SYSCALL_DEFINE(name) asmlinkage long sys_##name
#define __SYSCALL_DEFINEx(x, name, ...)					\
	asmlinkage __attribute__((tainted_args)) \
	long sys##name(__SC_DECL##x(__VA_ARGS__))

#define SYSCALL_DEFINE5(name, ...) SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)

/* Adapted from arch/alpha/include/asm/hwrpb.h.  */

struct hwrpb_struct {
	unsigned long phys_addr;	/* check: physical address of the hwrpb */
	unsigned long id;		/* check: "HWRPB\0\0\0" */
	unsigned long revision;
	unsigned long size;		/* size of hwrpb */
	/* [...snip...] */
};

extern struct hwrpb_struct *hwrpb;

/* Adapted from arch/alpha/kernel/osf_sys.c.  */

SYSCALL_DEFINE5(osf_getsysinfo, unsigned long, op, void __user *, buffer,
		unsigned long, nbytes, int __user *, start, void __user *, arg)
{
	/* [...snip...] */

	__analyzer_dump_state ("taint", nbytes);  /* { dg-warning "tainted" } */

	/* TODO: should have an event explaining why "nbytes" is treated as
	   attacker-controlled.  */

	/* case GSI_GET_HWRPB: */
		if (nbytes < sizeof(*hwrpb))
			return -1;

		__analyzer_dump_state ("taint", nbytes);  /* { dg-warning "has_lb" } */

		if (copy_to_user(buffer, hwrpb, nbytes) != 0) /* { dg-warning "use of attacker-controlled value 'nbytes' as size without upper-bounds checking" } */
			return -2;

		return 1;

	/* [...snip...] */
}

/* With the fix for the sense of the size comparison.  */

SYSCALL_DEFINE5(osf_getsysinfo_fixed, unsigned long, op, void __user *, buffer,
		unsigned long, nbytes, int __user *, start, void __user *, arg)
{
	/* [...snip...] */

	/* case GSI_GET_HWRPB: */
		if (nbytes > sizeof(*hwrpb))
			return -1;
		if (copy_to_user(buffer, hwrpb, nbytes) != 0) /* { dg-bogus "attacker-controlled" } */
			return -2;

		return 1;

	/* [...snip...] */
}
