/* Copyright (C) 2006-2010 Fujitsu Technology Solutions / PT
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
                                   B I O S A P I . C
     
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.42
******************************************+*****************************************/

#include <linux/version.h>          /* because we need conditional compiling */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)

/* This include was removed in Kernel. 3.4 */
#include <asm/system.h>

#endif

#include <linux/uaccess.h>	/* copy_to_user(), copy_from_user() */
#include <asm/io.h>
/* Maintenance fix for Kernel 4.2  - Rainer Koenig - 2015-09-09 */
/* inclusion of vmalloc.h was removed in asm/io.h in Kernel 4.2 */
#include <linux/vmalloc.h>
#include <linux/kernel.h>

#include <linux/pci.h>
#include <linux/cred.h>	 /* need for current macro */

#ifdef virt_to_machine
/* for XEN */
#define MY_CONFIG_XEN
#define USE_DMA_COHERENT
// Temporary fix for RH5.7 == Kernel 2.6.18-274.el5xen
#if  (LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 18))
//
// from x86_dma_fallback_dev in arch/x86/kernel/pci-dma.c
//
static struct device my_x86_dma_fallback_dev = {
        .bus_id = "bapi_device",
        .coherent_dma_mask = DMA_32BIT_MASK,
        .dma_mask = &my_x86_dma_fallback_dev.coherent_dma_mask,
};
#define MY_DMA_DEVICE  (strncmp(KERNELVERSION, "2.6.18-274.el5xen", 17) ? NULL : &my_x86_dma_fallback_dev)
#else
#define MY_DMA_DEVICE  NULL
#endif
#endif

/* Old kernel does not support GFP_DMA32, so when we might get physical addresses above 4GB (i.e pae or 64 bit)
 * then we use dma_alloc_coherent. That then means we get memory below 16 Mbyte. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15) && (defined(CONFIG_X86_PAE) || defined(__LP64__))
#ifndef USE_DMA_COHERENT
#define USE_DMA_COHERENT
#endif
#endif

#ifdef virt_to_machine
# define v2m(x) ((bapi_addr_t)virt_to_machine(x))
#else
# define v2m(x) ((bapi_addr_t)virt_to_bus(x))
#endif

#ifndef GFP_DMA32	// not defined on RH4.8 32 bit, but not needed either (and on pae ? )
#define GFP_DMA32 0
#endif

#include "drvstddef.h"       /* common types USHORT, ULONG, ... */
#include "drsobglobal.h"

#include "sobdev.h"
#include "biosapi.h"
#include "sobmain.h"

extern BOOL ApicCheckEnabled(void);    // see apic.S for implementation

/*===================================================================================
                      forward   F U N C T I O N  P R O T O T Y P E S
==========================================+========================================*/
static USHORT BiosApiCopyDataFromMap(PPHYS_MAP_LIST pMap, PUCHAR pData, ULONG ulImageSize);
static USHORT BiosApiCopyDataToMap(PPHYS_MAP_LIST pMap, PUCHAR pData, ULONG ulSzData);
static USHORT BiosApiAllocBlocks(PBIOS_API_INFO pApiInfo, ULONG ulRequired, PPHYS_MAP_LIST pMap, PBIOS_API_FLASH_MAP pFlashMap);
static USHORT PhysBlkAllocBlocks(ULONG ulRequired, ULONG ulMaxBlockSize, PPHYS_MAP_LIST pMap);
static void   PhysBlkReleaseAllBlocks(PPHYS_MAP_LIST pMap);
static void   PhysBlkFreeBlock(PADDRESS_INFO pAdrInfo);
static UCHAR  BiosApiChkSum(char *pAdr, USHORT usLength);
static USHORT BiosApiCtrlBufferAlloc(PADDRESS_INFO pAdrInfo, ULONG ulSize);
static USHORT BiosApiCtrlBuffersAlloc(PBIOS_API_INFO pApiInfo, ULONG ulCtrl, ULONG ulDataIn, ULONG ulDataOut);
static void   BiosApiCtrlBuffersFree(PBIOS_API_INFO pApiInfo);
static BOOL   BiosApiCheckAccess(PBIOS_API_CTRL_IN pCtrlIn);
static USHORT BiosApiCall(PBIOS_API_INFO pApiInfo, bapi_addr_t pCtrl, bapi_addr_t pDataIn, bapi_addr_t pDataOut);
#ifdef _DEBUG_
static void outputMap(PBIOS_API_FLASH_MAP pMap, ULONG ulImageSize);
#endif
/*----------------------------------------+------------------------------------------
                           f u n c t i o n  p r o t o t y p e s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                   C O N S T A N T  I D E N T I F I E R S
==========================================+========================================*/

#undef PAGE_KERNEL_EXEC

#ifndef _PAGE_KERNEL_EXEC
#	define _PAGE_KERNEL_EXEC \
		(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED)
