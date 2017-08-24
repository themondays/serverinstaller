************************************

LSI Corp. MegaRAID Firmware Release 

************************************

Release date: 10/21/14

====================== 
Supported Controllers:
======================

MegaRAID SAS 9260-4i
MegaRAID SAS 9260CV-4i
MegaRAID SAS 9260-8i
MegaRAID SAS 9260CV-8i
MegaRAID SAS 9260DE-8i
MegaRAID SAS 9261-8i
MegaRAID SAS 9280-4i4e
MegaRAID SAS 9280-8e
MegaRAID SAS 9280DE-8e
MegaRAID SAS 9280-24i4e
MegaRAID SAS 9280-16i4e
MegaRAID SAS 9260-16i

******* This firmware supports Cachecade Pro 2.0 (if activated) for the controllers mentioned above.
******* This single firmware image can be used on all of the controllers mentioned above (no longer need separate image for the high-port-count controllers).

Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 10/21/14


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.15.0-0205 (4.12)
Firmware 2.130.403-3835								
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0051
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000
ROMENV 1.08
WebBIOS 6.0-54-e_50-Rel
PCLI 4.04.20
UEFI_Driver 0x06060A05 								
Hii 03.07.12.04
BIOS 3.30.02.2
FCODE 4.16.08.00
			     
Current Firmware Package (with CacheCade Pro 2.0):  12.15.0-0189 (4.12)
Firmware 2.130.403-3066
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0049
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000
ROMENV 1.08
WebBIOS 6.0-52-e_48-Rel
PCLI 4.04.20
UEFI_Driver 0x06060900
Hii 03.07.11
BIOS 3.30.02
FCODE 4.16.08.00


Bug Fixes and Enhancements:
===========================
Firmware:
SCGCQ00579944 - (Closed) - System crashes and OS boot issues with 2008 R2 based on 9280-16i4e controllers after one drive is failing in a RAID 5.
SCGCQ00606211 - (Closed) - Delay POST by 90 Seconds but delayPOST = 0 in MFC data
SCGCQ00639695 - (Closed) - After setting a PD in savemode to a hotspare, the controller became unresponsive and will hang the OS on LSI SAS9220-8i/9240
SCGCQ00493898 - (Closed) - Liberator Card loses its boot VD after many reboots
SCGCQ00638416 - (Closed) - Fw hit montask with machinecheck exception on PD pushpull with IO on R5
SCGCQ00664200 - (Closed) - With SED drives and Safestore, RAID 1 is deleted after both drives replaced/rebuilt with 9260 and latest fw
SCGCQ00714186 - (Closed) - HDD Drive Speed shows as "UNKNOWN" in MSM with 9240/9260/9270
SCGCQ00477090 - (Closed) - able to change disableImmediate from MFI API.
SCGCQ00627867 - (Closed) - Controller in JBOD mode is allowing a PD designated as a Hot Spare to be a configured via OOB as the boot drive
SCGCQ00775246 - (Closed) - After Liberator FW update from 4.11 to 4.12 and a subsequent power cycle the MegaRAID card will stop with discovery error message
SCGCQ00778348 - (Closed) - Hot Spare are failing for no apparent reason after upgrading from 4.10 to 4.12
SCGCQ00738071 - (Implemented) - To reset FTXM bit (Bit 4) at byte offset 0x40 of the SBR.
SCGCQ00707274 - (Closed) - Need Drive Activity LED as user selectable option is WebBIOS or at least StorCl
SCGCQ00612703 - (Port_Complete) - storcli.efi /c0/v0 show fails
SCGCQ00634623 - (Port_Complete) - SSD causes system to hang at POST
SCGCQ00646141 - (Port_Complete) - After setting a PD in savemode to a hotspare, the controller became unresponsive and will hang the OS on LSI SAS9220-8i/9240
SCGCQ00647127 - (Port_Complete) - Controller in JBOD mode is allowing a PD designated as a Hot Spare to be a configured via OOB as the boot drive
SCGCQ00647431 - (Port_Complete) - Cannot recover data from a recoverable media error after Patrol Read encounters recovery failure for RAID-1
SCGCQ00650865 - (Port_Complete) - Media error on Last sector of stripe isn't added to LDBBM list
SCGCQ00719719 - (Port_Complete) - Patrol Read completion time is incorrect
SCGCQ00773044 - (Port_Complete) - Add event in FW to indicate that an INIT operation was halted due to reboot or other failure
SCGCQ00774007 - (Port_Complete) - Add Critical event in FW to indicate that an INIT operation was halted due to reboot or other failure
SCGCQ00610051 - (Port_Not_Required) - CLI Reporting of TMM VPD
SCGCQ00630216 - (Port_Complete) - SSD causes system to hang at POST
SCGCQ00692397 - (Port_Complete) - Cell Disk With Status not Present Was Found to be Showing a Green Status Light
SCGCQ00710521 - (Port_Complete) - Replaced BBU temporarily transitions to failed state

NVDATA:
Miscellaneous OEM changes

WebBIOS:
SCGCQ00386062 - (Closed) - Unable to read the correct value of controller property "Boot Error Handling" after changing
SCGCQ00683327 - (Closed) - LSI 2108 Advanced WebBIOS Locks Serve

UEFI Driver:
SCGCQ00688791 - (Port_Complete) - Remove default value from numeric primitive that is present under suppressif
SCGCQ00726757 - (Port_Complete) - Implement password EFI_IFR_FLAG_CALLBACK bit requirement from UEFI spec 2.4 Errata B
SCGCQ00696371 - (Port_Complete) - change memory type from EfiRuntimeServicesData to EfiMemoryMappedIO
SCGCQ00698249 - (Port_Complete) - Don't let user boot to OS until user discards dirty cache
SCGCQ00698254 - (Port_Complete) - Code refactor effort: Hii Image buffer clean-up
SCGCQ00696543 - (Port_Complete) - Implement mechanism to allow system BIOS to put entries for iMR mem allocation into proper table
SCGCQ00698238 - (Port_Complete) - Do not go to WebBIOS due to a bad health status when WebBIOS is disabled in NVDATA.
SCGCQ00699147 - (Port_Complete) - dont free the host memory during disconnect/unload process
SCGCQ00644509 - (Port_Complete) - CPU EXCEPTION ERROR During CSIOR Stage- After Updating
SCGCQ00644856 - (Port_Complete) - Install FMP on Controller Handle at Start() and the exception
SCGCQ00661595 - (Port_Complete) - Timeout is not passed correctly to FW
SCGCQ00644859 - (Port_Complete) - FMP GetImageInfo could result in stack corruption - Code Review Defect ID D26304
SCGCQ00726728 - (Port_Complete) - UEFI Driver not filling direction flag correctly for Read
SCGCQ00698248 - (Port_Complete) - Initilize the buffer that we allocate to load HII and return EFI_OUT_OF_RESOURCES if allocation fails
SCGCQ00698250 - (Port_Complete) - Return health status failed if platform calls GetHealthStatus w/ NULL handle after we return intermediate status

HII:
SCGCQ00723376 - (Closed) - Aspen UEFI creating a new VD in a "hole" in the drive group free space, hangs when viewing drive group properties
SCGCQ00718411 - (Closed) - Save controller events directory name is not appropriate
SCGCQ00642232 - (Closed) - Ability to enable/disable JBOD mode in HII
SCGCQ00644857 - (Port_Complete) - Remove FMP installed on the controller handle in HII
SCGCQ00644858 - (Port_Complete) - RSOD during POST after enabling SecureBoot - HEFU.ROM should report exact size of .EFI image
SCGCQ00661685 - (Port_Complete) - HII reports wrong data under 'Eligible JBOD Drives for conversion' while converting JBOD to UG
SCGCQ00698646 - (Port_Complete) - Unable to set spinup drive count in HII
SCGCQ00698664 - (Port_Complete) - Not showing PD to assign as DHS to R50/ R60 VD if PD is of lesser size than largest PD in V
SCGCQ00698671 - (Port_Complete) - After Rebuilding, DHS and Expand VD operations failed to update as per VD properties
SCGCQ00698309 - (Port_Complete) - Full initialization is not grayed out for R6 DIF arrays.
SCGCQ00698543 - (Port_Complete) - HII is gave a option to start reconstruction from any degraded RAID level to RAID 0.
SCGCQ00698650 - (Port_Complete) - HII does not display power state of a PD
SCGCQ00698667 - (Port_Complete) - In HII Create VD option is enabled when resconstruction is in progress (on partial VD)
SCGCQ00644575 - (Port_Complete) - Consistency Check Start - remove mm/dd/yyyy from the string as the format is out of control of LSI
SCGCQ00693695 - (Port_Complete) - When Controller is in Safe Mode don't remove UCM formset
SCGCQ00698429 - (Port_Complete) - change the numeric primitive's maximum from MAX_PHYSICAL_DRIVES to 0xff
SCGCQ00698644 - (Port_Complete) - Minor Typos and consistency issues in HEFU HII help text
SCGCQ00698320 - (Port_Complete) - HII is showing RAID6 and RAID5 levels while creating VD when we have 2 512n drives and 1 4k Drives
SCGCQ00698322 - (Port_Complete) - "Make Unconfigured good" option missing in HII even if JBOD's are present in the config
SCGCQ00698447 - (Port_Complete) - Not able to create VD with 4k drives on intel wildcat server.
SCGCQ00698627 - (Port_Complete) - not displaying the cachecade 512GB warning message
SCGCQ00644786 - (Port_Complete) - Enhance help message for PD/VD Operation field
SCGCQ00692302 - (Port_Complete) - creation in HII is showing GB
SCGCQ00698547 - (Port_Complete) - HII is showing RAID levels, irrespective of drives to configure a new RAID level.
SCGCQ00698661 - (Port_Complete) - Call UninstallStoreLib when we call StopControllerHiiImpl function

