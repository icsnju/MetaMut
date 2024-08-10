/* "An issue was discovered in drivers/scsi/aacraid/commctrl.c in the
   Linux kernel before 4.13. There is potential exposure of kernel stack
   memory because aac_send_raw_srb does not initialize the reply structure."

   Fixed e.g. by 342ffc26693b528648bdc9377e51e4f2450b4860 on linux-4.13.y 
   in linux-stable.

   This is a very simplified version of that code (before and after the fix). */

/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */
/* { dg-skip-if "structure layout assumption not met" { default_packed } } */

#include <string.h>

typedef unsigned int __u32;
typedef unsigned int u32;
typedef unsigned char u8;

/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);

/* Adapted from include/uapi/linux/types.h  */

#define __bitwise
typedef __u32 __bitwise __le32;

/* Adapted from drivers/scsi/aacraid/aacraid.h  */

#define		AAC_SENSE_BUFFERSIZE	 30

struct aac_srb_reply
{
	__le32		status;
	__le32		srb_status;
	__le32		scsi_status;
	__le32		data_xfer_length;
	__le32		sense_data_size;
	u8		sense_data[AAC_SENSE_BUFFERSIZE]; /* { dg-message "padding after field 'sense_data' is uninitialized \\(2 bytes\\)" } */
};

#define		ST_OK		0
#define SRB_STATUS_SUCCESS                  0x01

/* Adapted from drivers/scsi/aacraid/commctrl.c  */

static int aac_send_raw_srb(/* [...snip...] */
			    void __user *user_reply)
{
	u32 byte_count = 0;

	/* [...snip...] */

	struct aac_srb_reply reply; /* { dg-message "region created on stack here" "memspace message" } */
	/* { dg-message "capacity: 52 bytes" "capacity message" { target *-*-* } .-1 } */

	reply.status = ST_OK;
		
	/* [...snip...] */

	reply.srb_status = SRB_STATUS_SUCCESS;
	reply.scsi_status = 0;
	reply.data_xfer_length = byte_count;
	reply.sense_data_size = 0;
	memset(reply.sense_data, 0, AAC_SENSE_BUFFERSIZE);

	/* [...snip...] */

	if (copy_to_user(user_reply, &reply, /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" } */
					     /* { dg-message "2 bytes are uninitialized" "note how much" { target *-*-* } .-1 } */
			 sizeof(struct aac_srb_reply))) {
		/* [...snip...] */
	}
	/* [...snip...] */
}

static int aac_send_raw_srb_fixed(/* [...snip...] */
				  void __user *user_reply)
{
	u32 byte_count = 0;

	/* [...snip...] */

	struct aac_srb_reply reply;

	/* This is the fix.  */
	memset(&reply, 0, sizeof(reply));

	reply.status = ST_OK;
		
	/* [...snip...] */

	reply.srb_status = SRB_STATUS_SUCCESS;
	reply.scsi_status = 0;
	reply.data_xfer_length = byte_count;
	reply.sense_data_size = 0;
	memset(reply.sense_data, 0, AAC_SENSE_BUFFERSIZE);

	/* [...snip...] */

	if (copy_to_user(user_reply, &reply, /* { dg-bogus "" } */
			 sizeof(struct aac_srb_reply))) {
		/* [...snip...] */
	}
	/* [...snip...] */
}