#endif
#define PAGE_KERNEL_EXEC  __pgprot(_PAGE_KERNEL_EXEC)
#define VMALLOC_EXEC(size)      __vmalloc((size), GFP_KERNEL | __GFP_HIGHMEM, PAGE_KERNEL_EXEC)

#define PHYS_PAGE_SIZE 4096
/*----------------------------------------+------------------------------------------
                   c o n s t a n t  i d e n t i f i e r s
-----------------------------------------------------------------------------------*/
static void my_kfree(void *pVirt, unsigned long ulSpan)
{
    if (pVirt)
#ifdef USE_DMA_COHERENT
        dma_free_coherent(MY_DMA_DEVICE, ulSpan, pVirt, virt_to_bus(pVirt));
#else
        free_pages((unsigned long)pVirt, get_order(ulSpan));
#endif
}

#ifndef MAX_ORDER
#define MAX_ORDER 10
#endif

static void* my_kmalloc(unsigned long ulSpan)
{
    void *pVirt = NULL;
#ifdef USE_DMA_COHERENT
    // under XEN we have to use the dma api to get physical memeory below 4GB
    dma_addr_t dummy;
    if (ulSpan > (PAGE_SIZE<<(MAX_ORDER-1))) return NULL;
    pVirt = dma_alloc_coherent(MY_DMA_DEVICE, ulSpan, &dummy, GFP_DMA);
#else
    // without XEN ist is better to use __get_free_pages because the dma api adds additional constrains that are not needed.
    // kmalloc does not recognise GFP_DMA32, only GFP_DMA, we would get memory <16Mbyte.
    if (ulSpan > (PAGE_SIZE<<(MAX_ORDER-1))) return NULL;
    pVirt = (void*)__get_free_pages(GFP_KERNEL | GFP_DMA32, get_order(ulSpan));
#endif
#if defined(__LP64__) && defined(_DEBUG_)
    if (pVirt)
    {
        unsigned long pPhys = (unsigned long)v2m(pVirt);
        if (pPhys > (unsigned)0xffffffff)
        {
            printk("[my_kmalloc] - got address > 4GB: Virt=0x%p Phys=0x%p Machine=0x%p\n",
                                                      pVirt, (void*)virt_to_bus(pVirt), (void*)pPhys);
            my_kfree(pVirt, ulSpan);
            pVirt = NULL;
        }
    }
#endif
    return pVirt;
}

static UCHAR BiosApiChkSum(char *pAdr, USHORT usLength)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    Check, if sum of all bytes equals to zero
******************************************+*****************************************/
{
    USHORT us;
    UCHAR ucChkSum = 0;
    for (us = 0; us < usLength; us++, pAdr++)
    {
        ucChkSum += (UCHAR)*pAdr;
    }
    return ucChkSum;
} // BiosApiChkSum

static BOOL BiosApiCheckAccess(PBIOS_API_CTRL_IN pCtrlIn)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       14.09.2001
    Purpose:    
******************************************+*****************************************/
{
     // root groop is allowed to invoke all calls
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
    if (current->egid == 0)
#else

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 5, 0) /* GLOBAL_ROOT_GID was introduced in 3.5.0 */
      if (current_egid() == 0)
#else
	  if (gid_eq(current_egid(),GLOBAL_ROOT_GID))
#endif /* 3.5.0 test */
#endif /* 2.6.29 test */
        return TRUE;

    return FALSE;
} // BiosApiCheckAccess

#ifdef _DEBUG_
static void outputMap(PBIOS_API_FLASH_MAP pMap, ULONG ulImageSize)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    USHORT  us = 0;
    PDEBUG("[outputMap] - entered\n");

    if (pMap) {
        PDEBUG("[outputMap] - allocatedLength: %d\n", pMap->usAllocatedLength);
        while (pMap->Cluster[us].pPhysAdr32)
        {
            PDEBUG("[outputMap] - Cluster[%d] (Size: %d)\n", us, pMap->Cluster[us].ulLength);
//            printBin(p2v(pMap->Cluster[us].pPhysAdr32), pMap->Cluster[us].ulLength, 16, 8);
            us++;
        }
    }

    PDEBUG("[outputMap] - leaving\n");
}  // outputMap
#endif

static USHORT BiosApiCopyDataFromMap(PPHYS_MAP_LIST pMap, PUCHAR pData, ULONG ulImageSize)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
    03.04.2002, PT: Added parameter ulSzData
******************************************+*****************************************/
{
    USHORT  us;
    ULONG   ulSpan;
    PUCHAR p = pData;
    PDEBUG("[BiosApiCopyDataFromMap] - entered\n");

    for (us = 0; us < pMap->usItems && ulImageSize > 0; us++)
    {
        ulSpan = _min(pMap->Items[us].ulSpan, ulImageSize);
        PDEBUG("[BiosApiCopyDataFromMap] - Copy To User, Items[%d].ulSpan=%d\n", us, ulSpan);
        if (0 != copy_to_user(p, pMap->Items[us].Virtual, ulSpan))
        {
            PDEBUG("[BiosApiCopyDataFromMap] - ERROR on copying data from user space\n");
            return DRV_ERROR;
        }
        p += ulSpan;
        ulImageSize -= ulSpan;
    }

    PDEBUG("[BiosApiCopyDataFromMap] - leaving\n");
    return DRV_READY;
}  // BiosApiCopyDataFromMap

