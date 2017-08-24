#!/bin/bash

DIR="$(dirname $(readlink -f $0))"


echo "Looking for MX200 and do update if needed ..."
$DIR/msecli -U -i $DIR/mx200 -m MX200 -y

exit 0
