/* 		Joystick
 *
 *
 *
 *
 *
 * */


#include <stdio.h>
#include <bitset>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "socketmessage.h"
#include "joystickclass.h"
//20000000

#define PI	3.1416
#define MAX 2350000	// Máximo valor de giro de servomotor en nanosegundos
#define MIN 570000	// Mínimo valor de giro de servomotor en nanosegundos

using namespace std;
using namespace joystick;
using namespace socket_msg;

double signof(double a) { return (a == 0) ? 0 : (a<0 ? -1 : 1); }  //función que regresa el signo del valor a

int main()
{
	 socket_message BBBsock(231112,"127.0.0.1",2);
	 BBBsock.init_udp_sender_socket();

	 socket_message BBBrecv(231112,"127.0.0.1",2);
	 BBBrecv.init_udp_receiver_socket();

	 joystick_class xbox;


	int algorithm =1, dzone=6000 ;
	unsigned char buttons=0;
	float R,L,theta,rho,state=1460000;






	while( 1 ) 	// Ciclo principal
	{
			xbox.readjs(); // Lee los valores del joystick

			buttons = 	(xbox.button[7]<<7)|(xbox.button[6]<<6)|
						(xbox.button[5]<<5)|(xbox.button[4]<<4)|
						(xbox.button[3]<<3)|(xbox.button[2]<<2)|
						(xbox.button[1]<<1)|(xbox.button[0]);
						cout << bitset<8>(buttons) ; // Crea byte con dato de botones del joystick

			if(abs(xbox.axis[0])<dzone && abs(xbox.axis[1])<dzone)
				{
				xbox.axis[1]=0;
				xbox.axis[0]=0;
				} 	// crea una zona en la que los valores leidos serán cero

		switch(algorithm)
		{
			case 1:
				// algoritmo lineal
				L = -(int)(signof(xbox.axis[1])*pow(xbox.axis[1],2)+signof(xbox.axis[0])*pow(xbox.axis[0],2));
				R = -(int)(signof(xbox.axis[1])*pow(xbox.axis[1],2)-signof(xbox.axis[0])*pow(xbox.axis[0],2));

				R=round(R)/8000000+127;
				L=round(L)/8000000+127;

				//cout << "\t FB = " << xbox.axis[1]<< "\t RL = " << xbox.axis[0]<< "\t L = " << L << "\t R = " << R  ;
			break;

			case 2:
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

				cout << "\t theta = " << theta << "\t rho = " << rho << "\t R = " << R << "\t L = " << L  ;
			break;
		}



		if(R>255) R=255;
		if(R<0) R=0;

		if(L>255)L=255;
		if(L<0) L=0;

			cout <<"\t" <<(xbox.axis[5]/2+32767/2)/100 <<"\t" <<(xbox.axis[2]/2+32767/2)/100 <<endl;

			if(xbox.button[0])
			{

				if(xbox.axis[5]/240 > -125)
				{
					xbox.axis[5] = xbox.axis[5]/2+32767/2;
					state=state+xbox.axis[5]/3000.0;
					if(state>MAX)
						state=MAX;
				}
				else if(xbox.axis[2]/240 > -125)
				{
					xbox.axis[2] = xbox.axis[2]/2+32767/2;
					state=state-xbox.axis[2]/3000.0;
					if(state<MIN)
						state=MIN;
				}
			}

			cout << state/10000 << "\t";

			/*PWMsock.buffer[0]=(char)servo;
			PWMsock.buffer[1]=(char)set;
			PWMsock.buffer[2]=(char)reset;
			*/

			BBBsock.buffer[0]= 'a';
			BBBsock.buffer[1]='e';
			BBBsock.write_udp();
			cout<<"write"<<BBBsock.server<<endl;

			BBBrecv.read_udp();
			cout<<endl<<endl;
			cout<<"waiting\t"<<(char)BBBrecv.buffer[0]<<(char)BBBrecv.buffer[1]<<endl;

			BBBsock.buffer[0]= 'f';
			BBBsock.buffer[1]='d';
			BBBrecv.write_udp();
			BBBsock.read_udp();
			cout<<"waiting\t"<<(char)BBBsock.buffer[0]<<(char)BBBsock.buffer[1]<<endl;


		//if(xbox.button[2])cout<<system("shutdown -h now");
		fflush(stdout);
	}

	xbox.closejs();
	return 0;
}
