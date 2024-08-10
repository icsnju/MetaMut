/* "The sco_sock_getsockopt_old function in net/bluetooth/sco.c in the
   Linux kernel before 2.6.39 does not initialize a certain structure,
   which allows local users to obtain potentially sensitive information
   from kernel stack memory via the SCO_CONNINFO option."

   Fixed e.g. by c4c896e1471aec3b004a693c689f60be3b17ac86 on linux-2.6.39.y
   in linux-stable.  */

/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */
/* { dg-skip-if "structure layout assumption not met" { default_packed } } */

#include <string.h>

typedef unsigned char __u8;
typedef unsigned short __u16;

/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);

/* Adapted from include/asm-generic/uaccess.h.  */

#define get_user(x, ptr)					\
({								\
	/* [...snip...] */					\
	__get_user_fn(sizeof (*(ptr)), ptr, &(x));		\
	/* [...snip...] */					\
})

static inline int __get_user_fn(size_t size, const void __user *ptr, void *x)
{
	size = copy_from_user(x, ptr, size);
	return size ? -1 : size;
}

/* Adapted from include/linux/kernel.h.  */

#define min_t(type, x, y) ({			\
	type __min1 = (x);			\
	type __min2 = (y);			\
	__min1 < __min2 ? __min1: __min2; })

/* Adapted from include/linux/net.h.  */

struct socket {
	/* [...snip...] */
	struct sock		*sk;
	/* [...snip...] */
};

/* Adapted from include/net/bluetooth/sco.h.  */

struct sco_conninfo {
	__u16 hci_handle;
	__u8  dev_class[3]; /* { dg-message "padding after field 'dev_class' is uninitialized \\(1 byte\\)" } */
};

struct sco_conn {

	struct hci_conn	*hcon;
	/* [...snip...] */
};

#define sco_pi(sk) ((struct sco_pinfo *) sk)

struct sco_pinfo {
	/* [...snip...] */
	struct sco_conn	*conn;
};

/* Adapted from include/net/bluetooth/hci_core.h.  */

struct hci_conn {
	/* [...snip...] */
	__u16		handle;
	/* [...snip...] */
	__u8		dev_class[3];
	/* [...snip...] */
};

/* Adapted from sco_sock_getsockopt_old in net/bluetooth/sco.c.  */

static int sco_sock_getsockopt_old_broken(struct socket *sock, int optname, char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	/* [...snip...] */
	struct sco_conninfo cinfo; /* { dg-message "region created on stack here" "where" } */
				   /* { dg-message "capacity: 6 bytes" "capacity" { target *-*-* } .-1 } */
	/* Note: 40 bits of fields, padded to 48.  */

	int len, err = 0;

	/* [...snip...] */

	if (get_user(len, optlen))
		return -1;

	/* [...snip...] */

	/* case SCO_CONNINFO: */
		cinfo.hci_handle = sco_pi(sk)->conn->hcon->handle;
		memcpy(cinfo.dev_class, sco_pi(sk)->conn->hcon->dev_class, 3);

		len = min_t(unsigned int, len, sizeof(cinfo));
		if (copy_to_user(optval, (char *)&cinfo, len)) /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" "warning" { target *-*-* } } */
			/* { dg-message "1 byte is uninitialized" "how much note" { target *-*-* } .-1 } */
			err = -1;

	/* [...snip...] */
}

static int sco_sock_getsockopt_fixed(struct socket *sock, int optname, char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	/* [...snip...] */
	struct sco_conninfo cinfo;
	/* Note: 40 bits of fields, padded to 48.  */

	int len, err = 0;

	/* [...snip...] */

	if (get_user(len, optlen))
		return -1;

	/* [...snip...] */

	/* case SCO_CONNINFO: */
		/* Infoleak fixed by this memset call.  */
		memset(&cinfo, 0, sizeof(cinfo));
		cinfo.hci_handle = sco_pi(sk)->conn->hcon->handle;
		memcpy(cinfo.dev_class, sco_pi(sk)->conn->hcon->dev_class, 3);

		len = min_t(unsigned int, len, sizeof(cinfo));
		if (copy_to_user(optval, (char *)&cinfo, len)) /* { dg-bogus "exposure" } */
			err = -1;

	/* [...snip...] */
}
