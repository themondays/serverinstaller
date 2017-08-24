/* Copyright (C) 2010 Fujitsu Technology Solutions

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
			  S O B G A B I . H

	Project:    DeskFlash for Linux
	Created:    Fujitsu Technology Solutions
	Version:    1.50
******************************************+*****************************************/


/* Gabi BIOS entry point */
extern void *g_pEP_malloc;	// allocated with __vmalloc
extern void *g_pEP;		// and the entry point itself

int GabiExecute(UEFI_GabiRequest_T __user *userIn, unsigned in_size, UEFI_GabiResponse_T __user *userOut, unsigned out_size, unsigned *BytesCount);

/*
 *  file operations
 */
extern struct file_operations sob_fops;


typedef unsigned long gabi_addr_t;	// Physical machine address as seen by GABI, 32 or 64 bit

typedef struct
{
	unsigned char  *pVirtual;
	gabi_addr_t	Physical;
	unsigned	uSize;
} DriverBufferDescriptor_T;


