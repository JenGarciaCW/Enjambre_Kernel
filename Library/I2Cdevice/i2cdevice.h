/*
 * i2cdevice.h
 *
 *  Created on: Jul 26, 2015
 *      Author: kevin
 */

#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<fcntl.h>
#include<unistd.h>
#include<iostream>

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

namespace i2cdevn {

class i2cdevice {
public:
	int fd;
	int buffsize;
	unsigned char* buffer;	//Data send/receive buffer
	unsigned char bufferout[1];
	unsigned char* in;
	unsigned char*out;
	void writebyte(unsigned char data);
	void readreg(unsigned char data);
	void writebuff();
	void readpoint();
	void writeread();
	i2cdevice(unsigned char address, char bus, int buffersize);
	virtual ~i2cdevice();
};

class AS5048 : public i2cdevice::i2cdevice {
public:
	AS5048(unsigned char address, char bus, int buffersize);
	char diagnostic();
	float readang();
	virtual ~AS5048();
};

class HMC5883L : public i2cdevice::i2cdevice{
public:
	int xmag;
	int ymag;
	int zmag;
	HMC5883L(char bus);
	void readmag();
	virtual ~HMC5883L();
};


class ADXL345 : public i2cdevice::i2cdevice{
public:
	float xacc;
	float yacc;
	float zacc;
	ADXL345(char bus);
	void readacc();
	virtual ~ADXL345();
};

class ITG3200 : public i2cdevice::i2cdevice{
public:
	float temp;
	float xgyro;
	float ygyro;
	float zgyro;
	ITG3200(char bus);
	void readgyro();
	virtual ~ITG3200();
};



} /* namespace i2cdevice */

#endif /* I2CDEVICE_H_ */