BIOS:
SCGCQ00662761 - (Closed) - Display Warning Message when Preboot Configuration Utility are Disabled and User tries to lunch it
SCGCQ00690943 - (Implemented) - OEM wants the battery status text in BIOS to change from "missing" to "N/A" for iMR solutions.
SCGCQ00644761 - (Port_Complete) - Implement mechanism to allow system BIOS to put entries for iMR mem allocation into proper table
SCGCQ00645908 - (Port_Complete) - Firmware enters faults state after a reboot on flashing firmware with IO in progress.
SCGCQ00697504 - (Port_Complete) - setting deviceExposure greater than or eaual to 128 will show wrong VD handled by bios

Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom



==============================
Previous Releases
==============================

Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 05/09/14


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.15.0-0189 (4.12)
Firmware 2.130.403-3066
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0049
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000
ROMENV 1.08
WebBIOS 6.0-52-e_48-Rel
PCLI 4.04.20
UEFI_Driver 0x06060900
Hii 03.07.11
BIOS 3.30.02
FCODE 4.16.08.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.14.0-0167 (4.11)
Firmware 2.130.393-2551
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0045
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-52-e_48-Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.37.00
BIOS 3.28.00
FCODE 4.14.05.00


Bug Fixes and Enhancements:
===========================
Firmware:
SCGCQ00426160 - (Closed) - MegaRAID iMR FW with "enableReducedFeatureSet=1" issue
SCGCQ00599788 - (Closed) - storcli.efi /c0/v0 show fails
SCGCQ00584561 - (Closed) - Check-in Pl version 5.0.42.0
SCGCQ00475764 - (Closed) - after the newest image is installed, servers often will not boot.
SCGCQ00475589 - (Closed) - FW sending wrong configuration data during boot msg pending state
SCGCQ00466496 - (Closed) - Not able to change the properties of MSM.
SCGCQ00463043 - (Closed) - Upgrading from Software RAID to 9260-8i causes Alarm to sound
SCGCQ00455655 - (Closed) - Error in MR_DCMD_CTRL_EVENT_COUNT/SL_DIR_READ
SCGCQ00454839 - (Closed) - MSM log warning: Controller booted in headless mode with errors
SCGCQ00426916 - (Closed) - Data Corruption with R5 cachedIO mode
SCGCQ00349837 - (Closed) - Sequential Read performance is less on CacheCade associated LD's.
SCGCQ00573744 - (Closed) - Storcli lists "Support JBOD=No" in /c0/show all command
SCGCQ00572918 - (Closed) - Storcli version 1.09.01 and 1.07.07 :: Link Speed for second port is showing as Unknown
SCGCQ00569335 - (Closed) - All drives become unconf-bad when enclosure is connected
SCGCQ00467839 - (Closed) - Montask occurs when running host i/o and parallel OOB command
SCGCQ00464657 - (Closed) - VPD output of lspci –vvxxx showing [RV] Reserved: checksum bad, 86 byte(s) reserved
SCGCQ00450935 - (Closed) - When pinned cache present VD property not changing from WB to WT.
SCGCQ00440568 - (Closed) - 9260-8i(SAS2108) hitting OOB SRAT timeout for a long time causing OOB commands to fail
SCGCQ00595181 - (Closed) - drive becoming foreign, rebuild not kicking in automatically
SCGCQ00558885 - (Closed) - Checking-in new PL version :05.00.41
SCGCQ00488348 - (Closed) - To check-in new PL version 5.00.40.00
SCGCQ00427464 - (Closed) - Discard preserved cache takes excessive time or hangs
SCGCQ00574866 - (Closed) - LDBBM entry not getting populated on running CC on R5 with double media error
SCGCQ00458860 - (Closed) - Controller always gives an event MR_EVT_CTRL_BOOT_HEADLESS_MODE_HAD_ERRORS if controller is set in HSM mode.
SCGCQ00457324 - (Closed) - Montask occurs when running host i/o and parallel OOB command
SCGCQ00493435 - (Closed) - RM_SataBBMTotalCount always return 0
SCGCQ00455014 - (Resolved) - PDs go online without VD created & App doesn't allow any VD creation on passing a wrong parameter in Spanned VD.
SCGCQ00456152 - (Closed) - To Disable the logging of event MR_EVT_PD_EMERGENCY_SPARE_COMMISSIONED_REMINDER
SCGCQ00479336 - (Closed) - EEprom CRC error is detected when WebBIOS is running
SCGCQ00491037 - (Closed) - STP Passthrough Support through OOB
SCGCQ00471180 - (Closed) - FW is not declaring the battery bad if learn cycles fail continuously
SCGCQ00467625 - (Closed) - Validate the requested blocks for SCSI VERIFY commands on RAID volumes
SCGCQ00451971 - (Closed) - 9261-8i emergency spare being commissioned
SCGCQ00557365 - (Closed) - Output async event message to upper layer when it detected "Write Timeout" in between on-board flash
SCGCQ00433391 - (Port_Complete) - IO integrity error seen after Source VD associated with CacheCade VD is forcibly unblocked
SCGCQ00433403 - (Port_Complete) - During rebuild on PRL11 CacheCade VD and rebuild on RAID6 Source VD IO integrity error is seen.
SCGCQ00437904 - (Port_Complete) - Data Miscompare found while running I/O overnight with degraded virtual drives
SCGCQ00447415 - (Port_Complete) - Wrong event generated "Reminder: Potential non-optimal configuration due to drive PD xx(e0xfc/sx) commissioned..
SCGCQ00451982 - (Port_Complete) - Discard preserved cache takes excessive time or hangs
SCGCQ00456777 - (Port_Complete) - FW halts when you have pinned cahce and remove VD form the configuration.
SCGCQ00458859 - (Port_Complete) - FW halts when you have pinned cahce and remove VD form the configuration.
SCGCQ00458879 - (Port_Complete) - PCIe core has received a PCIe request that it does not support
SCGCQ00459017 - (Port_Complete) - Firmware returns asc value as 0 when failing IOs with medium errors
SCGCQ00462310 - (Port_Complete) - Transparent Learn Cycle is rescheduled even though it is not completed
SCGCQ00462516 - (Port_Complete) - Card Running iMR Machine Checks when Requesting Host Memory
SCGCQ00475895 - (Port_Complete) - Firmware hits Montask when drive is pulled after changing the volume property from read ahead to no read ahead.
SCGCQ00476387 - (Port_Complete) - Faulty protocol used for changing the Disk Write Cache Policy
SCGCQ00478812 - (Port_Complete) - Minimum Capacity needed for relearn is incorrect in 2208 MR FW
SCGCQ00505389 - (Port_Not_Required) - Firmware is not updating recovery timer as intended
SCGCQ00556476 - (Port_Complete) - Incorrect variable type in MR Read/Modify/Write path
SCGCQ00557366 - (Port_Complete) - Firmware is not updating recovery timer as intended
SCGCQ00558100 - (Port_Complete) - Incorrect variable type in MR Read/Modify/Write path
SCGCQ00558115 - (Port_Complete) - RM_SataBBMTotalCount always return 0
SCGCQ00558947 - (Port_Complete) - FW Automatically detect backplane SGPIO type to set the driveactivityLED parameter
SCGCQ00559344 - (Port_Complete) - Setting the ASO key with storcli 1.02.08 (MegaCli 8.07.06) fails with an error code 3 on Windows or Linux 
SCGCQ00567627 - (Port_Complete) - maintainpdfailhistory = 0, but PDs come back as unconfigured bad anyway.
SCGCQ00579790 - (Port_Complete) - LDBBM on RAID6 not eligible
SCGCQ00583755 - (Port_Complete) - Raid configuration loss after reboot - PDs in POWERSAVE
SCGCQ00433409 - (Port_Complete) - When a Cachecade PD marked as missing is reused to make Cachecade VD IO integrity error is observed.
SCGCQ00433563 - (Port_Complete) - Sequential Read performance is less on CacheCade associated LD's.
SCGCQ00434393 - (Port_Complete) - Saved Release Note DataSLIB returning bad data as good data after host reboots
SCGCQ00444038 - (Port_Complete) - Controller is running into a Machine Check loop during copyback test.Controller time clock refer
SCGCQ00447408 - (Port_Complete) - Set Drive State to JBOD in MR9266/MR2208 will cause Drive Fault LED light on
SCGCQ00447465 - (Port_Complete) - Performance Degradation in RAID 5 Sequential Writes due to command Out-of Ordering
SCGCQ00447651 - (Port_Complete) - Data Corruption seen when IOs are in progress and cachecade PD is marked as missing and reused to create another SSC VD.
SCGCQ00449634 - (Port_Complete) - Source VD gets blocked on OCR after one PD removal from R0 WB CCVD and reuse of that PD in creating another CCVD.
SCGCQ00458876 - (Port_Complete) - VPD (Vital Product Data) needs to identify PCB hardware level
SCGCQ00476060 - (Port_Complete) - Validate the requested blocks for SCSI VERIFY commands on RAID volumes
SCGCQ00487207 - (Port_Complete) - BU SOH bad message be posted after exhauting retry attempts. Stop monitoring battery once SOH bad is declared
SCGCQ00487654 - (Port_Complete) - RHEL6.4 Kernel crash after reboot
SCGCQ00488061 - (Port_Complete) - Medium error correction event is not logged during CC
SCGCQ00504041 - (Port_Complete) - When I/Os are running on SATA Physical Drives with interposers, a drive removal and reinsertion within drive-removal-delay comes back as failed.
SCGCQ00522384 - (Port_Complete) - Patrol Read causes data integrity issue if the LDBBM is fulll
SCGCQ00523075 - (Port_Complete) - Firmware is not updating recovery timer as intended
SCGCQ00558908 - (Port_Complete) - Firmware Version 2.130.383-2912 is hitting MonTask at line 1209 in file ../../raid/1078dma.c
SCGCQ00562799 - (Port_Complete) - VD goes offline when we start reconstruction on 32 Drive R0 VD
SCGCQ00396295 - (Port_Complete) - BIOS hangs while hot plug of 2 enclosures with more than 32 drives on FURY controller
SCGCQ00446507 - (Port_Complete) - iMR FW Package 14/Multipath on Camden controller breaks into montask after VD creation
SCGCQ00446561 - (Port_Complete) - StorCli should not allow turning on JBOD in MR mode
SCGCQ00446691 - (Port_Complete) - RAID Member disks state changed to (foreign) online and still can access.
SCGCQ00447429 - (Port_Complete) - Copyback is not automatically starting on 2208 MR board.
SCGCQ00476108 - (Port_Complete) - If a BOOT Passphrase is enabled with SafeStor "MegaCli adpbios dsply a0" will display "BIOS will Stop on error" even if any other mode is set.
SCGCQ00484210 - (Port_Complete) - "chip paused" causes high service time on all drives
SCGCQ00488447 - (Port_Complete) - Update the events
SCGCQ00558950 - (Port_Complete) - Firmware is not updating recovery timer as intended
SCGCQ00442852 - (Port_Complete) - immediate IO broken for R1
SCGCQ00447445 - (Port_Complete) - makes unconfigured bad (foreign) to unconfigured bad on reboot.
SCGCQ00450646 - (Port_Complete) - 9260-8i(SAS2108) hitting OOB SRAT timeout for a long time causing OOB commands to fail
SCGCQ00522373 - (Port_Complete) - OEM facing with incorrect SAS HDD's indication, MSM shows 2X drives connected
SCGCQ00447412 - (Port_Complete) - BGI does not correct ECC medium errors on 2nd arm of Raid 10 array
SCGCQ00447448 - (Port_Complete) - Hot Spare lost after FW update
SCGCQ00479291 - (Port_Complete) - system black screen during S1 Sleep test
SCGCQ00487664 - (Port_Complete) - Call DDF_discardInvalidPds only if DS3 is enabled
SCGCQ00522372 - (Port_Complete) - If the hotspare was spun down, the copyback after a drive failure wouldn't always start.
SCGCQ00522375 - (Port_Complete) - SES device is not found during cable pull testing
SCGCQ00558924 - (Port_Complete) - Upgrading from Software RAID to 9260-8i causes Alarm to sound
SCGCQ00446520 - (Port_Complete) - MegaRAID firmware breaks into montask after running online controller reset during I/O
SCGCQ00469989 - (Port_Complete) - Intermittent SEP errors on iMR and MR controllers
SCGCQ00446624 - (Port_Complete) - SCSI Pass-Through to SAS SSD returns data but disk reported medium error
SCGCQ00326488 - (Port_Complete) - Firmware montask in DDF_VerifySasSata mix violation while importing foreign configuration.

