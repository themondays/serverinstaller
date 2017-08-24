/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of sobcmos

   sobcmos is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobcmos is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sobcmos; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
                                  D I S P A T C H . C
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.30
    Implements: DrvOpen
	            DrvRelease
				DrvDispatchIoctl
				DrvCmosRead
				DrvCmosWrite
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
     
#include "drvstddef.h"       /* common types USHORT, ULONG, ... */
#include "sobdev.h"       /* common types USHORT, ULONG, ... */

#include "drsobglobal.h"       /* common types USHORT, ULONG, ... */
#include "system.h"    

int DrvCmosRead(PDRV_READ_CMOS ptr);
int DrvCmosWrite(PDRV_WRITE_CMOS ptr);
int DrvVersionGet(PDRV_GETVERSION arg);
int DrvTest(PDRV_TEST arg);

/*
 *  Global data
 */

int DrvOpen (struct inode *inode, struct file *filp)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
	Changes:    
******************************************+*****************************************/
{
    PDEBUG("DrvOpen - entered\n");
    return 0;
} // DrvOpen

int DrvRelease (struct inode * inode, struct file *filp)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
	Changes:    
******************************************+*****************************************/
{
    PDEBUG("DrvRelease - entered\n");
    return 0;
} // DrvRelease

int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Changes:    
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

    case IOCTL_DRV_READ_CMOS:
    {
        PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_READ_CMOS\n");
        return DrvCmosRead((PDRV_READ_CMOS) arg);
        break;
    }

    case IOCTL_DRV_WRITE_CMOS:
    {
        PDEBUG("DrvDispatchIoctl cmd = IOCTL_DRV_WRITE_CMOS\n");
        return DrvCmosWrite((PDRV_WRITE_CMOS) arg);
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
} // DrvDispatchIoctl

int DrvCmosRead(PDRV_READ_CMOS ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Changes:    
******************************************+*****************************************/
{
    int err = -EFAULT;
    USHORT  usStatus;
    ULONG   ulOffset, ulRead;
    ULLONG   pucData;                    /* virt. in user space   */
    UCHAR   ucBuffer[MAX_READ_CMOS_DATA];

    PDEBUG("DrvCmosRead - entered\n");

    if( get_user(ulOffset, &(ptr->Requ.ulOffset)) ||
        get_user(ulRead, &(ptr->Requ.ulRead)) ||
        copy_from_user(&pucData, &(ptr->Requ.pucData), sizeof(ULLONG)) > 0 )
    {
        PDEBUG("DrvCmosRead = error on calling get_user\n");
        return -EFAULT;
    }

    usStatus = SysCmosRead(ucBuffer, ulOffset, ulRead);
    if(usStatus == DRV_READY)
    {
#if defined(__LP64__)
        if(0 == copy_to_user((PUCHAR)pucData, ucBuffer, _min(ulRead, (ULONG)sizeof(ucBuffer))))
#else
        if(0 == copy_to_user((PUCHAR)(ULONG)pucData, ucBuffer, _min(ulRead, (ULONG)sizeof(ucBuffer))))
#endif
            err = 0;
        else
        {
            PDEBUG("DrvCmosRead = error on calling copy_to_user\n");
            return DRV_ERROR;
        }
        
        if( put_user(ulRead, &(ptr->Data.ulRead)) )
            err = -EFAULT;
    }

    if( put_user(usStatus, &(ptr->Data.usStatus)) )
        err = -EFAULT;

    return err;
} // DrvCmosRead

int DrvCmosWrite(PDRV_WRITE_CMOS ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Changes:    
******************************************+*****************************************/
{
    int err = -EFAULT;
    USHORT  usStatus;
    ULONG   ulOffset, ulWrite;
    ULLONG   pucData;                    /* virt. in user space   */
    UCHAR   ucBuffer[MAX_WRITE_CMOS_DATA];

    PDEBUG("DrvCmosWrite - entered\n");

    if( get_user(ulOffset, &(ptr->Requ.ulOffset)) ||
        get_user(ulWrite, &(ptr->Requ.ulWrite)) ||
        copy_from_user(&pucData, &(ptr->Requ.pucData), sizeof(ULLONG)) > 0 )
    {
        PDEBUG("DrvCmosWrite = error on calling get_user\n");
        return -EFAULT;
    }

#if defined(__LP64__)
    if(0 != copy_from_user(ucBuffer, (PUCHAR)pucData, ulWrite))
#else
    if(0 != copy_from_user(ucBuffer, (PUCHAR)(ULONG)pucData, ulWrite))
#endif
    {
        PDEBUG("DrvCmosWrite = error on calling copy_from_user\n");
        return -EFAULT;
    }

    usStatus = SysCmosWrite(ucBuffer, ulOffset, ulWrite);
    if(usStatus == DRV_READY)
    {
        err = 0;
        if( put_user(ulWrite, &(ptr->Data.ulWritten)) )
            err = -EFAULT;
    }

    if( put_user(usStatus, &(ptr->Data.usStatus)) )
        err = -EFAULT;

    return err;
} // DrvCmosWrite

int DrvVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Changes:    
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
    Date:       20.08.2001
	Changes:    
******************************************+*****************************************/
{
    int err = -EFAULT;
    PDEBUG("DrvTest - entered\n");

    return err;
} // DrvTest

