#!/bin/bash

echo "export SLOTS=/sys/devices/bone_capemgr.9/slots" >> /root/.profile
echo "export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins" >> /root/.profile
dtc -O dtb -o EBB-ENJ-Cape-00A0.dtbo -b 0 -@ EBB-ENJ-Cape.dts
cp EBB-ENJ-Cape-00A0.dtbo /lib/firmware/
echo "optargs=capemgr.enable_partno=BB-ADC,BB-I2C1,BB-UART4,am33xx_pwm,EBB-ENJ-Cape" >> /boot/uEnv.txt

echo "remove comment in cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN at /boot/uEnv.txt"
echo "add EBB-ENJ-Cape /etc/default/capemgr"
echo "remove comment at /root/.bashrc y hacer $source .bashrc to color ssh"

cp interfaces /etc/network/interfaces

cp init_pins.sh /usr/bin

cp init_pins.service /lib/systemd/
ln -s /lib/systemd/init_pins.service /etc/systemd/system/init_pins.service

systemctl daemon-reload
systemctl enable init_pins.service
systemctl start init_pins.service

echo " al finalizar hacer shutdown -r now"
