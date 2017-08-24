#/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/asusbios"
FLASHBIN="AsusLinuxBiosUpdater"
BIOS="Z10PA-U8-ASUS-3403-S17030101.CAP"
BIOSVER="3403"
MB=$(dmidecode -s baseboard-product-name | head -n1)
CURRENT=$(dmidecode -s bios-version | head -n1)
NOWAIT=0

if [ "$1" = '-y' ]; then 
 NOWAIT=1
fi


if [  "$MB" = "Z10PA-U8 Series" ]; then

  if [ "$NOWAIT" -eq 0 ]; then
    echo "WARNING: This will update the current BIOS from $CURRENT to $BIOSVER"
    read -p "Press 'y' to continue: "  ASK
    if [ "$ASK" != "y" ]; then
      echo "aborting"
      exit 0
    fi
  fi
  if [ -e "$BASEDIR/$BIOS" ]; then
    cp $BASEDIR/$BIOS /tmp
  else
    echo "bios not found."
    exit 1
  fi
  cd $BASEDIR
  if [ -x "$FLASHBIN" ]; then
      ./$FLASHBIN -i /tmp/$BIOS -fs 16
  else
    echo "flash binary not found."
    exit 1
  fi
else
 echo "wrong mainboard"
fi
