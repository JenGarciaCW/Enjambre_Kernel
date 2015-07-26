#!/bin/bash

echo "#!/bin/bash" > /usr/bin/init_pins.sh
echo "$PWD/init_pins.sh" >> /usr/bin/init_pins.sh
chmod +x /usr/bin/init_pins.sh

echo "[Unit]" > /lib/systemd/init_pins.service
echo "Description=Variable initialization BBB" >> /lib/systemd/init_pins.service
echo "After=syslog.target network.target" >> /lib/systemd/init_pins.service
echo "[Service]" >> /lib/systemd/init_pins.service
echo "Type=simple" >> /lib/systemd/init_pins.service
echo "ExecStart=/usr/bin/init_pins.sh" >> /lib/systemd/init_pins.service
echo "[Install]" >> /lib/systemd/init_pins.service
echo "WantedBy=multi-user.target" >> /lib/systemd/init_pins.service

ln -s /lib/systemd/init_pins.service /etc/systemd/system/init_pins.service

systemctl daemon-reload
systemctl start init_pins.service
systemctl enable init_pins.service

# shutdown -r now
