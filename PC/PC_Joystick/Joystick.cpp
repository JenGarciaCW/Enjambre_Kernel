
// ----------------------------------------------------------------------------
/**
   File: Joystick.cpp
   System:	Robot de enjambre para exploración en en estructuras colapsadas
   Component Name: Control
   Status:         Version 0.1
   Language: C++
   License: GNU Public License
   Address:
      AV. UNIVERSIDAD Nº 3000,
      UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO, C.U.,
      DISTRITO FEDERAL, 04510
   Author: Kevin Omar Arias García
   E-Mail: kevinoag@gmail.com
   Description: This code beside joystickclass.cpp joystickclass.h socketmessage.cpp socketmessage.h
   are intended to send control instructions to the robot.
   Limitations: You must have an xbox-USB controller.
   Platform Dependencies: Linux/Intel
   Note:
   	   Algorithm 1 and 2 are based on the paper
   	   Manual Control for Driving an Intelligent Wheelchair: A Comparative Study of Joystick Mapping Methods*
   	   by Brígida Mónica Faria, Luís Miguel Ferreira, Luís Paulo Reis, Nuno Lau, Marcelo Petry, and João Couto Soares
   Compiler Options:
   Revisions:
   Date         Author       	Description
   2014.07.20   KevinArias	 	Added dynamic video transfer adjustable to user defined resolution
**/
// -------------------------- --------------------------------------------------

#include <stdio.h>
#include <bitset>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "socketmessage.cpp"
#include "joystickclass.cpp"

#define PI	3.1416
#define MAX 2350000	// Máximo valor de giro de servomotor en nanosegundos
#define MIN 570000	// Mínimo valor de giro de servomotor en nanosegundos

using namespace std;
using namespace joystick;
using namespace socket_msg;

double signof(double a) { return (a == 0) ? 0 : (a<0 ? -1 : 1); }  //función que regresa el signo del valor a

int main(int argc, char *argv[])
{
	//Inicialización de socket transmisor UDP de envío de datos
	 socket_message BBBsock(2211122,argv[1],4);
	 BBBsock.init_udp_sender_socket();

	 //Creation of an xbox joystic object
	 joystick_class xbox;


	int algorithm =1, dzone=6000 ; //Selección de algoritmo (1 o 2), valor máximo de zona muerta
	unsigned char buttons=0;	// Variable para transmitir botones
	float R,L,theta,rho; // variables para los algoritmos
	float state=1460000; // estado inicial para PWM del servo




	while( 1 ) 	// Ciclo principal
	{

		xbox.readjs(); // Lee los valores del joystick

		buttons = 	(xbox.button[7]<<7)|(xbox.button[6]<<6)|
					(xbox.button[5]<<5)|(xbox.button[4]<<4)|
					(xbox.button[3]<<3)|(xbox.button[2]<<2)|
					(xbox.button[1]<<1)|(xbox.button[0]);
					//cout << bitset<8>(buttons) ; // Crea byte con dato de botones del joystick

		/*
		 * Generación de velocidades nominales de motores de transmisión
		 */

		if(abs(xbox.axis[0])<dzone && abs(xbox.axis[1])<dzone)
			{
			xbox.axis[1]=0;
			xbox.axis[0]=0;
			} 	// crea una zona en la que los valores leidos serán cero (zona muerta)

		switch(algorithm)	//Selección de algoritmo
		{
			// el valor L corresponde a el motor izquierdo
			// el valor R corresponde a el motor derecho
			case 1:
				// algoritmo lineal
				L = -(int)(signof(xbox.axis[1])*pow(xbox.axis[1],2)+signof(xbox.axis[0])*pow(xbox.axis[0],2));
				R = -(int)(signof(xbox.axis[1])*pow(xbox.axis[1],2)-signof(xbox.axis[0])*pow(xbox.axis[0],2));

				R=round(R)/8000000+127;
				L=round(L)/8000000+127;

				//cout << "\t FB = " << xbox.axis[1]<< "\t RL = " << xbox.axis[0]<< "\t L = " << L << "\t R = " << R  ;
				//impresión de valores intermedios y valor de los motores
			break;

			case 2:	// algoritmo vectorial
				rho=sqrt(pow(xbox.axis[0],2)+pow(xbox.axis[1],2));
				theta = atan2(xbox.axis[0],-xbox.axis[1]);


				if(theta >= 0 && theta <= PI/2)
				{
					R=rho;
					L=rho*(-theta+PI/4)/(PI/4);
				}
				else if(theta > PI/2 && theta <= PI)
				{
					R=rho*(-theta+3*PI/4)/(PI/4);
					L=-rho;

				}
				else if(theta <= 0 && theta >= -PI/2)
				{

					R=rho*(theta+PI/4)/(PI/4);
					L=rho;

				}
				else if(theta < -PI/2 && theta >= -PI)
				{
					R=-rho;
					L=rho*(theta+3*PI/4)/(PI/4);
				}

				R=round(R/260)+127;
				L=round(L/260)+127;

				//cout << "\t theta = " << theta << "\t rho = " << rho << "\t R = " << R << "\t L = " << L  ;
				//impresión de valores intermedios y valor de los motores
				break;
		}



		// Recorte de valores máximos de los motores
		if(R>255) R=255;
		if(R<0) R=0;
		if(L>255)L=255;
		if(L<0) L=0;

		/*
		 * Generación de valores para servomotor de inclinación
		 * 		Se requiere presionar el botón A y manipular
		 * 		los disparadores izquierdo o derecho para hacer
		 * 		que el servomotor gire en esas direcciones.
		 */

			if(xbox.button[0])	//Botón de seguridad (A)
			{

				if(xbox.axis[5]/240 > -125)	// Prioridad al disparador derecho
				{
					//Conversión de lectura analogica a un valor en nanosegundos
					//disparador derecho
					xbox.axis[5] = xbox.axis[5]/2+32767/2;
					state=state+xbox.axis[5];
					if(state>MAX)
						state=MAX;
				}
				else if(xbox.axis[2]/240 > -125)
				{
					//Conversión de lectura analogica a un valor en centésimas de segundos
					//disparador izquierdo
					xbox.axis[2] = xbox.axis[2]/2+32767/2;
					state=state-xbox.axis[2];
					if(state<MIN)
						state=MIN;
				}
			}

			//Asignación de valores de un byte a cada celda del arreglo buffer
			BBBsock.buffer[0]=buttons;	//Byte de valor de botones |Start|Back|RB|LB|Y|X|B|A|
			BBBsock.buffer[1]=(unsigned char)(state/10000); // Byte de valor de servomotor en décimas de microsegundos
			BBBsock.buffer[2]=(unsigned char)-L; // Byte con valor de motor derecho (Saberthoot)
			BBBsock.buffer[3]=(unsigned char)R; // Byte con valor de motor izquierdo (Saberthoot)

			usleep(31000);	// delay para no sobrecargar comunicación UDP

			BBBsock.write_udp(); //Envío de buffer de valores a través del socket

			cout<<(int)BBBsock.buffer[0]<< "\t"<<(int)BBBsock.buffer[1]<< "\t"<<(int)BBBsock.buffer[2]<< "\t"<<(int)BBBsock.buffer[3]<< "\t"<<endl;
		if(xbox.button[6])return -1;

		fflush(stdout);
	}

	xbox.closejs();	//Cierra archivo de lectura de joystick
	return 0;
}