static USHORT BiosApiCopyDataToMap(PPHYS_MAP_LIST pMap, PUCHAR pData, ULONG ulSzData)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
    Changes:    
    03.04.2002, PT: Added parameter ulSzData
******************************************+*****************************************/
{
    USHORT us;
    ULONG ulSpan;
    PUCHAR p = pData;

    PDEBUG("[BiosApiCopyDataToMap] - entered, Data: 0x%p, Size: %d\n", pData, ulSzData);
    if (pData) {
        for (us = 0; us < pMap->usItems && ulSzData; us++)
        {
            ulSpan = _min(ulSzData, pMap->Items[us].ulSpan);
            PDEBUG("[BiosApiCopyDataToMap] - Copy from User, Items[%d].Virtual=0x%p, Items[%d].ulSpan=%d\n", us, pMap->Items[us].Virtual, us, ulSpan);
            if (0 != copy_from_user(pMap->Items[us].Virtual, p, ulSpan))
            {
                PDEBUG("[BiosApiCopyDataToMap] - ERROR on copying data from user space\n");
                return DRV_ERROR;
            }
            p += ulSpan;
            ulSzData -= ulSpan;
        }
    }
    PDEBUG("[BiosApiCopyDataToMap] - leave\n");
    
    return DRV_READY;
}  // BiosApiCopyDataToMap

static void BiosApiCtrlBuffersFree(PBIOS_API_INFO pApiInfo)
/************************************************************************************
   Author:  Fujitsu Technology Solutions / PT
   Date:    21.04.2005
************************************************************************************/ 
/**
     <<Description>>
*/
{
    PDEBUG("[BiosApiCtrlBuffersFree] - entered \n");

    PhysBlkFreeBlock(&pApiInfo->ApiCtrl);
    PhysBlkFreeBlock(&pApiInfo->ApiDataOut);
    PhysBlkFreeBlock(&pApiInfo->ApiDataIn);

    PDEBUG("[BiosApiCtrlBuffersFree] - leave \n");
} // BiosApiCtrlBuffersFree

static USHORT BiosApiCtrlBufferAlloc(PADDRESS_INFO pAdrInfo, ULONG ulSize)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Params:  pApiInfo
            ulCtrl - Required size for control buffer
            ulDataInOut - Required size for data buffer
    Purpose:    
******************************************+*****************************************/
{
    USHORT        usStatus;

    usStatus = DRV_READY;

    PDEBUG("[BiosApiCtrlBufferAlloc] - entered : ulSize = %d \n", ulSize);

    if (ulSize > 0 && pAdrInfo) {
        if (pAdrInfo->Virtual == NULL || pAdrInfo->ulSpan < ulSize)
        // If buffer has not been allocated yet or is too small:
        // do the allocation
        {
            if (pAdrInfo->Virtual != NULL)
            // Free the previously allocated buffer
                PhysBlkFreeBlock(pAdrInfo);

            pAdrInfo->Virtual = my_kmalloc(ulSize);

            if (pAdrInfo->Virtual == NULL) {
                PDEBUG("[BiosApiCtrlBufferAlloc] - Virtual is NULL\n");
                usStatus = DRV_INSUFFICIENT_RESOURCES;
            } else {
	        pAdrInfo->Physical = v2m(pAdrInfo->Virtual);
                pAdrInfo->ulSpan = ulSize;

                PDEBUG("[BiosApiCtrlBufferAlloc] - Virtual(%u)=0x%p Physical=0x%p\n", pAdrInfo->ulSpan, pAdrInfo->Virtual, (void*)pAdrInfo->Physical);
                memset(pAdrInfo->Virtual, 0, pAdrInfo->ulSpan);
            }
        }
    }
    else {
        PDEBUG("[BiosApiCtrlBufferAlloc] - nothing needs to be allocated.\n");
        PhysBlkFreeBlock(pAdrInfo);
    }

    return usStatus;
} // BiosApiCtrlBufferAlloc

static USHORT BiosApiCtrlBuffersAlloc(PBIOS_API_INFO pApiInfo,
                               ULONG ulCtrl, ULONG ulDataIn, ULONG ulDataOut)
