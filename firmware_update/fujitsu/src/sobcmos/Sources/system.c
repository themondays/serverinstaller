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
                                  S Y S T E M . C
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.50
    Purpose: 
    Implements: SYST_CmosRead
                SYST_CmosWrite
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/

#include <linux/version.h>          /* because we need conditional compiling */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)

/* This include was removed in Kernel. 3.4 */
#include <asm/system.h>

#endif

#include <linux/uaccess.h>            /* copy_to_user(), copy_from_user() */
#include <asm/io.h>

#include "drvstddef.h"                 /* common types USHORT, ULONG, ... */
#include "drsobglobal.h"       /* common types USHORT, ULONG, ... */
#include "sobdev.h"
#include "system.h"

/*===================================================================================
                         C O N S T A N T  I D E N T I F I E R S
==========================================+========================================*/


/*----------------------------------------+------------------------------------------
                         c o n s t a n t  i d e n t i f i e r s
-----------------------------------------------------------------------------------*/
#ifdef DEFINE_SPINLOCK
static DEFINE_SPINLOCK(driver_lock);
#else
static spinlock_t driver_lock = SPIN_LOCK_UNLOCKED;
#endif

USHORT SysCmosRead(UCHAR *ucBuffer, ULONG ulOffset, ULONG ulRead)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       DD.MM.YYYY
	Purpose:
    Changes:
******************************************+*****************************************/
{  
    UCHAR ucIndex;
    
    PDEBUG("SysCmosRead(ulRead = %d, ulOffset = %d) = entered\n", ulRead, ulOffset);
    for(ucIndex = (UCHAR)ulOffset; ulRead > 0; 
        ulRead--, ucIndex++, ucBuffer++)
    {
        spin_lock_irq(&driver_lock);

        outb(ucIndex | NMIOFF, CMOS_INDEX);
        *ucBuffer = inb(CMOS_DATA);

        spin_unlock_irq(&driver_lock);
    }
    return DRV_READY;
} // SysCmosRead

USHORT SysCmosWrite(UCHAR *ucBuffer, ULONG ulOffset, ULONG ulCount)
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       DD.MM.YYYY
	Purpose:
    Changes:
******************************************+*****************************************/
{  
    UCHAR ucIndex;

    PDEBUG("SysCmosWrite(ulCount = %d, ulOffset = %d) = entered\n", ulCount, ulOffset);
    for(ucIndex = (UCHAR)ulOffset; ulCount > 0; 
        ulCount--, ucIndex++, ucBuffer++)
    {
        spin_lock_irq(&driver_lock);

        outb(ucIndex | NMIOFF, CMOS_INDEX);
        outb(*ucBuffer, CMOS_DATA);

        spin_unlock_irq(&driver_lock);
    }
    return DRV_READY;
} // SysCmosWrite

