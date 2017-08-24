#!/bin/bash

board="$(dmidecode -s baseboard-product-name|grep -v '#')"

if [ "${board}" != "S1200RP" ] ; then
  echo "The mainboard '${board}' is not supported!"
  exit 1
fi

dir="/root/.oldroot/nfs/firmware_update/intel_s1200v3rp"
sysdir="/root/.oldroot/nfs/ipmi/intel"
bios_pri_ver=$(${dir}/flashupdt -i|grep "Primary"| grep -oE [0-9]{2}\\.[0-9]{2}\\.[0-9]{4}|sed 's/\./-/g')
bios_sec_ver=$(${dir}/flashupdt -i|grep "Secondary"| grep -oE [0-9]{2}\\.[0-9]{2}\\.[0-9]{4}|sed 's/\./-/g')

bios_latest="03-03-0006"

function check_cfg {
  local cfg_value=$(/root/.oldroot/nfs/ipmi/intel/syscfg /d biossettings individual "${1}"|grep "Current Value"|sed 's/Current Value : //g')
  echo "${cfg_value}"
}

function update_primary {
  ${dir}/flashupdt -u ${dir}/${bios_latest}/update.cfg
  clear_bios
}

function update_secondary {
  ${dir}/flashupdt -u ${dir}/${bios_latest}/update-backup.cfg
  clear_bios
  ${sysdir}/syscfg /bcs "" "Console Redirection" 1
}

function check_clear {
  if [ "$(check_cfg "Console Redirection")" = "Disabled" ] && [ "$(check_cfg "Intel(R) VT for Directed I/O")" = "Disabled" ] && [ "$(/root/.oldroot/nfs/ipmi/intel/syscfg /bbosys|grep "Network Card")" != "1: Network Card" ] && [ "$(check_cfg "Resume on AC Power Loss")" = "Stay Off" ]; then
    (>&2 echo BIOS cleared)
    echo 0
  elif [ "$(check_cfg "Console Redirection")" = "Serial Port A" ] && [ "$(check_cfg "Intel(R) VT for Directed I/O")" = "Disabled" ] && [ "$(/root/.oldroot/nfs/ipmi/intel/syscfg /bbosys|grep "Network Card")" = "1: Network Card" ] && [ "$(sed -e 's/-//g' <<< ${bios_pri_ver})" -gt 1999999 ] && [ "$(check_cfg "Resume on AC Power Loss")" = "Power On" ]; then
    (>&2 echo Hetzner defaults succesfully configured)
    echo 0
  elif [ "$(sed -e 's/-//g' <<< ${bios_pri_ver})" -lt 2000000 ]; then
    (>&2 echo "BIOS version lower than 02.01.0002 and not cleared / clearing BIOS - REBOOT AFTERWARDS!!")
    (>&2 echo -e "\E[0m\E[01;31m\033[5mIf there is an installed system, you need to boot the server to PXE manually afterwards!\E[0m")
    echo 1
  else
    echo 0
  fi
}

function clear_bios {
  local return=$(2>&1 ${sysdir}/syscfg /bldfs "")
  echo "${return}"
  if [ $(grep -c 'Set variable failed from SMI driver' <<< ${return}) -gt 0 ]; then
    echo ""
    echo -e "         \E[0;101m***\E[0m    \E[01;31m\033[5m ERROR \E[0m   \E[0;101m***\E[0m\E[01;31m"
    echo -e "An error occurred. Please open a Ticket to\nclear the CMOS by removing the CMOS battery!"
    echo -e "         \E[0;101m***\E[0m    \E[01;31m \033[5mERROR \E[0m   \E[0;101m***\E[0m\E[01;31m"
    echo ""
    exit 1
  fi
}

### main

if [[ "${1}" != "-y" ]]; then
  echo -e "WARNING: The script needs to be run up to 3 times, until both primary and\nsecondary BIOS were updated to $(sed -e 's/-/./g' <<< ${bios_latest}). The server needs to be rebooted and\nbooted back to the Rescue System after each run. Please press Y to confirm."
  read -p "" -n 1 ;echo
  if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    exit 1
  fi
fi

if [ $(check_clear) = 1 ]; then
  echo clearing bios
  clear_bios
  exit 0
fi

if [ ${bios_pri_ver} = ${bios_latest} ] && [ ${bios_sec_ver} = ${bios_latest} ]; then
  echo "primary (${bios_pri_ver}) and secondary (${bios_sec_ver}) BIOS up to date (${bios_latest})"
elif [ ${bios_pri_ver} = ${bios_latest} ]; then
  echo "secondary BIOS ${bios_sec_ver} out of date / starting secondary BIOS update - REBOOT AFTERWARDS!!"
  update_secondary
else
  echo "primary (${bios_pri_ver}) and secondary (${bios_sec_ver}) BIOS out of date / starting primary BIOS update - REBOOT AFTERWARDS!!"
  update_primary
  if [ "$(sed -e 's/-//g' <<< ${bios_pri_ver})" -gt 1999999 ]; then
    echo "primary BIOS is version 2 or later / also updating secondary BIOS"
    update_secondary
  fi
fi

exit 0
