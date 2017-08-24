#/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/asusbios"
FLASHBIN="AsusLinuxBiosUpdater"
BIOS="P8B-WS-ASUS-2106_def.ROM"
BIOSVER="2106"
MB=$(dmidecode -s baseboard-product-name | head -n1)
CURRENT=$(dmidecode -s bios-version | head -n1)
NOWAIT=0

if [ "$1" = '-y' ]; then 
 NOWAIT=1
fi


if [  "$MB" = "P8B WS" ]; then

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

  if [ $CURRENT -lt 2000 ] ; then
    echo "WARNING: systems with bios version lower than 2000 \
      need to be flashed again after reboot!"
  fi

  cd $BASEDIR
  if [ -x "$FLASHBIN" ]; then
      ./$FLASHBIN -i /tmp/$BIOS -fs 4
  else
    echo "flash binary not found."
    exit 1
  fi
else
 echo "wrong mainboard"
fi
