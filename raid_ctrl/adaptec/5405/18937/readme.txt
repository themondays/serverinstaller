--------------------------------------------------------------------
README.TXT
                          
Adaptec ASR-5085/ASR-5405/ASR-5445/ASR-5805/ASR-51245/ASR-51645/ASR-52445 Unified Serial Controllers                       
Adaptec ASR-2045/ASR-2405/ASR-2405Q/ASR-2805 Unified Serial Controllers
Adaptec ASR-5405Z/ASR-5445Z/ASR-5805Z Unified Serial Controllers
Adaptec ASR-5805Q/ASR-5805ZQ Unified Serial Controllers
Adaptec ASR-6405/ASR-6445/ASR-6805/ASR-6405E/ASR-6805E Unified Serial Controllers
Adaptec ASR-6805Q/ASR-6405T/ASR-6805T/ASR-6805TQ Unified Serial Controllers
                          
NOTE:  All Adaptec by PMC products are UL listed and for use only with UL listed ITE.

as of January 12, 2012
--------------------------------------------------------------------
Please review this file for important information about issues
and errata that were discovered after completion of the standard
product documentation. In the case of conflict between various
parts of the documentation set, this file contains the most
current information.

NOTE: The latest firmware, BIOS, drivers and documentation can be 
downloaded from www.adaptec.com when they become available.

The following information is available in this file:

   1. Software and Documentation
      1.1  Controller & Utility Software
      1.2  Documentation on this DVD
   2. Installation and Setup
      2.1  Installation Instructions
      2.2  Windows Setup on Adaptec Series 6 Controllers
      2.3  Solaris 11 Express Setup
      2.4  Fedora Linux Setup
      2.5  SCSI Device Setup on Debian and Ubuntu Linux
      2.6  Other Linux Setup Issues
      2.7  Adaptec CIM Provider Setup
      2.8  Adaptec maxCache Setup
      2.9  Power Management Setup
   3. General Cautions
   4. Known Limitations
      4.1  ACU Utility
      4.2  AFU Utility      
      4.3  Creating an Array from the Adaptec Installation DVD
      4.4  SuSE Linux Enterprise Server 10 with Service Pack 2
      4.5  Using the Controller with an Adaptec S50 JBOD Enclosure
      4.6  Hot-adding Disk Drives
      4.7  SuperMicro Disk Drive Enclosures
      4.8  Using the Controller with a Seagate Barracuda 1TB SATA Drive 
      4.9  Intel SE7525RP2 and SE7320EP2 Motherboards
      4.10 Online Capacity Expansion (OCE) and RAID Level Migration (RLM) Issues
      4.11 Power Management under FreeBSD
      4.12 Failed Arrays under FreeBSD
      4.13 Power Management with Seagate SAS Drive ST31000640SS
      4.14 Linux Cache Synchronization
      4.15 Listing Devices in VMware ESX 4.0 Console
      4.16 BIOS Hangs When Booting RAID Controller with Batteryless Backup (ZMCP)
      4.17 Updating Firmware on Hitachi HUS1514xxVLS300 SAS drives
      4.18 Installing Windows OS from Direct-Attached Non-DASD Device     
      4.19 maxCache SSD Cache Performance
      4.20 Exceeding 2TB LUN Size on VMware ESX Server 
      4.21 PHY Rate Limitations on Western Digital Hard Drives
      4.22 Changing the Stripe Size for Large RAID Arrays 
      4.23 Hot-Adding maxCache SSDs
      4.24 Some SSDs Appear as SATA Drives
      4.25 Mismatch in RAID x0 creation in BIOS and ASM
      4.26 WDC WD3200BJKT-00F4T SATA Drives with Promise J630 Enclosures
      4.27 SATA 6G Drives Not Detected on Intel and Promise Backplanes
      4.28 Samsung Spinpoint SATA Drives Timeout on I/O Load
      4.29 RAID Array Creation Issue With STEC MACH 8 SSDs 
      4.30 Dual-Port SATA Enclosures
      4.31 Xtore and Xyratex Enclosure Timeout
      4.32 Boot delay with Plextor PX-716SA DVD-ROM Drive
      4.33 Series 6 Controllers Hang on Warm Reboot on Some ASUS Motherboards
      4.34 HDA Mode Reset for Adaptec Series 6 Controllers
      