NVDATA:
Miscellaneous OEM changes

UEFI Driver:
SCGCQ00378953 - (Closed) - HII not showing up + other enhancement for driver health formset
SCGCQ00413986 - (Closed) - HII app doesnt appear when there is critical message and when platform uses SetOptions
SCGCQ00479003 - (Closed) - Disable Debug print in UEFI Driver
SCGCQ00618669 - (Closed) - UEFI Driver: StartHiiBackEnd() has Print statement with %g
SCGCQ00408333 - (Closed) - Critical boot message formatting issue
SCGCQ00384884 - (Closed) - Customize health formset + few other issues
SCGCQ00395190 - (Closed) - HII - 03.02.03.00/When trying to create an R6/R5, unable to create though there are 3 SAS unconfigured good avail
SCGCQ00396051 - (Closed) - UEFI Driver (MR 5.7) should not display finite boot messages
SCGCQ00435384 - (Closed) - UEFI System hang if controller FW is in fault state
SCGCQ00408094 - (Closed) - System with a critical message does not enter EFI preboot config utility
SCGCQ00498925 - (Closed) - Not able to boot into BIOS boot menu
SCGCQ00518791 - (Closed) - In IsValidHandle() - Free the buffer if status is not EFI success for LocateHandle
SCGCQ00493654 - (Closed) - Memory Leak issue in UEFI driver when reconnect happens
SCGCQ00628627 - (Closed) - UEFI Driver- has debug print statements during POST
SCGCQ00601851 - (Closed) - System hang when we press F1 on second time when we are in HII form
SCGCQ00578194 - (Closed) - UEFI Driver should calculate checksum before setting bios data (MR_BIOS_DATA)
SCGCQ00589148 - (Closed) - Diskpart EFI utility can't create partition on a JBOD
SCGCQ00483847 - (Closed) - Remove private GUID and use EFI_KMS_PROTOCOL_GUID defined in UEFI 2.3.1 spec
SCGCQ00385365 - (Closed) - PCIDATA
SCGCQ00404216 - (Closed) - Add 3008 device ID for iMR product.
SCGCQ00395996 - (Closed) - Parse Boot message and remove \r\n at the beginning and replace \n with \r in other parts
SCGCQ00526851 - (Closed) - Support EFI_FIRMWARE_MANAGEMENT_PROTOCOL for controller firmware upgrade.
SCGCQ00331573 - (Closed) - Remove dependency on deprecated Driver Configuration2 protocol from UEFI Driver
SCGCQ00392836 - (Closed) - LSI to Support Remote Key Management
SCGCQ00436609 - (Closed) - add PnPID's for 3004 and 3108
SCGCQ00484086 - (Port_Complete) - UEFI hang when SATA is connected
SCGCQ00468948 - (Port_Complete) - Uninstall HII CAP and Driver Health formset if platform calls Start again without calling GetHealthStatus
SCGCQ00477828 - (Port_Complete) - 9240/9260/9270 UEFI driver issue when user having 2TB or 4TB VD
SCGCQ00606367 - (Port_Complete) - UEFI Driver returns EFI_NOT_FOUND when platform passes valid Target for ExtSPT.BuildDevicePath()
SCGCQ00478692 - (Port_Complete) - Block IO tests from SCT IHV test suite failed
SCGCQ00484114 - (Port_Complete) - Follow UEFI Spec on interpreting In and Out Transfer Length that upper layer sends
SCGCQ00580313 - (Port_Complete) - User can't access RAID HII in UEFI mode after removing a HDD

BIOS:
SCGCQ00479507 - (Closed) - FW 3.340.05-2739/Getting memory conflict during boot
SCGCQ00495471 - (Closed) - OOB function corrupts EBDA area used by other server function
SCGCQ00579962 - (Closed) - Initialize deviceExposure with MAX_INT13DISKS instead of 0xFF
SCGCQ00552308 - (Closed) - sytem is not allocating memory correctly when MR and iMR are in the same system
SCGCQ00493680 - (Closed) - Optimize reading IO port in INT15h path
SCGCQ00482283 - (Closed) - System reboots during IO because FW didnt receive permanent host memory from OpROM
SCGCQ00503046 - (Closed) - OpROM is displaying incorrect value for the number of INT13h device it supports
SCGCQ00572939 - (Closed) - Make sure all the make file adheres to the latest binary names.
SCGCQ00466276 - (Closed) - Check the controller status bit in the function which hooks INT15
SCGCQ00559095 - (Closed) - Provide an option to limit the max number of INT 13h devices presented to system BIOS
SCGCQ00454086 - (Closed) - Add boundary checking when allocating EBDA and PMM memory for BIOS post.
SCGCQ00460724 - (Port_Complete) - Memory access issue when loading Ctrl R
SCGCQ00460725 - (Port_Complete) - Memory allocation: Allocate only two 64k

