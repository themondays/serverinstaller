#!/bin/bash

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

exit 0