/************************************************************************************
   Author:  Fujitsu Technology Solutions / PT
   Date:    20.04.2005
   Params:  pApiInfo
            ulCtrl    - Required size for control buffer
            ulDataIn  - Required size for data buffer
            ulDataOut - Required size for data buffer
*
*    Allocate memory blocks for the control buffer, the output data buffer and the input buffer.
*    all sizes may be 0. 
************************************************************************************/ 
{
   USHORT        usStatus;

   PDEBUG("[BiosApiCtrlBuffersAlloc] - entered, ulCtrl=%d, ulDataIn=%d, ulDataOut=%d\n", ulCtrl, ulDataIn, ulDataOut);

   //
   // Alloc Ctrl Buffer
   //
   usStatus = BiosApiCtrlBufferAlloc(&pApiInfo->ApiCtrl, ulCtrl);
   if (usStatus != DRV_READY)
   {
        PDEBUG("[BiosApiCtrlBuffersAlloc] - failed to allocate control buffer\n");
        return usStatus;
   }

   //
   // Alloc Data Output Buffer
   //
   usStatus = BiosApiCtrlBufferAlloc(&pApiInfo->ApiDataOut, ulDataOut);
   if (usStatus != DRV_READY)
   {
        PDEBUG("[BiosApiCtrlBuffersAlloc] - failed to allocate output buffer\n");
        return usStatus;
   }

    //
    // Alloc Data Input Buffer
    //
    usStatus = BiosApiCtrlBufferAlloc(&pApiInfo->ApiDataIn, ulDataIn);
    if (usStatus != DRV_READY)
    {
        PDEBUG("[BiosApiCtrlBuffersAlloc] - failed to allocate input buffer\n");
        return usStatus;
    }
    PDEBUG("[BiosApiCtrlBuffersAlloc] - leaving, 0x%4.4x\n", usStatus);
    return usStatus;
} // BiosApiCtrlBuffersAlloc

static USHORT BiosApiAllocBlocks(PBIOS_API_INFO pApiInfo, ULONG ulRequired, 
                          PPHYS_MAP_LIST pMap, PBIOS_API_FLASH_MAP pFlashMap)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    USHORT usStatus = 0;
    USHORT us;

    PDEBUG("[BiosApiAllocBlocks] - entered\n");

    if (ulRequired > pMap->ulTotal)
    // Do allocation only, if it has not been done before
    {
        PhysBlkReleaseAllBlocks(pMap);
        usStatus = PhysBlkAllocBlocks(ulRequired, pApiInfo->ulMaxBlockSize, pMap);
    }
    else
        usStatus = DRV_READY;

    if (usStatus == DRV_READY)
    {
	for (us=0; us < pMap->usItems; us++)
        {
              pFlashMap->Cluster[us].pPhysAdr32 = (__u32)(pMap->Items[us].Physical);	// 32 bit physical machine address
              pFlashMap->Cluster[us].ulLength = (__u32)pMap->Items[us].ulSpan;
        }
        pFlashMap->Cluster[us].pPhysAdr32 = 0;
        pFlashMap->Cluster[us].ulLength = 0;
    }
    pFlashMap->usAllocatedLength = sizeof(*pFlashMap) + pMap->usItems*sizeof(pFlashMap->Cluster);
    PDEBUG("[BiosApiAllocBlocks] - leave\n");
    return usStatus;
} // BiosApiAllocBlocks

static USHORT PhysBlkAllocBlocks(ULONG ulRequired, ULONG ulMaxBlockSize, PPHYS_MAP_LIST pMap)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    
    USHORT           usStatus;
    ULONG            ulBlockSize;
    ULONG            ulMaxSize, ulStdBlockSize;       
    PVOID            Virtual;
    bapi_addr_t      Physical;
    
    PDEBUG("[PhysBlkAllocBlocks] - entered, ulRequired=%d, ulMaxBlockSize=%d\n", ulRequired, ulMaxBlockSize);

    usStatus         = DRV_READY;
    ulMaxSize        = ulRequired;
    pMap->ulTotal    = 0;
    pMap->usItems    = 0;
    ulStdBlockSize   = ulMaxBlockSize;

    while (ulMaxSize && pMap->usItems < MAX_PHYS_MEM_MAP_ENTIES)
    {
        // we need more then one buffer cause the needed buffer is bigger than
        // the buffersize we can alloc once and we are at the first item
        if (ulMaxSize > ulStdBlockSize && pMap->usItems == 0)
            // the BUP Header has 0x70 Byte, place it as one complete first buffer
            ulBlockSize = _min(0x70, ulMaxSize);
        else
            // use the smallest needed
            ulBlockSize = _min(ulStdBlockSize, ulMaxSize);

        Virtual = my_kmalloc(ulBlockSize);
        if (Virtual == NULL)
        {
            if (ulStdBlockSize > 4096)
            // Reduce allocation block size
            {
                ulStdBlockSize = ulStdBlockSize >> 1;
                continue; // try again
            }
            usStatus = DRV_INSUFFICIENT_RESOURCES;
            PDEBUG("[PhysBlkAllocBlocks] - out of memory while trying to allocate map\n");
            PhysBlkReleaseAllBlocks(pMap); // undo all allocations
            break; // error
        }

	Physical = v2m(Virtual);
        PDEBUG("[PhysBlkAllocBlocks] - got 0x%p, size: %d, Physical: 0x%p\n", Virtual, ulBlockSize, (void*)Physical);
        memset(Virtual, 0, ulBlockSize);

        pMap->Items[pMap->usItems].Virtual  = Virtual;
        pMap->Items[pMap->usItems].Physical = Physical;
        pMap->Items[pMap->usItems].ulSpan   = ulBlockSize;
        pMap->usItems++;
        if (ulMaxSize < ulBlockSize)
            ulMaxSize = 0;
        else
            ulMaxSize -= ulBlockSize;
        pMap->ulTotal += ulBlockSize;
    }

    PDEBUG("[PhysBlkAllocBlocks] - leaving, usStatus=0x%4.4x\n", usStatus);
    return usStatus;
} // PhysBlkAllocBlocks

