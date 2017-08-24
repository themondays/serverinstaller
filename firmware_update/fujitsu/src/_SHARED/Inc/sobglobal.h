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
                                 S O B G L O B A L . H


    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT
    Changes:
******************************************+*****************************************/

/**
   @file

   <<purpose>>

   @author  Fujitsu Technology Solutions / PT
   @date    03.02.2004
   @version 1.30
   @sa
*/
#pragma once
#include <linux/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include "stdtypes.h"

/*
 *  ioctl() magic numbers
 */

#define BIOS_API_IOC_MAGIC 'B'    /* choosen arbitrarily */


#define MAX_WRITE_CMOS_DATA         128
#define MAX_READ_CMOS_DATA          128
#define MAX_NVMEDIA                    32

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
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
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
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
    Changes:
    02.02.2004, PT: Added dwGtAllowedBiosId, fNvBapiPresent
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    uint16_t     wBoardHwConfig;
    uint32_t     dwBoardId;
    uint32_t     dwBiosId;
    uint32_t     dwGtAllowedBiosId;       // Lowest allowed BiosId to downgrade to
    char         szBiosExt[40];            // BIOS extention string
    uint32_t     dwCpuId;                  // CPU-ID of executing CPU
    uint32_t     dwPlatformId;             // Platform-Id of executing CPU
    uint32_t     dwNorthbridgeDeviceId;    // Id of northbridge device (AMD based systems)
    uint8_t      bNorthbridgeDeviceRevision;// Revision of northbridge device (AMD based systems)
    uint32_t     dwHypertransportHubId;    // Id of hypertransport io hub device (AMD based systems)
    uint8_t      bHypertransportHubRevision;// Revision of hypertransport io hub device
    uint16_t     usBiosApiVer;
    uint16_t     usMultiProcVer;
    uint32_t     dwCpuSig[MAX_CPUID];      // $$1 Array of CPU signatures from multi processor table
    uint16_t     usDmiVer;
    BOOL         fNvBapiPresent;
    char         szFDevName[16];
} _attribute_packed_ SYSTEMINFO, *PSYSTEMINFO;

typedef struct tagDrvConnectPara
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    uint16_t   usNoRecom;
    uint8_t    ucSmiDelay;
    uint16_t   usLogging;
    uint16_t   usIgnoreConnectFail;
} _attribute_packed_ DRV_CONN_PARA, *PDRV_CONN_PARA;

/*============ Request and response data structures for IOCTL's ============*/

/*============ Request and response data structures for IOCTL's ============*/

// *** IOCTL_DRV_CONNECT
typedef struct tagDrvConnect
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        DRV_CONN_PARA ConnectPara;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t   usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_CONNECT, *PDRV_CONNECT;

// *** IOCTL_DRV_DISCONNECT
typedef struct tagDrvDisconnect
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t     param;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_DISCONNECT, *PDRV_DISCONNECT;

// *** IOCTL_DRV_GET_STATUS
typedef struct tagDrvGetStatus
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t     usParam;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_GET_STATUS, *PDRV_GET_STATUS;

// *** IOCTL_DRV_GETVERSION
typedef struct tagDrvGetVersion
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t     usParam;
    } Requ;

    struct
    {
        uint32_t      dwVersion;
    } Data;
} _attribute_packed_ DRV_GETVERSION, *PDRV_GETVERSION;

// *** IOCTL_DRV_READ_CMOS
typedef struct tagDrvReadCmos
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint32_t     ulOffset;     // Random read
        uint32_t     ulRead;
        uint64_t    pucData;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t    usStatus;
        uint32_t     ulRead;                   // Data read from cmos
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_READ_CMOS, *PDRV_READ_CMOS;

// *** IOCTL_DRV_WRITE_CMOS
typedef struct tagDrvWriteCmos
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint32_t     ulOffset;                  // Random write
        uint32_t     ulWrite;                   // Data to write to cmos
        uint64_t    pucData;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usStatus;
        uint32_t      ulWritten;                // Data written
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_WRITE_CMOS, *PDRV_WRITE_CMOS;

