#!/bin/bash
#  User command line input uses the following format:
#
#  sh scriptfilename.sh IPADDRESS USERNAME PASSWORD
#
#  [1] Replace IPADDRESS with the actual IP address using the format "aa.bb.cc.dd"  
#  [2] Replace USERNAME with the actual user name to logon to the target system
#  [3] Replace PASSWORD with the actual password to logon to the target system

#
#  BEGINNING OF SCRIPT EXECUTION
#


# --Check for correct number of command line parameters
if [ $# -ne 5 ]
then
  echo "Usage: sh scriptfilename.sh IPADDRESS USERNAME PASSWORD MODEL ITEM"
  echo "Item can be: bios, nic, raid, dp|driverpack, diag, all"
  echo "It is assume that R720 have Intel NICs and 730/530 have Broadcom"
  exit 0
fi

source ./dell-functions.sh

IPADDRESS=$1
USERNAME=$2
PASSWORD=$3
MODEL=$4
WORKDIR=$(mktemp --dir)
BIOS_Instance_ID="DCIM:INSTALLED#741__BIOS.Setup.1-1"
BIOS_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/BIOS_MKCTM_WN32_2.5.2.EXE"
BIOS_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/BIOS_2H4CC_WN32_2.0.2.EXE"
BIOS_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/BIOS_W8YX1_WN32_1.6.2.EXE"

NIC_Instance_ID="DCIM:INSTALLED#701__NIC.Integrated.1-1-1"
NIC_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/Network_Firmware_F8H29_WN32_17.0.12_A00.EXE"
NIC_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/Network_Firmware_0MT4K_WN64_7.10.64.EXE
NIC_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/Network_Firmware_0MT4K_WN64_7.10.64.EXE"

RAID_Instance_ID="DCIM:INSTALLED#301_C_RAID.Integrated.1-1"
RAID_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/SAS-RAID_Firmware_1TJRK_WN32_21.3.2-0005_A07.EXE"
RAID_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/SAS-RAID_Firmware_VH28K_WN32_25.4.0.0017_A06.EXE"
RAID_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/SAS-RAID_Firmware_N3C08_WN32_25.4.0.0017_A04.EXE"

UEFI_Diag_Instance_ID="DCIM:INSTALLED#802__Diagnostics.Embedded.1:LC.Embedded.1"
UEFI_Diag_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/Diagnostics_Application_D5TM2_WN32_4247A1_4247.2.EXE"
UEFI_Diag_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/Diagnostics_Application_NCTT1_WN32_4239A29_4239.37.EXE"
UEFI_Diag_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/Diagnostics_Application_NCTT1_WN32_4239A29_4239.37.EXE"

DP_Instance_ID="DCIM:INSTALLED#802__DriverPack.Embedded.1:LC.Embedded.1"
DP_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/R720_Drivers-for-OS-Deployment_Application_TD96J_WN32_15.07.07_A00.EXE"
DP_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/Drivers-for-OS-Deployment_Application_0MGRT_WN32_16.03.00_A00.EXE
DP_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/Drivers-for-OS-Deployment_Application_D8XMV_WN32_16.03.00_A00.EXE"

### bios

get_jobs() {
  echo "INFO: Currently scheduled jobs:"
  echo "wsman enumerate http://schemas.dmtf.org/wbem/wscim/1/cim-schema/2/DCIM_LifecycleJob -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V | egrep  'Message>|Name>'"
  wsman enumerate http://schemas.dmtf.org/wbem/wscim/1/cim-schema/2/DCIM_LifecycleJob -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V | egrep "Message>|Name>"
}

do_update() {
  local url=$(eval echo \$${1}_${MODEL}_URL)
  local instance_id=$(eval echo \$${1}_Instance_ID)

  if [ "$MODEL" = 'R530' -a $1 = "NIC" ]; then
	instance_id="DCIM:INSTALLED#701__NIC.Embedded.1-1-1"
  fi
  echo "INFO: Uploading $1"
  generate_request_xml "$WORKDIR/request.xml" $url $instance_id
  invoke_install "$WORKDIR/output.xml"

  echo "INFO: Scheduling $1 update job"
  job_instance_id=$(cat $WORKDIR/output.xml | sed -e 's/> </>\n</g' | grep InstanceID | cut -d '>' -f 2- | cut -d '<' -f 1)

  generate_job_xml "$WORKDIR/job.xml" $job_instance_id
  schedule_job "$WORKDIR/job.xml"
}

case $5 in
  status)
    get_jobs
    ;;
  bios|BIOS)
    do_update "BIOS"
    get_jobs
    ;;
  nic|NIC)
    do_update "NIC"
    get_jobs
    ;;
  raid|RAID)
    do_update "RAID"
    get_jobs
    ;;
  diag|DIAG)
    do_update "UEFI_Diag"
    get_jobs
    ;;
  dp|driverpack)
    do_update "DP"
    get_jobs
    ;;
#  lcc|LCC)
#    do_update "LCC"
#    get_jobs
#    ;;
  all|*)
    do_update "BIOS"
    do_update "NIC"
    do_update "RAID"
    do_update "UEFI_Diag"
    do_update "DP"
#    do_update "LCC"
    get_jobs
  ;;
esac

rm -rf $WORKDIR