--------------------------------------------------------------------
1. Software and Documentation
   
   1.1. Controller & Utility Software

       - Adaptec Firmware/BIOS/Driver/Utilities Version 7.3.1

         NOTE: The latest versions of firmware, BIOS, driver software and
         utilities can be downloaded from the Adaptec Web Site at 
         www.adaptec.com.
                          
       - Drivers on this DVD
      
         The drivers on this DVD have been tested and certified on  
         the following operating systems. In general, you can load   
         the drivers on out-of-box operating system versions, the  
         latest service pack, or software update. Compatibility   
         issues may be seen with untested OS versions.
      
         NOTE: Not all operating systems are supported by all
         controllers. See notes below.
         
         - Windows Drivers 
  
           o Windows Server 2008, 32-bit and 64-bit
           o Windows Server 2008 R2, 64-bit
           o Windows Vista, 32-bit and 64-bit
           o Windows 7, 32-bit and 64-bit
          
         - Linux Drivers

           o Red Hat Enterprise Linux 5.5, IA-32 and x64
           o Red Hat Enterprise Linux 6.0, IA-32 and x64
           o SuSE Linux Enterprise Server 10, IA-32 and x64     
           o SuSE Linux Enterprise Server 11, IA-32 and x64
           o Debian Linux 5.0.7, 6.0, IA-32 and x64
           o Ubuntu Linux 10.10, 11.04, IA-32 and x64
           o Fedora Linux 12, 13, 14, IA-32 and x64

         - FreeBSD Drivers

           o FreeBSD 7.4, 8.2
        
         - VMware Drivers
       
           o VMware ESX 4.1 Classic 
           o VMware ESXi 5.0 
                     
         - Sun Solaris Drivers
          
           o Solaris 10
           o Solaris 11 Express*
           
           *Not Supported on Bootable Arrays; see Section 2.3
        
   1.2. Documentation on this DVD

        - Adaptec SAS RAID Controllers Installation and User's Guide 
        - Adaptec RAID Controller Command Line Utility User's Guide
        - Adaptec SAS RAID Controllers Quick Start Guide           
        - Adaptec SAS RAID Controllers README.TXT file

