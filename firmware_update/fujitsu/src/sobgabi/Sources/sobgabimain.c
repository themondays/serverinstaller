/* Copyright (C) 2006-2006 Fujitsu Technology Solutions

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
				S O B G A B I M A I N . C

	Project:    DeskFlash for Linux
	Created:    Fujitsu Technology Solutions
	Purpose:
	Version:    1.50

******************************************+*****************************************/

#define __NO_VERSION__			 /* don't define kernel_verion in module.h */
#include <linux/kernel.h>		 /* printk() */
#include <linux/errno.h>		 /* error codes */
#include <linux/types.h>		 /* size_t */
#include <asm/io.h>				 /* ioremap() */
/* Maintenance fix for Kernel 4.2  - Rainer Koenig - 2015-09-09 */
/* inclusion of vmalloc.h was removed in asm/io.h in Kernel 4.2 */
#include <linux/vmalloc.h>
#include <linux/fs.h>

#include "drvstddef.h"			 /* local definitions */
#include "sobdev.h"				 /* Specific definitions for the current device */

#include "drsobglobal.h"
#include "sobmain.h"
#include "gabimemory.h"
#include "sobgabi.h"

#define GABI_SIGNATURE "_GABIEP_"

/* Global data holding entry point of GABI code */
void *g_pEP_malloc = NULL;
void *g_pEP = NULL;
ULONG ulGabiVersion;

typedef struct
/********************************************************************************
 * Purpose: GABI entry point structure in bios
********************************************************************************/
{
	__u8	ucSignature[8];		 // structure signature
	__u8	ucLength;		 // structure length
	__u8	ucChecksum;		 // structure checksum
	__u8	ucVersion;		 // structure version
	__u8	reserved1[5];
	__u16	usApi16Ofs;		 // 16 bit entry point
	__u32   ulApi16Seg;		 //
	__u32	ulEntryPoint32;		 // 32 bit entry point
	__u64	ullEntryPoint64;	 // 64 bit entry point
	__u8	UselessStuff[0];	 // padding upto ucLength
}  __attribute__ ((packed)) GABI_HEADER, *PGABI_HEADER;

/* where to look for the entry point structure */
#define BIOS_MAP_ADDRESS	0xe0000
#define BIOS_MAP_LENGTH		0x20000
#define	BIOS_MAP_MASK		0x1ffff

/* and stuff used to allocate memory to hold a copy of the GABI BIOS code */
/* that is because older kernel versions did map memory with NX bit set */

#include <asm/pgtable.h>

#ifndef _PAGE_KERNEL_EXEC
#define _PAGE_KERNEL_EXEC   (_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED)
#endif

#define VMALLOC_EXEC(size)  __vmalloc((size), GFP_KERNEL | __GFP_HIGHMEM, __pgprot(_PAGE_KERNEL_EXEC))
#define BIOS_CODE_LENGTH	0x10000
#define	BIOS_CODE_MASK		0xffff

static int major_number = 0;			/* major number of the char-device */
ULONG ulGabiVersion;

static __u8 compute_cs(void *p, unsigned l)
/***********************************************************************************
	Author:		Fujitsu Technology Solutions
	Purpose: compute checksum of GABI HEADER structure
******************************************+****************************************/
{
	__u8 *puc = (__u8*)p;
	__u8 rv = 0;
	while (l--)
		rv += *puc++;
	return rv;
}

static __u8 *locate_and_map_gabi(void)
{
	PGABI_HEADER pHeader = NULL;
	__u8 *pucHeader;
	__u8 *pucMappedBios = ioremap(BIOS_MAP_ADDRESS, BIOS_MAP_LENGTH);
	__u8 *pucMappedBiosEnd = pucMappedBios + BIOS_MAP_LENGTH;

	if (!pucMappedBios)
		return NULL;

	/*******
         * You should not use the pointer returned by ioremap to access iomem directly, use readb/writeb etc.
         * But we do because we know that it is not io but ram and i386 or amd64. Nevertheless: FIX ME
         *******/
	for (pucHeader = pucMappedBios; (pucHeader + sizeof(*pHeader)) < pucMappedBiosEnd; pucHeader+=16)
	{
		PGABI_HEADER p = (PGABI_HEADER)pucHeader;
		// header found ?
		if ((memcmp(p->ucSignature, GABI_SIGNATURE, sizeof(p->ucSignature)) == 0) &&
		// not too long ?
		    ((pucHeader + p->ucLength) < pucMappedBiosEnd) &&
		// checksum ok ?
		    (compute_cs(p, p->ucLength) == 0) )
		{
			pHeader = p;
			break;
		}
	}
	// Does it contain a valid entry point ?
	if (pHeader &&
#if defined(__LP64__)
	    pHeader->ucLength >= sizeof(*pHeader) &&
#define GABI_EP ullEntryPoint64
#else
	    pHeader->ucLength >= (sizeof(*pHeader) - sizeof(pHeader->ullEntryPoint64)) &&
#define GABI_EP ulEntryPoint32
#endif
	    pHeader->GABI_EP &&
	    pHeader->ucVersion == 1)
	{
		__u8 *pucMap = ioremap(pHeader->GABI_EP & ~BIOS_CODE_MASK, BIOS_CODE_LENGTH);
		unsigned uEP_Offset = pHeader->GABI_EP & BIOS_CODE_MASK;
		__u8 *malloced = __vmalloc(BIOS_CODE_LENGTH, GFP_KERNEL | __GFP_HIGHMEM, __pgprot(_PAGE_KERNEL_EXEC));
		if (pucMap && malloced)
		{
			memcpy_fromio(malloced, pucMap, BIOS_CODE_LENGTH);
			g_pEP = malloced + uEP_Offset;
			g_pEP_malloc = (void *)malloced;
			ulGabiVersion = pHeader->ucVersion;
		}

		iounmap(pucMap);
	}
	iounmap(pucMappedBios);

	return g_pEP_malloc;
}

int init_module(void)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
 ******************************************+*****************************************/
{
	int result;
	int err = 0;

	PDEBUG("[init_module] - entered\n");

	if (!locate_and_map_gabi())
		PWARNING("[init_module] - GABI entry point not found\n");

	// Register device
	result = register_chrdev(major_number, DEV_NAME, &sob_fops);
	if (result < 0)
	{
		PWARNING("[init_module] - can't get major %d\n",major_number);
		err = result;
		goto ExitError;
	}

	if (major_number == 0)
		major_number = result;	 /* dynamic */

	PDEBUG("[init_module] - Module '%s' loaded succesfully, major number is %d\n", DEV_NAME, major_number);
	err = 0;					 /* success */

	ExitError:
	PDEBUG("[init_module] - leave\n");
	return err;
}								 // init_module


void cleanup_module(void)
/************************************************************************************
	Author:		Fujitsu Technology Solutions
******************************************+*****************************************/
{
	PDEBUG("[cleanup_module] - entered\n");
	unregister_chrdev(major_number, DEV_NAME);
	if (g_pEP_malloc) vfree(g_pEP_malloc);
	PDEBUG("[cleanup_module] - leave\n");
}								 // cleanup_module


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
}								 // DbgOutBinary
#endif
