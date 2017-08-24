#!/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/msi/aptio5"
FLASHMOD="$BASEDIR/$(uname -r)/amifldrv_mod.o"
FLASHPATH="/opt/aptio3/"
FLASHBIN="afulnx_64"
ROMDIR="$BASEDIR/MS-7816"
BIOS="E7816IMT.2EB"
BIOSVER="V2.14B11"
FLASHCMD="$FLASHBIN $ROMDIR/$BIOS"
MB=$(dmidecode -s system-product-name)
CURRENT=$(dmidecode -s bios-version)
NOWAIT=0

if [ "$1" = '-y' ]; then 
 NOWAIT=1
fi

if [  "$MB" = "MS-7816" ]; then
  if [ "$CURRENT" = "$BIOSVER" ]; then
    echo "not updating. BIOS is already at $BIOSVER"
    exit 0
  fi

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
      ./$FLASHCMD /CAF /Q
      ./$FLASHCMD /B /N /L /CAF /Q
    else
      ./$FLASHCMD /CAF
      ./$FLASHCMD /B /N /L /CAF
    fi
  else
    echo "flash binary not found."
    exit 1
  fi
else
 echo "wrong mainboard"
fi
