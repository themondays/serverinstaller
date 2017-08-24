*****************************************************************************
* ATEN Technology, Inc.                                                     *
*****************************************************************************
* FUNCTION   :  IPMI FIRMWARE UPDATE UTILITY                                *
* VERSION    :  1.20                                                        *
* BUILD DATE :  May 18 2011                                                 *
* USAGE      :                                                              *
*             (1)Update FIRMWARE : lUpdate -f filename.bin [OPTION]         *
*             (2)Dump FIRMWARE   : lUpdate -d filename                      *
*             (3)Restore CONFIG  : lUpdate -c -f filename.bin               *
*             (4)Backup CONFIG   : lUpdate -c -d filename.bin               *
*****************************************************************************
* OPTION                                                                    *
*   -i the IPMI channel, currently, kcs and lan are supported               *
* LAN channel specific arguments                                            *
*   -h remote BMC address and RMCP+ port, (default port is 623)             *
*   -u IPMI user name                                                       *
*   -p IPMI password correlated to IPMI user name                           *
*   -r Preserve Configuration (default is Preserve)                         *
*      n:No Preserve, reset to factory default settings                     *
*      y:Preserve, keep all of the settings                                 *
*   -c IPMI configuration backup/restore                                    *
*      -f [restore.bin] Restore configurations                              *
*      -d [backup.bin] Backup configurations                                *
*****************************************************************************
* EXAMPLE                                                                   *
*   we like to upgrade firmware through KCS channel                         *
*   lUpdate -f fwuperade.bin -i kcs -r y                                    *
*   lUpdate -d fwdump.bin -i kcs -r y                                       *
*                                                                           *
*   we like to restore/backup IPMI config through KCS channel               *
*   lUpdate -c -f restore.bin -i kcs -r y                                   *
*   lUpdate -c -d backup.bin -i kcs -r y                                    *
*                                                                           *
*   we like to upgrade firmware through LAN channel with                    *
*   - BMC IP address 10.11.12.13 port 623                                   *
*   - IPMI username is usr                                                  *
*   - Password for alice is pwd                                             *
*   - Preserve Configuration                                                *
*   lUpdate -f fw.bin -i lan -h 10.11.12.13 623 -u usr -p pwd -r y          *
*   lUpdate -d fwdump.bin -i lan -h 10.11.12.13 623 -u usr -p pwd -r y      *
*                                                                           *
*   we like to restore/backup IPMI config through LAN channel with          *
*   - BMC IP address 10.11.12.13 port 623                                   *
*   - IPMI username is usr                                                  *
*   - Password for alice is pwd                                             *
*   - Preserve Configuration                                                *
*   lUpdate -c -f fw.bin -i lan -h 10.11.12.13 623 -u usr -p pwd            *
*   lUpdate -c -d fwdump.bin -i lan -h 10.11.12.13 623 -u usr -p pwd        *
*****************************************************************************


1.12 add list:
(1) add preserve configuration.

1.14 add list:
(1) add error handling.
(2) use kcs.cfg to get KCS CMD/DATA address

1.15 add list:
(1) add dump flash feature
(2) default port number is 623 & default preserve configuration is yes
(3) fixed the flash tool will dump the kernel message
(4) if ipmi related driver exists, unload the ipmi driver before executing FW update 
	and load the ipmi driver after executing FW update.

1.18 add list:
(1) add IPMI configuration restore/backup mode     

1.19 add list:
(1) use cipher suit 1 to open session

1.20 add list
(1) if IPMI related driver exists , using origin ipmi driver to FW update.
(2) fix get device function error.
(3) In this version you can directly use update program without any other shell scripts

1.21 add list:
(1)  fix the issue for configuration restore/backup 