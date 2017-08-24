#!/bin/bash

CHECK_PATH="/root/.oldroot/nfs/check/bin"

echo "Configure stressapptest to measure power consumption."

threads=0

if [ "$#" -eq 0 ] ; then
  echo "No drives given!"
  exit 0
fi

disks=""
echo -n "These disks will be stressed:"
for disk in $@; do
  disks="$disks -d /dev/$disk"
  echo -n " /dev/$disk"
done
echo

echo "Start stressapptest for 5 minutes ..."
$CHECK_PATH/stressapptest -m $threads -C $threads -i $threads --cache-size 64mb --random-threads 1 -s 300 --destructive $disks 1>/dev/null 2>&1
echo "Done!"

exit 0