// *** IOCTL_DRV_GET_SYSINFO
typedef struct tagDrvGetSysInfo
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t     usParam;
    } _attribute_packed_ Requ;

    struct
    {
        SYSTEMINFO sysinfo;
        uint16_t     usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_GET_SYSINFO, *PDRV_GET_SYSINFO;

// *** IOCTL_DRV_GET_IMAGE_INFO
typedef struct tagDrvBapiControl
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
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
        uint64_t    pucCtrlIn;      // is a __u64 because it should interfaces directly with the driver
        uint32_t    ulSzCtrlIn;

        uint64_t    pucCtrlOut;     // had we implemented a compatible ioctl routine then we would not need this hack
        uint32_t    ulSzCtrlOut;

        uint64_t    pucDataIn;
        uint32_t    ulSzDataIn;

        uint64_t    pucDataOut;
        uint32_t    ulSzDataOut;

        uint32_t    ulUseMap;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_BAPI_CONTROL, *PDRV_BAPI_CONTROL;

// *** IOCTL_DRV_TEST
typedef struct tagDrvTest
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t     usParam;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usParam;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_TEST, *PDRV_TEST;

// *** IOCTL_DRV_BIOS_DATA_READ
typedef struct tagDrvBiosDataRead
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint64_t     pucData;
        uint32_t     ulAddress;
        uint32_t     ulSzData;
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t     usStatus;
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_BIOS_DATA_READ, *PDRV_BIOS_DATA_READ;

// *** IOCTL_DRV_PROC_INFO_GET
typedef struct tagDrvProcInfoGet
/************************************************************************************
    Author:        Fujitsu Technology Solutions / PT
    Date:        DD.MM.YYYY
******************************************+*****************************************/
/**
    <<purpose>>
*/
{
    struct
    {
        uint16_t   usParam;             // Dummy parameter
    } _attribute_packed_ Requ;

    struct
    {
        uint16_t    usStatus;
        char        szFamily[13];       // "GenuineIntel", "AuthenticAMD", ...
        uint32_t    ulCpuId;            // CPU-id of the running CPU
        uint32_t    ulPlatformId;       // Platform-ID of the running CPU
    } _attribute_packed_ Data;
} _attribute_packed_ DRV_PROC_INFO_GET, *PDRV_PROC_INFO_GET;

typedef struct tagFlashInfoItem
/************************************************************************************
    Author:     Fujitsu Technology Solutions / PT
    Date:       04.02.2004
******************************************+*****************************************/
{
    uint32_t               ulStartAddress;       // Start address (offset in PROM-image) of first Flashsector hosting the area.
    uint32_t               ulBlockSize;          // Number of bytes per block.
    // 0FFFFFFFFh    Marker for End-Of-Table
} _attribute_packed_ FLASH_INFO_ITEM, *PFLASH_INFO_ITEM;


typedef struct tagDrvFlashInfoGetData
/************************************************************************************
   Purpose: 
   Changes:
 ***********************************************************************************/
{
   uint16_t             usStatus;
   uint32_t              ulBlockMap;
   FLASH_INFO_ITEM  Data[33];   // Not more than 32 blocks possible by now (+sentinel)
} _attribute_packed_ DRV_FLASH_INFO_GET_DATA, *PDRV_FLASH_INFO_GET_DATA;

/************************************************************************************
    Author:     Fujitsu Technology Solutions / BU
    Date:       10.12.2009
************************************************************************************/
typedef struct tagBiosApi0WattManageabilityInterval 
{
    unsigned short          usAllocatedLength;
    uint8_t           ucFeature;
    union 
    {
        uint8_t       uc[3];
        struct 
        {
            uint8_t   ucSeconds;
            uint8_t   ucMinutes;
            uint8_t   ucHours;
        } _attribute_packed_ s;
    } start;
    union 
    {
        uint8_t       uc[3];
        struct 
        {
            uint8_t   ucSeconds;
            uint8_t   ucMinutes;
            uint8_t   ucHours;
        } _attribute_packed_ s;
    } stop;
} _attribute_packed_ BIOS_API_0WATT, *PBIOS_API_0WATT;

/************************************************************************************
    Author:     Fujitsu Technology Solutions / BU
    Date:       10.12.2009
************************************************************************************/
typedef struct tagBiosApi0WattGetDeviceInfo 
{
    uint16_t    usAllocatedLength;
    uint32_t    ulSerialNumber;
    uint8_t     ucFirmwareRev;
    uint8_t     ucBuild;
    uint8_t     ucCapabilites;
    uint8_t     ucBootLoaderFirmRev;
} _attribute_packed_ BIOS_API_0WATT_GDI, *PBIOS_API_0WATT_GDI;

// #pragma pack()

struct inode;
struct file;


typedef struct tokenInfo
{
    uint16_t   usAllocatedLength;   // Size of buffer allocated by the application

    BYTE       byMediaType;            // 0 - Standard CMOS
                                    // 1 - NVRAM 1
                                    // 2 - MVRAM 2
                                    // ...

    BYTE       byByteString;        // 0 - Token is a bit array (not a byte string)
                                    // 1 - Token is a byte string

    BYTE       byChecksummed;        // 0 - Token is NOT checksummed.
                                    // 1 - Token is checksummed.

    BYTE       byCRCChecksummed;    // Neither CRC nor checksummed.
                                    // 0 - Token is NOT a checksum field of CRC field.
                                    // 1 - Token is a checksum field or CRC field.

    uint16_t   usStartBitPosition;    // Starting bit position inside the NVRAM.

    BYTE       byTokenWidth;        // Bit or byte width.
                                    // If token is a bit array:    0=1Bit, 1=2Bit, ...
                                    // If token is a byte string:  0=1Byte, 1=2Byte, ...

}_attribute_packed_ TOKEN_INFO, *PTOKEN_INFO;


/*----------------------------------------+------------------------------------------
                           d a t a  s t r u c t u r e s
-----------------------------------------------------------------------------------*/
