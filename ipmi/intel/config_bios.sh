#!/bin/bash

[ -e "/tmp/bios.ini" ] && rm /tmp/bios.ini

echo -n "Check mainboard name ... "
board="$(dmidecode -s baseboard-product-name)"
if [ -z "$(echo $board | grep S1200RP)" ] ; then
  echo "failed"
  echo "It seems that your mainboard is not the S1200RP!"
  exit 1
fi
echo "ok"

echo -n "Check bios version ... "
bios="$(dmidecode -s bios-version)"
if [ "$bios" != "S1200RP.86B.01.02.0004.060920131231" ] ; then
  echo "failed"
  echo "It seems that your bios is not up to date!"
  echo "Plese do an bios update!"
  exit 1
fi
echo "ok"
echo " BIOS-Version: $bios"
echo

echo "Get current bios config ..."
./syscfg /s /tmp/bios.ini 1>/dev/null 2>&1

echo "Patch bios configuration ..."
patch -p0 < bios.patch 1>/dev/null 2>&1

if [ $? -ne 0 ] ; then
  echo "Patch failed, so your bios seems to be already configured!"
  exit 0
fi

echo "Apply current bios config ..."
./syscfg /r /f /b /tmp/bios.ini 1>/dev/null 2>&1

echo "Done!"

exit 0
