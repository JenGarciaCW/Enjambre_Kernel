/*
 * ThermalDisplay.h
 *
 *  Created on: Jul 22, 2015
 *      Author: kevin
 */
#include<iostream>
#include <stdlib.h>     /* atof */
#include<string>
#include<sstream>
#include<cstring>
#include<math.h>

#ifndef SRC_THERMALDISPLAY_H_
#define SRC_THERMALDISPLAY_H_

namespace thermal {

class Thermal_Display {
public:
	int sweep[512][3]; 	//Matriz de colores barrido Azul-Rojo-Amarillo
	double tdata[65],tmax,tmin,tamb;
	void gensweep();
	void convert(char *indata);
	float tempcalc(float To,float Tf, int ro, int rf,int r);
	int colorcalc(float To,float Tf, int ro, int rf,int r);
	Thermal_Display();
	virtual ~Thermal_Display();
};

} /* namespace thermal */

#endif /* SRC_THERMALDISPLAY_H_ */
