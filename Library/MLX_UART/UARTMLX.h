/*
 * UARTMLX.h
 *
 *  Created on: Apr 2, 2016
 *      Author: kevin
 */
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include <fstream>
#include <sys/ioctl.h>
#include <cmath>

#ifndef UARTMLX_H_
#define UARTMLX_H_


namespace data_sensor {

class UART_MLX {
public:
	char *portname ;
	unsigned char read_v[132];
	int fd;
	void UART_read();
	UART_MLX(char *port );
	UART_MLX();
	virtual ~UART_MLX();
};

} /* namespace data_sensor */

#endif /* UARTMLX_H_ */
