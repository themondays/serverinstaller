#!/bin/bash

DIR="$(dirname $(readlink -f $0))"


echo "Looking for MX100 and do update if needed ..."
$DIR/msecli -U -i $DIR/mx100 -m MX100 -y

exit 0
