/* Simplified versions of infoleak-CVE-2011-1078-1.c.  */

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

/* Adapted from include/net/bluetooth/sco.h.  */

struct sco_conninfo {
	__u16 hci_handle;
	__u8  dev_class[3]; /* { dg-message "padding after field 'dev_class' is uninitialized \\(1 byte\\)" } */
};

/* Adapted from sco_sock_getsockopt_old in net/bluetooth/sco.c.  */

int test_1 (char __user *optval, const struct sco_conninfo *in)
{
	struct sco_conninfo cinfo; /* { dg-message "region created on stack here" "where" } */
				   /* { dg-message "capacity: 6 bytes" "capacity" { target *-*-* } .-1 } */
	/* Note: 40 bits of fields, padded to 48.  */

	cinfo.hci_handle = in->hci_handle;
	memcpy(cinfo.dev_class, in->dev_class, 3);

	copy_to_user(optval, &cinfo, sizeof(cinfo)); /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" "warning" } */
	/* { dg-message "1 byte is uninitialized" "how much note" { target *-*-* } .-1 } */
}

int test_2 (char __user *optval, const struct sco_conninfo *in)
{
	struct sco_conninfo cinfo;
	/* Note: 40 bits of fields, padded to 48.  */

	memset(&cinfo, 0, sizeof(cinfo));
	cinfo.hci_handle = in->hci_handle;
	memcpy(cinfo.dev_class, in->dev_class, 3);

	copy_to_user(optval, &cinfo, sizeof(cinfo)); /* { dg-bogus "" } */
}
