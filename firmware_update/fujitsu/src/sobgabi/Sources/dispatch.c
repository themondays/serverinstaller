/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   
   This file is part of sobgabi

   sobgabi is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobgabi is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sobbapi; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
				   D I S P A T C H . H

	Project:    DeskFlash for Linux
	Created:    Fujitsu Technology Solutions
	Version:    1.50
******************************************+*****************************************/

#define __NO_VERSION__			 /* don't define kernel_verion in module.h */
#include <linux/module.h>
#include <linux/types.h>		 /* __u32, .. */
#include <linux/slab.h>			 /* kmalloc() */
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include <linux/version.h>          /* because we need conditional compiling */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)

/* This include was removed in Kernel. 3.4 */
#include <asm/system.h>

#endif

#include <linux/uaccess.h>		 /* copy_to_user(), copy_from_user() */
#include <asm/io.h>
#include <linux/cred.h>		 /* need for current macro */

#include "drvstddef.h"			 /* common types USHORT, ULONG, ... */
#include "sobdev.h"
#include "drsobglobal.h"			 /* common types USHORT, ULONG, ... */
#include "gabimemory.h"
#include "sobgabi.h"

#ifndef HAVE_COMPAT_IOCTL
#   ifdef CONFIG_COMPAT
#       define OLD_COMPAT_WAY 1
#   endif
#endif

#ifdef OLD_COMPAT_WAY
#   include <linux/ioctl32.h>    /* register_ioctl32_conversion */
#endif

/* forward prototypes */
static int DrvOpen (struct inode *inode, struct file *filp);
static int DrvRelease (struct inode * inode, struct file *filp);
static int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg);
#ifdef HAVE_UNLOCKED_IOCTL
long DrvUnlockedDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg);
#endif
#ifdef HAVE_COMPAT_IOCTL
static long DrvCompatDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg);
#endif
/*
 *  file operations
 */
struct file_operations sob_fops =
{
        open:       DrvOpen,
#ifdef HAVE_UNLOCKED_IOCTL
    unlocked_ioctl: DrvUnlockedDispatchIoctl,
#else
    ioctl:          DrvDispatchIoctl,
#endif
#ifdef HAVE_COMPAT_IOCTL
        compat_ioctl:   DrvCompatDispatchIoctl,
#endif
        release:    DrvRelease,
};

extern ULONG ulGabiVersion;

static BOOL CheckRoot(void)
/************************************************************************************
        Author:        Fujitsu Technology Solutions
******************************************+*****************************************/
{
        // root groop is allowed to invoke all calls
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
        if (current->egid == 0)
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 5, 0) /* GLOBAL_ROOT_GID was introduced in 3.5.0 */
      if (current_egid() == 0)
#else
	  if (gid_eq(current_egid(),GLOBAL_ROOT_GID))
#endif /* 3.5.0 test */
#endif /* 2.6.29 test */
               return TRUE;

        return FALSE;
}

static int DrvOpen (struct inode *inode, struct file *filp)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	PDEBUG("[DrvOpen] - entered\n");
	return (g_pEP) ? 0 : -ENODEV;
}								 // DrvOpen


static int DrvRelease (struct inode * inode, struct file *filp)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	PDEBUG("[DrvRelease] - entered\n");
	PDEBUG("[DrvRelease] - leave\n");
	return 0;
}								 // DrvRelease

static int DrvVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	ULONG ulVersion = DEV_VERSION;

	PDEBUG("[DrvVersionGet] - entered\n");

	if( put_user(ulVersion, &(ptr->Data.dwVersion)) )
		return -EFAULT;
	PDEBUG("[DrvVersionGet] - leave\n");
	return 0;
}								 // DrvVersionGet

static int DrvGabiVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	PDEBUG("[DrvGabiVersionGet] - entered\n");

	if( put_user(ulGabiVersion, &(ptr->Data.dwVersion)) )
		return -EFAULT;
	PDEBUG("[DrvGabiVersionGet] - leave\n");
	return 0;
}								 // DrvVersionGet


static int DrvTest(PDRV_TEST ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	int err = -EFAULT;
	PDEBUG("[drv_test] - entered\n");

	return err;
}								 // DrvTest


static int DrvGabiExecute(DeviceIoctlParameter_T __user *ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	int rv;
	DeviceIoctlParameter_T para; 
	__u32 BytesCount = 0;

	if (!g_pEP) return -EFAULT;	// famous last words: this cannot occur

	if (copy_from_user(&para, ptr, sizeof(para))) return -EFAULT;
	
#ifdef __LP64__
#define CAST_TO_VOID_USER_PTR (void __user*)
#else
#define CAST_TO_VOID_USER_PTR (void __user*)(unsigned)
#endif
        
	rv = GabiExecute(CAST_TO_VOID_USER_PTR para.pIn, para.InSize, CAST_TO_VOID_USER_PTR para.pOut, para.OutSize, &BytesCount);

	if (!rv && para.pdwBytesCount)
		if (copy_to_user(CAST_TO_VOID_USER_PTR para.pdwBytesCount, &BytesCount, sizeof(__u32)))
			return -EFAULT;
	return rv;			
}

static int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	int ret = -EINVAL;
	PDEBUG("[DrvDispatchIoctl] ----------------------------------------------------------------------------------------\n");
	switch(cmd)
	{

		case IOCTL_DRV_GETVERSION:
		{
			PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_GETVERSION\n");
			ret = DrvVersionGet((PDRV_GETVERSION) arg);
			break;
		}

		case IOCTL_GABI_GET_GABI_VERSION:
		{
			PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_GABI_GET_GABI_VERSION\n");
			ret = DrvGabiVersionGet((PDRV_GETVERSION) arg);
			break;
		}
		case IOCTL_DRV_TEST:
		{
			PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_TEST\n");
			ret = DrvTest((PDRV_TEST) arg);
			break;
		}

		case IOCTL_DRV_GABI_CALL:
		{
			PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_GABI_CALL\n");
			if (!g_pEP || !CheckRoot())
			{
				ret = -EACCES;				/* not a gabi system (should not occur) or no rights */
			}
			else
			{
				ret = DrvGabiExecute((DeviceIoctlParameter_T __user *) arg);
			}
			break;
		}

		default:
		{
			PDEBUG("[DrvDispatchIoctl] - cmd = %d - unknown\n", cmd);
			ret = -EINVAL;
			break;
		}
	}

	PDEBUG("[DrvDispatchIoctl] ----------------------------------------------------------------------------------------\n");
	return ret;
}								 // DrvDispatchIoctl

#ifdef HAVE_UNLOCKED_IOCTL
long DrvUnlockedDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	long ret;
	PDEBUG("[DrvUnlockedDispatchIoctl] - entered\n");
	ret = (long)DrvDispatchIoctl(NULL, filp, cmd, arg);
	PDEBUG("[DrvUnlockedDispatchIoctl] - returning: %ld\n", ret);
	return ret;
}
#endif

#ifdef HAVE_COMPAT_IOCTL
static long DrvCompatDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
        Author:     Fujitsu Technology Solutions
******************************************+*****************************************/
/*
        This function forwards all ioctl calls from a 32Bit component to this modules dispatchioctl function
        when it is 64bit kernel module.
*/
{
        long ret;
        PDEBUG("[DrvCompatDispatchIoctl] - entered\n");
        ret = (long)DrvDispatchIoctl(NULL, filp, cmd, arg);
        PDEBUG("[DrvCompatDispatchIoctl] - returning: %ld\n", ret);
        return ret;
}
#endif
