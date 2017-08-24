#!/bin/bash

if [ $# -eq 0 ] ; then
  echo "Not enough parameters!"
  echo "Usage: ./config_sol.sh <username|id>"
  exit 1
fi

check='^[0-9]+$'
if ! [[ $1 =~ $check ]] ; then
  user="$(ipmitool user list 1 | grep "$1" | awk '{print $1}')"
  if [ -z "$user" ] ; then
    echo "User $1 not found!"
    exit 1
  fi
else
  user="$1"
fi

echo "Enable serial-over-lan von user $user ..."
ipmitool sol payload enable 1 $user

echo "Done!"

exit 0

