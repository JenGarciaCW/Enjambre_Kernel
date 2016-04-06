/*
 * UARTMLX.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: kevin
 */

#include "UARTMLX.h"

namespace data_sensor {

UART_MLX::UART_MLX(char *port)
{

	/* Open the file descriptor in non-blocking mode */
	 this->fd = open(portname, O_RDWR | O_NOCTTY);

	/* Set up the control structure */
	 struct termios toptions;

	 /* Get currently set options for the tty */
	 tcgetattr(this->fd, &toptions);

	/* Set custom options */

	/* 9600 baud */
	 cfsetispeed(&toptions, B57600);
	 cfsetospeed(&toptions, B57600);
	 /* 8 bits, no parity, no stop bits */
	 toptions.c_cflag &= ~PARENB;
	 toptions.c_cflag &= ~CSTOPB;
	 toptions.c_cflag &= ~CSIZE;
	 toptions.c_cflag |= CS8;
	 /* no hardware flow control */
	 toptions.c_cflag &= ~CRTSCTS;
	 /* enable receiver, ignore status lines */
	 toptions.c_cflag |= CREAD | CLOCAL;
	 /* disable input/output flow control, disable restart chars */
	 toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
	 /* disable canonical input, disable echo,
	 disable visually erase chars,
	 disable terminal-generated signals */
	 toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	 /* disable output processing */
	 toptions.c_oflag &= ~OPOST;

	/* wait for 12 characters to come in before read returns */
	/* WARNING! THIS CAUSES THE read() TO BLOCK UNTIL ALL */
	/* CHARACTERS HAVE COME IN! */
	 toptions.c_cc[VMIN] = 132;
	 /* no minimum time to wait before read returns */
	 toptions.c_cc[VTIME] = 0;

	/* commit the options */
	 tcsetattr(this->fd, TCSANOW, &toptions);

	/* Wait for the Arduino to reset */


}


void UART_MLX::UART_read(){

	 /* Flush anything already in the serial buffer */

	 tcflush(this->fd, TCIFLUSH);
	 system("echo 0 > /sys/class/gpio/gpio69/value" );
	 system("echo 1 > /sys/class/gpio/gpio69/value" );
	 read(this->fd, this->read_v, 300);
	 /* read up to 128 bytes from the fd */
}

UART_MLX::UART_MLX() {
	// TODO Auto-generated constructor stub

}

UART_MLX::~UART_MLX() {
	// TODO Auto-generated destructor stub
}

} /* namespace data_sensor */
