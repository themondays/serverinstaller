#!/bin/bash

board="$(dmidecode -s baseboard-product-name)"
biosver=$(dmidecode -s bios-version | cut -d '.' -f3-5 | sed -e 's/\.//g')

dir="/root/.oldroot/nfs/firmware_update/intel_s1200v3rp"
sysdir="/root/.oldroot/nfs/ipmi/intel"

update="$1"

if [ -z "$update" ] ; then
  echo "Please specify the update!"
  exit 1
fi

if [ ! -e "${dir}/${update}" ] ; then
  echo "Your specified update folder does not exist!"
  echo "Folder: ${dir}/${update}"
  exit 1
fi

if [ "$board" = "S1200RP" ] ; then
  if [ "$biosver" -lt 2000000 ]; then
    echo "you need to update to 02-01-0002 first"
  fi
  echo
  echo "START UPDATE"
  $dir/flashupdt -u ${dir}/${update}/update.cfg
  echo
  echo "UPDATE DONE!"
  if [ -f ${dir}/${update}/update-backup.cfg ]; then
    echo "START UPDATE BACKUP BIOS"
    $dir/flashupdt -u ${dir}/${update}/update-backup.cfg
    echo "UPDATE BACKUP BIOS DONE!"
  fi
  echo "RESTORING BIOS SETTINGS"
  $sysdir/syscfg /bldfs ""
  $sysdir/syscfg /bcs "" "Console Redirection" 1
  echo "RESTORING BIOS SETTINGS DONE!"
else
  echo "The mainboard '$board' is not supported!"
  exit 1
fi

exit 0