static void PhysBlkReleaseAllBlocks(PPHYS_MAP_LIST pMap)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    USHORT us;
    PDEBUG("[PhysBlkReleaseAllBlocks] - entered\n");

    for (us = 0; us < pMap->usItems && us < MAX_PHYS_MEM_MAP_ENTIES; us++)
    {
        PhysBlkFreeBlock(pMap->Items + us);
    }
    pMap->usItems = 0;
    pMap->ulTotal = 0;
    PDEBUG("[PhysBlkReleaseAllBlocks] - leave\n");
}  // PhysBlkReleaseAllBlocks

static void PhysBlkFreeBlock(PADDRESS_INFO pAdrInfo)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    if (pAdrInfo->Virtual != NULL && pAdrInfo->ulSpan != 0)
    {
        my_kfree(pAdrInfo->Virtual, pAdrInfo->ulSpan);
    }

    pAdrInfo->Virtual = NULL;
    pAdrInfo->ulSpan  = 0;
    
}  // PhysBlkFreeBlock

USHORT BiosApiReadInfo(PUCHAR BaseAddress, PBIOS_API_INFO pApiInfo)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    Tries to find BIOS Api header inside the F-segment
******************************************+*****************************************/
{
    PUCHAR  FindAddrPtr;
    PULONG  FindPtr;
    ULONG   ulOfs;                  
    BOOL    bFound;
    PBIOS_API_HEADER pApiHdr;
    USHORT  usRet;

    PDEBUG("[BiosApiReadInfo] - entered\n");
    usRet = DRV_API_DATA_ERROR;

    for (ulOfs = 0, FindAddrPtr = BaseAddress, bFound = FALSE; 
         ulOfs < (BIOS_MAP_LENGTH - sizeof(BIOS_API_HEADER) - sizeof(ULONG)); 
         ulOfs++, FindAddrPtr++) 
    {
        FindPtr = (PULONG)FindAddrPtr;
        if (*FindPtr == API_MAGIC_DWORD)
        {
            PDEBUG("[BiosApiReadInfo] - Found Magic DWord\n");
            if (BiosApiChkSum((char *)FindAddrPtr, sizeof(BIOS_API_HEADER)) == 0) 
            {
                pApiHdr = (PBIOS_API_HEADER)FindAddrPtr;
                pApiInfo->ulApi32Seg = pApiHdr->ulApi32Seg;
                pApiInfo->usApi32Ofs = pApiHdr->usApi32Ofs;
                pApiInfo->ulApi64Seg = pApiHdr->ulApi64Seg;
                pApiInfo->usApi64Ofs = pApiHdr->usApi64Ofs;
                
                pApiInfo->usVersion = 0x0100 | (USHORT)pApiHdr->ucVersion;
                PDEBUG("[BiosApiReadInfo] - BAPI header version 0x%4.4x found\n", pApiInfo->usVersion);
                PDEBUG("[BiosApiReadInfo] - 32 bit entry point: - Segment: %d - Offset: %d\n", pApiInfo->ulApi32Seg, pApiInfo->usApi32Ofs);
                PDEBUG("[BiosApiReadInfo] - 64 bit entry point: - Segment: %d - Offset: %d\n", pApiInfo->ulApi64Seg, pApiInfo->usApi64Ofs);
                usRet = DRV_READY;
                break;
            }
        }
    }
    PDEBUG("[BiosApiReadInfo] - leaving, usRet = %d\n", usRet);
    return usRet;
} // BiosApiReadInfo

void BiosApiCleanUp(PBIOS_API_INFO pApiInfo)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    PDEBUG("[BiosApiCleanUp] - entered\n");

    PhysBlkReleaseAllBlocks(&(pApiInfo->MapIn));
    PhysBlkReleaseAllBlocks(&(pApiInfo->MapOut));
    BiosApiCtrlBuffersFree(pApiInfo);

    PDEBUG("[BiosApiCleanUp] - leaving\n");
} // BiosApiCleanUp

