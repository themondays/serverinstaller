/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
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
                                  B I O S A P I . H
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT / PT                            
    Purpose:    
    Version:    1.30
    Implements: tagBiosApiHeader
	            tagBiosApiFlashMap
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#include "bapimemory.h"

/*===================================================================================
                        C O N S T A N T  I D E N T I F I E R S
==========================================+========================================*/
#define  BIOS_START_ADDRESS      0xf0000
#define  BIOS_MAP_LENGTH         (0x10000 - 0x100)	

#define API_MAGIC_DWORD         0x4dc94253  /* "SNIBS" compressed */

#define MIN_CTL_BUF_LENGTH      1024        /* min. size of control buffer */
#define KMALLOC_MAX_BLOCK_SIZE  128 * 1024  /* max. size for kmalloc() (128k) */

/*----------------------------------------+------------------------------------------
                        c o n s t a n t  i d e n t i f i e r s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                                     M A C R O S
==========================================+========================================*/

/*----------------------------------------+------------------------------------------
                                     m a c r o s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                              D A T A  S T R U C T U R E S
==========================================+========================================*/

#define  MAX_PHYS_MEM_MAP_ENTIES   256

typedef unsigned long bapi_addr_t;	// Physical machine address as seen by BAPI, 32 or 64 bit

typedef struct tagAddressInfo
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
	Purpose:    
    Changes:
******************************************+*****************************************/
{
    bapi_addr_t       Physical;      // Physical machine address
    ULONG             ulSpan;        // address range
    PVOID             Virtual;       // Virtual pointer in addresss space
} ADDRESS_INFO, *PADDRESS_INFO;

typedef struct tagPhysMapList
/************************************************************************************
   Purpose: 
***********************************************************************************/
{
    USHORT          usItems;
    ULONG           ulTotal;
    ADDRESS_INFO    Items[MAX_PHYS_MEM_MAP_ENTIES];
} PHYS_MAP_LIST, *PPHYS_MAP_LIST;

typedef struct 
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
	Purpose:    
    Changes:
    16.02.2004, PT: Added items for 64 bit entry: usApi64Ofs, ulApi64Seg
******************************************+*****************************************/
{
    USHORT          usApi64Ofs;          // entry point 64-bit Protected Mode offset
    ULONG           ulApi64Seg;          // entry point 64-bit Protected Mode segment
    USHORT          usApi32Ofs;          // entry point 32-bit Protected Mode offset
    ULONG           ulApi32Seg;          // entry point 32-bit Protected Mode segment
    ADDRESS_INFO    ApiCtrl;
    ADDRESS_INFO    ApiDataIn;          // Data buffer for input
    ADDRESS_INFO    ApiDataOut;         // Data buffer for output

    USHORT          usVersion;
    ULONG           ulRequired;
    ULONG           ulMaxBlockSize;
    PHYS_MAP_LIST   MapIn;
    PHYS_MAP_LIST   MapOut;
}  BIOS_API_INFO, *PBIOS_API_INFO;

typedef struct tagBiosApiHeader
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:		25.05.1999
	Purpose:	Find product global profile
    Changes:
    16.02.2004, PT: Added items for 64 bit gateway: usApi64Ofs, ulApi64Seg
******************************************+*****************************************/
{
    ULONG           ulSignature;         // structure signature
    UCHAR           ucLength;            // structure length
    UCHAR           ucCheckSum;          // checksum of structure data
    UCHAR           ucVersion;           // structure version
    UCHAR           ucReserved1;         // reserved
    USHORT          usApi16Ofs;          // entry point 16-bit Protected Mode offset
    ULONG           ulApi16Seg;          // entry point 16-bit Protected Mode segment
    USHORT          usApi32Ofs;          // entry point 32-bit Protected Mode offset
    ULONG           ulApi32Seg;          // entry point 32-bit Protected Mode segment
    USHORT          usApi64Ofs;          // entry point 64-bit Protected Mode offset
    ULONG           ulApi64Seg;          // entry point 64-bit Protected Mode segment
} __attribute__ ((packed)) BIOS_API_HEADER, *PBIOS_API_HEADER;

typedef struct tagBiosApiFlashMap
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:	    25.05.1999
	Purpose:    
******************************************+*****************************************/
{
    __u16            usAllocatedLength;   // Size of buffer allocated by the application
    __u16            usReserved;          // Reserved
    struct 
    {
        __u32         pPhysAdr32;          // Start address of the physical memory block
        __u32         ulLength;            // Length of the physical memory block
    } __attribute__ ((packed)) Cluster[1];    // Minimum of 2 entries (usually more than 2)
} __attribute__ ((packed)) BIOS_API_FLASH_MAP, *PBIOS_API_FLASH_MAP;

/*----------------------------------------+------------------------------------------
                            d a t a  s t r u c t u r e s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                        F U N C T I O N  P R O T O T Y P E S
==========================================+========================================*/
USHORT BiosApiControl(PBIOS_API_INFO pApiInfo, 
                        PUCHAR pCtrlIn, ULONG ulSzCtrlIn, 
                        PUCHAR pCtrlOut, ULONG ulSzCtrlOut,
                        PUCHAR pDataIn, ULONG ulSzDataIn,
                        PUCHAR pDataOut, ULONG ulSzDataOut,
                        ULONG  ulUseMap);
USHORT BiosApiReadInfo(PUCHAR BaseAddress, PBIOS_API_INFO pApiInfo);
void BiosApiCleanUp(PBIOS_API_INFO pApiInfo);
/*----------------------------------------+------------------------------------------
                           f u n c t i o n  p r o t o t y p e s
-----------------------------------------------------------------------------------*/


