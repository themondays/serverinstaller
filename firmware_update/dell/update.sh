#!/bin/bash

power_edge_r720_available_items='ESM LCC ESM_WITH_LCC BIOS DIAG OSDA RAID NET'
power_edge_r730_available_items='ESM_WITH_LCC BIOS DIAG OSDA RAID NET'
power_edge_r530_available_items='ESM_WITH_LCC BIOS DIAG OSDA RAID NET'

power_edge_r720_system_model_regex='PowerEdge R720'
power_edge_r730_system_model_regex='PowerEdge R730'
power_edge_r530_system_model_regex='PowerEdge R530'

power_edge_r720_update_site='http://213.133.99.97/tools/Dell/firmware/r720'
power_edge_r730_update_site='http://213.133.99.97/tools/Dell/firmware/r730'
power_edge_r530_update_site='http://213.133.99.97/tools/Dell/firmware/r530'

exit_hook() {
  if [[ -d ${tempdir} ]]; then
    rm -r ${tempdir}
  fi
}
help() {
  cat <<EOH
help: ${0} <command>
  commands:
    list-available-items
    list-available-files [item]
    list-available-updates [item]
    update-item-by-file|item-by-file [-force] <item> <file>
    update-item|item [-force] <item>
    update-all-items|all-items|all [-force]
EOH
  exit 1
}
abort() {
  echo ${@} >&2
  echo Aborting >&2
  exit 1
}
strip() {
  cat - | xargs
}
racadm_binary() {
  if ! command -v racadm; then
    abort Missing racadm binary
  fi
}
system_model() {
  if [[ -f system_model ]]; then
    cat system_model
  else
    if ! $(racadm_binary) getsysinfo | grep -i 'system model' | cut -d = -f 2 | strip > system_model; then
      abort Unable to identify system model
    fi
    system_model
  fi
}
list_available_items() {
  system_model=$(system_model)
  case ${system_model} in
    ${power_edge_r720_system_model_regex}) echo ${power_edge_r720_available_items} | tr ' ' "\n";;
    ${power_edge_r730_system_model_regex}) echo ${power_edge_r730_available_items} | tr ' ' "\n";;
    ${power_edge_r530_system_model_regex}) echo ${power_edge_r530_available_items} | tr ' ' "\n";;
    *)
      abort Unspecified system model ${system_model}
    ;;
  esac
}
update_site() {
  system_model=$(system_model)
  case ${system_model} in
    ${power_edge_r720_system_model_regex}) echo ${power_edge_r720_update_site};;
    ${power_edge_r730_system_model_regex}) echo ${power_edge_r730_update_site};;
    ${power_edge_r530_system_model_regex}) echo ${power_edge_r530_update_site};;
    *)
      abort Unspecified system model ${system_model}
    ;;
  esac
}
grep_file() {
  item=${1}
  case ${item} in
    ESM) cat - | grep -i ^ESM.*;;
    ESM_WITH_LCC)  cat - | grep -i ^IDRAC-WITH-LIFECYCLE.*;;
    LCC) cat - | grep -i ^LIFECYCLE.*;;
    BIOS) cat - | grep -i ^BIOS.*;;
    DIAG) cat - | grep -i ^DIAGNOSTIC.*;;
    OSDA) cat - | egrep -i '^DRIVERS.*|R720_DRIVERS.*';;
    RAID) cat - | grep -i ^SAS-RAID.*;;
    NET) cat - | grep -i ^NET.*;;
  esac
}
list_available_files() {
  if [ ${#} -eq 0 ]; then
    for item in $(list_available_items); do
      list_available_files ${item}
    done
  else
    item=$(echo ${1^^} | strip)
    if ! list_available_items | grep -oq ${item}; then
      abort Unspecified item ${item}
    fi
    echo By item ${item}:
    links=$(wget -o /dev/null -O - $(update_site) | grep -o '<a href="[^"]*"' | cut -d '"' -f 2 | grep .EXE | grep_file ${item})
    for link in ${links}; do
      echo $(update_site)/${link}
    done | sed 's/WN32/#/g' | sort -V -t '#' -k 2 | sed 's/#/WN32/g'
  fi
}
swinventory() {
  if [[ -f swinventory ]]; then
    cat swinventory
  else
    system_model=$(system_model)
    if ! $(racadm_binary) swinventory > swinventory_raw; then
      abort Unable to inventory
    fi
    GLOBAL_IFS=${IFS}
    IFS=$'\n'
    for line in $(cat swinventory_raw); do
      case ${line} in
        FQDD*) item=$(echo ${line} | cut -d = -f 2 | strip);;
        C*Version*) version=$(echo ${line} | cut -d = -f 2 | strip);;
        -*)
          if ! [[ -z "${version}" ]]; then
            case ${item} in
              iDRAC*)
                case ${system_model} in
                  ${power_edge_r720_system_model_regex})
                    echo ESM ${version}
                  ;;
                  ${power_edge_r730_system_model_regex}|${power_edge_r530_system_model_regex})
                    echo ESM_WITH_LCC ${version}
                  ;;
                  *)
                    abort Unspecified system model ${system_model}
                  ;;
                esac
              ;;
              USC*)
                case ${system_model} in
                  ${power_edge_r720_system_model_regex})
                    echo LCC ${version}
                  ;;
                  ${power_edge_r730_system_model_regex}|${power_edge_r530_system_model_regex})
                    :
                  ;;
                  *)
                    abort Unspecified system model ${system_model}
                  ;;
                esac
              ;;
              BIOS*) echo BIOS ${version};;
              Diag*) echo DIAG ${version};;
              Driver*) echo OSDA ${version};;
              RAID*) echo RAID ${version};;
              NIC*) echo NET ${version};;
            esac
          fi
          item=''
          version=''
        ;;
      esac
    done | uniq > swinventory
    IFS=${GLOBAL_IFS}
    swinventory
  fi
}
version() {
  item=${1}
  swinventory | grep ${item} | cut -d ' ' -f 2 | strip
}
version_by_file() {
  item=${1}
  echo ${item} | grep -o 'WN32_.*.EXE' | cut -d '_' -f 2- | rev | cut -d '.' -f 2- | cut -d '_' -f 2- | rev | strip
}
latest_file() {
  item=${1}
  list_available_files ${item} | tail -1
}
latest_version() {
  item=${1}
  version_by_file $(latest_file ${item})
}
update_available() {
  item=${1}
  ! [[ "$(echo $(version ${item}) $(latest_version ${item}) | tr ' ' "\n" | sort -V | tail -1)" == "$(version ${item})" ]]
}
list_available_updates() {
  if [ ${#} -eq 0 ]; then
    for item in $(list_available_items); do
      list_available_updates ${item}
    done
  else
    item=$(echo ${1^^} | strip)
    if ! list_available_items | grep -oq ${item}; then
      abort Unspecified item ${item}
    fi
    if update_available ${item}; then
      echo "${item} update $(latest_version ${item}) available ( Replacing installed update $(version ${item}) )."
    else
      echo "Item ${item} is up to date ( Update $(version ${item}) )."
    fi
  fi
}
update_item_by_file() {
  item=$(echo ${1^^} | strip)
  file=$(echo ${2} | strip)
  if ! list_available_items | grep -oq ${item}; then
    abort Unspecified item ${item}
  fi
  if ! list_available_files | grep -v "By item ${1}"| grep -oq ${file}; then
    abort Unspecified file ${file}
  fi
  if [[ "$(echo $(version ${item}) $(version_by_file ${file}) | tr ' ' "\n" | sort -V | tail -1)" == "$(version ${item})" ]]; then
    echo "Item ${item} is up to date ( Installed version $(version ${item}) )."
    if [[ "${force}" == "true" ]]; then
      echo Forcing anyway.
    else
      return
    fi
  else
    if ! update_available ${item}; then
      echo "Item ${item} is up to date ( Installed version $(version ${item}) )."
      if [[ "${force}" == "true" ]]; then
        echo Forcing anyway.
      else
        return
      fi
    fi
  fi
  if [[ -f "/var/lock/${item}-update-lockfile" ]]; then
    echo ${item} updates locked by lockfile.
    if [[ "${force}" == "true" ]]; then
      echo Forcing anyway.
    else
      return
    fi
  fi
  system_model=$(system_model)
  case ${system_model} in
    ${power_edge_r720_system_model_regex})
      if [[ "${item}" == "NET" ]]; then
        if ! [[ "$(echo $(version ${item}) 15.5.0 | tr ' ' "\n" | sort -V | tail -1)" == "$(version ${item})" ]]; then
          echo NET update rejected. Please update manually.
          return
        fi
      fi
      if [[ "${item}" == "ESM_WITH_LCC" ]]; then
        if ! [[ "$(echo $(version ${item}) 1.66.65 | tr ' ' "\n" | sort -V | tail -1)" == "$(version ${item})" ]]; then
          echo ESM_WITH_LCC update rejected. Please update manually.
          echo "You may either update using the iDRAC ( iDRAC-Webinterface -> iDRAC-Settings -> Update and Rollback -> Upload ${file} ) or using the LCC."
          return
        fi
      fi
    ;;
    ${power_edge_r730_system_model_regex}|${power_edge_r530_system_model_regex})
      :
    ;;
    *)
      abort Unspecified system model ${system_model}
    ;;
  esac
  echo Locking ${item} updates.
  touch /var/lock/${item}-update-lockfile
  echo Fetching update file ${file##*/}.
  wget -o /dev/null -O file.EXE ${file}
  echo "Installing update $(version_by_file ${file}) ( Replacing installed update $(version ${item}) )."
  $(racadm_binary) update -f file.EXE &> /dev/null
  case ${item} in
    ESM*|ESM_WITH_LCC*)
      while :; do
        if [ -z "$(racadm getsysinfo 2>&1 | grep -i 'firmware version' | cut -d '=' -f 2 | tr -d ' ')" ]; then
          break
        fi
        sleep 8
      done
      while :; do
        if ! [ -z "$(racadm getsysinfo 2>&1 | grep -i 'firmware version' | cut -d '=' -f 2 | tr -d ' ')" ]; then
          break
        fi
        sleep 8
      done
    ;;
  esac
  echo Finished.
  echo
}
update_item() {
  item=$(echo ${1^^} | strip)
  if ! list_available_items | grep -oq ${item}; then
    abort Unspecified item ${item}
  fi
  update_item_by_file ${item} $(latest_file ${item})
}
update_all_items() {
  for item in $(list_available_items); do
    update_item ${item}
  done
}
trap exit_hook EXIT
tempdir=$(mktemp -d)
cd ${tempdir}
if [ ${#} -eq 0 ]; then
  help
fi
force='false'
args=''
for arg in ${@}; do
  case ${arg} in
    -force)
      force='true'
    ;;
    *)
      args+="${arg} "
    ;;
  esac
done
set -- ${args}
case ${1} in
  list-available-items)
    list_available_items
  ;;
  list-available-files)
    list_available_files ${2}
  ;;
  list-available-updates)
    list_available_updates ${2}
  ;;
  update-item-by-file|item-by-file)
    if [[ ${#} -lt 3 ]]; then
      help
    fi
    update_item_by_file ${2} ${3}
  ;;
  update-item|item)
    if [[ ${#} -lt 2 ]]; then
      help
    fi
    update_item ${2}
  ;;
  update-all-items|all-items|all)
    update_all_items
  ;;
  *)
    help
  ;;
esac