USHORT BiosApiControl(PBIOS_API_INFO pApiInfo, 
            PUCHAR pCtrlIn, ULONG ulSzCtrlIn, 
            PUCHAR pCtrlOut, ULONG ulSzCtrlOut,
            PUCHAR pDataIn, ULONG ulSzDataIn,
            PUCHAR pDataOut, ULONG ulSzDataOut,
            ULONG ulUseMap)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:    
******************************************+*****************************************/
{
    USHORT               usStatus = DRV_ERROR;
    PBIOS_API_FLASH_MAP  pMapOut = NULL;
    PBIOS_API_FLASH_MAP  pMapIn = NULL;
    ULONG                ulCtrlSize, ulMapSize, ulSig;

    PDEBUG("[BiosApiControl] - entered\n");

    ulMapSize = sizeof(BIOS_API_FLASH_MAP) + MAX_PHYS_MEM_MAP_ENTIES * sizeof(pMapOut->Cluster[0]);
       pApiInfo->ulRequired = _max(ulSzDataIn, ulSzDataOut);
    ulCtrlSize = _max(ulSzCtrlIn, ulSzCtrlOut);

    if (get_user(ulSig, &(((PBIOS_API_CTRL_IN)pCtrlIn)->ulServiceSignature)))
    {
        usStatus = DRV_ERROR;
        goto Leave_SkipCall;
    }

    PDEBUG("[BiosApiControl] - was called with Sig: %X\n", ulSig);
    
    if (ulUseMap)
    // If memory map shall be used instead of a small memory block
    {
        pApiInfo->ulMaxBlockSize = _min(pApiInfo->ulRequired, (ULONG)KMALLOC_MAX_BLOCK_SIZE);

        usStatus = BiosApiCtrlBuffersAlloc(pApiInfo, ulCtrlSize, ulMapSize, ulMapSize);
        if (usStatus != DRV_READY)
        {
            PDEBUG("[BiosApiControl] - error on calling BiosApiCtrlBuffersAlloc\n");
            goto Leave_SkipCall;
        }

        if (0 != copy_from_user(pApiInfo->ApiCtrl.Virtual, pCtrlIn, ulSzCtrlIn))
        {
            PDEBUG("[BiosApiControl] - ERROR, on copy_from_user\n");
            usStatus = DRV_ERROR;
            goto Leave_SkipCall;
        }

        pMapIn = (PBIOS_API_FLASH_MAP)pApiInfo->ApiDataIn.Virtual;
        pMapIn->usAllocatedLength = ulMapSize;
        usStatus = BiosApiAllocBlocks(pApiInfo, ulSzDataIn, &(pApiInfo->MapIn), pMapIn);
        if (usStatus != DRV_READY)
        {
            PDEBUG("[BiosApiControl] - error on calling BiosApiAllocBlocks(.. MapIn)\n");
            goto Leave_SkipCall;
        }
        else
        {
            usStatus = BiosApiCopyDataToMap(&(pApiInfo->MapIn), pDataIn, ulSzDataIn);
        }

        pMapOut    = (PBIOS_API_FLASH_MAP)pApiInfo->ApiDataOut.Virtual;
        pMapOut->usAllocatedLength      = ulMapSize;
        usStatus = BiosApiAllocBlocks(pApiInfo, ulSzDataOut, &(pApiInfo->MapOut), pMapOut);
        if (usStatus != DRV_READY)
        {
            PDEBUG("[BiosApiControl] - error on calling BiosApiAllocBlocks(.. MapOut)\n");
            goto Leave_SkipCall;
        }
    }
    else
    {
        usStatus = BiosApiCtrlBuffersAlloc(pApiInfo, ulCtrlSize, ulSzDataIn, ulSzDataOut);
        if (usStatus != DRV_READY)
        {
            PDEBUG("[BiosApiControl] - error on calling BiosApiCtrlBuffersAlloc\n");
            goto Leave_SkipCall;
        }

        if (pCtrlIn && 0 != copy_from_user(pApiInfo->ApiCtrl.Virtual, pCtrlIn, ulSzCtrlIn))
        {
            PDEBUG("[BiosApiControl] - error on calling copy_from_user\n");
            usStatus = DRV_ERROR;
            goto Leave_SkipCall;
        }

        if (ulSzDataIn) {
            if (!pApiInfo->ApiDataIn.Virtual) {
                PDEBUG("[BiosApiControl] - error, we have datain to copy, but no buffer to copy to.\n");
                usStatus = DRV_ERROR;
                goto Leave_SkipCall;
            }
            else if (!pDataIn) {
                PDEBUG("[BiosApiControl] - error, we have datain to copy, but no buffer to copy from.\n");
                usStatus = DRV_ERROR;
                goto Leave_SkipCall;
            }
            else if (0 != copy_from_user(pApiInfo->ApiDataIn.Virtual, pDataIn, ulSzDataIn))
            {
                PDEBUG("[BiosApiControl] - error on calling copy_from_user\n");
                usStatus = DRV_ERROR;
                goto Leave_SkipCall;
            }
        }
        usStatus = DRV_READY;
    }

    if (usStatus == DRV_READY)
    {
        if (!BiosApiCheckAccess((PBIOS_API_CTRL_IN)pApiInfo->ApiCtrl.Virtual))
        {
            PDEBUG("[BiosApiControl] - invalid access rights\n");
            usStatus = DRV_ACCESS_DENIED;
            goto Leave_SkipCall;
        }

#if defined(_DEBUG_)
       {
            PBIOS_API_CTRL_IN pDBGCtrlIn;

            pDBGCtrlIn = (PBIOS_API_CTRL_IN)pApiInfo->ApiCtrl.Virtual;
            printBin(pApiInfo->ApiCtrl.Virtual, ulCtrlSize > 32 ? 32 : ulCtrlSize, 16, 8);
            PDEBUG("[BiosApiControl] - ServiceCode: 0x%X; Code: 0x%X\n", pDBGCtrlIn->ServiceCode.ulValue, pDBGCtrlIn->FunctionControl.Generic.usCode);
            PDEBUG("[BiosApiControl] - Ctrl: Physical: 0x%p\n", (void*)pApiInfo->ApiCtrl.Physical);
            PDEBUG("[BiosApiControl] - DataIn: Physical: 0x%p\n", (void*)pApiInfo->ApiDataIn.Physical);
            if (ulUseMap)
                    outputMap((PBIOS_API_FLASH_MAP)pApiInfo->ApiDataIn.Virtual, ulSzDataIn);
            else
                    printBin(pApiInfo->ApiDataIn.Virtual, ulSzDataIn, 16, 8);
            PDEBUG("[BiosApiControl] - DataOut: Physical: 0x%p\n", (void*)pApiInfo->ApiDataOut.Physical);

            PDEBUG("[BiosApiControl] - scheduling timeout of one HZ\n");
            set_current_state(TASK_INTERRUPTIBLE);
            schedule_timeout (HZ);
        }
#endif

        usStatus = BiosApiCall(pApiInfo,
            pApiInfo->ApiCtrl.Physical,
            pApiInfo->ApiDataIn.Physical,
            pApiInfo->ApiDataOut.Physical);

#if defined(_DEBUG_)
        {

            PBIOS_API_CTRL_OUT pDBGCtrlOut;
            PDEBUG("[BiosApiControl] - Ctrl: Physical: 0x%p\n", (void*)pApiInfo->ApiCtrl.Physical);
            printBin(pApiInfo->ApiCtrl.Virtual, ulCtrlSize > 32 ? 32 : ulCtrlSize, 16, 8);
            PDEBUG("[BiosApiControl] - DataIn: Physical: 0x%p\n", (void*)pApiInfo->ApiDataIn.Physical);
            PDEBUG("[BiosApiControl] - DataOut: Physical: 0x%p\n", (void*)pApiInfo->ApiDataOut.Physical);
            if (!ulUseMap)
                    outputMap((PBIOS_API_FLASH_MAP)pApiInfo->ApiDataOut.Virtual, ulSzDataOut);
            else
                    printBin(pApiInfo->ApiDataOut.Virtual, ulSzDataOut, 16, 8);
            pDBGCtrlOut = (PBIOS_API_CTRL_OUT)pApiInfo->ApiCtrl.Virtual;
            PDEBUG("[BiosApiControl] - ucStatus: 0x%x, usErrorCode 0x%x\n", pDBGCtrlOut->ucStatus, pDBGCtrlOut->usErrorCode);
        }
#endif

        if (usStatus == DRV_READY)
        {
            if (0 != copy_to_user(pCtrlOut, pApiInfo->ApiCtrl.Virtual, ulSzCtrlOut))
            {
                PDEBUG("[BiosApiControl] - error on calling copy_to_user\n");
                usStatus = DRV_ERROR;
                goto Leave_SkipCall;
            }

            if (pDataOut && ulSzDataOut)
            // If big memory block has to be passed to BIOS API function
            {
                if (ulUseMap)
                {
                    usStatus = BiosApiCopyDataFromMap(&(pApiInfo->MapOut), pDataOut, ulSzDataOut);
                }
                else
                {

                    if (0 != copy_to_user(pDataOut, pApiInfo->ApiDataOut.Virtual, ulSzDataOut))
                    {
                        PDEBUG("[BiosApiControl] - error on calling copy_to_user\n");
                        usStatus = DRV_ERROR;
                        goto Leave_SkipCall;
                    }
                }
            } // if (pDataOut && ulSzDataOut)
        } // if (usStatus == DRV_READY)
    }

Leave_SkipCall:
    PDEBUG("[BiosApiControl] - leaving; Status: %d\n", (LONG)usStatus);
    BiosApiCleanUp(pApiInfo);
    return usStatus;
} // BiosApiControl

