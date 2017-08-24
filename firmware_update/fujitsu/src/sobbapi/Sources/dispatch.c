/* Copyright (C) 2006-2006 Fujitsu Technology Solutions / PT
   Thilo Cestonaro
   This file is part of sobbapi

   sobbapi is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobbapi is distributed in the hope that it will be useful, but WITHOUT
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
    Created:    Fujitsu Technology Solutions / PT / PT                            
    Purpose:    
    Version:    1.30
******************************************+*****************************************/
     
#define __NO_VERSION__    /* don't define kernel_verion in module.h */
#include <linux/module.h>
#include <linux/types.h>    /* __u32, .. */
//#include <linux/slab.h>		/* kmalloc() */
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/uaccess.h>    /* copy_to_user(), copy_from_user() */
#include <asm/io.h>

#include "drvstddef.h"       /* common types USHORT, ULONG, ... */
#include "sobdev.h"       
#include "drsobglobal.h"       /* common types USHORT, ULONG, ... */
//#include "system.h"  
#include "biosapi.h"  
#include "sobmain.h"     

int DrvVersionGet(PDRV_GETVERSION arg);
int DrvTest(PDRV_TEST arg);
int DrvBapiCall(PDRV_BAPI_CONTROL arg);

/*
 *  Global data
 */

BIOS_API_INFO   g_BApiInfo;
PUCHAR          g_pucBiosSegBase = NULL;

int DrvOpen (struct inode *inode, struct file *filp)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       17.08.2001
	Purpose:    
******************************************+*****************************************/
{
    int rv = -ENODEV;

    PDEBUG("[DrvOpen] - entered\n");
    if (g_pucBiosSegBase == NULL)
    /* 
        Map pointer to F-Segment
        Search for BAPI pointer
    */
    {
	g_pucBiosSegBase = ioremap(BIOS_START_ADDRESS, BIOS_MAP_LENGTH);
        if (!g_pucBiosSegBase)
        {
            PDEBUG("[DrvOpen] - ioremap did return an error\n");
        }
        memset(&g_BApiInfo, 0, sizeof(g_BApiInfo));
        if (BiosApiReadInfo(g_pucBiosSegBase, &g_BApiInfo) != DRV_READY)
        {
            PDEBUG("[DrvOpen] - BiosApiReadInfo returned an error\n");
        }
    }
    if (g_BApiInfo.
#if !defined(__LP64__)
                   usApi32Ofs)
#else
                   usApi64Ofs)
#endif
        rv = 0;

    return rv;
} // DrvOpen

int DrvRelease (struct inode * inode, struct file *filp)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Purpose:    
******************************************+*****************************************/
{
    PDEBUG("[DrvRelease] - entered\n");
    BiosApiCleanUp(&g_BApiInfo);
    PDEBUG("[DrvRelease] - leave\n");
    return 0;
} // DrvRelease

int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Purpose:    
******************************************+*****************************************/
{
	int ret;
	PDEBUG("[DrvDispatchIoctl] ----------------------------------------------------------------------------------------\n");
    switch(cmd)
    {

    case IOCTL_DRV_GETVERSION:   
    {
        PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_GETVERSION\n");
        LEAVEIT(DrvVersionGet((PDRV_GETVERSION) arg));
        break;
    }

    case IOCTL_DRV_TEST:         
    {
        PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_TEST\n");
        LEAVEIT(DrvTest((PDRV_TEST) arg));
        break;
    }

    case IOCTL_DRV_BAPI_CALL:
    {
        PDEBUG("[DrvDispatchIoctl] - cmd = IOCTL_DRV_BAPI_CALL\n");
		LEAVEIT(DrvBapiCall((PDRV_BAPI_CONTROL) arg));
        break;
    }

    default:
    {
        PDEBUG("[DrvDispatchIoctl] - cmd = %d - unknown\n", cmd);
        LEAVEIT(-EINVAL);
    }
    }

Leave:
	PDEBUG("[DrvDispatchIoctl] ----------------------------------------------------------------------------------------\n");
    return ret;
} // DrvDispatchIoctl

