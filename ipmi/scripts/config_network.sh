#!/bin/bash

function current_config() {
  local config="$(ipmitool lan print $lan_chan | egrep "IP Address Source|IP Address|Subnet Mask|Default Gateway" | sed 's/.* : \(.*\)/\1/g')"
  local config_array=( $config )

  echo
  echo "Current config:"
  echo "IP source : ${config_array[0]} ${config_array[1]}"
  echo "IP        : ${config_array[2]}"
  echo "Netmask   : ${config_array[3]}"
  echo "Gateway   : ${config_array[4]}"
  echo
}

if [ $# -lt 1 ] ; then
  echo "Not enough parameters!"
  echo "Usage: ./set_link_intel.sh <static|dhcp>"
  exit 1
fi


if [ "$1" = "static" ] ; then
  if [ $# -lt 3 ] ; then
    echo "Not enough parameters for static configuration!"
    echo "Usage: ./set_link_intel.sh static <ip> <netmask> <gateway_ip> [<lan_channel>]"
    exit 1
  fi

  ip="$2"
  nm="$3"
  gw_ip="$4"

  lan_chan="$5"
  [ -z "$lan_chan" ] && lan_chan=1

  current_config

  echo "Set static ip ..."
  ipmitool lan set $lan_chan ipsrc static > /dev/null

  echo "Set ip to $ip ..."
  ipmitool lan set $lan_chan ipaddr $ip > /dev/null

  echo "Set netmask to $nm ..."
  ipmitool lan set $lan_chan netmask $nm > /dev/null

  echo "Set gateway ip to $gw_ip ..."
  ipmitool lan set $lan_chan defgw ipaddr $gw_ip > /dev/null

  echo "Set arp respon on ..."
  ipmitool lan set $lan_chan arp respond on > /dev/null
elif [ "$1" = "dhcp" ] ; then
  current_config

  echo "Set dhcp for ip ..."
  ipmitool lan set $lan_chan ipsrc dhcp > /dev/null
else
  echo "Wrong option: $1"
  exit 1
fi

echo "Set auth on ..."
ipmitool lan set $lan_chan auth ADMIN MD5 > /dev/null

echo "Set access on ..."
ipmitool lan set $lan_chan access on > /dev/null

if [ "$1" = "dhcp" ] ; then
  echo "Wait some time to receive ip via dhcp ..."
  sleep 5
fi

current_config

echo "Done!"
