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
                                 D R S O B G L O B A L . H


    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT
	Changes:    renamed to drsobglobal.h to distinguish it for GPL_Drivers 2014-07-29
******************************************+*****************************************/

/**
   @file

   <<purpose>>

   @author  Fujitsu Technology Solutions / PT
   @date    03.02.2004
   @version 1.30
   @sa
*/
#ifndef __SOBGLOBAL_H__
#define __SOBGLOBAL_H__

#include <linux/ioctl.h>
#include <linux/types.h>

/*
 *  ioctl() magic numbers
 */

#define BIOS_API_IOC_MAGIC 'B'    /* choosen arbitrarily */


#define MAX_WRITE_CMOS_DATA         128
#define MAX_READ_CMOS_DATA          128
#define MAX_NVMEDIA					32

#define IOCTL_DRV_CONNECT           _IO(BIOS_API_IOC_MAGIC,  1)
#define IOCTL_DRV_DISCONNECT        _IO(BIOS_API_IOC_MAGIC,  2)
#define IOCTL_DRV_GET_STATUS        _IO(BIOS_API_IOC_MAGIC,  3)
#define IOCTL_DRV_GETVERSION        _IO(BIOS_API_IOC_MAGIC,  4)
#define IOCTL_DRV_READ_CMOS         _IO(BIOS_API_IOC_MAGIC,  5)
#define IOCTL_DRV_WRITE_CMOS        _IO(BIOS_API_IOC_MAGIC,  6)
#define IOCTL_DRV_GET_SYSINFO       _IO(BIOS_API_IOC_MAGIC,  7)
#define IOCTL_DRV_TEST              _IO(BIOS_API_IOC_MAGIC,  8)

#define IOCTL_DRV_BAPI_CALL         _IO(BIOS_API_IOC_MAGIC,  9)
#define IOCTL_BAPI_GET_BAPI_VERSION _IO(BIOS_API_IOC_MAGIC, 10)

#define IOCTL_DRV_BIOS_DATA_READ    _IO(BIOS_API_IOC_MAGIC, 14)
#define IOCTL_DRV_PROC_INFO_GET     _IO(BIOS_API_IOC_MAGIC, 15)

#define IOCTL_DRV_GABI_CALL         _IO(BIOS_API_IOC_MAGIC, 16)
#define IOCTL_GABI_GET_GABI_VERSION _IO(BIOS_API_IOC_MAGIC, 17)


enum e_snifstatus
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
   DRV_READY = 0,               // Driver is ready

   DRV_ERROR,                   // General error
   DRV_BUSY,                    // Driver is busy, try again later
   DRV_SECTOR_NOT_OPEN,         // Sector hasn't been opened before
                                 // ReadSector or WriteSector calls.
   DRV_SECTOR_STILL_OPEN,       // There is a sector still open, close it
   DRV_SECTOR_STILL_ERASING,    // There is a sector marked as erasing,
                                 // use GetStatus in order to check busy state
   DRV_SECTOR_STILL_WRITING,    // Sector is still in programming mode
   DRV_SECTOR_PROTECTED,        // Sector is write or erase protected
   DRV_UNKNOWN_DEVICE,          // No known device according to this board
   DRV_UNKNOWN_BOARD,           // This board is not from SNI
   DRV_TIMEOUT,                 // Last operation caused a time out
   DRV_DMI_DATA_ERROR,          // Error while reading DMI data
   DRV_CCR_DATA_ERROR,          // Error while searching chipcard data
   DRV_INSUFFICIENT_RESOURCES,  // Could not allocate resources
   DRV_LOCATION_NOT_WRITEABLE,  // Sector must be erased befor writing
   DRV_ACCESS_DENIED,           // Hardware is locked by another SNI program
   DRV_NO_CONNECTION,           // Could not connect to driver
   DRV_DRIVER_NOT_LOADED,       // Hardware driver not loaded
   DRV_WRONG_DRIVER_VERSION,    // Driver version does not fit
   DRV_API_DATA_ERROR,          // BIOS API header in BIOS segment not found or corrupted
   DRV_LOCKING_ERROR,           // Error on locking of non pageable memory

   DRV_CANNOT_OPEN_SERVICE,     // Service (NT) could not be opened
   DRV_CANNOT_OPEN_SERVICE_MANAGER, // Service manager (NT) could not be opened
   DRV_WINDOWS_NT_REQUIRED,     // Windows NT 4.0 is required
   DRV_MP_DATA_ERROR,           // Multi processor information error
   DRV_WRITE_PROTECTED_DEVICE,  // Flash device is write protected
   DRV_INVALID_PARAMETERS,      // Invalid parameters for calling the I/O function
};

/////////////////////////////////////////////////////////////////////////////////////
enum e_TxTStates
{
    txtInactive = 0,
    txtActive,
    txtNotSupported
};

