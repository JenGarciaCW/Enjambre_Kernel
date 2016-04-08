#!/bin/bash

if [ $# -eq 2 ]
  then
 	sed -i "s/192.168.0.*/192.168.0.$1/g" /etc/network/interfaces 
	
        if [ $2 == "y" ] || [ $2 == "Y" ] 
	 then
 		ifdown wlan0 	
		ifup wlan0 
	else
		shutdown -h now
	fi
   else
 	echo "	Error: Arguments missing"
 	echo "		ip_assign [ip_ls] [y/n]"
	echo "			ip_ls 	less significant part of ip address (int value)"
	echo "			y 	replace ip and update ifconfig"
	echo "		 	n 	replace ip and shutdown "
fi

