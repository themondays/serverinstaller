/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
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
								 S O B D E V . H

	Project:    DeskFlash for Linux
	Created:    Fujitsu Technology Solutions / PT
	Purpose:
	Version:    1.50
******************************************+*****************************************/
#include <linux/module.h>

#define DEV_VERSION  150
#define DEV_NAME     "sobgabi"

MODULE_DESCRIPTION("FTS Gabi");

#undef PDEBUG					 /* undef it, just in case */
#ifdef _DEBUG_
#  define PDEBUG(fmt, args...) printk( KERN_DEBUG "sobgabi: " fmt, ## args)
#  define PWARNING(fmt, args...) printk( KERN_WARNING "sobgabi: " fmt, ## args)
#else
#  define PDEBUG(fmt, args...)	 /* not debugging: nothing */
#  define PWARNING(fmt, args...)
#endif
