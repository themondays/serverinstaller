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
                                    V O R L A G E 
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Version:    1.32
    Implements: 
    Purpose:    
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#include <linux/module.h>

#define DEV_VERSION  132
#define DEV_NAME     "sobproc"

MODULE_DESCRIPTION("FSC Proc");

#undef PDEBUG /* undef it, just in case */
#ifdef _DEBUG_
#  define PDEBUG(fmt, args...) printk( KERN_DEBUG "sobproc: " fmt, ## args)
#  define PWARNING(fmt, args...) printk( KERN_WARNING "sobproc: " fmt, ## args)
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#  define PWARNING(fmt, args...) 
#endif
