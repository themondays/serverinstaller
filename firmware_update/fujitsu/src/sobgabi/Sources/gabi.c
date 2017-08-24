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
				  G A B I . C

	Project:    DeskFlash for Linux
	Created:    Fujitsu Technology Solutions
	Purpose:
	Version:    1.70
******************************************+*****************************************/

#include <linux/version.h>          /* because we need conditional compiling */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)

/* This include was removed in Kernel. 3.4 */
#include <asm/system.h>

#endif

#include <linux/uaccess.h>		 /* copy_to_user(), copy_from_user() */
#include <asm/io.h>
#include <linux/dma-mapping.h>
#include <linux/kernel.h>

#ifdef virt_to_bus
# define v2p virt_to_bus
# define p2v bus_to_virt
#else
# define v2p virt_to_pyhs
# define p2v phys_to_virt
#endif

#include <linux/page-flags.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/pci.h>

#include "drvstddef.h"			 /* common types USHORT, ULONG, ... */
#include "drsobglobal.h"

#include "sobdev.h"
#include "gabimemory.h"
#include "sobgabi.h"
#include "sobmain.h"

#ifdef virt_to_machine
/* for XEN */
#define USE_DMA_COHERENT
// Temporary fix for RH5.7 == Kernel 2.6.18-274.el5xen
#if  (LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 18))
//
// from x86_dma_fallback_dev in arch/x86/kernel/pci-dma.c
//
static struct device my_x86_dma_fallback_dev = {
        .bus_id = "gabi_device",
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
# define v2m(x) ((gabi_addr_t)virt_to_machine(x))
#else
# define v2m(x) ((gabi_addr_t)virt_to_bus(x))
#endif

#ifndef GFP_DMA32	// not defined on RH4.8 32 bit, but not needed either (and on pae ? )
#define GFP_DMA32 0
#endif


#if 0
void DumpIt(char *s, UEFI_GabiResponse_T *r)
{
        printk("%s: ServiceCategory=%x; Service=%x; Status=%x; ErrorCode=%x\n", s,
                r->Header.ServiceCategory, r->Header.Service, r->Header.Status, r->Header.ErrorCode);
}
#endif

static unsigned u_min(unsigned i, unsigned j)
{
        return (i<j) ? i : j;
}

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
    // under XEN we have to use the dma api to get physical memory below 4GB
    dma_addr_t dummy;
    if (ulSpan > (PAGE_SIZE<<(MAX_ORDER-1))) return NULL;
    pVirt = dma_alloc_coherent(MY_DMA_DEVICE, ulSpan, &dummy, GFP_DMA);
#else
    if (ulSpan > (PAGE_SIZE<<(MAX_ORDER-1))) return NULL;
    // without XEN ist is better to use __get_free_pages because the dma api adds additional constrains that are not needed.
    // kmalloc does not recognise GFP_DMA32, only GFP_DMA, we would get memory <16Mbyte.
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

/**************************************************************************************************************
 * Most integer functions follow the unix convention that return values != 0 denote failure and 0 means success.
 **************************************************************************************************************/
static int AllocateContiguousBuffer(DriverBufferDescriptor_T * pB, unsigned Size)
{
	Size = (Size < 1024) ? 1024 : Size;
	pB->pVirtual = my_kmalloc(Size);
	if (pB->pVirtual == NULL)
		return 1;
	pB->Physical = v2m(pB->pVirtual);
	pB->uSize = Size;
	memset(pB->pVirtual, 0, pB->uSize);
	return 0;
}

typedef enum { FLASH_UPDATE, FLASH_ARCHIVE, READ_SYS_DATA, WRITE_SYS_DATA,
               OTHERS_UEFI, UEFI_8205,
               OTHERS } Command_T;

/* what type of command ist this, used to determine wether and how a descriptor list should be build */
static Command_T Command(UEFI_GabiHeader_T *r)
{
	if (r->ServiceCategory == 3)		        // Japan FLASH
		switch (r->Service)
		{
		    case 3: return FLASH_UPDATE;
		    case 4: return FLASH_ARCHIVE;
		    default: return OTHERS;
		}

	if (r->ServiceCategory == 0x8000 || r->ServiceCategory == 0x5)		// Japan SYSTEM DATA
		switch (r->Service)
		{
		    case 4: return READ_SYS_DATA;
		    case 5: return WRITE_SYS_DATA;
		    default: return OTHERS;
		}

	if   ((r->ServiceCategory & 0xfeff)== 0x8205)     // Catch for service 8205 because it will be treated differently
		return UEFI_8205;	// All UEFI commands use buffer descriptors when more then 0x10 data
		
        if ((r->ServiceCategory == 7) ||                // UEFI NVRAM data
	    (r->ServiceCategory & 0xfe00)== 0x8200)	// Assume these are all UEFI services
		return OTHERS_UEFI;	// All UEFI commands use buffer descriptors when more then 0x10 data
		
	return OTHERS;
}

static void FreeBuffers(DriverBufferDescriptor_T *p)
{
	if (p)
	{
		DriverBufferDescriptor_T *temp = p;
		while (p->uSize)
		{
			my_kfree(p->pVirtual, p->uSize);
			p++;
		}
		kfree(temp);
	}
}

static DriverBufferDescriptor_T * BuildDescriptorList(PVOID pInOutBuffer, unsigned uSize, unsigned StupidSize)
{
	const unsigned uMaxSegCount = 4096/16-2;	// max # of descriptors in *pInOutBuffer
	unsigned uSegmentCount = 0;
	char *pGabiDescr = (char*)pInOutBuffer;	// the gabi memory descriptors in in- or out buffer
	DriverBufferDescriptor_T *p;

	unsigned uSegSize = uSize;
	unsigned uAllocSize = uMaxSegCount*sizeof(DriverBufferDescriptor_T);

	DriverBufferDescriptor_T *pMyDescr = (DriverBufferDescriptor_T*)kmalloc(uAllocSize, GFP_KERNEL);
	if (!pMyDescr) return NULL;

	memset(pMyDescr, 0, uAllocSize);

	p = pMyDescr;

	while (uSize)
	{
		unsigned s = u_min(uSegSize, uSize);
		void *a = my_kmalloc(s);
		if (!a)
		{
			uSegSize = uSegSize/2;
			if (uSegSize < PAGE_SIZE)
			{
				FreeBuffers(pMyDescr);
				return NULL;
			}
			uSegSize = PAGE_ALIGN(uSegSize);
			continue;
		}
		p->uSize = s;
		p->pVirtual = a;
		p->Physical = v2m(a);

		PDEBUG( "[BuildDescriptorList]  p->uSize = 0x%lX, p->pVirtual = %p, p->Physical = %lX\n",
		        (unsigned long)s, (void*)a, (unsigned long)p->Physical );

		*(__u64*)pGabiDescr = p->Physical;
		pGabiDescr += sizeof(__u64);
		// 2 variants of the length field DWORD or QWORD
		if (StupidSize == 8)
		{
			*(__u64*)pGabiDescr = s;
			pGabiDescr += 8;
		}
		else /* assume 4 */
		{
			*(u32*)pGabiDescr = s;
			pGabiDescr += 4;
		}

		p++;
		uSize -= s;
		uSegmentCount++;
		// No more space for descriptors ?
		if ((uSegmentCount > uMaxSegCount) && uSize)
		{
			FreeBuffers(pMyDescr);
			return NULL;
		}
	}
	// pMyDescr was cleared so no need for terminating
	// Terminate Gabi descriptor list
	*(__u64*)pGabiDescr = 0;

	return pMyDescr;
}

static unsigned CopyFromBuffersToUser(char __user *p, DriverBufferDescriptor_T *d, unsigned Max)
{
	while (d->uSize && Max)
	{
		unsigned c = u_min(Max, d->uSize);
		if (copy_to_user(p, d->pVirtual, c))		// should not occur and
			break;					// count is not really correct, but who cares ?
		Max -= c;
		p += c;
		d++;
	}
	return Max;
}


static unsigned CopyFromBuffersToUserEx ( UEFI_GabiBufferDescriptor_T __user *u , DriverBufferDescriptor_T *d, unsigned Max )
{
	PDEBUG( "[CopyFromBuffersToUser] --------------------------------------------------------\n" );

	__u64  ou = 0;    // offset in current user buffer
	__u64  od = 0;    // offset in current driver buffer

	while ( Max > 0 )
	{
		PDEBUG( "Max = 0x%X\n", Max );
		PDEBUG( "u->PhAddress = 0x%lX, d->Physical = 0x%lX\n",
		        (long unsigned int)u->PhAddress, d->Physical );

		if ( 0 == u->PhAddress || 0 == d->Physical )    break;    // at end of buffer chunks

		// Compute the remaining sizes within each buffer.
		__u64  ru = u->Size - ou;
		PDEBUG( "ru = 0x%lX\n", (long unsigned int)ru );
		if ( 0 == ru )    { u++; ou=0; continue; }    // nothing left -> next buffer and try again
		
		__u32  rd = d->uSize - od;
		PDEBUG( "rd = 0x%X\n", rd );
		if ( 0 == rd )    { d++; od=0; continue; }    // ... the same here


		// Now compute the amount we may copy from/to one of those buffers.
		unsigned Count = _min( ru, rd );

		// Still, this might be too much if Max is less.
		Count = _min( Max, Count );
		PDEBUG( "Count = 0x%X\n", Count );

		// Now do the copying.
		const char * src = (const char*)&d->pVirtual[od];
		      char * tgt = (char*)(u->PhAddress + ou);

		if ( copy_to_user(tgt,src,Count) )    // should not occur ...
			break;

		// Adjust the offsets.
		// Here, either Max falls to 0, or one (or both) of the buffers are exhausted.
		ou  += Count;
		od  += Count;
		Max -= Count;

	} // while bytes to copy left

	PDEBUG( "[CopyFromBuffersToUser] ---- (returning %X) ------------------------------------\n", Max );
	return Max;

} // CopyFromBuffersToUserEx ( UEFI_GabiBufferDescriptor_T __user *u , DriverBufferDescriptor_T *d, unsigned Max )


static unsigned CopyFromUserToBuffers(DriverBufferDescriptor_T *d, char __user * p, unsigned Max)
{
	PDEBUG( "[CopyFromUserToBuffers] --------------------------------------------------------\n" );
	while (d->uSize && Max)
	{
		unsigned i = 0;
		unsigned c = u_min(Max, d->uSize);
		PDEBUG( "[CopyFromUserToBuffers]  d->uSize = %X, Max = %X\n", d->uSize, Max);
		PDEBUG( "[CopyFromUserToBuffers]  copying %X bytes\n", c );

		if (copy_from_user(d->pVirtual, p, c))		// should not occur and
			break;					// count is not really correct, but who cares ?
		Max -= c;
		p += c;
		d++;
	}
	return Max;
}



static unsigned CopyFromUserToBuffersEx ( DriverBufferDescriptor_T * d, UEFI_GabiBufferDescriptor_T __user * u, unsigned Max )
{
	PDEBUG( "[CopyFromUserToBuffersEx] --------------------------------------------------------\n" );

	__u64  ou = 0;    // offset in current user buffer
	__u64  od = 0;    // offset in current driver buffer

	while ( Max > 0 )
	{
		PDEBUG( "Max = 0x%X\n", Max );
		PDEBUG( "u->PhAddress = 0x%lX, d->Physical = 0x%lX\n",
		        (long unsigned int)u->PhAddress, d->Physical );

		if ( 0 == u->PhAddress || 0 == d->Physical )    break;    // at end of buffer chunks

		// Compute the remaining sizes within each buffer.
		__u64  ru = u->Size - ou;
		PDEBUG( "ru = 0x%lX\n", (long unsigned int)ru );
		if ( 0 == ru )    { u++; ou=0; continue; }    // nothing left -> next buffer and try again
		
		__u32  rd = d->uSize - od;
		PDEBUG( "rd = 0x%X\n", rd );
		if ( 0 == rd )    { d++; od=0; continue; }    // ... the same here


		// Now compute the amount we may copy from/to one of those buffers.
		unsigned Count = _min( ru, rd );

		// Still, this might be too much if Max is less.
		Count = _min( Max, Count );
		PDEBUG( "Count = 0x%X\n", Count );

		// Now do the copying.
		const char * src = (const char *)(u->PhAddress + ou);
		char       * tgt = (char*)&d->pVirtual[od];

		if ( copy_from_user(tgt,src,Count) )    // should not occur ...
			break;

		// Adjust the offsets.
		// Here, either Max falls to 0, or one (or both) of the buffers are exhausted.
		ou  += Count;
		od  += Count;
		Max -= Count;

	} // while bytes to copy left

	PDEBUG( "[CopyFromUserToBuffersEx] ---- (returning %X) ------------------------------------\n", Max );
	return Max;

} // CopyFromUserToBuffersEx ( DriverBufferDescriptor_T * d, UEFI_GabiIO_T __user * u, unsigned Max )

int GabiExecute(UEFI_GabiRequest_T __user *userIn, unsigned in_size, UEFI_GabiResponse_T __user *userOut, unsigned out_size, unsigned *BytesCount)
{
	int rv = -EFAULT;
	DriverBufferDescriptor_T Control, In, Out;
	DriverBufferDescriptor_T *pInDescriptorList = NULL;
	DriverBufferDescriptor_T *pOutDescriptorList = NULL;

	PDEBUG("[GabiExecute] ----------------------------------------------------------------------------------------\n");

	if (in_size < sizeof(userIn->Header) || out_size < sizeof(userOut->Header)) return -EINVAL;


	for (;;)
	{
		UEFI_GabiHeader_T *pHeader;		// pointers to request/response in kernel space
		UEFI_GabiIO_T *pInBuffer, *pOutBuffer;
		unsigned i;
		int ComputedUEFIRequestDataSize = 0;
		int ComputedUEFIResponseDataSize = 0;
		if (AllocateContiguousBuffer(&Control, 1024) ||
		    AllocateContiguousBuffer(&In, PAGE_SIZE) ||
		    AllocateContiguousBuffer(&Out, PAGE_SIZE))
		{
			rv = -ENOMEM;
			break;
		}

		// fill the control buffer
		*(__u16*)Control.pVirtual = (__u16)Control.uSize;
		pHeader = (UEFI_GabiHeader_T*)(Control.pVirtual+2);
		if (copy_from_user(pHeader, &userIn->Header, sizeof(userIn->Header))) break;

		// Handle standard fields of output buffer, size is the only field we do here
		pOutBuffer = (UEFI_GabiIO_T*)Out.pVirtual;
		pOutBuffer->BufferSize = (__u16)Out.uSize;

		// Handle standard fields of input buffer
		pInBuffer = (UEFI_GabiIO_T*)In.pVirtual;
		// user does not know the buffer size
		pInBuffer->BufferSize = (__u16)In.uSize;

		switch (Command(pHeader))
		{
		    case FLASH_UPDATE:
		    case FLASH_ARCHIVE:
			rv = -EINVAL;
			break;
			
		    case OTHERS_UEFI:
		    PDEBUG( "in OTHERS_UEFI" );
			if (in_size < sizeof(*userIn) || out_size < sizeof(*userOut)) { rv = -EINVAL; break; }

			if (copy_from_user(&pInBuffer->StructureVersion, &userIn->Request.StructureVersion,
							sizeof(userIn->Request.StructureVersion)+
							sizeof(userIn->Request.TransactionHandle)+
							sizeof(userIn->Request.DataCount))) break;
			ComputedUEFIRequestDataSize = in_size - sizeof(*userIn);
		    PDEBUG( "    ComputedUEFIRequestDataSize = 0x%X\n", ComputedUEFIRequestDataSize );
			// build descriptors				
			if (ComputedUEFIRequestDataSize && pInBuffer->DataCount)
			{
				pInDescriptorList = BuildDescriptorList(&pInBuffer->DescriptorArray[0], ComputedUEFIRequestDataSize, 8);
				if (!pInDescriptorList) { rv = -ENOMEM; break; }
				if (CopyFromUserToBuffers(pInDescriptorList,
							  (char __user *)&userIn->Request.DescriptorArray[0],
							  ComputedUEFIRequestDataSize))
					break;

			}
			ComputedUEFIResponseDataSize = out_size - sizeof(*userOut);
			if (ComputedUEFIResponseDataSize)
			{
				pOutDescriptorList = BuildDescriptorList(&pOutBuffer->DescriptorArray[0], ComputedUEFIResponseDataSize, 8);
				if (!pOutDescriptorList) { rv = -ENOMEM; break; }
			}
			rv = 0;
			break;

		    case UEFI_8205:
		    PDEBUG( "in UEFI_8205" );
			if (in_size < sizeof(*userIn) || out_size < sizeof(*userOut)) { rv = -EINVAL; break; }

			if (copy_from_user(&pInBuffer->StructureVersion, &userIn->Request.StructureVersion,
							sizeof(userIn->Request.StructureVersion)+
							sizeof(userIn->Request.TransactionHandle)+
							sizeof(userIn->Request.DataCount))) break;
			ComputedUEFIRequestDataSize = in_size - sizeof(*userIn);
		    PDEBUG( "    ComputedUEFIRequestDataSize = 0x%X\n", ComputedUEFIRequestDataSize );
			// build descriptors				
			//	if (ComputedUEFIRequestDataSize && pInBuffer->DataCount)
			if ( pInBuffer->DataCount )
			{
				pInDescriptorList = BuildDescriptorList( &pInBuffer->DescriptorArray[0], pInBuffer->DataCount, 8);
				if (!pInDescriptorList) { rv = -ENOMEM; break; }
				if (CopyFromUserToBuffersEx( pInDescriptorList,
					                         userIn->Request.DescriptorArray,
				                             pInBuffer->DataCount))
				{
					break;
				}

			}

			if (copy_from_user(&pOutBuffer->StructureVersion, &userOut->Response.StructureVersion,
							sizeof(userOut->Response.StructureVersion)+
							sizeof(userOut->Response.TransactionHandle)+
							sizeof(userOut->Response.DataCount))) break;
			ComputedUEFIResponseDataSize = pOutBuffer->DataCount;
			//	if ( 0 == ComputedUEFIResponseDataSize )    ComputedUEFIResponseDataSize = 64 * 1024;    // MUST BE MinBufSize from service 1!!!
		    PDEBUG( "    ComputedUEFIResponseDataSize = 0x%X\n", ComputedUEFIResponseDataSize );
			if (ComputedUEFIResponseDataSize)
			{
				pOutDescriptorList = BuildDescriptorList(&pOutBuffer->DescriptorArray[0], ComputedUEFIResponseDataSize, 8);
				if (!pOutDescriptorList) { rv = -ENOMEM; break; }
			}
			rv = 0;
			break;

		    case WRITE_SYS_DATA:
			// copy input buffer from user, skip over BufferSize field
			i = u_min(in_size - sizeof(userIn->Header), In.uSize);
			if (copy_from_user(In.pVirtual+2, &userIn->Request.StructureVersion, i - 2))
			        break;
			{
				// copied from windows driver
				unsigned WriteDataSize = in_size - sizeof(userIn->Header) - 0x14;
				pInDescriptorList = BuildDescriptorList(In.pVirtual + 0x14, WriteDataSize, 4);
				if (!pInDescriptorList) { rv = -ENOMEM; break; }
				if (copy_from_user(In.pVirtual + 2, &userIn->Request.StructureVersion, 0x12)) break;
				if (CopyFromUserToBuffers(pInDescriptorList, In.pVirtual + 0x14, WriteDataSize)) break;
			}
			rv = 0;
			break;

		    case READ_SYS_DATA:
			// copy input buffer from user, skip over BufferSize field
			i = u_min(in_size - sizeof(userIn->Header), In.uSize);
			if (copy_from_user(In.pVirtual+2, &userIn->Request.StructureVersion, i - 2))
			        break;
			{
				unsigned ReadDataSize = out_size - sizeof(userOut->Header) - 0xc;
				pOutDescriptorList = BuildDescriptorList(Out.pVirtual + 0xc, ReadDataSize, 4);
				if (!pOutDescriptorList) { rv = -ENOMEM; break; }
			}
			rv  = 0;
			break;

		    case OTHERS:
			// copy input buffer from user, skip over BufferSize field
			i = u_min(in_size - sizeof(userIn->Header), In.uSize);
			if (copy_from_user(In.pVirtual+2, &userIn->Request.StructureVersion, i - 2)) break;
			rv = 0;
			break;
		}
		if (rv) break;
		rv = -EFAULT;	// reset to default error

// DumpIt(("B4 SMI", Control.pVirtual+2));

		//
		// Do the GABI call
		//
#if defined(__LP64__)
		asm(
			"pushq %%rax\n\t"
			"pushq %%rbx\n\t"
			"pushq %%rcx\n\t"
			"call *%%rdx\n\t"
			"popq %%rcx\n\t"
			"popq %%rbx\n\t"
			"popq %%rax"
			:						 // no output parameters
		        : "a" (Control.Physical), "b" (Out.Physical), "c" (In.Physical), "d" (g_pEP)
			);
#else
		asm(
			"push %%eax\n\t"
			"push %%ebx\n\t"
			"push %%ecx\n\t"
			"call *%%edx\n\t"
			"pop %%ecx\n\t"
			"pop %%ebx\n\t"
			"pop %%eax"
			:						 // no output parameters
		        : "a" (Control.Physical), "b" (Out.Physical), "c" (In.Physical), "d" (g_pEP)
			);
#endif

// DumpIt(("After SMI", Control.pVirtual+2));

		// copy control buffer
		if (copy_to_user( &userOut->Header, pHeader, sizeof(*pHeader))) break;
		*BytesCount = sizeof(*pHeader);
		switch (Command(pHeader))
		{		
		    case OTHERS_UEFI:
			if (copy_to_user(&userOut->Response, pOutBuffer, sizeof(userOut->Response))) break;
			*BytesCount += sizeof(userOut->Response);
			// SIZE ??
			if (pOutDescriptorList)
				if (CopyFromBuffersToUser((char __user *)&userOut->Response.DescriptorArray[0],
							  pOutDescriptorList,  ComputedUEFIResponseDataSize))
					break;
			*BytesCount += ComputedUEFIResponseDataSize;
			rv = 0;
			break;

		    case UEFI_8205:
		    PDEBUG( "in UEFI_8205 (after GABI)" );
			if (copy_to_user(&userOut->Response, pOutBuffer, sizeof(userOut->Response))) break;
			//	*BytesCount += sizeof(userOut->Response);
			// SIZE ??
			PDEBUG( "userOut->Response.DataCount = 0x%lX\n", (unsigned long)userOut->Response.DataCount );
			if (pOutDescriptorList)
			{
				if (CopyFromBuffersToUserEx( userOut->Response.DescriptorArray,
				                             pOutDescriptorList,
				                             _min(ComputedUEFIResponseDataSize,userOut->Response.DataCount)  ))
				{
					break;
				}
			}
			//	*BytesCount += ComputedUEFIResponseDataSize;
			*BytesCount = userOut->Response.DataCount;
			rv = 0;
			break;

		    case READ_SYS_DATA:
			{
				unsigned ReadDataSize = out_size - sizeof(userOut->Header) - 0xc;
				if (copy_to_user(&userOut->Response, pOutBuffer, 0xc)) break;
				*BytesCount += 0xc;
 				if (CopyFromBuffersToUser(((char __user *)&userOut->Response) + 0xc,
							  pOutDescriptorList,  ReadDataSize))
					break;
				*BytesCount += ReadDataSize;
			}
			rv = 0;
			break;

		    default:	// should not occur
		    case WRITE_SYS_DATA:
		    case OTHERS:
			i = u_min(out_size - sizeof(userOut->Header), Out.uSize);
			if (copy_to_user( &userOut->Response, pOutBuffer, i)) break;
			*BytesCount += i;
			rv = 0;
			break;
		}
		break;
	}

	// cleanup
	if (pInDescriptorList) FreeBuffers(pInDescriptorList);
	if (pOutDescriptorList) FreeBuffers(pOutDescriptorList);
	if (Control.pVirtual) my_kfree(Control.pVirtual, Control.uSize);
	if (In.pVirtual) my_kfree(In.pVirtual, In.uSize);
	if (Out.pVirtual) my_kfree(Out.pVirtual, Out.uSize);

	PDEBUG("[GabiExecute] ----------------------------------------------------------------------------------------\n");
	return rv;
}