HII:
SCGCQ00294957 - (Closed) - Manage SSD Caching is showing all the VD's in the list in HII.
SCGCQ00263845 - (Closed) - HDD port number showed abnormal in HII storage configuration utility
SCGCQ00269837 - (Closed) - 2KB TTY log save as the wrong size
SCGCQ00293398 - (Closed) - Can not enable SSD caching in HII
SCGCQ00279922 - (Closed) - Return appropriate error status when browser calls extractConfig with result Null
SCGCQ00262874 - (Closed) - SATA drives are showing "0" for rpm under drive properties
SCGCQ00271079 - (Closed) - When security is enabled with Local Key Mgmt, changing security reports current mode as External Key Mgmnt
SCGCQ00281514 - (Closed) - Emergency Hotspare is reported for UG and HSP drives as "eligible/ineligible"
SCGCQ00267995 - (Closed) - browser gives error message but changes the drives tate to global hotspare.
SCGCQ00268038 - (Closed) - enter black screen/timeout when user switching between fields/form sub_form and during Pd rebuild.
SCGCQ00279161 - (Closed) - If adding an extra span during VD creation, you are unable to save the config after this.
SCGCQ00262511 - (Closed) - Drive performing copyback displays the state as "Replacing" but other utilities show state as "Copyback"
SCGCQ00286953 - (Closed) - "Drive Cache Policy" default setting shows "Use Drive Default" it needs to be set to "Disabled"
SCGCQ00319010 - (Closed) - while in safe mode locks up when entering View drive
SCGCQ00327331 - (Closed) - Mismatch found for PD listing between Enclosure Managment and View Physical Disk Operation fields.
SCGCQ00271081 - (Closed) - Changing the security key reports "The requested command has invalid arguments".
SCGCQ00319005 - (Closed) - will not allow config to be cleared no drives attached
SCGCQ00318010 - (Closed) - Allows to select Cachecade VD as Boot device
SCGCQ00349349 - (Closed) - HII switch to enable / disable Gen 3 support 
SCGCQ00395508 - (Closed) - System with SSD HDD will pop up an error message after creating VD via HII
SCGCQ00317934 - (Implemented) - Add MFI State error string for the new MFI stat error codes
SCGCQ00317991 - (Closed) - Remove 'Adaptive Read Ahead' option from every MegaRAID management utility
SCGCQ00293056 - (Closed) - Disables all requirement for human input at boot time
SCGCQ00312131 - (Closed) - Enhance Informational message that HII displays when there is no more space to create additional VD
SCGCQ00364156 - (Port_Complete) - Remove debug message which is not applicable for non Mercury customers
SCGCQ00347493 - (Port_Complete) - HII Screen updated reset fields in Configuration Management > Create Virtual Disk- Advanced screen
SCGCQ00395211 - (Port_Complete) - Support new actions added in UEFI 2.3.1


Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom



Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 09/23/13


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.14.0-0167 (4.11)
Firmware 2.130.393-2551
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0045
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-52-e_48-Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.37.00
BIOS 3.28.00
FCODE 4.14.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.13.0-0154 (4.10 P1)
Firmware 2.130.383-2315
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0045
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.36.00
BIOS 3.27.00
FCODE 4.12.05.00


Bug Fixes and Enhancements:
===========================
Firmware:
SCGCQ00395533 - (Closed) - CC schedule is changed to an unjust date by "Configuration Clear"
SCGCQ00349837 - (Closed) - Sequential Read performance is less on CacheCade associated LD's.
SCGCQ00361312 - (Closed) - MR4.7 FW reports detected problem with iBBU07 charging circuit
SCGCQ00360938 - (Closed) - When attempting to secure LD during RLM, receive option to secure LD, LD not secured (message incorrect/misleading.)
SCGCQ00392840 - (Closed) - Prevent overtemp warnings while charging
SCGCQ00399692 - (Closed) - Modify MR fw to eliminate four second window where drive swap is not recognized.
SCGCQ00406955 - (Closed) - Prevent data correction in pagefil.sys to happen during Check Consistency after heavy I/O test.
SCGCQ00422473 - (Port_Complete) - When a Cachecade PD marked as missing is reused to make Cachecade VD IO integrity error is observed.
SCGCQ00432851 - (Port_Complete) - Saved Release Note DataSLIB returning bad data as good data after host reboots
SCGCQ00436896 - (Port_Complete) - Minimum Capacity needed for relearn is incorrect in 2208 MR FW
SCGCQ00447652 - (Port_Complete) - Potential data integrity issue seen when IOs are in progress and cachecade PD is marked as missing and reused to create another SSC VD.
SCGCQ00448732 - (Port_Complete) - Card Running iMR Machine Checks when Requesting Host Memory
SCGCQ00449871 - (Port_Complete) - Verify drive reinsertion within/before drive-removal-delay setting -verify discovery (Without IO)
SCGCQ00414148 - (Port_Complete) - Firmware returns invalid data in OOB packet
SCGCQ00437920 - (Port_Complete) - Data Miscompare found while running I/O overnight with degraded virtual drives
SCGCQ00439197 - (Port_Complete) - Potential data integrity issue with R5 cachedIO mode
SCGCQ00421933 - (Port_Complete) - IO integrity error seen after Source VD associated with CacheCade VD is forcibly unblocked
SCGCQ00423725 - (Port_Complete) - Sequential Read performance is less on CacheCade associated LD's.
SCGCQ00421891 - (Port_Complete) - Assertion failure in ../../raid/raidpci.c at line 8323 while running IO with cable pull/push.
SCGCQ00421960 - (Port_Complete) - During rebuild on PRL11 CacheCade VD and rebuild on RAID6 Source VD IO integrity error is seen.
SCGCQ00450056 - (Port_Complete) - Source VD gets blocked on OCR after one PD removal from R0 WB CCVD and reuse of that PD in creating another CCVD.

BIOS:
SCGCQ00408767: OpROM not getting relocated to runtime segment if the segment address is 0xC0000

WebBIOS:
SCGCQ00386062: Unable to read the correct value of controller property "Boot Error Handling" after changing

HII:
SCGCQ00395508: System with SSD HDD will pop up an error message after creating VD via HII on Catera Hatteras



Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom





Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 03/05/13


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.13.0-0154 (Merged)
Firmware 2.130.383-2315
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0045
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.36.00
BIOS 3.27.00
FCODE 4.12.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.13.0-0151 (Merged)
Firmware 2.130.383-2230
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0041
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.36.00
BIOS 3.27.00
FCODE 4.12.05.00


Bug Fixes and Enhancements:
===========================
Firmware:
SCGCQ00307050	Defect	Not able to flash both 9265-8i and 9285-8e card with 5.3 patch 1 firmware.However, it is ok on 9266-8i.
SCGCQ00338490	Defect	Invalid target ID after new logical drive creation
SCGCQ00341791	Defect	Support for mutlipath topology is broken
SCGCQ00361303	Defect	On iMR FW update caused card to FW fault state and card cannot boot to OS.
SCGCQ00371477	Defect	During Reconstruction (R0->R5) when system reboots the reconstruction doesnt resume. System hangs.
SCGCQ00375250	Defect	OEM BMC see clock stetching on I2C bus
SCGCQ00375854	Defect	adapterOperations.alarmControl is being set to 1 even if controller doesnot have alarm on board
SCGCQ00384381	Defect	Drive temperature is shown very high on 9240
SCGCQ00391094	Defect	9260-16i's drive locate/fault LED header doesn't work
SCGCQ00392062	Defect	Firmware halts when flashing components other than app.rom for OEM controller
SCGCQ00392063	Defect	PL library and files checkin for PL version 5.38
SCGCQ00396335	Defect	9260-16i's drive locate/fault LED header is not working properly.
SCGCQ00363619	EnhancementRequest	Set the Recovery mode page (WriteRetryCount and recoveryTimeLimit)


Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom




Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 03/02/13


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.13.0-0151 (Merged)
Firmware 2.130.383-2230
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0041
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05270000
Hii 1.36.00
BIOS 3.27.00
FCODE 4.12.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.12.0-0139 (Merged)
Firmware 2.130.373-2022
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0041
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05210000
Hii 1.33.00
BIOS 3.26.00
FCODE 4.12.05.00


Bug Fixes and Enhancements:
===========================
Firmware:
SCGCQ00341980 - (Closed) - Current Access policy is not getting updated for iMR
SCGCQ00353935 - (Resolved) - Add support for Ringbuffer and Dq compilation
SCGCQ00356925 - (Closed) - disableHII: Needs to be updated from nvdata into MFCD
SCGCQ00357046 - (Closed) - 9240-4i on particular motherboard is not getting required memory resources on boot
SCGCQ00349562 - (Closed) - Unable to open "Set Patrol Read Properties" in MSM with 2008-iMR
SCGCQ00362340 - (Closed) - multi path with Consistency Check LED behavior problem
SCGCQ00342405 - (Implemented) - Enable or disable 512E using Applications
SCGCQ00342397 - (Implemented) - Provide the count of LBAs that are present in the sata bbm table.
SCGCQ00350996 - (Closed) - Fail the IO in case the medium error recovery is taking more time.
SCGCQ00359200 - (Port_Complete) - offline drive becomes automatically online during Raid Level Migration
SCGCQ00371803 - (Port_Complete) - During Reconstruction (R0->R5) when system reboots the reconstruction doesnt resume. System hangs.
SCGCQ00358361 - (Port_Complete) - Bad disk being serviced hung Linux boot for 2 hours
SCGCQ00373802 - (Port_Complete) - Auto Foreign import fails when controller is booted with pinned cache.

