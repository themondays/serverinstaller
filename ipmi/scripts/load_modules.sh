#!/bin/bash

echo -n "Load ipmi modules ... "
modprobe ipmi_devintf 1>/dev/null 2>&1
modprobe ipmi_si 1>/dev/null 2>&1
echo "done"

exit 0