static USHORT BiosApiCall(PBIOS_API_INFO pApiInfo, bapi_addr_t physCtrl, bapi_addr_t physDataIn, bapi_addr_t physDataOut)
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:       07.06.2001
    Purpose:
    Changes:
    16.02.2004, PT: Added Support for 64 bit
******************************************+*****************************************/
{
    typedef void (*PFN_BAPI)(void/*PVOID pIn, PVOID pOut, PVOID pCtl*/);
    PFN_BAPI func;

    PUCHAR  BaseAddress;
    void    *BapiFuncExecutable = NULL;
#ifndef MY_CONFIG_XEN
    BOOL    fApicModeEnabled = FALSE;
    PUCHAR  pLocalApicBaseAddr = NULL;
    ULONG   ulIoApicPtr = 0;
#define LOCAL_APIC_ADDR        0xFEE00000
#define LOCAL_APIC_TASK_PRIORITY_REG    0x80
#endif


    // Bapi supports no physical addresses above 4GB, so we can truncate them here

    unsigned long pCtrl = (unsigned long)physCtrl;
    unsigned long pDataIn = (unsigned long)physDataIn;
    unsigned long pDataOut = (unsigned long)physDataOut;

    PDEBUG("[BiosApiCall] - entered, Ctrl=0x%p, DataIn=0x%p, DataOut=0x%p\n", (void*)pCtrl, (void*)pDataIn, (void*)pDataOut);
    BaseAddress = ioremap(BIOS_START_ADDRESS, BIOS_MAP_LENGTH);
    if (!BaseAddress)
    {
        PDEBUG("[BiosApiCall] - error on allocationg BIOS segment\n");
        return DRV_ERROR;
    }

#if !defined(__LP64__)
    func = (PFN_BAPI)(BaseAddress + pApiInfo->usApi32Ofs);
#else
    func = (PFN_BAPI)(BaseAddress + pApiInfo->usApi64Ofs);
#endif

#ifndef MY_CONFIG_XEN
    if (pApiInfo->usVersion <= 0x0101)
    {
        fApicModeEnabled = ApicCheckEnabled();

        if (fApicModeEnabled)
        {
            //Map APIC register to virtual address space
	    pLocalApicBaseAddr = ioremap(LOCAL_APIC_ADDR, 0x400);
            if (!pLocalApicBaseAddr) 
            {
                PDEBUG("[BiosApiCall] - pLocalApicBaseAddr == NULL !!!\n");
                return DRV_ERROR;
            }

            //Save APIC Task Priority register
            ulIoApicPtr = *(PULONG)(pLocalApicBaseAddr + (ULONG) LOCAL_APIC_TASK_PRIORITY_REG );
            
            //Set to Higher than high, i.e. "cli"
            *(PULONG)(pLocalApicBaseAddr + LOCAL_APIC_TASK_PRIORITY_REG) |= 0xFF;
        }
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // NX-Bit Fix
    BapiFuncExecutable = VMALLOC_EXEC(4096);
    if (BapiFuncExecutable) {
        PDEBUG("[BiosApiCall] - func (ori)  = 0x%p\n",   func);
        PDEBUG("[BiosApiCall] - BapiFuncExecutable    = 0x%p\n",   BapiFuncExecutable);
        memcpy(BapiFuncExecutable, (void *)func, 4096);
        func = (PFN_BAPI)BapiFuncExecutable;
    }
    ///////////////////////////////////////////////////////////////////////////

    //
    // Do the BIOS API call
    //
#    if defined(__LP64__)
    asm(
        "pushq %%rax\n\t"
        "pushq %%rbx\n\t"
        "pushq %%rcx\n\t"
        "call *%%rdx\n\t"
        "popq %%rcx\n\t"
        "popq %%rbx\n\t"
        "popq %%rax"
        : // no output parameters
        : "a" (pCtrl), "b" (pDataOut), "c" (pDataIn), "d" (func)
    );
#    else
    asm(
        "push %%eax\n\t"
        "push %%ebx\n\t"
        "push %%ecx\n\t"
        "call *%%edx\n\t"
        "pop %%ecx\n\t"
        "pop %%ebx\n\t"
        "pop %%eax"
        : // no output parameters
        : "a" (pCtrl), "b" (pDataOut), "c" (pDataIn), "d" (func)
    );
#    endif

#ifndef MY_CONFIG_XEN
    if (pApiInfo->usVersion <= 0x0101)
    {
        if (fApicModeEnabled)
        {
            //Restore APIC Task Priority register
            *(PULONG)(pLocalApicBaseAddr + LOCAL_APIC_TASK_PRIORITY_REG) = ulIoApicPtr;

            //Unmap IOSpace
           if (pLocalApicBaseAddr) iounmap(pLocalApicBaseAddr);

           PDEBUG("[BiosApiCall] - Local APIC address unmapped\n");
        }
    }
#endif

    if (BaseAddress) iounmap(BaseAddress);
    vfree(BapiFuncExecutable);
    PDEBUG("[BiosApiCall] - leave\n");
    return DRV_READY;
} // BiosApiCall