UEFI Driver:
SCGCQ00347171 - (Closed) - MR: on having pinned cache on OEM controller, the option to invoke storage dosent come.
SCGCQ00369636 - (Closed) - UEFI: Settime only when FW time is not set; DHP: Return appropriate health status when there is no critical boot message
SCGCQ00340581 - (Implemented) - UEFI Driver should check pMRCtrlInfo ->maxHANodes with > 1
SCGCQ00318101 - (Closed) - Timed critical boot message under UEFI mode
SCGCQ00347492 - (Closed) - UEFI: Retry read capacity if scsi status is not success
SCGCQ00310476 - (Implemented) - Load HII when user responds to critical boot message by pressing C

HII:
SCGCQ00268038 (DFCT) - Hii enter black screen/timeout when user switching between fields/form sub_form and during Pd rebuild.
SCGCQ00364156 (CSET) - Remove debug message which is not applicable for non Mercury customers

Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom




Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 12/28/2012


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.12.0-0139 (Merged)
Firmware 2.130.373-2022
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0041
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05210000
Hii 1.33.00
BIOS 3.26.00
FCODE 4.12.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.12.0-0124 (Merged)
Firmware 2.130.363-1846
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0039
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05180000
Hii 1.32.00
BIOS 3.25.00
FCODE 4.12.05.00


Bug Fixes and Enhancements:
===========================
Firmware defects:
    SCGCQ00262426  -  If VD is secured while reconstruction is going on does not stay secured once reconstruction is done.
    SCGCQ00313883  -  Battery continually stating Battery Yearly Deep Discharge Relearn Pending. Please Initiate a manual Relearn
    SCGCQ00319537  -  System hang on IBM machine with a critical boot message
    SCGCQ00327006  -  Event (MR_EVT_CTRL_RESETNOW_DONE ) is not generated after OFU is completed
    SCGCQ00329851  -  EEPROM corruption(BQ2060 errata) due to corrupted value of design Capacity written in FCC for iBBU07
    SCGCQ00332181  -  INVALID SGL causes dump into montask, OCR does not recover. MonTask: line 377 in file ../../dm/mpt2/mptrec.c & MonTask: line
    SCGCQ00333679  -  reverting the changes of SCGCQ00295604
    SCGCQ00333696  -  EEPROM corruption(BQ2060 errata) due to corrupted value of design Capacity written in FCC for iBBU07
    SCGCQ00334032  -  Code review checkin
    SCGCQ00335607  -  Firmware upgrade/downgrade not possible due to missing env stamping
    SCGCQ00336716  -  Megamon issue when disabling the drive security on a MR controller with spun down FDE drives.
    SCGCQ00337894  -  OFU is not happening when tried to upgrade firmware.
    SCGCQ00338008  -  Flash of package with OFU request
    SCGCQ00345497  -  Code review: With the new approach for CtrlOFUPossibleSet/Get() shutdown path broken
    SCGCQ00346275  -  Copyback on a drive hits kill adapter
    SCGCQ00347253  -  Firmware fails to return proper value when flashed same firmware image with different stamping with OFU request.
    SCGCQ00353344  -  Data Corruption during Raid Level Migration
    SCGCQ00335935  -  Coverity Defect fixes
    SCGCQ00286152  -  GetConnectorIndex() Out of bound array access
    SCGCQ00334889  -  Multiple switch cases are not handles correctly in debug functions
    SCGCQ00314781  -  Controller always enter HSM when it is set to HCOE
    SCGCQ00317767  -  Code Review Defect: Correct the error in copy back path smart errors path
    SCGCQ00333346  -  Dont interpret the cachecade BVD flags if the volume is created with old FW which does not support cachecade.
    SCGCQ00340772  -  Address Action Item SCGCQ00340495 (Check coding standard for structure OfuRequestState)
    SCGCQ00332658  -  Out-of-bounds access (OVERRUN_STATIC) of an array "ar" in pdCopyBackSpinupCallBack
    SCGCQ00332651  -  Out-of-bounds access (OVERRUN_STATIC) of an array "nvInfo->lockKeyBlob.keyCheck" in displayNvKeyInfo
    SCGCQ00329070  -  Coverity Defect
    SCGCQ00315029  -  PD clear operation is not handle correctly with spun drive drives
    SCGCQ00332588  -  Fix coverity defects 10216 and 10425 (overrun and potential NULL pointer access)
    SCGCQ00307050  -  Not able to flash both 9265-8i and 9285-8e card with 5.3 patch 1 firmware.However, it is ok on 9266-8i.
    SCGCQ00353322  -  Unable to enable ocr using application due to FW ocr setting taking precedence
    SCGCQ00328109  -  Out of bound access in LDBBMListInit
    SCGCQ00337060  -  Coverity Defects
    SCGCQ00341791  -  Support for mutlipath topology is broken
    SCGCQ00330344  -  FW halts with a a kill adaptor on performing an onffline of the drive under copyback. line 1188 in file ../../raid/1078dma.c
    SCGCQ00332099  -  Out-of-bounds access (OVERRUN_STATIC) of an array "ar" in pdDcmdRebuildCB

Firmware Enhancements:
    SCGCQ00302600  -  Added more debug information
    SCGCQ00307949  -  True FW Online update for Liberator
    SCGCQ00307950  -  BGI is in endless loop continuously restarting after completion
    SCGCQ00308988  -  Improve foreign config import for iMR with mixed JBOD/RAID settings
    SCGCQ00314258  -  Need the ability to determine what disk a logical LBA associate with on the fly, api call/algorithim/etc. on Liberator
    SCGCQ00323446  -  Add NVDATA support to make 512 Emulated drive support as configurable for customers.
    SCGCQ00345203  -  megacli reports battery pack is missing on hot insertion
    SCGCQ00314965  -  iBBU07 - SOH bad will be declared for low capacity battery before starting learn cycle
    SCGCQ00307956  -  iBBU07: EEPROM and register values will be updated to revised values at first boot after FW flash.


BIOS:
Version change only

UEFI Driver:
SCGCQ00262858	Defect	Debug message is shown from EFI app choice prompt
SCGCQ00308870	Defect	EFI Driver 518\When disconnecting the driver in EFI, I am still able to exectue the EFI utility menu.
SCGCQ00312683	Defect	UEFI Driver not timing out in 3 mins when FW is in undefined state
SCGCQ00320619	Defect	UEFI: Build Device Path and Get Next Target function of SCSI Pass Thru is not returning correct EFI status for certain cases
SCGCQ00335617	Defect	Drives are reporting timeout on Write Buffer cmd 
SCGCQ00339371	Defect	UEFI Driver fails SCT Test
SCGCQ00343869	Defect	Enhance critical boot message handling
SCGCQ00338066	EnhancementRequest	UEFI - HAMR: Fire LD list query based on maxHANodes
SCGCQ00338782	EnhancementRequest	If SetOptions is called for Invader dont show EFI WebBIOS and EFI CLI option
SCGCQ00340581	EnhancementRequest	UEFI Driver should check pMRCtrlInfo ->maxHANodes with > 1 to determine HAMR

HII:
SCGCQ00318010 (DFCT) - Allows to select Cachecade VD as Boot device
SCGCQ00319005 (DFCT) - HII will not allow config to be cleared no drives attached
SCGCQ00319010 (DFCT) - HII while in safe mode locks up when entering View drive
SCGCQ00293056 (ENHREQ) - HII: LSIP200198414/LSIP200229508/LSIP200231613: Disables all requirement for human input at boot time
SCGCQ00317934 (ENHREQ) - Add MFI State error string for the new MFI stat error codes
SCGCQ00317991 (ENHREQ) - Remove 'Adaptive Read Ahead' option from every MegaRAID management utility (PR: SCGCQ00295908)


Installation:
=============
Use MegaCLI or StorCLI to flash the SAS controllers.  These tools can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  
MegaCli -adpfwflash -f mr2108fw.rom -a0
or
StorCLI /c0 download file=mr2108fw.rom




Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 10/15/2012


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.12.0-0124 (Merged)
Firmware 2.130.363-1846
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00 
NVDATA 2.09.03-0039
BootBlockCommon 09.250.01.219
BootBlock 2.02.00.00-0000 
ROMENV 1.08
WebBIOS 6.0-51-e_47_Rel
PCLI 4.04.20
UEFI_Driver 0x05180000
Hii 1.32.00
BIOS 3.25.00
FCODE 4.12.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.12.0-0111 (Merged)
Firmware 2.130.353-1663 
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00  
BootBlockCommon 09.250.01.219  
BootBlock 2.02.00.00-0000  
ROMENV 1.08  
NVDATA 2.09.03-0032  
PCLI 4.04.20  
WebBIOS 6.0-49-e_45-Rel 
Hii 1.28.00  
UEFI_Driver 0x05160000  
BIOS 3.24.00  
FCODE 4.12.05.00