typedef enum e_TxTStates TXT_STATUS, *PTXT_STATUS;
/////////////////////////////////////////////////////////////////////////////////////

#define FLASHMEM_PROTECTED      0x0001
#define CHIP_CARD_INSTALLED     0x0002
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
// #pragma pack(1)
#define MAX_CPUID 8
typedef struct tagSystemInfo
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
    Changes:
    02.02.2004, PT: Added dwGtAllowedBiosId, fNvBapiPresent
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    USHORT  wBoardHwConfig;
    ULONG   dwBoardId;
    ULONG   dwBiosId;
    ULONG   dwGtAllowedBiosId;       // Lowest allowed BiosId to downgrade to
    char    szBiosExt[40];            // BIOS extention string
    ULONG   dwCpuId;                  // CPU-ID of executing CPU
    ULONG   dwPlatformId;             // Platform-Id of executing CPU
    ULONG   dwNorthbridgeDeviceId;    // Id of northbridge device (AMD based systems)
    UCHAR   bNorthbridgeDeviceRevision;// Revision of northbridge device (AMD based systems)
    ULONG   dwHypertransportHubId;    // Id of hypertransport io hub device (AMD based systems)
    UCHAR   bHypertransportHubRevision;// Revision of hypertransport io hub device
    USHORT  usBiosApiVer;
    USHORT  usMultiProcVer;
    ULONG   dwCpuSig[MAX_CPUID];      // $$1 Array of CPU signatures from multi processor table
    USHORT  usDmiVer;
    BOOL    fNvBapiPresent;
    char    szFDevName[16];
} __attribute__ ((packed)) SYSTEMINFO, *PSYSTEMINFO;

typedef struct tagDrvConnectPara
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    USHORT usNoRecom;
    UCHAR  ucSmiDelay;
    USHORT usLogging;
    USHORT usIgnoreConnectFail;
} __attribute__ ((packed)) DRV_CONN_PARA, *PDRV_CONN_PARA;

/*============ Request and response data structures for IOCTL's ============*/

/*============ Request and response data structures for IOCTL's ============*/

// *** IOCTL_DRV_CONNECT
typedef struct tagDrvConnect
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        DRV_CONN_PARA ConnectPara;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_CONNECT, *PDRV_CONNECT;

// *** IOCTL_DRV_DISCONNECT
typedef struct tagDrvDisconnect
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT   param;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_DISCONNECT, *PDRV_DISCONNECT;

// *** IOCTL_DRV_GET_STATUS
typedef struct tagDrvGetStatus
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT   usParam;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_GET_STATUS, *PDRV_GET_STATUS;

// *** IOCTL_DRV_GETVERSION
typedef struct tagDrvGetVersion
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT   usParam;
    } Requ;

    struct
    {
        ULONG    dwVersion;
    } Data;
} __attribute__ ((packed)) DRV_GETVERSION, *PDRV_GETVERSION;

// *** IOCTL_DRV_READ_CMOS
typedef struct tagDrvReadCmos
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        ULONG   ulOffset;     // Random read
        ULONG   ulRead;
        ULLONG  pucData;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT  usStatus;
        ULONG   ulRead;                   // Data read from cmos
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_READ_CMOS, *PDRV_READ_CMOS;

// *** IOCTL_DRV_WRITE_CMOS
typedef struct tagDrvWriteCmos
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        ULONG   ulOffset;                  // Random write
        ULONG   ulWrite;                   // Data to write to cmos
        ULLONG  pucData;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usStatus;
        ULONG    ulWritten;                // Data written
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_WRITE_CMOS, *PDRV_WRITE_CMOS;

