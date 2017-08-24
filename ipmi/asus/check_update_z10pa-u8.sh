#!/bin/bash

DIR="/root/.oldroot/nfs/ipmi/asus"

FWREV="112"
FWDIR="$DIR/Z10PAU8_$FWREV"

# if update="true" bmc will be updated to latest version
update="$1"

get_fw_version() {
 local bmc_fw_ver="$(ipmitool mc info 2>/dev/null | grep 'Firmware Revision' | cut -d ':' -f 2 | sed -e 's/ //g;s/\.//g')"
 echo $bmc_fw_ver
}

get_ipmitool_version() {
 local ipmi_ver="$(ipmitool -V | cut -d ' ' -f 3- | sed -e 's/\.//g')"
 echo $ipmi_ver
}

bmc_current_ver=$(get_fw_version)
ipmitool_ver=$(get_ipmitool_version)

#if [ "$ipmitool_ver" -lt 1814 ]; then
#  FWREV="18"
#fi

if [ -n "$bmc_current_ver" ]; then
  if [ "$bmc_current_ver" -lt "$FWREV" ]; then
    echo "BMC Firmware needs to be updated!"
    if [ "$update" = "true" ]; then
      echo "Disabling virtual media for firmware update."
      #disable virtual media
      ipmitool raw 0x32 0xaa 0x00
      sleep 1
      echo -n "Starting Firmware update..."
      cd $FWDIR
      ./update.sh
      echo "done."
      echo "Waiting 120 seconds for BMC reboot..."
      sleep 120
      bmc_current_ver=$(get_fw_version)
      if [ "$bmc_current_ver" = "$FWREV" ]; then
        echo "Update successful."
        echo "Re-enabling virtual media."
        #re-enable virtual media
        ipmitool raw 0x32 0xaa 0x01
        exit 0
      else
        echo "Update failed"
        exit 1
      fi
    fi
    # exit with error, as we should have updated
    exit 1
  else 
    echo "BMC FW is up2date ($bmc_current_ver)."
    exit 0
  fi
else
 echo "No BMC found."
 exit 0
fi