Bug Fixes and Enhancements:
===========================
Firmware:
=========
SCGCQ00263446 - (Closed) - Support SGPIO SideBand with/without Activity
SCGCQ00296744 - (Closed) - enable Emergency hotspare capability for channel
SCGCQ00323446 - (Closed) - Add NVDATA support to make 512 Emulated drive support as configurable for customers.
SCGCQ00286613 - (Closed) - ER corresponding to LSIP200230920
SCGCQ00290530 - (Closed) - Band Gap charging for iBBU08
SCGCQ00288679 - (Implemented) - ER corresponding to LSIP200122939
SCGCQ00290531 - (Closed) - Learn Improvement - Do not take Command block if there is no LD for which cache policy will change
SCGCQ00308820 - (Closed) - Bbumode is 5 instead of 0 for iBBU07
SCGCQ00311062 - (Closed) - Fatal firmware error: Line 1842 during FC import
SCGCQ00284536 - (Closed) - 9260 controller VD goes offline during Slow/Full/Foreground Initialize with 3TB Seagate SED SAS drives
SCGCQ00301160 - (Closed) - Limited band Charging for all OEMs
SCGCQ00298687 - (Closed) - Code Review defect to handle SA_INVALID_FIELD_IN_CDB
SCGCQ00292830 - (Closed) - FW correctly removes entries in the SBBM table by properly shifting the entries to the right.
SCGCQ00292943 - (Closed) - Allocate BBLog for iMR Initally during boot up
SCGCQ00295474 - (Closed) - BBU mode is set to 5 for BBU07 in ctrl-h
SCGCQ00294981 - (Closed) - FW Montask on every boot
SCGCQ00252606 - (Closed) - Drive fails after I/O timeout and subsequent target reset during continuous firmware flash.
SCGCQ00295130 - (Closed) - spare pointer is incorrectly used in MR_CONFIG_DATA
SCGCQ00290512 - (Closed) - Checked in the review comment for SCGCQ00286613
SCGCQ00313883 - (Closed) - Battery continually stating Battery Yearly Deep Discharge Relearn Pending. Please Initiate a manual Relearn
SCGCQ00319537 - (Closed) - System hang on OEM machine with a critical boot message

WebBIOS:
========
SCGCQ00295052 (DFCT) - Change BBU Mode description for Liberator batteries.

HII:
====
SCGCQ00294957 (DFCT) - Manage SSD Caching is showing all the VD's in the list in HII.


Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mr2108fw.rom -a0




Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 06/18/2012


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.12.0-0111 (Merged)
Medusa_Expander SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00  
Firmware 2.130.353-1663  
BootBlockCommon 09.250.01.219  
BootBlock 2.02.00.00-0000  
ROMENV 1.08  
NVDATA 2.09.03-0032  
PCLI 4.04.20  
WebBIOS 6.0-49-e_45-Rel 
Hii 1.28.00  
UEFI_Driver 0x05160000  
BIOS 3.24.00  
FCODE 4.12.05.00
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.12.0-0102 (Merged)
Previous Package Details:   
APP-2.130.293-1580_BB-2.02.00.00-0000_BIOS-3.24.00_4.12.05.00_0x05160000_HII-_WEBBIOS-6.0-48-e_44-Rel_PCLI-04.04-01900008_BOOT-09.250.01.219_NVDATA-2.09.03-0031_2012_03_22


Bug Fixes and Enhancements:
===========================
Firmware:
=========
SCGCQ00263235 (DFCT) - SCGCQ00279191 Native configuration is no longer supported message in post
SCGCQ00265898 (DFCT) - FW quiesces IO when LC starts on BBU even in "WB always" mode
SCGCQ00280109 (DFCT) - Patrol Reads on SSD drives
SCGCQ00281507 (DFCT) - N/A
SCGCQ00281546 (DFCT) - Implemented the limited band charging
SCGCQ00284230 (DFCT) - Megamon during Injecting errors to VD with 512e PDs on iMR controller
SCGCQ00281888 (CSET) - Native configuration is no longer supported message in post
SCGCQ00283734 (CSET) - CRC errors during TSOC communication
LSIP200231744 (DFCT) Update BBU eeprom offset 0x08 and 0x09 
LSIP200212236 (DFCT) iMR FW : Fail to read the header info from CFI Flash ROM during repeated system reboot. 
LSIP200231285 (DFCT) Disable logging in flash for specific OEM 


Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mr2108fw.rom -a0




Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 05/21/2012


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.12.0-0102 (Merged)
Current Package Details:   
APP-2.130.293-1580_BB-2.02.00.00-0000_BIOS-3.24.00_4.12.05.00_0x05160000_HII-_WEBBIOS-6.0-48-e_44-Rel_PCLI-04.04-01900008_BOOT-09.250.01.219_NVDATA-2.09.03-0031_2012_03_22
			     
Previous Firmware Package (with CacheCade Pro 2.0):  12.12.0-0091
Previous Package Details:   
APP-2.130.243-1483_BB-2.02.00.00-0000_BIOS-3.24.00_4.12.05.00_0x05120000_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01900008_BOOT-09.250.01.219_NVDATA-2.09.03-0029_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2012_01_07


Bug Fixes and Enhancements:
===========================
Firmware:
=========
LSIP200223753 (DFCT) (CL LSIP200221295) Reboot Causes HSP to Revert to Unconfigured Good 
LSIP200222937 (DFCT) (CL LSIP200210145) Adding/Removing FoD key causes Legacy BIOS to stop and not continue 
LSIP200222809 (DFCT) Amber LED flashes during copyback after rebuilding degraded VD 
LSIP200223758 (DFCT) Falcon FW - 1509/recon starting and progressing in 1.21 HII 
LSIP200223351 (DFCT) (CL LSIP200223304) R5/R6 enabled using FOD and do a OCR on controller;R5 VD comes up as foreign once OCR is done 
LSIP200229774 (DFCT) 10M09 Liberator: Querying SMART / PFA Status Returns All Zeros in Sense Data 
LSIP200225195 (DFCT) Clearing excess drive error 
LSIP200230774 (DFCT) FW fails to create configuration and utilites fail to report the VD size after the reconstruction. 
LSIP200222944 (DFCT) HDD powersave options still shown in Ctrl Info,VD info & VD creation menus of HII becase FW reports it supports PS 
LSIP200187938 (DFCT) SATA FDE disk not recognized as FDE, SAS model is 
LSIP200223120 (DFCT) Secured Drives Counts as Two Physical Drives 
LSIP200230641 (DFCT) Megamon during import of FC from MR to iMR contoller 
LSIP200210843 (DFCT) Battery is < 623mAH and LUNS in writeback mode 
LSIP200229432 (DFCT) Port LSIP200211420 to Liberator - Upon OCR, Recon Ghost LD needs to be created before WJ processing 
LSIP200223461 (DFCT) (CL LSIP200223341) If you enable security on controller using FOD and create secure VD it goes missing upon reboot. 
LSIP200221295 (DFCT) Reboot Causes HSP to Revert to Unconfigured Good 
LSIP200222969 (DFCT) (RW LSIP200222956) (CL LSIP200222934) After OCR+reboot, user is asked to hit enter to ack the activated FOD features on 
LSIP200199420 (DFCT) 'clear' tasks on disks always show 0% complete when doing 'init' task on RAID volume 
LSIP200222391 (DFCT) BBU Charging gets disabled when Transparent Relearn fails to start due to low capacity 
LSIP200223703 (DFCT) HWR: 10M09: PD other error counter never increments 
LSIP200229673 (DFCT) Fix compilation issue 
LSIP200230643 (DFCT) Error message pop out in HII when set to factory default. 
LSIP200222594 (DFCT) Windows blue screen while in background rebuild process 
LSIP200222918 (DFCT) cannot auto rebuild while we put a clean - JBOD 
LSIP200222956 (DFCT) (CL LSIP200222934) After OCR+reboot, user is asked to hit enter to ack the activated FOD features on OEM system. 
LSIP200210345 (CO) (auto4comp FW_SAS_LSI) Read Locate LED state via BMC 
LSIP200229612 (CO) (auto4comp FW_SAS_LSI) Change EDV values in iBBU07 to improve accuracy with aging batery packs 
LSIP200151207 (CR) Read Locate LED state via BMC 
LSIP200224034 (CR) Change EDV values in iBBU07 to improve accuracy with aging batery packs 


WebBIOS:
========
LSIP200223149 (DFCT) HII/HII browser sluggishness is seen when exiting browser, going to EFI WebBIOS, and then going back to HII browser 
LSIP200223217 (DFCT) IMR-Web Bios shown Configured drives option in Manage power save settings 
LSIP200230109 (DFCT) Grammatical error in WebBIOS. 
LSIP200224127 (DFCT) In WebBIOS, Drive Group Definition screen, enclosures should not be selectable in the Drives column 
LSIP200230031 (DFCT) In EFI-WebBIOS (ver: 4.0-e_45-Rel), the white bars do not match up under VD properties page. 


BIOS:
========

Numerous UEFI and HII updates


NVDATA:
=======
Numerous NVDATA changes


Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mr2108fw.rom -a0





Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 01/30/2012


Version Numbers:	
===============
Current Firmware Package (with CacheCade Pro 2.0):  12.12.0-0091
Current Package Details:   
APP-2.130.243-1483_BB-2.02.00.00-0000_BIOS-3.24.00_4.12.05.00_0x05120000_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01900008_BOOT-09.250.01.219_NVDATA-2.09.03-0029_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2012_01_07
			     
