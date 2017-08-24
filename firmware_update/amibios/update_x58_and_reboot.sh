#!/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/amibios"
#cd "$(dirname "$(readlink -f "$0")")"

$BASEDIR/update_x58.sh -y
echo "update finished"
sleep 10
reboot
