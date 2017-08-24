#!/bin/bash

NFSPATH="/root/.oldroot/nfs/ipmi/asus/"
SCRIPTSPATH="/root/.oldroot/nfs/ipmi/scripts/"

# check if bmc is available
ipmitool mc info 1>/dev/null 2>&1
if [ $? -ne 0 ] ; then
  echo "No BMC found!"
  exit 0
fi

# reset bmc
echo -n "Resetting BMC to factory defaults ... "
ipmitool raw 0x32 0x66 > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed"
  exit 1
fi

echo " Wait 5 seconds to let BCM restart ..."
sleep 5

echo -n " Wait for BMC getting up again ... "
count=0
found=0

ipmitool mc info > /dev/null &
ipmitool_pid=$!

while [ $count -lt 30 ] ; do
  sleep 3
  if ! kill -0 $ipmitool_pid 1>/dev/null 2>&1 ; then
    found=1
    break
  fi
  let count=count+1
done

if [ $found -eq 1 ] ; then
  echo "up"

  count=0
  found=0
  echo -n " Check if BMC is available ... "
  while [ $count -lt 15 ] ; do
    sleep 2
    if [ "$(ipmitool mc info 2>/dev/null | egrep "^Device Available" | awk '{print $4}')" == "yes" ] ; then
      found=1
      break
    fi
    let count=count+1
  done

  if [ $found -eq 1 ] ; then
    echo "yes"
  else
    echo "no"
    echo "Please check with command 'ipmitool mc info'!"
    exit 4
  fi
else
  echo "down"
  exit 3
fi

# reset network config
echo "Set network config to 0.0.0.0 ..."
$SCRIPTSPATH/config_network.sh static 0.0.0.0 255.255.255.0 0.0.0.0 8

# disable ssh
echo "Disable ssh ..."
ipmitool raw 0x32 0x6a 0x20 0x00 0x00 0x00 0x00 0x46 \
  0x46 0x46 0x46 0x46 0x46 0x46 0x46 0x46 0x46 0x46 \
  0x46 0x46 0x46 0x46 0x46 0x00 0xff 0xff 0xff 0xff \
  0x16 0x00 0x00 0x00 0x08 0x07 0x00 0x00 0xff 0xff

exit 0
