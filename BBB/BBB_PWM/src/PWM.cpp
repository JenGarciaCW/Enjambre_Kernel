#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "socketmessage.cpp"


using namespace std;
using namespace socket_msg;


int main () {

	/*Creación de socket entre BBB_Joystick y PWM*/
	socket_message JoyPWM("/root/messPWM",2);
		//crea socket UNIX en la dirección /root/messPWM de dos bytes
	JoyPWM.init_unix_client_socket();		//inicializa el socket como cliente

	/*Creación de links simbólicos para modificar valores de pwm*/
	system("rm dutyServo");	 //Llamada al sistema para eliminar link simbólico
	system("ln -s /sys/devices/ocp.3/pwm_test_P8_13*/duty dutyServo" );

	system("rm dutyLed"); //Llamada al sistema para eliminar link simbólico
	system("ln -s /sys/devices/ocp.3/pwm_test_P9_42*/duty dutyLed" );

  /*Manejo de archivos*/
  ofstream Servo, Led; //Crea objetos de tipo ofstream
  Servo.open ("dutyServo",fstream::out);
  	  //Abre archivo referido mediante link simbólico
  Led.open("dutyLed",fstream::out);
  	  //Abre archivo referido mediante link simbólico


  bool past=0,present=0; //Estados de toggle de led
  int count=0; // Contador de valor del led


  while(1)
  {
		usleep(100);

		/*Lectura de valores*/
		JoyPWM.read_unix();
		//Lee datos socket unix, regresa [0] valores de botones [1] valor servo

		/*Toggle y contador de intensidad de Led*/
		present=(JoyPWM.buffer[0]==40)||(JoyPWM.buffer[0]==24);
		// Si está preionado Y y alguno de los botones traseros laterales

		if(present && past==0)// si el valor de present cambió de 0 a 1
		{
			if(JoyPWM.buffer[0]==40)//si se presionó el botón derecho junto a Y
			{
				count++;	//incrementa contador de intensidad de luz en uno
				if(count>10)	count=10;	//limita contador a 10 como máximo
				Led << to_string(count*75)<<endl;//Escribe valor pwm a archivo
			}
			else //si se presionó el botón izquierdo junto a Y
			{
				count--; //Decrementa contador de intencidad de luz en uno
				if(count<0)	count=0; //Limita contador a 0 como mínimo
				Led << to_string(count*75)<<endl;//Escribe valor  pwm a archivo
			}
		}
		past=present;


		/*Escritura en servomotor*/
		Servo << to_string((int)JoyPWM.buffer[1]*10000)<<endl;
			//escribe valor de pwm de servo
  }
  Led.close(); //Cierra archivo de PWM de led
  Servo.close(); //Cierra archivo de PWM de servo
  return 0;
}
