#include<iostream>
#include<stdlib.h>
#include<cstdint>
#include<fstream>
#include <sys/time.h>
#include <sys/types.h>
#include "mpuclass.h"
#include "socketmessage.h"


using namespace std;
using namespace i2cdevn;
using namespace socket_msg;


struct timeval tp;	//estructura de tiempo que contiene segundos y microsegundos
double sec, usec, startt, endt;	//variables para cálculo de tiempo

void starttimer(); /*Función para iniciar conteo de tiempo*/
void stoptimer(); /*Función para detener conteo de tiempo*/


int main() {
	double time;

	//AS5048 one(0x042,2,3);	//Crea objeto de tipo AS5048, sensor de rotación
	//AS5048 two(0x040,2,3);
	HMC5883L mag(1);	//Crea objeto de tipo HMC5883L Magnetómetro
	ADXL345 acc(1);		//Crea objeto de tipo ADXL345 Acelerómetro
	ITG3200 gyro(1);	//Crea objeto de tipo ITG3200 Giroscopio

	/*
	 * Creación de socket que transmitira valores para estimación
	 * de orientación y posición a PC, inálambrico
	 * Envía en buffer:
	 * |6Mag|6Acc|8Gyro|8Time|
	 */
	socket_message IMU_BBB(25433,"192.168.0.2",28);
	IMU_BBB.init_tcp_server_socket();

	/*
	 * Creación de socket que transmitirá valores para control de
	 * motores laterales, interno
	 * Envía en buffer
	 * |2Enc1|2Enc2|4Time|
	 */
	//socket_message CTL_ENCO("/root/messENC",2*sizeof(int)+sizeof(float));
	//CTL_ENCO.init_unix_server_socket();

	starttimer();


	while (1) {


		/*Lee datos del magnetómetro y los copia al buffer*/
		mag.readmag();
		memcpy(IMU_BBB.buffer,mag.buffer,mag.buffsize);

		/*Lee datos del acelerómetro y los copia al buffer*/
		acc.readacc();
		memcpy(&*(IMU_BBB.buffer+6),acc.buffer,acc.buffsize);

		/*Lee datos del giroscopio y los copia al buffer*/
		gyro.readgyro();
		memcpy(&*(IMU_BBB.buffer+6+6),gyro.buffer,gyro.buffsize);


		/*Lee datos del cronómetro y los copia al buffer*/
		stoptimer();
		time = (endt - startt);		// Time calculation (in seconds)
		starttimer();
		memcpy(&*(IMU_BBB.buffer+8+6+6),&time,sizeof(double));

		IMU_BBB.write_tcp(); //Envía datos a PC

		usleep(5000);

	}

	return 0;
}

void starttimer()
{

	/* Inicizlización de medición de tiempo to */
	gettimeofday( &tp, NULL ); //Almacena dato de tiempo en tp
	sec = static_cast<double>( tp.tv_sec ); //obtiene tiempo en segundos
	usec = static_cast<double>( tp.tv_usec )/1E6;//obtiene fracción microsecs
	startt = sec + usec; //define el tiempo de inicio en segundos
}

void stoptimer()
{
	// Time stamp after the computations
	gettimeofday( &tp, NULL ); //Almacena dato de tiempo en tp
	sec = static_cast<double>( tp.tv_sec ); //obtiene tiempo en segundos
	usec = static_cast<double>( tp.tv_usec )/1E6;//obtiene fracción microsecs
	endt = sec + usec;
}
