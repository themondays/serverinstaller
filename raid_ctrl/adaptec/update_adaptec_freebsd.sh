#!/bin/bash

BASEDIR="/root/.oldroot/nfs/raid_ctrl/adaptec"
EXTRA=""
VERSION="$(arcconf getconfig 1 ad | grep Firmware | awk -F '[()]' '{print $2}')"
CURRENT_SER8='32722'

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
  echo "this update is for 8405 only."
  exit 0
elif lspci -n | grep -q 9005:028d; then
  echo "Series 8 controller found"
  if [ "$VERSION" = "$CURRENT_SER8" ]; then
    echo "already at $VERSION, skipping"
    exit 0
  else
    if arcconf getconfig 1 ad | grep -q ASR8405; then
      echo "8405 found"
      cd "$BASEDIR/8405/$CURRENT_SER8"
      arcconf romupdate 1 as840501 $EXTRA
    elif arcconf getconfig 1 ad | grep -q ASR81605; then
      echo "81605Z found"
      echo "this update is for 8405 only."
      exit 0
    fi
  fi
else
  echo "no Adaptec controller found"
fi
