#!/bin/bash

DIR="$(dirname $(readlink -f $0))"

if [ -n "$1" ] && [ -b "$1" ]; then
  $DIR/msecli -U -i $DIR/5100 -n $1
else
  echo "Looking for 5100ECO and update if needed ..."
  $DIR/msecli -U -i $DIR/5100 -m 5100ECO -y

  echo "Looking for 5100PRO and update if needed ..."
  $DIR/msecli -U -i $DIR/5100 -m 5100PRO -y
fi

exit 0
