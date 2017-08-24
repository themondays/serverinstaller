#!/bin/bash

BASEDIR="/root/.oldroot/nfs/raid_ctrl/adaptec"
EXTRA=""
VERSION="$(arcconf getconfig 1 ad | grep Firmware | awk -F '[()]' '{print $2}')"
CURRENT_SER8='33173'

if [ "$1" ]; then
  if [ "$1" = 'noprompt' ]; then
    EXTRA="noprompt"
  else
    echo "unknown option"
    exit 1
  fi
fi

if lspci -n | grep -q 9005:0285; then
  echo "5405 found"
  cd "$BASEDIR/5405/current"
  arcconf-200 romupdate 1 as5405 $EXTRA
elif lspci -n | grep -q 9005:028d; then
  echo "Series 8 controller found"
  if [ "$VERSION" = "$CURRENT_SER8" ]; then
    echo "already up2date, skipping"
    exit 0
  else
    if arcconf getconfig 1 ad | grep -q ASR8405; then
      echo "8405 found"
      cd "$BASEDIR/8405/current"
      arcconf romupdate 1 as840501 $EXTRA
    elif arcconf getconfig 1 ad | grep -q ASR81605; then
      echo "81605Z found"
      cd "$BASEDIR/81605Z/current"
      arcconf romupdate 1 AS816Z01 $EXTRA
    fi
  fi
else
  echo "no Adaptec controller found"
fi
