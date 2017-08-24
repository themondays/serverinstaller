#!/bin/bash

DIR="/root/.oldroot/nfs/ipmi/smi"

FWREV="3.50"
FWDIR="$DIR/X9SRI_$FWREV"

# if update="true" bmc will be updated to latest version
update="$1"

get_fw_version() {
 #local bmc_fw_ver="$($DIR/ipmicfg -ver | cut -d ':' -f 2 | sed -e 's/ //g')"
 local bmc_fw_ver="$(ipmitool mc info | grep 'Firmware Revision' | cut -d ':' -f 2 | sed -e 's/ //g')"
 echo $bmc_fw_ver
}

bmc_current_ver=$(get_fw_version)

if [ "$bmc_current_ver" != "$FWREV" ]; then
  echo "BMC Firmware needs to be updated!"
  if [ "$update" = "true" ]; then
    echo -n "Starting Firmware update..."
    cd $FWDIR
    ./update.sh
    echo "done."
    echo "Waiting 90 seconds for BMC reboot..."
    sleep 90
    bmc_current_ver=$(get_fw_version)
    if [ "$bmc_current_ver" = "$FWREV" ]; then
      echo "Update successful."
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

