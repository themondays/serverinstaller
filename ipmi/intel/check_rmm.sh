#!/bin/bash

status="$(ipmitool raw 0x30 0x71 0x00 0x01 0x00 0x00 | awk '{print $2}')"

if [ "$status" = "00" ] ; then
  echo "RMM4 not present!"
elif [ "$status" = "02" ] ; then
  echo "RMM4 is present!"
fi

exit 0
