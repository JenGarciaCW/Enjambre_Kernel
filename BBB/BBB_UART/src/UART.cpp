#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include <fstream>
#include <sys/ioctl.h>
#include "socketmessage.cpp"


using namespace std;
using namespace socket_msg;

int addr=128; //Dirección de control de puente H

int fnc = 0;
//Función de Puente H 0,2 = M*Adelante 1,3= M*Reversa donde * es el motor 1 o 2

int val = 0; // Valor entre 0 y 127 de velocidad deseada
unsigned char transmit[4] = {addr,fnc,val,(addr+fnc+val)&127};
// Inicialización de valores a enviar

int fduart,count;

char receive[500];


void sendSaberthoot();
//void *thermograph(void * val);

int main()
{


	/*Habilitación de dispositivos UART*/
	system("echo 1 > /sys/class/gpio/gpio69/value" );
	system("echo 1 > /sys/class/gpio/gpio60/value" );

	//socket_message Thermo(30000,"192.168.0.2",500);
	//Creación de socket UDP para envío de datos del termógrafo
	//Thermo.init_udp_sender_socket();

	socket_message JoyUART("/root/messUART",2);
	//Creación de socket interno UNIX para recepción de datos del Joystick
	JoyUART.init_unix_client_socket();


	/*Apertura de archivo de control de comunicación serial*/
	if((fduart=open("/dev/ttyO2",O_WRONLY | O_NOCTTY | O_NDELAY))<0)
	{
		cout<<"UART:Falló al abrir archivo de comunicación"<< endl;
	}
	/*Configuración de comunicación serial:
	*9600 baud, 8-bit, enable receiver, no modem control lines
	*/
	struct termios options;
	tcgetattr(fduart, &options);
	options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	options.c_iflag = IGNPAR | IGNCR;
	options.c_cc[VMIN]=350;
	options.c_cc[VTIME]=0;
	// ignore partity errors, CR -> newline
	tcflush(fduart,TCIFLUSH);
		// discard file information not transmitted
	tcsetattr(fduart, TCSANOW, &options); // changes occur immmediately

	int bytes;







	while(1){




		/*
		 * Lectura y reenvío de datos de termógrafo
		 */



		// BYTES MIGHT BE RECEIVED BY HARDWARE/OS HERE!
/*
		if ((count = read(fduart, (void*)receive, 500))>0){
		cout<<"The following was read in ["<< count <<"]:"<<receive<<endl;
		Thermo.buffer=receive;
		Thermo.write_udp();
		memset(receive,0,sizeof(receive));  //Limpia buffer de lectura serial
		tcflush(fduart,TCIFLUSH);
		}*/


	/*
	 * Control de motores laterales
	 */
	JoyUART.read_unix(); 	//Lectura de valores del Joystick

	/*
	 * Control Motor 1
	 */
	 val = (int)JoyUART.buffer[0]-127;  // Valor deseado motor 1
	 //Control
	 //val salida

	 if(val>=0)fnc=0;	//Acondicionamiento de la señal de salida
	 else{
		 val=-val;
	 	 fnc=1;
	 }
	 sendSaberthoot(); //Envío a controlador de motores

	/*
	 * Control Motor 2
	 */
	 val = (int)JoyUART.buffer[1]-127;// Valor deseado motor 2
	 if(val>=0)fnc=4;
	 else{
		 val=-val;
	 	 fnc=5;
	 }
	 sendSaberthoot();

	}
	close(fduart);



return 0;
}

void sendSaberthoot()
{
	 if (val>127)
		 val=127;
	transmit[0]=addr;
	transmit[1]=fnc;
	transmit[2]=val;
	transmit[3]=(addr+fnc+val)&127;
	if ((count = write(fduart, &transmit,4))<0){
	cout<<"Failed to write to the output\n";
	}

}

/*void *thermograph(void * val)
		{
	while(1){
}
		}
		*/