int DrvVersionGet(PDRV_GETVERSION ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Purpose:    
******************************************+*****************************************/
{
    ULONG ulVersion = DEV_VERSION;

    PDEBUG("[DrvVersionGet] - entered\n");

    if( put_user(ulVersion, &(ptr->Data.dwVersion)) )
        return -EFAULT;
    PDEBUG("[DrvVersionGet] - leave\n");
    return 0;
} // DrvVersionGet

int DrvTest(PDRV_TEST ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Purpose:    
******************************************+*****************************************/
{
    int err = -EFAULT;
    PDEBUG("[drv_test] - entered\n");

    return err;
} // DrvTest

int DrvBapiCall(PDRV_BAPI_CONTROL ptr)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       20.08.2001
	Purpose:    
******************************************+*****************************************/
{
    USHORT usStatus;
    ULLONG  in, out, ctlIn, ctlOut;                    /* virt. in user space   */
    ULONG  in_buf_length, out_buf_length, ctlIn_buf_length, ctlOut_buf_length;
    ULONG  use_map;

#ifdef _DEBUG_
	// printBin((void*)ptr, sizeof(DRV_BAPI_CONTROL), 16, 8);
#endif

	PDEBUG("[DrvBapiCall] - entered, ptr: 0x%p\n", ptr);

	usStatus = DRV_ERROR;
	in = out = ctlIn = ctlOut = 0;
	in_buf_length = out_buf_length = ctlIn_buf_length = ctlOut_buf_length = 0;

    /* get addresses of buffers in user space */
    if( copy_from_user(&in, &(ptr->Requ.pucDataIn), sizeof(ULLONG)) > 0 ||
        copy_from_user(&out, &(ptr->Requ.pucDataOut), sizeof(ULLONG)) > 0 ||
        copy_from_user(&ctlIn, &(ptr->Requ.pucCtrlIn), sizeof(ULLONG)) > 0 ||
        copy_from_user(&ctlOut, &(ptr->Requ.pucCtrlOut), sizeof(ULLONG)) > 0 )
    {
        PDEBUG("[DrvBapiCall] - could not get user buffer\n");
        return -EFAULT;
    }
	PDEBUG("[DrvBapiCall] - buffer pointer we got:\n");
	PDEBUG("[DrvBapiCall] - ctlIn: 0x%p, ctlOut: 0x%p\n", (ULLONG2PTR)ctlIn, (ULLONG2PTR)ctlOut);
	PDEBUG("[DrvBapiCall] - in: 0x%p, out: 0x%p,\n", (ULLONG2PTR)in, (ULLONG2PTR)out);

    /*
    *  get the length of buffers in user space,
    *  the first __u16 of each buffer indicates the length
    */
    if( get_user(in_buf_length, &(ptr->Requ.ulSzDataIn)) ||
        get_user(out_buf_length, &(ptr->Requ.ulSzDataOut)) ||
        get_user(ctlIn_buf_length, &(ptr->Requ.ulSzCtrlIn)) ||
        get_user(ctlOut_buf_length, &(ptr->Requ.ulSzCtrlOut)) ||
        get_user(use_map, &(ptr->Requ.ulUseMap)))
    {
        PDEBUG("[DrvBapiCall] - could not get user buffer sizes\n");
        return -EFAULT;
    }
	PDEBUG("[DrvBapiCall] - sizes we got: in: %d, out: %d, ctlIn: %d, ctlOut: %d, usemap: %d\n",
			in_buf_length, out_buf_length, ctlIn_buf_length, ctlOut_buf_length, use_map);

    /*
    *  control buffer may be randomly used by the BIOS services
    *  as a scratch buffer - just make sure it is big enough
    */

    if(ctlOut_buf_length < MIN_CTL_BUF_LENGTH || ctlIn_buf_length < MIN_CTL_BUF_LENGTH )
    {
        PDEBUG("[DrvBapiCall] - size of control buffer falls short of MIN_CTL_BUF_LENGTH\n");
        return -EINVAL;
    }

    if(g_pucBiosSegBase == NULL)
    /* 
        Map pointer to F-Segment
        Search for BAPI pointer
    */
    {
	g_pucBiosSegBase = ioremap(BIOS_START_ADDRESS, BIOS_MAP_LENGTH);
        if (!g_pucBiosSegBase)
        {
            PDEBUG("[DrvBapiCall] - ioremap did return an error\n");
            goto AllocFailed;
        }
        memset(&g_BApiInfo, 0, sizeof(g_BApiInfo));
        usStatus = BiosApiReadInfo(g_pucBiosSegBase, &g_BApiInfo);
        if(usStatus != DRV_READY)
        {
            PDEBUG("[DrvBapiCall] - BiosApiReadInfo returned an error\n");
            goto BapiInitFailed;
        }
    }

    /* allocate kernel memory for the buffers */
    usStatus = BiosApiControl(&g_BApiInfo, 
            (ULLONG2PTR)ctlIn, ctlIn_buf_length, 
            (ULLONG2PTR)ctlOut, ctlOut_buf_length,
            (ULLONG2PTR)in, in_buf_length,
            (ULLONG2PTR)out, out_buf_length,
            use_map);

    if( put_user(usStatus, &(ptr->Data.usStatus)) )
    {
        PDEBUG("[DrvBapiCall] - can't transfer status info to user buffer\n");
        return -EFAULT;
    }
    PDEBUG("[DrvBapiCall] - leave good\n");
    return 0;

BapiInitFailed:
	PDEBUG("[DrvBapiCall] - leave, BapiInitFailed\n");
    iounmap(g_pucBiosSegBase);
    g_pucBiosSegBase = NULL;
AllocFailed:
    if( put_user(usStatus, &(ptr->Data.usStatus)) )
        PDEBUG("[DrvBapiCall] - can't transfer status info to user buffer\n");
    return -EFAULT;
} // DrvBapiCall
