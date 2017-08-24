#!/bin/bash

BASEDIR="/root/.oldroot/nfs/firmware_update/fujitsu"
FLASHPATH="/opt/deskview/deskflash/bin"
FLASHBIN="deskflash"

MB=$(dmidecode -s baseboard-product-name | tail -n1)

BIOSDIR="$BASEDIR/bios"

kvers=$(uname -r)

RUNNING_BIOS="$(dmidecode -s bios-version | tail -n1)"
RUNNING_BIOSVERSION="$(echo "$RUNNING_BIOS" | awk '{print $2}' | sed -e 's/^R//g')"

if [ "$MB" = "D3417-B1" ]; then
  BIOSVERSION="1.21.0"
  RELEASE="SR.2"
  if [ "$RUNNING_BIOSVERSION" = '1.12.0' ]; then
    BIOSVERSION="1.12.0"
    RELEASE="SR.2"
  fi
else
  BIOSVERSION="1.8.0"
  RELEASE="SR.1"
fi

TARGET_BIOSVERSION="$BIOSVERSION"
if [ -n "$RELEASE" ]; then
  TARGET_BIOSVERSION="$BIOSVERSION.$RELEASE"
fi

FORCE=0

version_newer() {
  local version=$1 new=$2
  local compared=$(echo -e "$version\n$new" | sed '/^$/d' | sort --version-sort -r | head -1)
  [[ "$version" = "$new" ]] && return 2
  [[ "$compared" = "$new" ]] && return 0
  return 1
}

copy_modules() {
  echo "copy modules for kernel $kvers"
  if [ -d "$BASEDIR/$kvers" ]; then
    rsync -au "$BASEDIR/$kvers/" /opt/deskview/
  else
    echo "no modules found for kernel $kvers"
    exit 1
  fi
}

while getopts ":fb::m" opt ; do
  case $opt in
    f)
      FORCE=1
      ;;
    b)
      BIOSVERSION=$OPTARG
      ;;
    m)
      copy_modules
      exit 0
      ;;
    \?)
      echo "invalid option: -$OPTARG"
      exit 1
      ;;
    :)
      echo "option -$OPTARG requires an argument."
      exit 1
      ;;
  esac
done

NEWBIOS="${BIOSDIR}/${MB}x.R${TARGET_BIOSVERSION}.BUP"

# check if bios exists
if [ ! -e $NEWBIOS ] ; then
  echo "bios $NEWBIOS does not exist!"
  exit 1
fi

if [ "$MB" = "D3417-B1" ] || [ "$MB" = "D3417-B2" ]; then
  # copy modules from nfs
  copy_modules
  # check if new bios is newer than installed bios
  version_newer "$RUNNING_BIOSVERSION" "$TARGET_BIOSVERSION"
  if [ $? -gt 0 -a $FORCE -eq 0 ] ; then
    echo "installed bios is already updated or newer ($RUNNING_BIOSVERSION installed, latest $TARGET_BIOSVERSION)"
    exit 0
  else
    echo "updating bios from $RUNNING_BIOSVERSION to $TARGET_BIOSVERSION"
    output="$($FLASHPATH/$FLASHBIN -ov -narb -o $NEWBIOS)"
    ret=$?
    if [ $ret -eq 0 ] ; then
      echo "scheduling successfully done. Reboot to start update. If that does not work. Retry with init 0"
    else
      echo "OUTPUT: $(echo "$output" | tail -n1)"
      echo "scheduling failed!"
      exit 1
    fi
  fi
else
  echo "wrong mainboard"
  exit 1
fi
