#!/bin/bash

DIR="$(dirname $(readlink -f $0))"


echo "Looking for M600 and do update if needed ..."
$DIR/msecli -U -i $DIR/m600 -m M600 -y

exit 0
