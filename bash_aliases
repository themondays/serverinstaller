alias hwcheck='i="" ; until [ "$i" ] ; do /root/.oldroot/nfs/check/menu.in_screen.sh ; echo -e "PRESS ANY KEY NOW TO EXIT FROM HARDWARE CHECK" ; read -t3 -n1 i ; done'
alias hwcheck-noscreen='until false ; do /root/.oldroot/nfs/check/menu.sh ; done'
alias installimage='/root/.oldroot/nfs/install/installimage'
alias update_adaptec='/root/.oldroot/nfs/raid_ctrl/adaptec/update_adaptec.sh'
alias downgrade_adaptec8_freebsd='/root/.oldroot/nfs/raid_ctrl/adaptec/update_adaptec_freebsd.sh'
alias update_3ware='cd /root/.oldroot/nfs/raid_ctrl/3ware; tw_cli /c0 update fw=current.img; cd - > /dev/null'
alias update_lsi_90='cd /root/.oldroot/nfs/raid_ctrl/lsi; megacli -adpfwflash -f 12.12.0-0090.rom -a0; cd - > /dev/null'
alias update_lsi_beta='cd /root/.oldroot/nfs/raid_ctrl/lsi; megacli -adpfwflash -f beta.rom -a0; cd - > /dev/null'
alias update_lsi='cd /root/.oldroot/nfs/raid_ctrl/lsi; megacli -adpfwflash -f current.rom -a0; cd - > /dev/null'
alias update_ocz='cd /root/.oldroot/nfs/firmware_update/ocz_vertex3 ; ./clout --force --fwupd /dev/sd*; cd - > /dev/null'
alias update_x58='/root/.oldroot/nfs/firmware_update/amibios/update_x58.sh'
alias update_asus_p8h67='/root/.oldroot/nfs/firmware_update/asusbios/update_asus_p8h67.sh'
alias update_asus_p8b='/root/.oldroot/nfs/firmware_update/asusbios/update_asus_p8b.sh'
alias update_asus_h77='/root/.oldroot/nfs/firmware_update/asusbios/update_asus_h77.sh'
alias update_asus_z10pa-u8='/root/.oldroot/nfs/firmware_update/asusbios/update_asus_z10.sh'
alias update_smc_x9sri='/root/.oldroot/nfs/firmware_update/aptio3/update_bios_x9sri.sh'
alias update_intel_s1200v3='/root/.oldroot/nfs/firmware_update/intel_s1200v3rp/update_intel_s1200v3.sh'
alias update_fts_d3401h='/root/.oldroot/nfs/firmware_update/fujitsu/update_fts_d3401h.sh'
alias update_fts_d3417='/root/.oldroot/nfs/firmware_update/fujitsu/update_fts_d3417.sh'
alias update_ipmi_asus_z10='/root/.oldroot/nfs/ipmi/asus/check_update_z10pa-u8.sh true'
alias update_hba='/root/.oldroot/nfs/firmware_update/lsi_hba/sas2flash -o -f /root/.oldroot/nfs/firmware_update/lsi_hba/2114ir.bin && /root/.oldroot/nfs/firmware_update/lsi_hba/sas2flash -biosall /root/.oldroot/nfs/firmware_update/lsi_hba/mptsas2.rom'
alias send_robot_hwdata='/root/.oldroot/nfs/check/send_robot_hwdata'
alias dmesg='dmesg -T'
alias kernelmsgon='echo "4 4 1 7" > /proc/sys/kernel/printk'
alias kernelmsgoff='echo "1 4 1 7" > /proc/sys/kernel/printk'
alias bios_info='dmidecode -t baseboard | egrep -i "base board information|manufacturer|product name|version|serial number"; dmidecode -t bios | egrep -i "bios information|vendor|version|release"'
alias speedtest='/root/.oldroot/nfs/benchmark/speedtest-client/speedtest-client.py'
alias power_stress='/root/.oldroot/nfs/benchmark/power/power_stressapptest.sh'
alias hos-hdd_automount='/root/.oldroot/nfs/tools/hos-hdd_automount.sh'
alias unlock_hdd='/root/.oldroot/nfs/check/unlock_hdd.sh'
alias update_qlogic='/root/.oldroot/nfs/firmware_update/qlogic/nic_57810/Network_Firmware_PX6V4_LN_7.12.17.BIN -qf'
alias update_micron5100='/root/.oldroot/nfs/firmware_update/crucial/update_5100.sh'
