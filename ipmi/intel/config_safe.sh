#!/bin/bash

echo "Install freeipmi-tools ..."
aptitude -y install freeipmi-tools

echo "Save current bmc config ..."
bmc-config --checkout --filename /tmp/bmc.config

echo "Remove cipher_suite 0 posibility ..."
sed -i 's/.*Maximum_Privilege_Cipher_Suite_Id_0.*/        Maximum_Privilege_Cipher_Suite_Id_0           Unused/g' /tmp/bmc.config

echo "Reload new bmc config ..."
bmc-config --commit --filename /tmp/bmc.config 1>/dev/null 2>&1

echo "Disable ipmi ssh ..."
ipmitool raw 0x30 0xb1 0x00 0x80 0x00 1>/dev/null 2>&1

echo "Done!"

exit 0