Previous Firmware Package (2108 with CacheCade Pro 2.0) :    12.12.0-0077
Previous Package Details: 
APP-2.130.163-1437_BB-2.02.00.00-0000_BIOS-3.22.00_4.11.05.00_0x05060000_HII-_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01800008_BOOT-09.250.01.219_NVDATA-2.09.03-0026_2011_11_01

Previous Firmware Package (High-Port-Count controllers with CacheCade Pro 2.0):  12.12.0-0080
Previous Package Details:   
APP-2.130.163-1437_BB-2.02.00.00-0000_BIOS-3.22.00_4.11.05.00_0x05060000_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01800008_BOOT-09.250.01.219_NVDATA-2.09.03-0026_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2011_11_10
		 
Previous Firmware Package (High-Port-Count controllers without CacheCade Pro 2.0):  12.12.0-0074
Previous Package Details:   
APP-2.120.183-1415_BB-2.02.00.00-0000_BIOS-3.23.00_4.11.05.00_0x05080000_WEBBIOS-6.0-45-e_40-Rel_PCLI-04.04-01800008_BOOT-09.250.01.219_NVDATA-2.09.03-0024_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2011_10_06
			      

Bug Fixes and Enhancements:
===========================
Firmware:
=========
LSIP200209948 (CO) Generally available support of selected battery modes for iBBU08. 
LSIP200199484 (CO) Enable copy back on SMART - Liberator 
LSIP200151375 (CO) Need StopOnError functionality when SafeStore passphrase is enabled 
LSIP200199858 (CO) add display of ibbu communication retry counter in fw log 
LSIP200188345 (DFCT) (CL LSIP200185468) extended retries of write verify command causes driver timeout 
LSIP200211360 (DFCT) (CL LSIP200209847) Pulling VD 0 from an R60 and rebooting, 2 foreign configs when array 0 
LSIP200199863 (DFCT) UnconfigGood PD fails to kick in as EHS (Emergency Hot Spare) 
LSIP200212811 (DFCT) Advanced features key have to be re-entered 
LSIP200050209 (DFCT) DATA TLB error exception in case of manual removal/insertion of drives. 
LSIP200051385 (DFCT) R1 rebuild hangs at 50% mark after CC is run 
LSIP200051392 (DFCT) Fw broke into MegaMon while writing a test file to a VD (R6) from a non MR disk. 
LSIP200221925 (DFCT) SATA workaround handles LBA incorrectly for 6-byte read CDB
LSIP200186584 (DFCT) Logical Unit ID (VPD page 0x83) changed after foreign import 
LSIP200198521 (DFCT) P16 set properties command fails 
LSIP200212818 (DFCT) Port forward LSIP200211068 to main stream liberator maintenance 
LSIP200212644 (DFCT) Sepaton noticed additional fan speed event messages every 2 minutes 
LSIP200186884 (DFCT) DelaySOD is not working on customer Liberator boards 
LSIP200150532 (DFCT) Liberator FW cause system (sles11sp1/rhel6) reset while lspci -vvx 
LSIP200188226 (DFCT) can't sustain at least 950 MBps performance with Cache Flush setting 1 sec 
LSIP200200459 (DFCT) MegaCli prematurely returns exit 0x0 status on drive firmware flash 
LSIP200209349 (DFCT) Enable PCI capability Bit for VPD exsupport. 
LSIP200198466 (DFCT) wrong slot # reported by megaraid compared to physical slot for EG disk 
LSIP200199687 (DFCT) “Non-Transparent” learns seen with Mode 3 
LSIP200210349 (DFCT) BBUmode incorrectly set 
LSIP200210823 (DFCT) No data is copied to target array after more than 15 secs 
LSIP200212264 (DFCT) Multibit ECC error with customer Liberator MR board and vmware ESXi5.0 
LSIP200199735 (DFCT) LSI Mega RAID SAS-MFI BIOS Register state 0xF0010002 
LSIP200211484 (DFCT) MR bios show "adapter at baseport is not responding" intermittence 
LSIP200186966 (DFCT) configuration becomes foreign after fw fails 3-4 attempts to access raid key during boot 
LSIP200210596 (DFCT) too many messages in fw log "ArDiskTypeMisMatch : SAS_SATA_MIXING_VIOLATION” 
LSIP200155948 (DFCT) FW seems to incorrectly mark ibbu's SOH bad (iBBU07, ibbu05, ibbu01) 
LSIP200209279 (DFCT) additional reboot required after updating ibbu FCC to design capacity 
LSIP200210505 (DFCT) OEM requests qty of prints to be reduced in fw log for ibbu i2c communication retries 
LSIP200212471 (DFCT) FW automatically starts learn cycles on ibbu08 every reboot 
LSIP200211810 (DFCT) Battery failed message during learn cycle 
LSIP200212311 (DFCT) Battery failed message during learn cycle 
LSIP200199812 (DFCT) Alarm sounds with online VD's after flashing latest 10M09 Patch 16 firmware (2.120.163-1343) 
LSIP200199961 (DFCT) LDBBM entries are incorrect after foreign import 
LSIP200186336 (DFCT) Controller does not work with pci set to nomsi in kdump kernel 
LSIP200209455 (DFCT) Potential data integrity issue while running the IO on degraded R5 VDs with 32 drives. 
LSIP200200117 (DFCT) unexpected Power State Change events occur 
LSIP200185441 (DFCT) Disk performance disparity between WB controller cache on and off on system disks 
LSIP200210751 (DFCT) Patrol Read fails on R1 with around 2000 double medium errors 
LSIP200210768 (DFCT) Enabling security causes the config to be missing, and a configured FDE array to be seen as foreign on reboot. 
LSIP200209385 (DFCT) Intermittent RAID key failure during power-on 
LSIP200209329 (DFCT) FoD features will not activate or deactivate 
LSIP200199416 (DFCT) Change BBU-08 support Firmware Mode5 WB capacity from 960mAh to 674mAh for low power DDR-2 
LSIP200199331 (DFCT) Older ibbu batteries display 65K capacity and are marked SOH bad 
LSIP200199335 (DFCT) Request FW to not mark ibbu battery as bad when very high capacity is detected 
LSIP200196885 (DFCT) Batteries Not Charging properly on OEM controller
LSIP200209376 (DFCT) OEM requests ibbu i2c communication retries to be displayed in fw log 



Webbios:
========
None


BIOS:
========
LSIP200199737 (DFCT) server System posts 'R00' as 'R0' during the reboot
LSIP200199163 (DFCT) Legacy BIOS BBS Boot Issues
LSIP200196810 (DFCT): SWR-Macon: After installing the OS, unable to boot from SCU VD
LSIP200199602 (DFCT): FW hangs during POST after flashing new BIOS
LSIP200018807 (DFCT): BIOS sees extra characters "0%" at end of particular product name
LSIP200156493 (DFCT): There is no space between FW version and capacity in the table where Adp/VD/PD info is displayed
Numerous UEFI and HII updates


NVDATA:
=======
Numerous NVDATA changes


Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mrCCP2.rom -a0



Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 11/15/11
Includes support for CacheVault and CacheCade Pro 2.0 features (on appropriately enabled controllers)


Version Numbers:	
===============
Current Firmware Package:    12.12.0-0077
Current Package Details: 
APP-2.130.163-1437_BB-2.02.00.00-0000_BIOS-3.22.00_4.11.05.00_0x05060000_HII-_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01800008_BOOT-09.250.01.219_NVDATA-2.09.03-0026_2011_11_01

Previous Firmware Package (CacheVault):    PKG_CFL_12.12.0-0006
Previous Package Details: 
APP-2.120.73-1289_BB-2.02.00.00-0000_BIOS-3.22.00_4.11.05.00_0x05030000_WEBBIOS-6.0-35-e_30-Rel_PCLI-04.04-017_SPRINT400008_BOOT-01.250.04.219_NVDATA-2.09.03-0014_2011_05_25 
		
Previous Firmware Package (CacheCade):    12.13.0-0104
Previous Package Details: 
APP-2.130.03-1332_BB-2.02.00.00-0000_BIOS-3.20.00_4.11.05.00_0x0418A000_WEBBIOS-6.0-42-e_37-Rel_PCLI-04.04-01900008_BOOT-01.250.04.219_NVDATA-2.09.03-0012_2011_08_05


