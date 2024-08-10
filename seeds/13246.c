/* { dg-do compile } */
// TODO: remove need for -fanalyzer-checker=taint here:
// TODO: remove need for --param=analyzer-max-svalue-depth=25 here:
/* { dg-options "-fanalyzer -fanalyzer-checker=taint --param=analyzer-max-svalue-depth=25" } */
/* { dg-options "-fanalyzer -fanalyzer-checker=taint" } */
/* { dg-require-effective-target analyzer } */

/* See notes in this header.  */
/* Shared header for the various taint-CVE-2011-0521-*.c tests.
   These are a series of successively simpler reductions of the reproducer.
   Ideally the analyzer would detect the issue in all of the testcases,
   but currently requires some simplification of the code to do so.

   "The dvb_ca_ioctl function in drivers/media/dvb/ttpci/av7110_ca.c in the
   Linux kernel before 2.6.38-rc2 does not check the sign of a certain integer
   field, which allows local users to cause a denial of service (memory
   corruption) or possibly have unspecified other impact via a negative value."

   Adapted from Linux 2.6.38, which is under the GPLv2.

   Fixed in e.g. cb26a24ee9706473f31d34cc259f4dcf45cd0644 on linux-2.6.38.y  */

#include <string.h>
/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);
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

typedef unsigned int u32;

/* Adapted from include/linux/compiler.h  */

#define __force

/* Adapted from include/asm-generic/errno-base.h  */

#define	ENOMEM		12	/* Out of memory */
#define	EFAULT		14	/* Bad address */
#define	ENODEV		19	/* No such device */
#define	EINVAL		22	/* Invalid argument */

/* Adapted from include/linux/errno.h  */

#define ENOIOCTLCMD	515	/* No ioctl command */

/* Adapted from include/linux/fs.h  */

struct file {
	/* [...snip...] */
	void			*private_data;
	/* [...snip...] */
};

/* Adapted from drivers/media/dvb/dvb-core/dvbdev.h  */

struct dvb_device {
	/* [...snip...] */
	int (*kernel_ioctl)(struct file *file, unsigned int cmd, void *arg);

	void *priv;
};


/* Adapted from include/linux/dvb/ca.h  */

typedef struct ca_slot_info {
	int num;               /* slot number */

	int type;              /* CA interface this slot supports */
#define CA_CI            1     /* CI high level interface */
#define CA_CI_LINK       2     /* CI link layer level interface */
	/* [...snip...] */
} ca_slot_info_t;


/* Adapted from drivers/media/dvb/ttpci/av7110.h  */

struct av7110 {
	/* [...snip...] */
	ca_slot_info_t		ci_slot[2];
	/* [...snip...] */
	u32		    arm_app;
	/* [...snip...] */
};

/* Adapted from drivers/media/dvb/ttpci/av7110_hw.h  */

#define FW_CI_LL_SUPPORT(arm_app) ((arm_app) & 0x80000000)

/* Adapted from include/asm-generic/ioctl.h  */

#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8

#define _IOC_SIZEBITS	14
#define _IOC_DIRBITS	2

#define _IOC_SIZEMASK	((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK	((1 << _IOC_DIRBITS)-1)
#define _IOC_NRSHIFT	0
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC_NONE	0U
#define _IOC_WRITE	1U
#define _IOC_READ	2U

#define _IOC_DIR(nr)		(((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_SIZE(nr)		(((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

/* Adapted from include/linux/mutex.h  */

struct mutex {
	/* [...snip...] */
};

#define __MUTEX_INITIALIZER(lockname) \
		{ /* [...snip...] */ }

#define DEFINE_MUTEX(mutexname) \
	struct mutex mutexname = __MUTEX_INITIALIZER(mutexname)

extern void mutex_lock(struct mutex *lock);
extern void mutex_unlock(struct mutex *lock);

/* Adapted from include/linux/types.h  */

#define __bitwise__
typedef unsigned __bitwise__ gfp_t;

/* Adapted from include/linux/gfp.h  */

#define ___GFP_WAIT		0x10u
#define ___GFP_IO		0x40u
#define ___GFP_FS		0x80u
#define __GFP_WAIT	((__force gfp_t)___GFP_WAIT)
#define __GFP_IO	((__force gfp_t)___GFP_IO)
#define __GFP_FS	((__force gfp_t)___GFP_FS)
#define GFP_KERNEL  (__GFP_WAIT | __GFP_IO | __GFP_FS)

/* Adapted from include/linux/slab.h  */

void kfree(const void *);
void *kmalloc(size_t size, gfp_t flags)
  __attribute__((malloc (kfree)));

/* Adapted from dvb_ca_ioctl in drivers/media/dvb/ttpci/av7110_ca.c and
   dvb_usercopy in drivers/media/dvb/dvb-core/dvbdev.c

   Further simplified from -3; merge into a single function; drop the mutex,
   remove control flow.  */

int test_1(struct file *file, unsigned int cmd, unsigned long arg)
{
	char    sbuf[128];
	void    *parg = sbuf;

	if (copy_from_user(parg, (void __user *)arg, sizeof(sbuf)))
	  return -1;

	{
		struct dvb_device *dvbdev = file->private_data;
		struct av7110 *av7110 = dvbdev->priv;
		unsigned long arg = (unsigned long) parg;

		/* case CA_GET_SLOT_INFO:  */
		ca_slot_info_t *info=(ca_slot_info_t *)parg;

		if (info->num > 1)
			return -EINVAL;
		av7110->ci_slot[info->num].num = info->num; /* { dg-warning "attacker-controlled value" "" { xfail *-*-* } } */
		// TODO(xfail)
		av7110->ci_slot[info->num].type = FW_CI_LL_SUPPORT(av7110->arm_app) ?
							CA_CI_LINK : CA_CI;
		memcpy(info, &av7110->ci_slot[info->num], sizeof(ca_slot_info_t));
	}

	copy_to_user((void __user *)arg, parg, sizeof(sbuf));

	return 0;
}
