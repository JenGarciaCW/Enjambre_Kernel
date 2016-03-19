#! /bin/bash

echo 60 > /sys/class/gpio/export
echo 69 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio60/direction
echo "out" > /sys/class/gpio/gpio69/direction

echo 0 > /sys/class/gpio/gpio60/value
echo 0 > /sys/class/gpio/gpio60/value
