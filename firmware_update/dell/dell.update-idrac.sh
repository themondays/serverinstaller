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
if [ $# -ne 4 ]
then
  echo Usage: sh scriptfilename.sh IPADDRESS USERNAME PASSWORD MODEL
  exit 0
fi

source ./dell-functions.sh

IPADDRESS=$1
USERNAME=$2
PASSWORD=$3
MODEL=$4
WORKDIR=$(mktemp --dir)
IDRAC_Instance_ID="DCIM:INSTALLED#iDRAC.Embedded.1-1#IDRACinfo"
IDRAC_R720_URL="http://213.133.99.97/tools/Dell/firmware/r720/iDRAC-with-Lifecycle-Controller_Firmware_JHF76_WN32_2.30.30.30_A00.EXE"
IDRAC_R730_URL="http://213.133.99.97/tools/Dell/firmware/r730/iDRAC-with-Lifecycle-Controller_Firmware_5GCHC_WN32_2.30.30.30_A00.EXE"
IDRAC_R530_URL="http://213.133.99.97/tools/Dell/firmware/r530/iDRAC-with-Lifecycle-Controller_Firmware_5GCHC_WN32_2.30.30.30_A00.EXE"

### iDRAC (must be updated separately)

echo "INFO: Uploading iDRAC FW"
IDRAC_URL=$(eval echo \$IDRAC_${MODEL}_URL)
generate_request_xml "$WORKDIR/request.xml" $IDRAC_URL $IDRAC_Instance_ID
invoke_install "$WORKDIR/output.xml"

echo "INFO: iDRAC will reboot automaticall once upload is complete"

# List jobs

echo "INFO: Currently scheduled jobs:"
echo wsman enumerate http://schemas.dmtf.org/wbem/wscim/1/cim-schema/2/DCIM_LifecycleJob -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V | egrep "Message>|Name>"
wsman enumerate http://schemas.dmtf.org/wbem/wscim/1/cim-schema/2/DCIM_LifecycleJob -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V | egrep "Message>|Name>"


rm -rf $WORKDIR
