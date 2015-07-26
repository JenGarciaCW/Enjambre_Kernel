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
	// Creación de objeto tipo joystick

	if( ( this->joy_fd = open( JOY_DEV , O_RDONLY)) == -1 ) // abre el descriptor de archivo del joystick
	{
		std::cout << "Couldn't open joystick\n" ;
	}

	ioctl( this->joy_fd, JSIOCGAXES, &this->num_of_axis );		// lee el número de valores analógicos
	ioctl( this->joy_fd, JSIOCGBUTTONS, &this->num_of_buttons );// lee el número de valores digitales
	ioctl( this->joy_fd, JSIOCGNAME(80), &this->name_of_joystick );	//obtiene el nombre del joystick

	this->axis = (int *) calloc( this->num_of_axis, sizeof( int ) );	//asigna tamaño a puntero de valores
	this->button = (char *) calloc( this->num_of_buttons, sizeof( char ) ); //asigna tamaño a puntero de valores

	fcntl( this->joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
}

void joystick_class::readjs()
{
	//Lectura de valores instantaneos del joystick

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
}

} /* namespace joystick */