--------------------------------------------------------------------
2. Installation and Setup

  2.1  Installation Instructions

       The Adaptec SAS RAID Controllers Installation and User's Guide
       contains complete installation information for the controllers
       and drivers, as well as complete instructions for all 
       utilities. The Adaptec RAID Controllers Command Line Utility
       User's Guide contains complete installation information for
       ARCCONF, Remote ARCCONF, and the Adaptec CIM Provider.
       The Adaptec Storage Manager User's Guide contains complete 
       installation information for the Adaptec Storage Manager software.        
       
  2.2  Windows Setup on Adaptec Series 6 Controllers
         
       On Windows 2008 R2 64-bit systems, the Setup program tries to load
       Inbox storage drivers first, by default. As a result, you must load  
       the driver for Adaptec Series 6/6Q/6E/6T controllers TWICE. 
              
       The first time you load the driver, the OS displays the message
       "No drives were found'. Select 'Load Driver' again, uncheck "Hide
       drivers that are not compatible...', then select the Series 6 
       controller driver. On the second attempt, the driver will load
       successfully.    
      
  2.3  Solaris 11 Express Setup

       Installing Solaris 11 Express on a bootable array is not
       supported in this release.                           
  
  2.4  Fedora Linux Setup
  
       To avoid a known PCIe Active State Power Management issue under
       Fedora Linux 13 and 14, you must add the OS option `pcie_aspm=off` 
       in GRUB bootloader file "menu.lst". Follow these instructions:
       
       1. When the first Fedora installation screen appears, press
          the `Tab` key. 
          
       2. Before pressing the first `reboot` button, edit grub/menu.lst:

          o Press Alt+F2
          o cd /mnt/sysimage/boot/grub/
          o Open the menu.lst file 
          
       3. Add `pcie_aspm=off` just after `rhgb quiet`. The new line
          should look like this (wrapped for clarity):

          root (hd0,0)
          kernel /vmlinuz-2.6.33.3-85.fc13.i686.PAE ro root=/dev/mapper/VolGroup-lv_root 
          rd_LVM_LV=VolGroup/lv_root rd_LVM_LV=VolGroup/lv_swap rd_NO_LUKS rd_NO_MD rd_NO_DM LANG=en_US.UTF-8 
          SYSFONT=latarcyrheb-sun16 KEYTABLE=us rhgb quiet pcie_aspm=off

  2.5  SCSI Device Setup on Debian and Ubuntu Linux
  
       o After installing the Debian Linux driver, you must replace the 
         SCSI Device Partition Name in Debian’s GRUB bootloader with  
         a UUID. Failure to update the partition name may cause the   
         system to hang when the OS is rebooted. In the GRUB bootloader  
         file "menu.lst", replace the SCSI device partition name 
         (for instance, /dev/sda1), with UUID=<string>.
         
       o After installing the Ubuntu Linux driver, you must increase
         the SCSI command timeout value from 30 seconds (the default 
         value) to 90 seconds. Failure to increase the timeout may  
         cause the system to hang with a EXT4 error when a good drive  
         is removed from a rebuilding array. Use these commands to   
         increase the timeout in SYSFS, assuming /dev/sda, /dev/sdb,  
         and /dev/sdc are the device LUNs on the Ubuntu Linux host:

         echo ``90``> /sys/block/sda/device/timeout
         echo ``90``> /sys/block /sdb/device/timeout
         echo ``90``> /sys/block/sdc/device/timeout     
         
  2.6  Other Linux Setup Issues
    
       Before installing the Linux operating system on a logical drive,  
       be sure to clear (remove) old data first. If you do not
       remove old data prior to installation, the OS may not boot.
       As a workaround, use the boot parameter 'aacraid.wwn=2'.         
       
  2.7  Adaptec CIM Provider Setup  
  
       This release supports the Adaptec CIM Provider on
       VMware ESXi 4.1 and VMware ESXi 5.0 only. The documentation 
       (in the CLI guide) INCORRECTLY states that you can install
       the CIM Provider on VMware ESXi 4.0, which is not supported.

  2.8  Adaptec maxCache Setup
 
       o Adaptec maxCache SSD caching is supported on Adaptec Series Q 
         controllers only. 
                    
       o Adaptec Series Q controllers support any maxCache-compatible
         SSD on the compatibility list; see www.adaptec.com/compatibility 
         for a full list of compatible SSD drives.
         
       o You can install a maximum of 8 SSDs on a controller for 
         maxCache caching applications.
                
       See the user's guide for complete maxCache SSD installation and
       setup instructions.
    
  2.9  Power Management Setup 

       You must use a compatible combination of Adaptec Storage Manager  
       and controller firmware and driver software to use the power  
       management feature. All software components must support power
       management. You can download the latest controller firmware 
       and drivers from the Adaptec Web site at www.adaptec.com. 
                     
--------------------------------------------------------------------
3. General Cautions 

   - While an array is builing or cleared, do not remove and 
     re-insert any drive in that array. Doing so may cause 
     unpredictable results for any arrays on the controller.
     
   - Do not move drives containing an array from one controller
     to another while the power is on. Doing so could cause
     the loss of the array configuration or data, or both. 
     Instead, power off both affected controllers, move the  
     drives, and then restart.
          
