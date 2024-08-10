/* "An issue was discovered in drivers/scsi/aacraid/commctrl.c in the 
   Linux kernel before 4.13. There is potential exposure of kernel stack
   memory because aac_get_hba_info does not initialize the hbainfo structure."

   Fixed e.g. by 342ffc26693b528648bdc9377e51e4f2450b4860 on linux-4.13.y
   in linux-stable.

   This is a simplified version of that code (before and after the fix). */

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

struct aac_hba_info {

	u8	driver_name[50]; /* { dg-message "field 'driver_name' is uninitialized \\(50 bytes\\)" } */
	u8	adapter_number;
	u8	system_io_bus_number;
	u8	device_number; /* { dg-message "padding after field 'device_number' is uninitialized \\(3 bytes\\)" } */
	u32	function_number;
	u32	vendor_id;
	u32	device_id;
	u32	sub_vendor_id;
	u32	sub_system_id;
	u32	mapped_base_address_size; /* { dg-message "field 'mapped_base_address_size' is uninitialized \\(4 bytes\\)"  } */
	u32	base_physical_address_high_part;
	u32	base_physical_address_low_part;

	u32	max_command_size;
	u32	max_fib_size;
	u32	max_scatter_gather_from_os;
	u32	max_scatter_gather_to_fw;
	u32	max_outstanding_fibs;

	u32	queue_start_threshold;
	u32	queue_dump_threshold;
	u32	max_io_size_queued;
	u32	outstanding_io;

	u32	firmware_build_number;
	u32	bios_build_number;
	u32	driver_build_number;
	u32	serial_number_high_part;
	u32	serial_number_low_part;
	u32	supported_options;
	u32	feature_bits;
	u32	currentnumber_ports;

	u8	new_comm_interface:1; /* { dg-message "field 'new_comm_interface' is uninitialized \\(1 bit\\)" } */
	u8	new_commands_supported:1;
	u8	disable_passthrough:1;
	u8	expose_non_dasd:1;
	u8	queue_allowed:1;
	u8	bled_check_enabled:1;
	u8	reserved1:1;
	u8	reserted2:1;

	u32	reserved3[10]; /* { dg-message "field 'reserved3' is uninitialized \\(40 bytes\\)" } */

};

struct aac_dev
{
	/* [...snip...] */
	int			id;
	/* [...snip...] */
	struct pci_dev		*pdev;		/* Our PCI interface */
	/* [...snip...] */
};

/* Adapted from include/linux/pci.h  */

struct pci_dev {
	/* [...snip...] */
	struct pci_bus	*bus;		/* bus this device is on */
	/* [...snip...] */
	unsigned int	devfn;		/* encoded device & function index */
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;
	/* [...snip...] */
};

struct pci_bus {
	/* [...snip...] */
	unsigned char	number;		/* bus number */
	/* [...snip...] */
};

/* Adapted from drivers/scsi/aacraid/commctrl.c  */

static int aac_get_hba_info(struct aac_dev *dev, void __user *arg)
{
	struct aac_hba_info hbainfo; /* { dg-message "region created on stack here" "memspace message" } */
	/* { dg-message "capacity: 200 bytes" "capacity message" { target *-*-* } .-1 } */

	hbainfo.adapter_number		= (u8) dev->id;
	hbainfo.system_io_bus_number	= dev->pdev->bus->number;
	hbainfo.device_number		= (dev->pdev->devfn >> 3);
	hbainfo.function_number		= (dev->pdev->devfn & 0x0007);

	hbainfo.vendor_id		= dev->pdev->vendor;
	hbainfo.device_id		= dev->pdev->device;
	hbainfo.sub_vendor_id		= dev->pdev->subsystem_vendor;
	hbainfo.sub_system_id		= dev->pdev->subsystem_device;

	if (copy_to_user(arg, &hbainfo, sizeof(struct aac_hba_info))) { /* { dg-warning "potential exposure of sensitive information by copying uninitialized data from stack" "warning" } */
		/* { dg-message "177 bytes are uninitialized" "how much" { target *-*-* } .-1 } */
		/* [...snip...] */
	}

	return 0;
}

static int aac_get_hba_info_fixed(struct aac_dev *dev, void __user *arg)
{
	struct aac_hba_info hbainfo;

	memset(&hbainfo, 0, sizeof(hbainfo));
	hbainfo.adapter_number		= (u8) dev->id;
	hbainfo.system_io_bus_number	= dev->pdev->bus->number;
	hbainfo.device_number		= (dev->pdev->devfn >> 3);
	hbainfo.function_number		= (dev->pdev->devfn & 0x0007);

	hbainfo.vendor_id		= dev->pdev->vendor;
	hbainfo.device_id		= dev->pdev->device;
	hbainfo.sub_vendor_id		= dev->pdev->subsystem_vendor;
	hbainfo.sub_system_id		= dev->pdev->subsystem_device;

	if (copy_to_user(arg, &hbainfo, sizeof(struct aac_hba_info))) { /* { dg-bogus "" } */
		/* [...snip...] */
	}

	return 0;
}

/* An alternate fix using "= {0}" rather than memset.  */

static int aac_get_hba_info_fixed_alt(struct aac_dev *dev, void __user *arg)
{
	struct aac_hba_info hbainfo = {0};

	memset(&hbainfo, 0, sizeof(hbainfo));
	hbainfo.adapter_number		= (u8) dev->id;
	hbainfo.system_io_bus_number	= dev->pdev->bus->number;
	hbainfo.device_number		= (dev->pdev->devfn >> 3);
	hbainfo.function_number		= (dev->pdev->devfn & 0x0007);

	hbainfo.vendor_id		= dev->pdev->vendor;
	hbainfo.device_id		= dev->pdev->device;
	hbainfo.sub_vendor_id		= dev->pdev->subsystem_vendor;
	hbainfo.sub_system_id		= dev->pdev->subsystem_device;

	if (copy_to_user(arg, &hbainfo, sizeof(struct aac_hba_info))) { /* { dg-bogus "" } */
		/* [...snip...] */
	}

	return 0;
}
