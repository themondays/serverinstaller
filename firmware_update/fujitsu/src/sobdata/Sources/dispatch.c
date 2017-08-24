/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of sobdata

   sobdata is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobdata is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sobdata; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
                                  D I S P A T C H . H
 
    Project:    deskflash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    BIOS update utility
    Platform:   SuSE Linux 7.2
    Version:    1.30
    Implements: DrvOpen
				DrvRelease
				DrvDispatchIoctl
				SysSegAlloc
				SysSegRelease
				SysSegReAlloc
				DrvBiosDataRead
				DrvVersionGet
				DrvTest
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#define __NO_VERSION__    /* don't define kernel_verion in module.h */
#include <linux/module.h>
     
#include <linux/types.h>    /* __u32, .. */
#define _SUSE_LINUX_80_
#ifdef _SUSE_LINUX_80_
#   include <linux/slab.h>   /* kmalloc() */
#else
#   include <linux/malloc.h>   /* kmalloc() */
#endif // #ifdef _SUSE_LINUX_80_
#include <linux/uaccess.h>    /* copy_to_user(), copy_from_user() */
#include <asm/io.h>

#include "drvstddef.h"       /* common types USHORT, ULONG, ... */
#include "sobdev.h"       /* common types USHORT, ULONG, ... */

#include "drsobglobal.h"       /* common types USHORT, ULONG, ... */

int DrvBiosDataRead(PDRV_BIOS_DATA_READ ptr);
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

int DrvRelease (struct inode * inode, struct file *filp)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       09.08.2001
******************************************+*****************************************/
{
    PDEBUG("DrvRelease - entered\n");
    return 0;
} //DrvRelease

int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       09.08.2001
******************************************+*****************************************/
{
	int ret = 0;

	PDEBUG("DrvDispatchIoctl entered\n");

    switch(cmd)
    {
		case IOCTL_DRV_GETVERSION:   
			PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_GETVERSION\n");
			ret = DrvVersionGet((PDRV_GETVERSION) arg);
			PDEBUG("After DrvVersionGet\n");
			goto Leave;
			break;
		case IOCTL_DRV_BIOS_DATA_READ:   
			PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_BIOS_DATA_READ\n");
			LEAVEIT(DrvBiosDataRead((PDRV_BIOS_DATA_READ) arg));
			break;
		case IOCTL_DRV_TEST:         
			PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_TEST\n");
			LEAVEIT(DrvTest((PDRV_TEST) arg));
			break;
		default:
			PDEBUG("DrvDispatchIoctl cmd = %d - unknown\n", cmd);
			LEAVEIT(-EINVAL);
    } // switch(cmd)
Leave:
	PDEBUG("DrvDispatchIoctl leave\n");
    return ret;
} //DrvDispatchIoctl

BOOL SysSegAlloc(PUCHAR *BaseAddress, ULONG ulAddress, ULONG ulLength)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       10.08.2001
******************************************+*****************************************/
{
    PDEBUG("[SysSegAlloc] - entered, Address: %x, Length: %d\n", ulAddress, ulLength);
	*BaseAddress = (PUCHAR)ioremap(ulAddress, ulLength);
    if(!*BaseAddress)
    {
        *BaseAddress = NULL;
    	PDEBUG("[SysSegAlloc] - leave, FALSE\n");
        return FALSE;
    }
    PDEBUG("[SysSegAlloc] - leave, TRUE\n");
    return TRUE;
} // SysSegAlloc

void SysSegRelease(PUCHAR BaseAddress)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       10.08.2001
******************************************+*****************************************/
{
	if(BaseAddress)	
        iounmap(BaseAddress);
} // SysSegRelease

USHORT SysSegReAlloc(PUCHAR *BaseAddress, ULONG ulAddress, 
                     ULONG ulLengthOld, ULONG ulLengthNew)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       10.08.2001
******************************************+*****************************************/
{
    SysSegRelease(*BaseAddress);
    return SysSegAlloc(BaseAddress, ulAddress, ulLengthNew);
} // SysSegReAlloc

int DrvBiosDataRead(PDRV_BIOS_DATA_READ ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       10.08.2001
******************************************+*****************************************/
{
    USHORT  usStatus    = DRV_READY;
    PUCHAR  pucBiosData = NULL;
    ULLONG	pucData;        /* virt. in user space   */
    ULONG   ulAddress;      /* virt. in user space   */
    ULONG   ulSzData;       /* virt. in user space   */

    PDEBUG("[DrvBiosDataRead] - entered\n");

	pucData = 0;

    PDEBUG("[DrvBiosDataRead] - get Data from userland\n");
    /* get addresses of buffers in user space */
    if( copy_from_user(&pucData, &(ptr->Requ.pucData), sizeof(ULLONG)) > 0 ||
        get_user(ulAddress, &(ptr->Requ.ulAddress)) ||
        get_user(ulSzData, &(ptr->Requ.ulSzData)))
	    return -EFAULT;

	PDEBUG("Got from user: pucData: 0x%p; ulAddress: %x; SzData: %d\n", (ULLONG2PTR)pucData, ulAddress, ulSzData);

    PDEBUG("[DrvBiosDataRead] - SysSegAlloc\n");
    if(!SysSegAlloc(&pucBiosData, ulAddress, ulSzData))
        goto AllocFailed;

    PDEBUG("[DrvBiosDataRead] - copy_to_user\n");
    if(0 != copy_to_user((ULLONG2PTR)pucData, pucBiosData, ulSzData))
    {
        PDEBUG("[DrvBiosDataRead] - error on calling copy_to_user\n");
        usStatus = DRV_ERROR;
    }

    SysSegRelease(pucBiosData);

AllocFailed:
    PDEBUG("[DrvBiosDataRead] - leaving after put_user\n");
    if( put_user(usStatus, &(ptr->Data.usStatus)) )
        return -EFAULT;
    return (usStatus == DRV_READY)? 0 : -EFAULT;
} // DrvBiosDataRead

int DrvVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       09.08.2001
******************************************+*****************************************/
{
    ULONG ulVersion = DEV_VERSION;

    PDEBUG("drv_getversion - entered\n");

    if( put_user(ulVersion, &(ptr->Data.dwVersion)) )
        return -EFAULT;
	PDEBUG("drv_getversion - done\n");
    return 0;
} // DrvVersionGet

int DrvTest(PDRV_TEST ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       10.08.2001
******************************************+*****************************************/
{
    int err = -EFAULT;
    PDEBUG("drv_test - entered\n");

    return err;
} // DrvTest

