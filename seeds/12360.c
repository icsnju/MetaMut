/* "The yam_ioctl function in drivers/net/hamradio/yam.c in the Linux kernel
   before 3.12.8 does not initialize a certain structure member, which allows
   local users to obtain sensitive information from kernel memory by
   leveraging the CAP_NET_ADMIN capability for an SIOCYAMGCFG ioctl call."

   Fixed e.g. by e7834c71c2cacc621ddc64bd71f83ef2054f6539 on linux-3.12.y
   in linux-stable.  */

#include <string.h>

/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);

/* Adapted from include/linux/yam.h  */

struct yamcfg {
	unsigned int mask;		/* Mask of commands */
	unsigned int iobase;	/* IO Base of COM port */
	unsigned int irq;		/* IRQ of COM port */
	unsigned int bitrate;	/* Bit rate of radio port */
	unsigned int baudrate;	/* Baud rate of the RS232 port */
	unsigned int txdelay;	/* TxDelay */
	unsigned int txtail;	/* TxTail */
	unsigned int persist;	/* Persistence */
	unsigned int slottime;	/* Slottime */
	unsigned int mode;		/* mode 0 (simp), 1(Dupl), 2(Dupl+delay) */
	unsigned int holddly;	/* PTT delay in FullDuplex 2 mode */
};

struct yamdrv_ioctl_cfg {
	int cmd; /* { dg-message "field 'cmd' is uninitialized \\(4 bytes\\)" } */
	struct yamcfg cfg;
};

/* Adapted from include/asm-generic/errno-base.h  */

#define	EFAULT		14	/* Bad address */

/* Adapted from drivers/net/hamradio/yam.c  */

struct yam_port {
	/* [...snip...] */

	int bitrate;
	int baudrate;
	int iobase;
	int irq;
	int dupmode;

	/* [...snip...] */

	int txd;				/* tx delay */
	int holdd;				/* duplex ptt delay */
	int txtail;				/* txtail delay */
	int slot;				/* slottime */
	int pers;				/* persistence */

	/* [...snip...] */
};

/* Broken version, leaving yi.cmd uninitialized.  */

static int yam_ioctl(/* [...snip...] */
		     void __user *dst, struct yam_port *yp)
{
	struct yamdrv_ioctl_cfg yi; /* { dg-message "region created on stack here" "memspace event" } */
	/* { dg-message "capacity: 48 bytes" "capacity event" { target *-*-* } .-1 } */

	/* [...snip...] */

	/* case SIOCYAMGCFG: */
		yi.cfg.mask = 0xffffffff;
		yi.cfg.iobase = yp->iobase;
		yi.cfg.irq = yp->irq;
		yi.cfg.bitrate = yp->bitrate;
		yi.cfg.baudrate = yp->baudrate;
		yi.cfg.mode = yp->dupmode;
		yi.cfg.txdelay = yp->txd;
		yi.cfg.holddly = yp->holdd;
		yi.cfg.txtail = yp->txtail;
		yi.cfg.persist = yp->pers;
		yi.cfg.slottime = yp->slot;
		if (copy_to_user(dst, &yi, sizeof(struct yamdrv_ioctl_cfg))) /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" "warning" } */
			/* { dg-message "4 bytes are uninitialized" "how much note" { target *-*-* } .-1 } */
			 return -EFAULT;
	/* [...snip...] */

	return 0;
}

/* Fixed version, with a memset.  */

static int yam_ioctl_fixed(/* [...snip...] */
			   void __user *dst, struct yam_port *yp)
{
	struct yamdrv_ioctl_cfg yi;

	/* [...snip...] */

	/* case SIOCYAMGCFG: */
		memset(&yi, 0, sizeof(yi));
		yi.cfg.mask = 0xffffffff;
		yi.cfg.iobase = yp->iobase;
		yi.cfg.irq = yp->irq;
		yi.cfg.bitrate = yp->bitrate;
		yi.cfg.baudrate = yp->baudrate;
		yi.cfg.mode = yp->dupmode;
		yi.cfg.txdelay = yp->txd;
		yi.cfg.holddly = yp->holdd;
		yi.cfg.txtail = yp->txtail;
		yi.cfg.persist = yp->pers;
		yi.cfg.slottime = yp->slot;
		if (copy_to_user(dst, &yi, sizeof(struct yamdrv_ioctl_cfg))) /* { dg-bogus "" } */
			 return -EFAULT;
	/* [...snip...] */

	return 0;
}
