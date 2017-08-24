#!/bin/bash

board="$(dmidecode -s baseboard-product-name)"

dir="/root/.oldroot/nfs/firmware_update/intel_s1200v3rp"

update="$1"

if [ -z "$update" ] ; then
  echo '# no update specified'
  exit 1
fi

if [ ! -e "${dir}/${update}" ] ; then
  echo '# update folder does not exist'
  exit 1
fi

if [ "$board" = "S1200RP" ] ; then
  $dir/flashupdt -u ${dir}/${update}/frusdr.cfg >/dev/null 
  #1>/dev/null 2>&1
fi

exit 0
