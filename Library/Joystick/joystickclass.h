/*
 * joystickclass.h
 *
 *  Created on: Jul 20, 2015
 *      Author: kevin
 */
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef JOYSTICKCLASS_H_
#define JOYSTICKCLASS_H_
#define JOY_DEV "/dev/input/js0"	// dirección del joystick en el sistema

namespace joystick {

class joystick_class {
public:
	int joy_fd; //descriptor de archivo del joystick
	int *axis=NULL; // puntero a valor de lecturas analógicas del joystick
	int num_of_axis=0; // número de lecturas analógicas del joystick
	int num_of_buttons=0; //número de lecturas digitales del joystick
	char *button=NULL; // puntero a valor de lecturas digitales de joystick
	char name_of_joystick[80];	// cadena de caracteres con dirección absoluta del joystick en el sistema
	struct js_event js;	//estructura para manejar evento de lectura de joystick
	void readjs();	//método para leer valores analógicos y digitales de joystick
	void closejs();	//método para cerrar el descriptor de archivo
	joystick_class(); //clase joystick
	virtual ~joystick_class();
};

} /* namespace joystick */

#endif /* JOYSTICKCLASS_H_ */
