// Filename: myUartProject.cpp
// Author: Yiğit Yüce - ygtyce@gmail.com

#include "GYRO.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots

int main()
{
	IMU::GYRO G();
	G.read();
	std::cout<<G.getX();
return 0;

}