--------------------------------------------------------------------
4. Known Limitations

  4.1  ACU Utility

       o ACU for DOS is deprecated and no longer supported.
                                      
       o For degraded arrays on Adaptec Series 6E controllers, the ACU 
         displays "junk' characters in the array properties list for 
         the missing drive.
              
       o On some Intel and IBM systems, when you try to run the ACU
         utility, this message appears:
       
         "Not enough free memory to load the utility!
          Press any key to attempt loading the utility forcibly
          OR Wait for the system initialization to be completed 
          [Default]"
       
         This is normal. On Intel systems, wait for the system 
         initialization to be completed. Then the ACU will run. On IBM
         systems, press any key when prompted to load the utility 
         forcibly.
         
       o The ACU erroneously displays deleted logical drives in the
         JBOD list. The logical drive(s) continue to appear in the  
         JBOD list until you delete all of the JBODs.
         
       o With some enclosures, the ACU displays incorrect box/slot 
         information for managed disk drives; for example, 
         Exp/Phy instead of Box/Slot.             

  4.2  AFU Utility

       o When running the AFU in Menu Mode, you can update (flash) only 
         one controller at a time. You must reboot the system before 
         updating the next controller.
       
       o Flashing a Series 5 high-port controller (51245, 51645, 52445)
         with down-level firmware and an older version of the AFU crashes
         the system. To recover, you must use a Mode 0 reset using the
         latest firmware version. See the user's guide for details on 
         the Mode 0 reset method. 

  4.3  Creating an Array from the Adaptec Installation DVD

       When you create an array with Adaptec Storage Manager in
       bootable-CD Mode, the maximum size of the array is 2TB.
       
  4.4  SuSE Linux Enterprise Server 10 with Service Pack 2
  
       If your boot array is installed on the controller, and the boot
       OS is SLES10 SP2 with driver version 1.1.5-2458, you must add 
       'aacraid.wwn=1' to the kernel boot command line. Alternatively, 
       install the latest aacraid driver. 
       
  4.5  Using the Controller with an Adaptec S50 JBOD Enclosure

       Temperature warnings from the Adaptec S50 JBOD Enclosure with 
       firmware version T016 are not shown in Adaptec Storage Manager 
       (or on the enclosure). To correct the problem, upgrade to the 
       latest firmware version.                     
       
  4.6  Hot-adding Disk Drives

      o  If you hot-add multiple disks to a large configuration (100 
         disk drives or more), it may take a significant amount of  
         time before those disk drives appear in Adaptec Storage Manager.

       o With Intel Backplanes AXX4DRV3GEXP and AXX6DRV3GEXP, if a drive 
         bay is empty when the enclosure is powered up, then a SATA disk   
         drive is hot-added into the empty bay, the controller does not 
         detect the new disk drive. To work around this issue, remove   
         and re-insert the SATA drive. 

  4.7  SuperMicro Disk Drive Enclosures

       o Due to an auto backplane detection issue with SuperMicro 
         SC836TQ enclosures, the red fault LED does not go on when 
         a failed member of an array is pulled from the enclosure, 
         for both SGPIO and I2C modes. WORKAROUND: Use the BIOS
         utility to force the backplane type to I2C or SGPIO.
       
       o If the controller does not detect disk drives installed in a 
         SuperMicro M28E2 Mobile Rack, use backplane SAS connectors  
         marked with "SAS In" only.       
       
  4.8  Using the Controller with a Seagate Barracuda 1TB SATA Drive 
       
       To ensure reliability when using Adaptec Series 2 and Series 5 
       controllers with a Seagate Barracuda ES.2 1TB SATA Drive 
       (ST31000340NS), Adaptec recommends using Seagate firmware 
       version AN05 or SN06 or higher. 
                
  4.9  Intel SE7525RP2 and SE7320EP2 Motherboards       
    
       The Intel SE7525RP2 and SE7320EP2 motherboards do not support 
       Mode 0 flash. These motherboards use 64-bit PCI addressing.
       The AFU currently supports 32-bit PCI addresses only.
              
  4.10 Online Capacity Expansion (OCE) and RAID Level Migration (RLM) Issues
       
       o The system may fail to boot after performing a boot drive OCE/RLM.
         To correct the problem, verify that the boot drive is still 
         listed as the first logical device in Adaptec Storage 
         Manager (ASM). If not, use the BIOS utility to reselect the 
         proper boot device.
       
       o After performing an OCE on a Solaris 10 U9 system, the OS   
         may fail to start if you pull a drive from the reconfigured  
         logical device before rebooting.
         
       o Removing a member of an array while an OCE is in progress
         can cause the array to be deleted. 
         
       NOTE: We strongly recommend that you do not remove any drive  
             during an OCE/RLM.
             
       o This release supports a maximum of 8 concurrent OCE tasks in 
         the RAID array migration wizard. The following RLM and OCE
         tasks are NOT supported:
     
         o RAID 50 to RAID 5 RLM
         o RAID 60 to RAID 6 RLM 
         o RAID 50 to RAID 60 OCE         
  
  4.11 Power Management under FreeBSD
  
       The FreeBSD driver does not support power management in  
       this release.
       
  4.12 Failed Arrays under FreeBSD
  
       o On FreeBSD 7, deleting an array under IO load causes the
         OS to reboot automatically.       
       
       o On FreeBSD 8, if an array fails (including pulling a drive
         from the array), the firmware may crash causing the OS to 
         hang or reboot automatically.
  
  4.13 Power Management with Seagate SAS Drive ST31000640SS
     
       Power Management is not supported by the Seagate ST31000640SS
       SAS drive. (The drive powers down but will not power up without 
       rebooting.)       
                  
  4.14 Linux Cache Synchronization

       With Linux driver 1.1-5-2459 (or higher), you may see an improvement
       in performance if you suppress cache synchronization. For a 
       controller with battery back-up, add 'options aacraid cache=6' to 
       the /etc/modprobe.conf.local file. To completely suppress cache 
       synchronization, add 'options aacraid cache=2'. Then, reboot. 
       Note that the smaller value provides no protection in case of a       
       power outage.
       
  4.15 Listing Devices in VMware ESX 4.x Console
  
       After deleting an array with ARCCONF, the VMware ESX Console
       will hang if you list devices with the 'fdisk -l' command. 
       As a work-around, rescan first, then try 'fdisk -l'.
       
  4.16 BIOS Hangs When Booting RAID Controller with Batteryless Backup (ZMCP)

       Possible Causes: After a system shutdown, the ZMM-100DB daughterboard 
       remains active for several minutes. The Super Cap connector may have  
       been improperly removed and/or re-inserted with a charge still present. 

       Solution: Plug the Super Cap connector in when the system is off 
       and, if the yellow activity LED is illuminated on the ZMM-100DB  
       daughterboard attached to RAID Controller, wait for the LED 
       activity indicator to go out. This may take several minutes.  
       The system should then boot normally. 

  4.17 Updating Firmware on Hitachi HUS1514xxVLS300 SAS Drives       

       Firmware upgrade on Hitachi HUS151473VLS300 and HUS151436VLS300 
       SAS drives is not supported for packet sizes below 4K (512/1024/2048).

  4.18 Installing Windows OS from Direct-Attached Non-DASD Device
  
       Adaptec RAID controllers do not support Windows OS installation from  
       direct-attached Non-DASD devices ("Direct Access Storage Devices"),    
       such as a tape drive, CD-ROM, or DVD-ROM. Adaptec recommends  
       using a motherboard IDE or SATA DVD-ROM/CD-ROM device to install
       the Windows OS on a RAID array.

  4.19 maxCache SSD Cache Performance
            
       The maxCache cache contents is lost if the OS is not shut down
       cleanly (for example, by pressing the reboot button or
       pulling the power). When the system is restarted, the cache
       is rebuilt. Until that time, you may see a temporary loss
       in performance.
              
  4.20 Exceeding 2TB LUN Size on VMware ESX Server
  
       Under VMware, if you create a logical drive with greater than 
       2TB of capacity, the OS will only be able to access the remaining 
       storage above 2TB. For instance, with the 3.5TB LUN, only
       1.5TB will be accessible; with a 5TB LUN, only 1TB will
       be accessible (first 2TB+2TB are skipped); and so on.      
       
  4.21 PHY Rate Limitations on Western Digital Hard Drives
    
       To avoid data-phase errors with certain Western Digital hard 
       drives attached to Adaptec Series 5 controllers, the PHY rate  
       must be set 1.5Gb/s rather than the maximum rate of 3.0Gb/s. 
       
       o For Western Digital WD2002FYPS and WD20EADS hard drives, the 
         PHY rate is set automatically to 1.5Gb/s.
                     
       o You can set the PHY rate manually to Auto (default),  
         1.5Gb/s, or 3.0Gb/s in the BIOS utility.       
              
       NOTE: For Adaptec low-port RAID controllers, the device  
             properties in ASM and the BIOS utility will continue    
             to report the maximum PHY rate (3.0Gb/s), not the  
             reduced rate. For high-port controllers, the PHY rate
             is reported correctly.
             
  4.22 Changing the Stripe Size for Large RAID Arrays
  
       For large RAID arrays, a RAID-level migration may fail with a 
       firmware crash if you reconfigure the array with a different
       stripe size. No data is lost, however. 
       
       Known migrations that result in a FW crash include:
       
       o 16/32/64/128 drive R0- 256/16 to 16/256 stripe change
       o 16/32/64/128 drive R0- 512/16 to 16/512 stripe change
       o 16/32/64/128 drive R0- 1024/16 to 16/1024 stripe change
       o 16/32 drive R5- 16 to 512/1024 stripe change
       o 16/32 drive R5- 512/1024 to 16 stripe change
       o 16/32 drive R6- 1024/16 to 16/1024 stripe change
       
  4.23 Hot-Adding maxCache SSDs
       
       If you remove two or more maxCache SSDs then re-insert the 
       drives at the same time, only one will be recognized as part 
       of the maxCache pool. To reconstitute the pool, use Adaptec
       Storage Manager or the BIOS utility to reconfigure  
       the maxCache cache. 

  4.24 Some SSDs Appear as SATA Drives
  
       Some solid state drives (such as the MemoRight MR25.2-S032G)
       identify themselves as ROTATING media. As a result, these SSDs:
     
       o Appear as SATA drives in the ASM Physical Devices View
       o Cannot be used as Adaptec maxCache devices
       o Cannot be used within a hybrid RAID array (comprised of 
         SSDs and hard disks)
       
  4.25 Mismatch in RAID x0 creation in BIOS and ASM
       
       The BIOS utility creates RAID x0 arrays with an odd number of 
       drives by default. Adaptec Storage Manager creates RAID x0 
       arrays with an even number of drives by default. 
       
  4.26 WDC WD3200BJKT-00F4T SATA Drives with Promise J630 Enclosures

       A compatibility issue between Western Digital WD3200BJKT-00F4T 
       SATA drives and Promise J630 enclosures can result in a firmware 
       crash during a RAID array build or bus rescan.
       
  4.27 SATA 6G Drives Not Detected on Promise and Intel Backplanes 
       
       o With Promise J830s enclosures, SATA 6G drives are reported as
         3G in ASM and the BIOS utility, due to a speed negotiation  
         issue between the drive and the expander.
         
       o With Adaptec Series 5 controllers and a Promise J830s enclosure
         running Firware version v6.12, SATA 6G drives cause the 
         BIOS to hang. WORKAROUND: use firmware version v6.04.       
       
       o With Intel AXX6DRV3G and AXX6DRV3GEXP backplanes, SATA 6G drives 
         are not detected in the BIOS. With Intel AXX6DRV3G backplanes, 
         the BIOS hangs; with Intel AXX6DRV3GEXP backplanes, no drives  
         are detected in the BIOS utility.      
       
  4.28 Samsung Spinpoint SATA Drives Timeout on I/O Load 
   
       With Samsung Spinpoint SATA drives, models HD103UJ/HD103UI/HE103UJ, 
       the controller can reset due to a timeout when the drive is under 
       heavy I/O load.
       
  4.29 RAID Array Creation Issue With STEC MACH 8 SSDs 
  
       With STEC MACH 8 SSDs with firmware version 1766, RAID array
       creation fails when using the Adaptec Storage Manager wizard. 
       WORKAROUND: Upgrade to the latest STEC firmware version to
       resolve the problem.
       
  4.30 Dual-Port SATA Enclosures
  
       Dual-ported SATA enclosures are not supported in this release.
       However, on Series 6 controllers (only), redundant paths to SATA   
       disks are detected in the firmware, but treated and shown (in ASM,    
       BIOS, etc) as two single path enclosures with the correct number    
       of drives. Although the drives are shown as available for  
       configuration, the configuration will fail. The firmware  
       currently supports dual path SAS drives only.
       
       NOTE: Mixing SATA and SAS drives in the same logical device
             is not recommended.
             
  4.31 Xtore and Xyratex Enclosure Timeout
    
       o With Adaptec Series 2, 5, and 6 controllers, Xtore XJ-SA12-005 
         and XJ-SA26-224R-S enclosures timeout when they receive a
         Diagnostic Page 04 command. The error occurs at approximately
         10 minute intervals (and is reported in the ASM controller 
         log file), but has little or no effect on performance.
       
       o With Adaptec Series 5 controllers, Xyratex RS-1603-E3-EBD-2 
         enclosures periodically timeout when they receive a SES2   
         diagnostic command, due to a protocol violation. The error   
         occurs at 5 to 15 minute intervals, and may degrade performance.
         
  4.32 Boot delay with Plextor PX-716SA DVD-ROM Drive
  
       When booting from a Plextor PX-716SA DVD-ROM drive attached
       directly to an Adaptec controller, the boot appears to fail 
       with a "Could not find boot CD-ROM" error, but resumes
       and boots successfully if you wait approximately 30 seconds
       or until the front LED light turns green.

  4.33 Series 6 Controllers Hang on Warm Reboot on Some ASUS Motherboards
  
       Due to a PCIe configuration problem with ASUS P7P55-WS, KCMA-D8,  
       and KGPE-D16 motherboards, Adaptec Series 6 controllers may hang 
       on warm reboot if installed in a slot with x8 links. (The problem
       is not seen with Adaptec Series 5 controllers.)
       WORKAROUND: Use a slot with x16 links.
       
  4.34 HDA Mode Reset for Adaptec Series 6 Controllers
  
       Adaptec Series 6 controllers do not support Mode 0 reset, using
       the AFU utility on the Adaptec Installation DVD. To flash 
       a Series 6 controller using the HDA mode jumper on the controller  
       board, you must use the PFU.exe utility, which uses a FDL   
       firmware file (as opposed to a UFI file) to reset the board.  
       PFU.exe is available only from your Adaptec by PMC service  
       representative; it is not included on the installation DVD. For
       the location of the HDA jumper on Series 6 controllers, see the
       user's guide. 
              
--------------------------------------------------------------------
(c) 2012 PMC-Sierra, Inc. All Rights Reserved. 

This software is protected under international copyright laws and 
treaties. It may only be used in accordance with the terms 
of its accompanying license agreement.

The information in this document is proprietary and confidential to
PMC-Sierra, Inc., and for its customers' internal use. In any event,
no part of this document may be reproduced or redistributed in any
form without the express written consent of PMC-Sierra, Inc.,  
1380 Bordeaux Drive, Sunnyvale, CA 94089.

P/N DOC-01699-01-A Rev B
