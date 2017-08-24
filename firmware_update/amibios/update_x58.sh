#!/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/amibios"
FLASHMOD="$BASEDIR/$(uname -r)/amifldrv_mod.o"
FLASHPATH="/opt/amibios/"
FLASHBIN="afulnx_64"
ROMDIR="$BASEDIR/MS-7522"
BIOS="A7522IMT.8E8"
BIOSVER="V8.14B8"
FLASHCMD="$FLASHBIN $ROMDIR/$BIOS /L2"
MB=$(dmidecode -s system-product-name)
CURRENT=$(dmidecode -s bios-version)
NOWAIT=0

if [ "$1" = '-y' ]; then 
 NOWAIT=1
fi

if [  "$MB" = "MS-7522" ]; then
#  if [ "$CURRENT" = "$BIOSVER" ]; then
#    echo "not updating. BIOS is already at $BIOSVER"
#    exit 0
#  fi 

  if [ "$NOWAIT" -eq 0 ]; then
    echo "WARNING: This will update the current BIOS from $CURRENT to $BIOSVER"
    read -p "Press 'y' to continue: "  ASK
    if [ "$ASK" != "y" ]; then
      echo "aborting"
      exit 0
    fi
  fi
  if [ -e "$FLASHMOD" ]; then
    cp $FLASHMOD $FLASHPATH
  else
    echo "flash module not found."
    exit 1
  fi
  cd $FLASHPATH
  if [ -x "$FLASHBIN" ]; then
    if [ $NOWAIT -eq 1 ]; then
      ./$FLASHCMD /Q
    else
      ./$FLASHCMD
    fi
  else
    echo "flash binary not found."
    exit 1
  fi
else
 echo "wrong mainboard"
fi
