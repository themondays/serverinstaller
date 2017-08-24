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
                                    S T D D E F . H
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.30
    Implements: 

	Changes:    19.11.2001, PT: Added macros min and max
******************************************+*****************************************/

/*===================================================================================
                         C O N S T A N T  I D E N T I F I E R S
==========================================+========================================*/

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#ifndef UCHAR
typedef __u8 UCHAR;
#endif

#ifndef BYTE
typedef UCHAR BYTE;
#endif

#ifndef USHORT
typedef __u16 USHORT;
#endif

#ifndef ULONG
typedef __u32 ULONG;
#endif

#ifndef ULLONG
typedef __u64 ULLONG;
#endif

#ifndef PUCHAR
typedef __u8 * PUCHAR;
#endif

#ifndef PUSHORT
typedef __u16 * PUSHORT;
#endif

#ifndef PULONG
typedef __u32 * PULONG;
#endif

#ifndef SBYTE
typedef __s8 SBYTE;
#endif

#ifndef BOOL
typedef __s16 BOOL;
#endif

#ifndef LONG
typedef __s32 LONG;
#endif

#ifndef PCHAR
typedef __s8 * PCHAR;
#endif

#ifndef LPCSTR
typedef const PCHAR LPCSTR;
#endif

#ifndef PVOID
typedef void * PVOID;
#endif

#ifndef DWORD
typedef ULONG DWORD;
#endif

#ifndef FALSE
#   define FALSE 0
#   define TRUE !FALSE
#endif

#ifndef _MAX_PATH
#   define _MAX_PATH 256
#endif

#ifndef LEAVEIT
#	define LEAVEIT(a) if((ret = a) || 1) goto Leave
#endif

/*----------------------------------------+------------------------------------------
                         c o n s t a n t  i d e n t i f i e r s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                                     M A C R O S
==========================================+========================================*/

#ifndef _min
#   define _min(a, b) ((a) < (b))? (a) : (b)
#endif
#ifndef _max
#   define _max(a, b) ((a) > (b))? (a) : (b)
#endif

#if defined(__LP64__)
#	define ULLONG2PTR	void*
#	define PTR2ULONG	ULONG)(ULLONG
#else
#	define ULLONG2PTR	void*)(ULONG
#	define PTR2ULONG	ULONG
#endif

/*----------------------------------------+------------------------------------------
                                     m a c r o s
-----------------------------------------------------------------------------------*/

