#!/bin/bash

TIEMPO=0.01

while true
do
echo none > /sys/class/leds/beaglebone\:green\:usr0/trigger
echo none > /sys/class/leds/beaglebone\:green\:usr1/trigger
echo none > /sys/class/leds/beaglebone\:green\:usr2/trigger
echo none > /sys/class/leds/beaglebone\:green\:usr3/trigger
sleep $TIEMPO
echo default-on > /sys/class/leds/beaglebone\:green\:usr0/trigger
sleep $TIEMPO
echo default-on > /sys/class/leds/beaglebone\:green\:usr1/trigger
sleep $TIEMPO
echo none > /sys/class/leds/beaglebone\:green\:usr0/trigger
echo default-on > /sys/class/leds/beaglebone\:green\:usr2/trigger
sleep $TIEMPO
echo none > /sys/class/leds/beaglebone\:green\:usr1/trigger
echo default-on > /sys/class/leds/beaglebone\:green\:usr3/trigger
sleep $TIEMPO
echo none > /sys/class/leds/beaglebone\:green\:usr2/trigger
sleep $TIEMPO
echo none > /sys/class/leds/beaglebone\:green\:usr3/trigger
done
