#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include <fstream>
#include <sys/ioctl.h>
#include <cmath>
#include "MLX90620_registers.h"
#include "socketmessage.cpp"


using namespace socket_msg;



/* My Arduino is on /dev/ttyACM0 */
char *portname = "/dev/ttyO4";
unsigned char read_v[132]={0};



int main(int argc, char *argv[])
{

	 socket_message thermal(332584,"192.168.0.1",132); //Creates a socket port, server, client, buffer size
	 thermal.init_udp_sender_socket();



 int fd;



/* Open the file descriptor in non-blocking mode */
 fd = open(portname, O_RDWR | O_NOCTTY);

/* Set up the control structure */
 struct termios toptions;

 /* Get currently set options for the tty */
 tcgetattr(fd, &toptions);

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
 tcsetattr(fd, TCSANOW, &toptions);

/* Wait for the Arduino to reset */





while(1){
	 /* Flush anything already in the serial buffer */

	 tcflush(fd, TCIFLUSH);
	 system("echo 0 > /sys/class/gpio/gpio69/value" );
	 system("echo 1 > /sys/class/gpio/gpio69/value" );
	 read(fd, read_v, 300);
	 /* read up to 128 bytes from the fd */



	 memcpy ( thermal.buffer, read_v,132);
	 thermal.write_udp();

}


return 0;
}




