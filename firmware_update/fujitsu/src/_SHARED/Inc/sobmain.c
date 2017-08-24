/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of some kernel drivers of Fujitsu Technology Solutions

   It is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
                                    S O B M A I N . C
     
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.30
    Implements: init_module
                cleanup_module

    Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/


#define __NO_VERSION__    /* don't define kernel_verion in module.h */
#include <linux/kernel.h> /* printk() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <asm/io.h>       /* ioremap() */
#include <linux/fs.h>

#include "drvstddef.h"         /* local definitions */
#include "sobdev.h"         /* Specific definitions for the current device */

#include "drsobglobal.h"
#include "sobmain.h"     

#ifndef HAVE_COMPAT_IOCTL
#	ifdef CONFIG_COMPAT
#		define OLD_COMPAT_WAY 1
#	endif
#endif


#ifdef OLD_COMPAT_WAY
#	include <linux/ioctl32.h>  /* register_ioctl32_conversion */
#endif

int major_number = 0;               /* major number of the char-device */

/*
 *  file operations
 */

struct file_operations sob_fops = 
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       17.08.2001
******************************************+*****************************************/
{
    open:           DrvOpen,
#ifdef HAVE_UNLOCKED_IOCTL
    unlocked_ioctl: DrvUnlockedDispatchIoctl,
#else
    ioctl:          DrvDispatchIoctl,
#endif
#ifdef HAVE_COMPAT_IOCTL
	compat_ioctl:	DrvCompatDispatchIoctl,
#endif
    release:        DrvRelease,
};

int init_module(void)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       17.08.2001
    Purpose:    
******************************************+*****************************************/
{
    int result;
    int err = 0;

    PDEBUG("[init_module] - entered\n");

    // Register device
    result = register_chrdev(major_number, DEV_NAME, &sob_fops);
    if (result < 0) 
    {
        PWARNING("[init_module] - can't get major %d\n",major_number);
        err = result;
        goto ExitError;
    }

    if (major_number == 0) 
        major_number = result; /* dynamic */

#ifdef OLD_COMPAT_WAY
    register_ioctl32_conversion(IOCTL_DRV_CONNECT, NULL);
    register_ioctl32_conversion(IOCTL_DRV_DISCONNECT, NULL);
    register_ioctl32_conversion(IOCTL_DRV_GET_STATUS, NULL);
    register_ioctl32_conversion(IOCTL_DRV_GETVERSION, NULL);
    register_ioctl32_conversion(IOCTL_DRV_READ_CMOS, NULL);
    register_ioctl32_conversion(IOCTL_DRV_WRITE_CMOS, NULL);
    register_ioctl32_conversion(IOCTL_DRV_GET_SYSINFO, NULL);
    register_ioctl32_conversion(IOCTL_DRV_TEST, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BAPI_CALL, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BAPI_ALLOC_PHBUF, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BAPI_FREE_PHBUF, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BAPI_SET_PHBUF, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BAPI_GET_PHBUF, NULL);
    register_ioctl32_conversion(IOCTL_DRV_BIOS_DATA_READ, NULL);
    register_ioctl32_conversion(IOCTL_DRV_PROC_INFO_GET, NULL);
#endif
    
    PDEBUG("[init_module] - Module '%s' loaded succesfully, major number is %d\n", DEV_NAME, major_number);
    err = 0; /* success */

ExitError:
    PDEBUG("[init_module] - leave\n");
    return err;
} // init_module

void cleanup_module(void)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       17.08.2001
	Purpose:    
******************************************+*****************************************/
{
    PDEBUG("[cleanup_module] - entered\n");
    unregister_chrdev(major_number, DEV_NAME);
    PDEBUG("[cleanup_module] - leave\n");
} // cleanup_module

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
long DrvCompatDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg)
/************************************************************************************
    Author:     Fujitsu Technology Solutions / TC
    Date:       15.02.2007
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

#ifdef _DEBUG_
void printBin(void *pIn, int iSize, int iLine, int iGroup)
/************************************************************************************
	Author:		Paul Trinkl, OPUS Trinkl + Trinkl GmbH
	Date:		19.04.2005
    Remarks:
FF FF FF FF FF FF FF FF  FF FF FF FF FF FF FF FF  ABCDEFGH IJKLMNOP
******************************************+*****************************************/
/**
   @arg Format string like printf
   @arg Variable number of parameters
*/
{
    int i, iOfs, iPos = 0, iPosAscii, iGroups, iTmp;
    char d[3], c;
    char buff[256];
    char *p = (char*)pIn;

    PDEBUG("[printBin] - entered\n");
    if(!pIn)
        return;

    iGroups     = iLine / iGroup;
    iPosAscii   = iLine * 3 + iGroups;

    memset(buff, '\0', 256);

    *(buff + iPosAscii + iLine + iGroups - 1) = '\n';
    *(buff + iPosAscii + iLine + iGroups + 0) = '\0';

    memset(buff, ' ', iPosAscii + iLine + iGroups - 1);

    for(i = 0; i < iSize; i++)
    {
        iPos = i % iLine;
        c = (char)(p[i] & 0xf);
        d[1] = (char)((c < (char)10)? c + '0' : c - 10 + 'A');
        c = (char)((p[i] & 0xf0) >> 4);
        d[0] = (char)((c < (char)10)? c + '0' : c - 10 + 'A');
        c = p[i];

        iOfs = (iPos / iGroup);
        {
            *(buff + iPos * 3 + iOfs) = d[0];
            *(buff + iPos * 3 + iOfs + 1) = d[1];
				
			iTmp = (int)p[i];
            if(iTmp < 32 || iTmp > 127)
                c = '.';

            *(buff + iPosAscii + iPos + iOfs) = c;
        }

        if(iPos == (iLine - 1))
        {
			PDEBUG("%s", buff);
        }
    }

    iPos = i % iLine;
    if(0 < iPos && iPos < (iLine - 1))
    {
        for(; iPos < iLine; iPos++)
        {
            iOfs = (iPos / iGroup);

            *(buff + iPos * 3 + iOfs + 0) = ' ';
            *(buff + iPos * 3 + iOfs + 1) = ' ';
            *(buff + iPosAscii + iPos + iOfs) = ' ';
        }
		PDEBUG("%s", buff);
    }

    PDEBUG("[printBin] - leave\n");
} // DbgOutBinary
#endif
