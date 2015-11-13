/*
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "socketmessage.h"

using namespace std;
using namespace socket_msg;

int main () {

	/*Declaracion de sockets*/

	  unlink("/root/messPWM");	// Elimina archivo messPWM
	  unlink("/root/messUART"); // Elimina archivo messUART

	  socket_message JoySock(2211122,"192.168.0.2",4); // Crea objeto socket
	  JoySock.init_udp_receiver_socket(); // Genera socket udp para recibir

	  socket_message JoyPWM("/root/messPWM",2); //Crea objeto socket en archivo
	  JoyPWM.init_unix_server_socket(); // Genera socket interno unix

	  socket_message JoyUART("/root/messUART",2); //Crea objeto socket archivo
	  JoyUART.init_unix_server_socket(); // Genera socket interno unix

	  /*Loop principal*/
	  while(1)
	  {
		  JoySock.read_udp(); // Lee socket de entrada joystick en pc

		  if(JoyPWM.buffer[0]==64)	// dato de inicio de comunicacion
		  {
			  	/* Inicializa gipio 69 y 60 (led f y servo)*/
				system("echo 0 > /sys/class/gpio/gpio69/value" );
				system("echo 0 > /sys/class/gpio/gpio60/value" );
		  }

		  /*envio de datos para servo y led*/
		  JoyPWM.buffer[0]=JoySock.buffer[0];
		  JoyPWM.buffer[1]=JoySock.buffer[1];
		  JoyPWM.write_unix();

		  /*envio de datos para motores en saberthoot (X2)*/
		  JoyUART.buffer[0]=JoySock.buffer[2];
		  JoyUART.buffer[1]=JoySock.buffer[3];
		  JoyUART.write_unix();
  }
  return 0;
}
