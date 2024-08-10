/* See notes in this header.  */
/* Shared header for the various taint-CVE-2020-13143.h tests.
   
   "gadget_dev_desc_UDC_store in drivers/usb/gadget/configfs.c in the
   Linux kernel 3.16 through 5.6.13 relies on kstrdup without considering
   the possibility of an internal '\0' value, which allows attackers to
   trigger an out-of-bounds read, aka CID-15753588bcd4."

   Fixed by 15753588bcd4bbffae1cca33c8ced5722477fe1f on linux-5.7.y
   in linux-stable.  */

// TODO: remove need for this option
/* { dg-additional-options "-fanalyzer-checker=taint" } */

#include <stddef.h>

/* Adapted from include/uapi/asm-generic/posix_types.h  */

typedef unsigned int     __kernel_size_t;
typedef int              __kernel_ssize_t;

/* Adapted from include/linux/types.h  */

//typedef __kernel_size_t		size_t;
typedef __kernel_ssize_t	ssize_t;

/* Adapted from include/linux/kernel.h  */

#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	/* [...snip...] */						\
	((type *)(__mptr - offsetof(type, member))); })

/* Adapted from include/linux/configfs.h  */

struct config_item {
	/* [...snip...] */
};

struct config_group {
	struct config_item		cg_item;
	/* [...snip...] */
};

static inline struct config_group *to_config_group(struct config_item *item)
{
	return item ? container_of(item,struct config_group,cg_item) : NULL;
}

#define CONFIGFS_ATTR(_pfx, _name)				\
static struct configfs_attribute _pfx##attr_##_name = {	\
	/* [...snip...] */				\
	.store		= _pfx##_name##_store,		\
}

/* Adapted from include/linux/compiler.h  */

#define __force

/* Adapted from include/asm-generic/errno-base.h  */

#define	ENOMEM		12	/* Out of memory */

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

/* Adapted from include/linux/compiler_attributes.h  */

#define __malloc                        __attribute__((__malloc__))

/* Adapted from include/linux/string.h  */

extern char *kstrdup(const char *s, gfp_t gfp) __malloc;

/* Adapted from drivers/usb/gadget/configfs.c  */

struct gadget_info {
	struct config_group group;
	/* [...snip...] */				\
};

// TODO: remove need for this option
/* { dg-additional-options "-fanalyzer-checker=taint" } */

struct configfs_attribute {
	/* [...snip...] */
	ssize_t (*store)(struct config_item *, const char *, size_t) /* { dg-message "\\(1\\) field 'store' of 'struct configfs_attribute' is marked with '__attribute__\\(\\(tainted_args\\)\\)'" } */
		__attribute__((tainted_args)); /* (this is added).  */
};
static inline struct gadget_info *to_gadget_info(struct config_item *item)
{
	 return container_of(to_config_group(item), struct gadget_info, group);
}

static ssize_t gadget_dev_desc_UDC_store(struct config_item *item,
		const char *page, size_t len)
{
	struct gadget_info *gi = to_gadget_info(item);
	char *name;
	int ret;

#if 0
	/* FIXME: this is the fix.  */
	if (strlen(page) < len)
		return -EOVERFLOW;
#endif

	name = kstrdup(page, GFP_KERNEL);
	if (!name)
		return -ENOMEM;
	if (name[len - 1] == '\n') /* { dg-warning "use of attacker-controlled value 'len \[^\n\r\]+' as offset without upper-bounds checking" } */
		name[len - 1] = '\0'; /* { dg-warning "use of attacker-controlled value 'len \[^\n\r\]+' as offset without upper-bounds checking" } */
	/* [...snip...] */				\
}

CONFIGFS_ATTR(gadget_dev_desc_, UDC); /* { dg-message "\\(2\\) function 'gadget_dev_desc_UDC_store' used as initializer for field 'store' marked with '__attribute__\\(\\(tainted_args\\)\\)'" } */
