ip_addr=$(ip -f inet -o addr show wlan0|cut -d\  -f 7 | cut -d/ -f 1 )
ip_last=$(expr $(echo $ip_addr | cut -d. -f 4 ) - 1 )
ip_addr=$(echo $ip_addr | sed -e "s/192.168.0.*/192.168.0.$ip_last/g" )
echo "$ip_addr"
#ping -c 1 "$ip_addr" > /dev/null
#if [ $? -eq 0 ]
#then
#	echo "se pudo"
#else	
#	echo "no se pudo"
#fi




