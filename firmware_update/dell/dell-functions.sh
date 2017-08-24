generate_request_xml() {
cat > $1 <<EOT
<p:InstallFromURI_INPUT xmlns:p="http://schemas.dell.com/wbem/wscim/1/cim-schema/2/DCIM_SoftwareInstallationService">
<p:URI>$2</p:URI>  <p:Target xmlns:a="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:w="http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd">
    <a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address>
    <a:ReferenceParameters>
      <w:ResourceURI>http://schemas.dell.com/wbem/wscim/1/cim-schema/2/DCIM_SoftwareIdentity</w:ResourceURI>
      <w:SelectorSet>
        <w:Selector Name="InstanceID">$3</w:Selector>
      </w:SelectorSet>
    </a:ReferenceParameters>
  </p:Target>
</p:InstallFromURI_INPUT>
EOT
}

generate_job_xml() {
cat > $1 <<EOT
<p:SetupJobQueue_INPUT xmlns:p="http://schemas.dell.com/wbem/wscim/1/cim-schema/2/DCIM_JobService">
<p:JobArray>$2</p:JobArray>
<p:RunMonth>6</p:RunMonth>
  <p:RunDay>18</p:RunDay>
<p:StartTimeInterval>TIME_NOW</p:StartTimeInterval>
</p:SetupJobQueue_INPUT>
EOT
}

invoke_install() {
  output=$(wsman invoke -a "InstallFromURI" http://schemas.dell.com/wbem/wscim/1/cim-schema/2/DCIM_SoftwareInstallationService?CreationClassName="DCIM_SoftwareInstallationService",SystemName="IDRAC:ID",Name="SoftwareUpdate",SystemCreationClassName="DCIM_ComputerSystem" -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V -J "$WORKDIR/request.xml")
  echo $output > $1
}

schedule_job() {
 output=$(wsman invoke -a "SetupJobQueue" http://schemas.dell.com/wbem/wscim/1/cim-schema/2/DCIM_JobService?CreationClassName="DCIM_JobService",SystemName="Idrac",Name="JobService",SystemCreationClassName="DCIM_ComputerSystem" -N root/dcim -u $USERNAME -p $PASSWORD -h $IPADDRESS -P 443 -v -j utf-8 -y basic -o -m 256 -c dummy.cert -V -J "$1")
}


