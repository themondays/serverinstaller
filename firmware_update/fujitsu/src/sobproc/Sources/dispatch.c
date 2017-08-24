/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of sobproc

   sobproc is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobproc is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sobproc; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
                                  D I S P A T C H . H
 
    Project:    deskflash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    BIOS update utility
    Version:    1.30
    Implements: DrvOpen
				DrvRelease
				DrvDispatchIoctl
				DrvProcInfoGet
				DrvVersionGet
				DrvTest
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#define __NO_VERSION__    /* don't define kernel_verion in module.h */
#include <linux/module.h>
// CT 15.02.2006 definition of printk
#include <linux/kernel.h>     

#include <linux/types.h>    /* __u32, .. */
#define _SUSE_LINUX_80_
#ifdef _SUSE_LINUX_80_
#   include <linux/slab.h>   /* kmalloc() */
#else
#   include <linux/malloc.h>   /* kmalloc() */
#endif // #ifdef _SUSE_LINUX_80_
#include <linux/uaccess.h>    /* copy_to_user(), copy_from_user() */
#include <asm/io.h>
#include <asm/processor.h> /* identify_cpu */

#include "drvstddef.h"       /* common types USHORT, ULONG, ... */
#include "sobdev.h"       /* common types USHORT, ULONG, ... */

#include "drsobglobal.h"       /* common types USHORT, ULONG, ... */

// CT 15.02.06 for debugging
#ifdef DEBUG
#	ifndef PDEBUG
#		define PDEBUG(fmt, args...) printk(KERN_DEBUG "FSC: " fmt, ## args)
#	endif
#else
#	ifndef PDEBUG
#		define PDEBUG(fmt, args...)
#	endif
#endif

int DrvProcInfoGet(PDRV_PROC_INFO_GET ptr);
int DrvVersionGet(PDRV_GETVERSION arg);
int DrvTest(PDRV_TEST arg);

/*
 *  Global data
 */

int DrvOpen (struct inode *inode, struct file *filp)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    PDEBUG("DrvOpen - entered\n");
    return 0;
} // DrvOpen

int DrvRelease (struct inode *inode, struct file *filp)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    PDEBUG("DrvRelease - entered\n");
    return 0;
} //DrvRelease

int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    switch(cmd)
    {
    case IOCTL_DRV_GETVERSION:   
    {
        PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_GETVERSION\n");
        return DrvVersionGet((PDRV_GETVERSION) arg);
        break;
    }

    case IOCTL_DRV_PROC_INFO_GET:   
    {
        PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_PROC_INFO_GET\n");
        return DrvProcInfoGet((PDRV_PROC_INFO_GET) arg);
        break;
    }

    case IOCTL_DRV_TEST:         
    {
        PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_TEST\n");
        return DrvTest((PDRV_TEST) arg);
        break;
    }

    default:
    {
        PDEBUG("DrvDispatchIoctl cmd = %d - unknown\n", cmd);
        return -EINVAL;
    }
    } // switch(cmd)
    return 0;
} //DrvDispatchIoctl

int DrvProcInfoGet(PDRV_PROC_INFO_GET ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    USHORT  usStatus = DRV_READY;
    char    szFamily[13];				// "Unknown", "GenuineIntel", "AuthenticAMD", ...
    ULONG   ulCpuId;					// CPU-id of the running CPU
    ULONG   ulPlatformId = 0;				// Platform-ID of the running CPU
    int     ret = 1;
    u32     msr_lo, msr_hi;

#ifdef current_cpu_data
	struct cpuinfo_x86 *cpu = &current_cpu_data;
#else
#ifdef __this_cpu_ptr
// KERNEL >= 3.0.1
	struct cpuinfo_x86 *cpu = __this_cpu_ptr(&cpu_info);
#else
// KERNEL >= 3.15 renamed __this_cpu_ptr to raw_cpu_ptr
        struct cpuinfo_x86 *cpu = raw_cpu_ptr(&cpu_info);
#endif
#endif

	PDEBUG("[DrvProcInfoGet] - entered\n");
	if(cpu->cpuid_level > 0) {
		ulCpuId = cpuid_eax(1);

		// PlatformId exists on Intel CPU only
#ifndef MSR_IA32_PLATFORM_ID
#define MSR_IA32_PLATFORM_ID 0x17
#endif
		if (!memcmp(cpu->x86_vendor_id, "GenuineIntel", 12)) {
			rdmsr(MSR_IA32_PLATFORM_ID, msr_lo, msr_hi);
			ulPlatformId = ((msr_hi >> 18) & 7) + 1;
		}
		strcpy(szFamily, cpu->x86_vendor_id);
	}
	else {
		szFamily[0] = '\0';
		ulPlatformId = 0;
		ulCpuId = 0;
	}
	
	PDEBUG("[DrvProcInfoGet] - CPU Family: %i, Vendor: (String) %s, (Number) %d\n", ulCpuId, szFamily, ulPlatformId);
	put_user(usStatus, &(ptr->Data.usStatus));
	put_user(ulCpuId, &(ptr->Data.ulCpuId));
	put_user(ulPlatformId, &(ptr->Data.ulPlatformId));
	if(!copy_to_user((char*)ptr->Data.szFamily, szFamily, sizeof(ptr->Data.szFamily))) {
	    ret = 0;   // OK
	}

	PDEBUG("[DrvProcInfoGet] - leave, ret = %s\n", ret != 0 ? "negativ" : "positive");
	return ret;
} // DrvProcInfoGet

int DrvVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    ULONG ulVersion = DEV_VERSION;

    PDEBUG("DrvVersionGet - entered\n");

    if( put_user(ulVersion, &(ptr->Data.dwVersion)) )
        return -EFAULT;
    return 0;
} // DrvVersionGet

int DrvTest(PDRV_TEST ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    int err = -EFAULT;
    PDEBUG("DrvTest - entered\n");

    return err;
} // DrvTest

