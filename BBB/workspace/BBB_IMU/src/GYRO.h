/*
 * GYRO.h
 *
 *  Created on: Jan 7, 2015
 *      Author: kevin
 */

#ifndef GYRO_H_
#define GYRO_H_

namespace IMU {

class GYRO {
public:
	int TEMP_OUT_H, TEMP_OUT_L, GYRO_XOUT_H, GYRO_XOUT_L,GYRO_YOUT_H, GYRO_YOUT_L,GYRO_ZOUT_H, GYRO_ZOUT_L;
	int TEMP,X,Y,Z;
	GYRO(void);
	void read();
	int getX();
	int getY();
	int getZ();
	virtual ~GYRO();
};

} /* namespace IMU */

#endif /* GYRO_H_ */
