 dtc -O dtb -o EBB-ENJ-Cape-00A0.dtbo -b 0 -@ EBB-ENJ-Cape.dts
 cp EBB-ENJ-Cape-00A0.dtbo /lib/firmware/
 echo "EBB-ENJ-Cape" >  /etc/default/capemgr 
