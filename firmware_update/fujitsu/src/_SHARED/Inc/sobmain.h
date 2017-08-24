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
                                    S O B M A I N . H
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.30
    Implements: 

	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#include <linux/module.h>

#ifndef __SOBMAIN_H__
#define __SOBMAIN_H__

MODULE_AUTHOR("Cestonaro Thilo <thilo.cestonaro.external@fujitsu-siemens.com>");
MODULE_LICENSE("GPL");

/*
 *   Functions
 */
// Declaration of dispatch function
#ifdef HAVE_UNLOCKED_IOCTL
long DrvUnlockedDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg);
#endif
#ifdef HAVE_COMPAT_IOCTL
long DrvCompatDispatchIoctl (struct file *filp, unsigned int cmd, unsigned long arg);
#endif
int DrvDispatchIoctl (struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg);
int DrvOpen (struct inode *inode, struct file *filp);
int DrvRelease (struct inode * inode, struct file *filp);
int init_module(void);

#ifdef _DEBUG_
void printBin(void *pIn, int iSize, int iLine, int iGroup);
#endif

#endif /* __SOBMAIN_H__ */
