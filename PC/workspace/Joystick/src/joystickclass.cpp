/*
 * joystickclass.cpp
 *
 *  Created on: Jul 20, 2015
 *      Author: kevin
 */

#include "joystickclass.h"
#include <iostream>
namespace joystick {

joystick_class::joystick_class() {
	// TODO Auto-generated constructor stub
	if( ( this->joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		std::cout << "Couldn't open joystick\n" ;
	}

	ioctl( this->joy_fd, JSIOCGAXES, &this->num_of_axis );
	ioctl( this->joy_fd, JSIOCGBUTTONS, &this->num_of_buttons );
	ioctl( this->joy_fd, JSIOCGNAME(80), &this->name_of_joystick );

	this->axis = (int *) calloc( this->num_of_axis, sizeof( int ) );
	this->button = (char *) calloc( this->num_of_buttons, sizeof( char ) );


	fcntl( this->joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
}

void joystick_class::readjs()
{
	read(this->joy_fd, &this->js, sizeof(struct js_event));

		/* see what to do with the event */
	switch (this->js.type & ~JS_EVENT_INIT)
	{
		case JS_EVENT_AXIS:
 			this->axis   [ this->js.number ] = this->js.value;
			break;
		case JS_EVENT_BUTTON:
			this->button [ this->js.number ] = this->js.value;
			break;
	}
}

void joystick_class::closejs()
{
	close( this->joy_fd );	/* too bad we never get here */
}

joystick_class::~joystick_class() {
	// TODO Auto-generated destructor stub
}

} /* namespace joystick */
