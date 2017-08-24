#!/bin/bash

DIR="/root/.oldroot/nfs/ipmi"
PWD="$DIR/smi/"

# if reset="reset" bmc will be reset to factory defaults
reset="$1"

# if disable="yes" fans and intrussion detection will be disabled
disable="$2"
# default value is yes due to our standard configuration
[ -z "$disable" ] && disable="yes"
[ "$disable" != "yes" ] && disable="no"

if [ "$reset" = "reset" ] ; then
  $DIR/scripts/unload_modules.sh > /dev/null

  echo -n "Reset BMC to factory defaults ... "
  $PWD/ipmicfg -fde > /dev/null
  if [ $? -eq 0 ] ; then
    echo "done"
  else
    echo "failed!"
    exit 1
  fi

  echo -n "Perform a cold-reset of the BMC to load defaults ... "
  $PWD/ipmicfg -r > /dev/null
  if [ $? -eq 0 ] ; then
    echo "done"
  else
    echo "failed!"
    exit 1
  fi

  echo -n "Wait until the BMC is ready again ... "

  sleep 10

  $PWD/ipmicfg -ver > /dev/null
  if [ $? -eq 0 ] ; then
    echo "ready"
  else
    echo "not ready!"
    exit 1
  fi

  $DIR/scripts/load_modules.sh > /dev/null
fi

echo -n "Disable dhcp on interface ... "
$PWD/ipmicfg -dhcp off > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed!"
  exit 1
fi

echo -n "Wait some time to disable dhcp ... "
sleep 10
echo "done"

echo -n "Reset interface ip ..."
ip=""
while [ "$ip" != "0.0.0.0" ] ; do
  ip="$($PWD/ipmicfg -m 0.0.0.0 | cut -d= -f2)"
  if [ $? -ne 0 ] ; then
    echo "failed!"
    exit 1
  fi
  echo -n "."
done
echo " [${ip}] - done"

echo -n "Reset gateway ip ..."
sleep 5
gw=""
while [ "$gw" != "0.0.0.0" ] ; do
  gw="$($PWD/ipmicfg -g 0.0.0.0 | cut -d= -f2)"
  if [ $? -ne 0 ] ; then
    echo "failed!"
    exit 1
  fi
  echo -n "."
done
echo " [${gw}] - done"

# remove all fans except fan1 and intrussion detection
if [ "$disable" = "yes" ] ; then
  # reset values
  values_x9sri="339 406 473 540 1210"
  values_h8sgl="1009 1076 1143 1210 1277 1344 1411"

  echo -n "Remove unused values from the SDR ... "
  board_name="$(dmidecode -s baseboard-product-name)"
  values=""
  if [ "$board_name" = "H8SGL" ] ; then
    values=$values_h8sgl
  elif [ -n "$(echo $board_name | grep X9SRi)" ] ; then
    values=$values_x9sri
  fi
  for i in $values ; do
    [ -z "$($PWD/ipmicfg -sdr | egrep "(${i})")" ] && continue
    $PWD/ipmicfg -sdr del $i > /dev/null
    if [ $? -ne 0 ] ; then
      echo "failed!"
      exit 1
    fi
  done
  echo "done"
fi

echo "BMC was successfully resetted!"

exit 0
