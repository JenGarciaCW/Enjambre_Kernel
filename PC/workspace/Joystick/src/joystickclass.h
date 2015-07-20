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
#define JOY_DEV "/dev/input/js0"

namespace joystick {

class joystick_class {
public:
	int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0;
	char *button=NULL, name_of_joystick[80];
	struct js_event js;
	void readjs();
	void closejs();
	joystick_class();
	virtual ~joystick_class();
};

} /* namespace joystick */

#endif /* JOYSTICKCLASS_H_ */
