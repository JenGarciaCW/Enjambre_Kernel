#!/bin/bash

chmod +x /usr/bin/init_pins.sh
ln -s
cp init_pins.service /lib/systemd/
ln -s /lib/systemd/init_pins.service /etc/systemd/system/init_pins.service

systemctl daemon-reload
systemctl start init_pins.service
systemctl enable init_pins.service

shutdown -r now
