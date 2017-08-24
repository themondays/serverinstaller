#!/bin/bash

NFSPATH="/root/.oldroot/nfs/ipmi/intel/"

echo -n "Unload all ipmi modules ... "
modules_unload=0
while [ $modules_unload -eq 0 ] ; do
  modules_ipmi="$(lsmod | grep ipmi)"
  if [ -z "$modules_ipmi" ] ; then
    modules_unload=1
    break
  fi
  while IFS= read -r line ; do
    module="$(echo "$line" | awk '{print $1}')"
    modprobe -r $module 1>/dev/null 2>&1
  done <<< "$modules_ipmi"
done
echo "done"

# reset bmc
echo -n "Resetting BMC to factory defaults ... "
$NFSPATH/syscfg -rfs > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed"
  exit 1
fi

echo "Try to cold reset the BMC (wait max. 60 seconds) ..."
echo -n " Send reset signal ... "
$NFSPATH/syscfg -rbmc > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed"
  exit 2
fi
sleep 2
echo -n " Wait for BMC getting up again ... "
count=0
found=0

$NFSPATH/syscfg -i > /dev/null &
syscfg_pid=$!

while [ $count -lt 30 ] ; do
  sleep 3
  if [ -z "$(ps -p $syscfg_pid | grep -v PID)" ] ; then
    found=1
    break
  fi
done
if [ $found -eq 1 ] ; then
  echo "up"
else
  echo "down"
  echo "You have to reset the whole server!"
  exit 3
fi

echo "Reload ipmi modules ..."
modprobe ipmi_si
modprobe ipmi_watchdog
modprobe ipmi_poweroff
modprobe ipmi_devintf
modprobe ipmi_msghandler

exit 0
