#/bin/bash

FLASHPATH="/opt/amibios_efi"
FLASHBIN="afulnx_26_64"
ROMDIR="/root/.oldroot/nfs/firmware_update/amibios_efi"
BIOS="E7756IMS.1B0"
BIOSVER="V1.11"
FLASHCMD="$FLASHBIN $ROMDIR/$BIOS /L2"
MB=$(dmidecode -s system-product-name)
CURRENT=$(dmidecode -s bios-version)
NOWAIT=0

if [ "$1" = '-y' ]; then 
 NOWAIT=1
fi

if [  "$MB" = "MS-7756" ]; then
#  if [ "$CURRENT" = "$BIOSVER" ]; then
#    echo "not updating. BIOS is already at $BIOSVER"
#    exit 0
#  fi 

  if [ "$NOWAIT" -eq 0 ]; then
    echo "WARNING: This will update the current BIOS from $CURRENT to $BIOSVER"
#    echo "Press 'y' to continue"
    read -p "Press 'y' to continue: "  ASK
    if [ "$ASK" != "y" ]; then
      echo "aborting"
      exit 0
    fi
  fi
  cd $FLASHPATH
  if [ $NOWAIT -eq 1 ]; then
    ./$FLASHCMD /Q
  else
    ./$FLASHCMD
  fi
else
 echo "wrong mainboard"
fi