Bug Fixes and Enhancements:
===========================
Firmware:
Based on 10M09 P15
LSIP200188321 	(CO) 	(auto4comp FW_SAS_LSI) Support CacheVault with CacheCade Pro 2.0
LSIP200187926 	(CO) 	(auto4comp FW_SAS_LSI) Change Liberator BBU-08 support Firmware Mode5 WB capacity from 960mAh to 674mAh for low power D
LSIP200187931 	(CO) 	(auto4comp FW_SAS_LSI) disable bad phy checking logic from iMR FW for certain ROMB design
LSIP200149890 	(CO) 	(auto4comp FW_SAS_LSI) SW licensing. Support FoD for PF activations on iMR/2008
LSIP200187910 	(CO) 	(auto4comp FW_SAS_LSI) Request to use Unconfigured Good Drives for a HotSpare if no spare defined in liberator
LSIP200187724 	(CO) 	(auto4comp FW_SAS_LSI) MR fw should Enable puncturing for SATA drives
LSIP200187937 	(CO) 	(auto4comp FW_SAS_LSI) MegaRaid does not pick up replacement drives in PD if replacement is added during power cycle
LSIP200186977 	(CO) 	(auto4comp FW_SAS_LSI) Alert should be sent when one phy connector between Liberator card and SAS expander is degraded
LSIP200186343 	(CO) 	(LSIP200140211) Add support to pass SMART errors for JBOD drives to MSM
LSIP200147589 	(CO) 	(LSIP200170593) B/G: manufacturing: suppress first relearn
LSIP200200361 	(DFCT) 	FW hit monstak when create PRL 11
LSIP200209556 	(DFCT) 	DATA TLB ERROR exception: sf c01ffd60 when pull out power cord while I/O are running
LSIP200209619 	(DFCT) 	Possible data integrity issue with multiple RAID + changing VD policy while I/O running
LSIP200185599 	(DFCT) 	LED behavior incorrect, drive 3 blinks when drive 0 is accessed
LSIP200150304 	(DFCT) 	memory test for 2108 in 10M09 FW fails 
LSIP200149986 	(DFCT) 	MSM log shows "Battery cannot initiate transparent learn cycles" due to BBU is under mode 5.
LSIP200060949 	(DFCT) 	iBBU07 reported remaining capacity is much too high (64751 mAh) after a learn cycle
LSIP200151502 	(DFCT) 	BBU Absolute Charge is displayed differently
LSIP200149843 	(DFCT) 	VPD (Vital Product Data) needs to show correct Part number
LSIP200149844 	(DFCT) 	VPD (Vital Product Data) needs to identify PCB hardware level
LSIP200151221 	(DFCT) 	CLI/EFI CLI/PCLI fail to create a VD on MR
LSIP200148150 	(DFCT) 	WHQL WLK1.6 - SCSI Compliance Test 2.0 and Read Capacity 16 Unit FAILS on Liberator
LSIP200184970 	(DFCT) 	HWR: 10M09: not able to disable BGI with MegaCLI
LSIP200173287 	(DFCT) 	HWR: bbuMode is not in WebBIOS after flashing from 10M05 to 10M09
LSIP200186406 	(DFCT) 	Code Review defect to deliver rest of changes for PFK preboot support.
LSIP200186594 	(DFCT) 	R5 pfk feature not getting enabled at boot time on iMR
LSIP200186595 	(DFCT) 	After loading FoD key, iMR controller does not perform upgrade of features
LSIP200184943 	(DFCT) 	Need ability to boot with pinned cache with any raid config on Gen 2 MegaRAID/Liberator defaulted to ON
LSIP200185931 	(DFCT) 	Bootwithpinnedcache not default set to '1' for castorlite when suboem is '1'
LSIP200151328 	(DFCT) 	HWR: EF & 10M09: bluescreen occurs handling R1 with HSP and medium errors
LSIP200149597 	(DFCT) 	Liberator FW : Entire VD Configuration is cleared on next boot after snapshot feature is disabled.
LSIP200100479 	(DFCT) 	NVDIMM unrecoverable memory errors treated similar to ECC errors
LSIP200100537 	(DFCT) 	Adding Premiumfeatured reqd bit and ENABLE_SAVE in set and reset LED
LSIP200100786 	(DFCT) 	Cache offload : Fw initialization faild after flashing the Cache offload app to Liberator board.
LSIP200100800 	(DFCT) 	In pinned cache to be in WT, bootup error cases, bbumain and bootup with regular controller
LSIP200105734 	(DFCT) 	IO path size increased after merging to 10m09 code base and the ECC error fix
LSIP200113125 	(DFCT) 	For cache offload modules to stage BIOS from SRAM
LSIP200114431 	(DFCT) 	Update the cap pacakge version information to the VPD area
LSIP200114426 	(DFCT) 	CacheOffload : Write Property is changing from WB to WT during rebuild operation.
LSIP200114471 	(DFCT) 	CacheOffload : Firmware breaks to MegaMon while doing OCR when pinned cache cache is present.
LSIP200119766 	(DFCT) 	The I2C commands should not be sent during the Learn cycle
LSIP200119787 	(DFCT) 	Firmware breaks to MegaMon while doing OCR when pinned cache cache is present.(2)
LSIP200120034 	(DFCT) 	Adding 2msec delay after every I2C transaction
LSIP200121171 	(DFCT) 	CacheOffload : While reading the VPD information firmware is breaking to MegaMon.
LSIP200123172 	(DFCT) 	pfk_nvarm_clear option added for updating the PFK NVRAM both romid and crc
LSIP200123191 	(DFCT) 	To support the Subvendor and Sub Device Ids for the cacheoffload
LSIP200095206 	(DFCT) 	Cache ofload : Reconstruction continues even when Pinned cache is present.
LSIP200134429 	(DFCT) 	The upload feature tested with 0.6 FW and FW download timeout value reduced
LSIP200135106 	(DFCT) 	To support the tmmfru id in EepromWriteMfg2Data
LSIP200125315 	(DFCT) 	CacheOffload : Controller cache discarded due to memory/battery problems.
LSIP200137154 	(DFCT) 	Cacheoffload :Online upgrade of Powergem with 06 fw shows error.
LSIP200125290 	(DFCT) 	with 0.6 FW the download process takes 9 to 10 min's approxmiately and the upload process takes 3 to 4min's
LSIP200144816 	(DFCT) 	Fault LED on module glows continously
LSIP200146636 	(DFCT) 	CacheOffload : 5% reserve pool gf module is not posting the expected events.
LSIP200153376 	(DFCT) 	After FW download reset after exiting from PMODE
LSIP200147680 	(DFCT) 	CacheOffload : Online upgarde of .8 fw corrupts the VPD information.
LSIP200147681 	(DFCT) 	Cacheoffload : communication loss while running the relearn script for multiple times.
LSIP200147688 	(DFCT) 	pack energy calculation, support for backup inhibit in 1.0, increase in nvdata size and code review changes
LSIP200147767 	(DFCT) 	Increase in nvdata uncompress size after nvdata rebase to 10m09
LSIP200148965 	(DFCT) 	CacheOFFLoad : Reconstruction does not start when "enableCTIO" is set to "1".
LSIP200149213 	(DFCT) 	fru id event logged only if valid id present
LSIP200149034 	(DFCT) 	Cacheoffload : SATA PD not getting detected while connecting it as direct attached.
LSIP200096885 	(DFCT) 	Cacheoffload 10m09 merge
LSIP200097050 	(DFCT) 	Cacheoffload: Write Policy is not changing while Battery relearn is in progress.
LSIP200097349 	(DFCT) 	Cache offload: VD with WB can be created even when enableCacheOffload is dissabled in MFc settings.
LSIP200098524 	(DFCT) 	Cache offload : Battery learn starts even when cache offload feature is disable in MFc.
LSIP200099276 	(DFCT) 	PFK Disable,extended header for FW download and gtg timeout after reset
LSIP200099393 	(DFCT) 	The check for GTG added after releasing the NF image
LSIP200134428 	(DFCT) 	Double dip and 3.2 spec compatibility

BIOS:
======
LSIP200042149 DAS Cache Offload Management
LSIP200139406 Three 9260/80 controllers cant boot from MR LD with post 10M09 12.12.0-0036 
LSIP200152136 Attempting to load WebBIOS causes some systems to hang
LSIP200134837 FW revision is not displayed in the Bios' controller list


Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mr2108fw.rom -a0





Component:
=========
SAS MegaRAID Firmware Release for MegaRAID Controllers
Release date: 04/15/2011


Version Numbers:	
===============
Current Firmware Package:  12.12.0-0044  
Current Package Details:   
APP-2.120.43-1223_BB-2.02.00.00-0000_BIOS-3.22.00_4.11.05.00_0x05020000_WEBBIOS-6.0-34-e_29-Rel_PCLI-04.04-01700008_BOOT-01.250.04.219_NVDATA-2.09.03-0012_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2011_03_17
			     
Previous Firmware Package: 12.12.0-0044  
Previous Package Details:   
APP-2.120.23-1181_BB-2.02.00.00-0000_BIOS-3.21.00_4.11.05.00_0x05000000_WEBBIOS-6.0-34-e_29-Rel_PCLI-04.04-01700008_BOOT-01.250.04.219_NVDATA-2.09.03-0010_SAS2XFW-25.05.04.00_MFGIMG-25.05.04.00_2011_02_12
		 
			      

Bug Fixes and Enhancements:
===========================
Firmware:
=========
LSIP200158868 (DFCT) Power save mode hangs firmware causing driver to send kill adpter command 
LSIP200145892 (DFCT) Liberator FW 10M09 cause system (sles11sp1/rhel6) reset while lspci -vvxxx is used 
LSIP200153318 (DFCT) FW doesn’t detect the iBBU08 if it is remotely mounting with OEM controller 

 BIOS:
========
HII changes only



Installation:
=============
Use MegaCLI to flash the SAS controllers.  MegaCLI can be downloaded from the support and download
section of www.lsi.com.

Command syntax:  MegaCli -adpfwflash -f mrhighpc.rom -a0




