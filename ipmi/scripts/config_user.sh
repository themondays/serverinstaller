#!/bin/bash

if [ $# -lt 2 ] ; then
  echo "Not enough parameters!"
  echo "Usage: ./create_user_intel.sh username password"
  exit 1
fi

user="$1"
pass="$2"

echo -n "Check if user already exists ... "
found_user="$(ipmitool user list 1 | grep $user)"
if [ -n "$found_user" ] ; then
  echo "found"
  echo "Cannot create user with same name!"
  exit 1
fi
echo "not found"

echo -n "Get last user id ..."
user_id=$(ipmitool user list 1 | tail -n1 | awk '{print $1}')
echo " ($user_id)"

new_user_id=$((user_id + 1))

echo "Create user (ID $new_user_id) with name $user ..."
ipmitool user set name $new_user_id $user

echo "Set password  ..."
ipmitool user set password $new_user_id $pass

echo "Set privileges ..."
ipmitool channel setaccess 1 $new_user_id link=on ipmi=on callin=on privilege=4

echo "Enable user ..."
ipmitool user enable $new_user_id

echo "Enable SOL for user ..."
ipmitool sol payload enable 1 $new_user_id

echo "Done!"

exit 0
