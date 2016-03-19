#!/bin/bash
name=$(cat /sys/bus/iio/devices/iio\:device0/in_voltage1_raw)

if [ "$name" -le "3450" ];then
echo "low bat"
killbbb.sh
else
val=$(expr $name - 3350)
echo $val > /sys/devices/ocp.3/pwm_test_P9_14*/duty
echo $val > /sys/devices/ocp.3/pwm_test_P9_16*/duty
echo $name               #test
fi
