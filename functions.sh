
# some common functions for the rescue-system
# source this file via ". filename"
#
# david.mayr@hetzner.de - 2007.09.26




# sleep some time and print dots ...
#
sleep_dots() {
  count=1
  max=$1
  until [ $count -gt $max ] ; do
    sleep 1
    echo -en '.'
    count=$(( $count + 1 ))
  done
  echo
}


# get ip address of network interface
#
get_ip() {
  # use first parameter as device name, if present
  [ "$1" ] && DEVICE="$1" || DEVICE="eth0"
  # get the IP with ifconfig
  ifconfig $DEVICE | grep inet | tr -s ' ' | cut -d\  -f3 | cut -d: -f2
}


# get mac address of network interface
#
get_mac() {
  # use first parameter as device name, if present
  if [ -z "$1" ]
  then  DEVICE="eth0"
  else  DEVICE="$1"
  fi

  # get the MAC with ifconfig
  ifconfig $DEVICE | grep HWaddr | tr -s ' ' | cut -d\  -f5
}


# send command to reboot into rescue system
#
reboot_rescue() {
  SERVER="213.133.99.100"
  arch="$1"
  args="$2 $3 $4 $5 $6"
  [ "$arch" ] || return 1
  curl -s  http://$SERVER/bootserver/remote_cmd?command=startrescue$arch\&ip=$(get_ip)\&mac=$(get_mac)\&args=$args \
    | grep -v "keine Ausgabe" \
    || ( echo -n "OK. REBOOT in 3 seconds " ; sleep_dots 3 ; reboot )
}
reboot_rescue32() {
  reboot_rescue 32 $*
}
reboot_rescue64() {
  reboot_rescue 64 $*
}