// *** IOCTL_DRV_GET_SYSINFO
typedef struct tagDrvGetSysInfo
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT   usParam;
    } __attribute__ ((packed)) Requ;

    struct
    {
        SYSTEMINFO sysinfo;
        USHORT   usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_GET_SYSINFO, *PDRV_GET_SYSINFO;

// *** IOCTL_DRV_GET_IMAGE_INFO
typedef struct tagDrvBapiControl
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       07.06.2000
    Changes:
    03.04.2002, PT: Added ulUseMap
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        __u64   pucCtrlIn;      // is a __u64 because it should interfaces directly with the driver
        ULONG   ulSzCtrlIn;

        __u64   pucCtrlOut;     // had we implemented a compatible ioctl routine then we would not need this hack
        ULONG   ulSzCtrlOut;

        __u64   pucDataIn;
        ULONG   ulSzDataIn;

        __u64   pucDataOut;
        ULONG   ulSzDataOut;

        ULONG   ulUseMap;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_BAPI_CONTROL, *PDRV_BAPI_CONTROL;

// *** IOCTL_DRV_TEST
typedef struct tagDrvTest
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT   usParam;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usParam;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_TEST, *PDRV_TEST;

// *** IOCTL_DRV_BIOS_DATA_READ
typedef struct tagDrvBiosDataRead
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
		ULLONG	pucData;
        ULONG   ulAddress;
        ULONG   ulSzData;
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT   usStatus;
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_BIOS_DATA_READ, *PDRV_BIOS_DATA_READ;

// *** IOCTL_DRV_PROC_INFO_GET
typedef struct tagDrvProcInfoGet
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
	Date:		DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        USHORT usParam;             // Dummy parameter
    } __attribute__ ((packed)) Requ;

    struct
    {
        USHORT  usStatus;
        char    szFamily[13];       // "GenuineIntel", "AuthenticAMD", ...
        ULONG   ulCpuId;            // CPU-id of the running CPU
        ULONG   ulPlatformId;       // Platform-ID of the running CPU
    } __attribute__ ((packed)) Data;
} __attribute__ ((packed)) DRV_PROC_INFO_GET, *PDRV_PROC_INFO_GET;

typedef struct tagFlashInfoItem
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       04.02.2004
******************************************+*****************************************/
{
    ULONG             ulStartAddress;       // Start address (offset in PROM-image) of first Flashsector hosting the area.
    ULONG             ulBlockSize;          // Number of bytes per block.
    // 0FFFFFFFFh	Marker for End-Of-Table
} __attribute__ ((packed)) FLASH_INFO_ITEM, *PFLASH_INFO_ITEM;


typedef struct tagDrvFlashInfoGetData
/************************************************************************************
   Purpose: 
   Changes:
 ***********************************************************************************/
{
   USHORT           usStatus;
   ULONG            ulBlockMap;
   FLASH_INFO_ITEM  Data[33];   // Not more than 32 blocks possible by now (+sentinel)
} __attribute__ ((packed)) DRV_FLASH_INFO_GET_DATA, *PDRV_FLASH_INFO_GET_DATA;

/************************************************************************************
    Author:     Fujitsu Technology Solutions / BU
    Date:       10.12.2009
************************************************************************************/
typedef struct tagBiosApi0WattManageabilityInterval 
{
    unsigned short          usAllocatedLength;
    unsigned char           ucFeature;
    union 
    {
        unsigned char       uc[3];
        struct 
        {
            unsigned char   ucSeconds;
            unsigned char   ucMinutes;
            unsigned char   ucHours;
        } __attribute__ ((packed)) s;
    } start;
    union 
    {
        unsigned char       uc[3];
        struct 
        {
            unsigned char   ucSeconds;
            unsigned char   ucMinutes;
            unsigned char   ucHours;
        } __attribute__ ((packed)) s;
    } stop;
} __attribute__ ((packed)) BIOS_API_0WATT, *PBIOS_API_0WATT;

/************************************************************************************
    Author:     Fujitsu Technology Solutions / BU
    Date:       10.12.2009
************************************************************************************/
typedef struct tagBiosApi0WattGetDeviceInfo 
{
    unsigned short          usAllocatedLength;
    unsigned long           ulSerialNumber;
    unsigned char            ucFirmwareRev;
    unsigned char            ucBuild;
    unsigned char            ucCapabilites;
    unsigned char            ucBootLoaderFirmRev;
} __attribute__ ((packed)) BIOS_API_0WATT_GDI, *PBIOS_API_0WATT_GDI;

// #pragma pack()

struct inode;
struct file;


typedef struct tokenInfo
{
	USHORT     usAllocatedLength;   // Size of buffer allocated by the application

	BYTE       byMediaType;			// 0 - Standard CMOS
									// 1 - NVRAM 1
									// 2 - MVRAM 2
									// ...

	BYTE       byByteString;		// 0 - Token is a bit array (not a byte string)
									// 1 - Token is a byte string

	BYTE       byChecksummed;		// 0 - Token is NOT checksummed.
									// 1 - Token is checksummed.

	BYTE       byCRCChecksummed;	// Neither CRC nor checksummed.
									// 0 - Token is NOT a checksum field of CRC field.
									// 1 - Token is a checksum field or CRC field.

	USHORT     usStartBitPosition;	// Starting bit position inside the NVRAM.

	BYTE       byTokenWidth;		// Bit or byte width.
									// If token is a bit array:    0=1Bit, 1=2Bit, ...
									// If token is a byte string:  0=1Byte, 1=2Byte, ...

}__attribute__ ((packed)) TOKEN_INFO, *PTOKEN_INFO;




/*----------------------------------------+------------------------------------------
                           d a t a  s t r u c t u r e s
-----------------------------------------------------------------------------------*/
#endif /* __SOBGLOBAL_H__ */
