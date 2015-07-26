#! /bin/bash

sleep 20
echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_16 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_42 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P8_13 > /sys/devices/bone_capemgr.9/slots
sleep 1
echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.*/polarity

echo 0 > /sys/devices/ocp.3/pwm_test_P9_16.*/polarity

echo 0 > /sys/devices/ocp.3/pwm_test_P9_42.*/polarity

echo 0 > /sys/devices/ocp.3/pwm_test_P9_42.*/polarity

echo 0 > /sys/devices/ocp.3/pwm_test_P8_13.*/run
echo 0 > /sys/devices/ocp.3/pwm_test_P8_13.*/polarity
echo 20000000 > /sys/devices/ocp.3/pwm_test_P8_13.*/period
echo 1 > /sys/devices/ocp.3/pwm_test_P8_13.*/run
echo  1460000 > /sys/devices/ocp.3/pwm_test_P8_13.*/duty
