#!/bin/bash

echo -n "Add i386 to dpkg ... "
dpkg --add-architecture i386 > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed"
  exit 1
fi

echo -n "Add i386 to apt sources.list ... "
sed -i 's/deb http:\/\/mirror\.hetzner\.de\/debian\/packages/deb \[arch=amd64,i386\] http:\/\/mirror\.hetzner\.de\/debian\/packages/g' /etc/apt/sources.list > /dev/null
if [ $? -eq 0 ] ; then
  echo "done"
else
  echo "failed"
  exit 1
fi

echo "Doing apt-get update ... "
apt-get update

echo "Install ia32-libs and libncurses5-dev:i386 ..."
apt-get -y install ia32-libs libncurses5-dev:i386

exit 0
