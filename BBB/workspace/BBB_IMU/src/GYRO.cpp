/*
 * GYRO.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: kevin
 */
#include "v2_0/BlackUART.h"
#include "v2_0/BlackCore.cpp"
#include "v2_0/BlackCore.h"
#include "v2_0/BlackI2C.h"
#include "v2_0/BlackI2C.cpp"
#include "GYRO.h"

namespace IMU {

void GYRO::read()
{
	uint8_t buff[8];
	BlackLib::BlackI2C I2CG(BlackLib::I2C_2, 0x68);
	//I2CG.open(BlackLib::ReadWrite|BlackLib::NonBlock);
	//I2CG.readBlock(uint8_t(0x1B), buff, sizeof(buff) );
	this->TEMP_OUT_H = buff[0];
	this->TEMP_OUT_L = buff[1];
	this->GYRO_XOUT_H = buff[2];
	this->GYRO_XOUT_L = buff[3];
	this->GYRO_YOUT_H = buff[4];
	this->GYRO_YOUT_L = buff[5];
	this->GYRO_ZOUT_H = buff[6];
	this->GYRO_ZOUT_L = buff[7];
	I2CG.close();
}

int GYRO::getX()
{
	int x;
	x=((((int)this->GYRO_XOUT_H<<8)|(int)this->GYRO_XOUT_L)&0x7F)/14.375;
	return x;
}

int GYRO::getY()
{
	return 1;
}

int GYRO::getZ()
{
	return 1;
}


GYRO::GYRO(void) {
	// TODO Auto-generated constructor stub
	this->GYRO_XOUT_H=0;
	this->GYRO_XOUT_L=0;
	this->GYRO_YOUT_H=0;
	this->GYRO_YOUT_L=0;
	this->GYRO_ZOUT_H=0;
	this->GYRO_ZOUT_L=0;
	this->TEMP_OUT_H=0;
	this->TEMP_OUT_L=0;
	this->TEMP=0;
	this->X=0;
	this->Y=0;
	this->Z=0;

}

GYRO::~GYRO() {
	// TODO Auto-generated destructor stub
}


} /* namespace IMU */
