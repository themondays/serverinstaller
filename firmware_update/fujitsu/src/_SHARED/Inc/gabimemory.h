#ifndef _GABIMEMORY_H_
#define _GABIMEMORY_H_

typedef struct
{
	__u64		pIn;
	__u64		pOut;
	__u64		pdwBytesCount;	// pointer to a 32 bit count !!! so it is Windows compatible ....
	__u32		InSize;
	__u32		OutSize;
} __attribute__ ((packed)) DeviceIoctlParameter_T;

typedef struct
{
	__u16	ServiceCategory;
	__u16	Service;
	__u16	Status;
	__u16	ErrorCode;
	__u16	ActionCode;
	union {
	        struct {
	                __u64	SpecialCommand;
           	        __u16	ProgressState;
        	        __u16	ProgressCode;
        	        __u64	DescriptorTableOffset;
        	} __attribute__ ((packed)) flash;
        	struct {
        	        __u16   SubFunction;
        	        __u16   ExtraStatus;
        	        __u8    Reserved[16];
        	} __attribute__ ((packed)) settings;
        } u;
} __attribute__ ((packed)) UEFI_GabiHeader_T;

typedef struct
{
	__u64	PhAddress;
	__u64	Size;
} __attribute__ ((packed)) UEFI_GabiBufferDescriptor_T;

typedef struct
{
	__u16	BufferSize;
	__u16	StructureVersion;
	__u32	TransactionHandle;
	__u64	DataCount;
	UEFI_GabiBufferDescriptor_T	DescriptorArray[0];
} __attribute__ ((packed)) UEFI_GabiIO_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
	UEFI_GabiIO_T Request;
} __attribute__ ((packed)) UEFI_GabiRequest_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
	UEFI_GabiIO_T Response;
} __attribute__ ((packed)) UEFI_GabiResponse_T;

typedef struct
{
	__u64	PhAddress;
	__u32	Size;
} __attribute__ ((packed)) FJ_GabiSysDataBufferDescriptor_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
	struct {
	        __u16	BufferSize;
        	__u16	StructureVersion;
        	__u32	TransactionHandle;
        	__u32	DataID;
        	__u32	AccessKey;
        } __attribute__ ((packed)) Request;
} __attribute__ ((packed)) FJ_Read_Sys_Data_Request_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
	struct {
	        __u16	BufferSize;
        	__u16	StructureVersion;
	        __u32	TransactionHandle;
	        __u32	ReadDataSize;
        	FJ_GabiSysDataBufferDescriptor_T DescriptorArray[0];
	} __attribute__ ((packed)) Response;
} __attribute__ ((packed)) FJ_Read_Sys_Data_Response_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
        struct {
	        __u16	BufferSize;
	        __u16	StructureVersion;
	        __u32	TransactionHandle;
	        __u32	DataID;
	        __u32	AccessKey;
	        __u32	WriteDataSize;
        	FJ_GabiSysDataBufferDescriptor_T DescriptorArray[0];
	} __attribute__ ((packed)) Request;
} __attribute__ ((packed)) FJ_Write_Sys_Data_Request_T;

typedef struct
{
	UEFI_GabiHeader_T Header;
        struct {
	        __u16	BufferSize;
	        __u16	StructureVersion;
	        __u32	TransactionHandle;
	} __attribute__ ((packed)) Response;
} __attribute__ ((packed)) FJ_Write_Sys_Data_Response_T;

#endif
