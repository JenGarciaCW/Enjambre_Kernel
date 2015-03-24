
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include "socketmessage.h"


#define JOY_DEV "/dev/input/js0"
using namespace socket_msg;

int main()
{

	 socket_message BBBsock(23222,"192.168.0.137",2);
	 BBBsock.init_udp_sender_socket();


	int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
	char *button=NULL, name_of_joystick[80];
	struct js_event js;
	float mi,md,V,H;

	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		printf( "Couldn't open joystick\n" );
		return -1;
	}

	ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
	ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

	axis = (int *) calloc( num_of_axis, sizeof( int ) );
	button = (char *) calloc( num_of_buttons, sizeof( char ) );

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons );



	fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */

	while( 1 ) 	/* infinite loop */
	{

			/* read the joystick state */
		read(joy_fd, &js, sizeof(struct js_event));

			/* see what to do with the event */
		switch (js.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_AXIS:
	 			axis   [ js.number ] = js.value;
				break;
			case JS_EVENT_BUTTON:
				button [ js.number ] = js.value;
				break;
		}



			V=-1*axis[3]/32767.0 ;
			H=axis[2]/32767.0;

			if(H<0)
			mi=V+2*V*H;
			else
			mi=V-2*V*H;
			md=V;


			 BBBsock.buffer[0]=(mi+1)*127;
			 BBBsock.buffer[1]=(md+1)*127;
			 BBBsock.write_udp();

			printf("mi: %6f  ",mi);

			printf("md: %6f  ",md);

		fflush(stdout);
	}

	close( joy_fd );	/* too bad we never get here */
	return 0;
}
