#include<iostream>
#include<stdlib.h>
#include<cstdint>
#include<fstream>
#include <sys/time.h>
#include <sys/types.h>
#include "mpuclass.h"
#include "socketmessage.h"
#include <math.h>       /* sqrt */

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
	//HMC5883L mag(1);	//Crea objeto de tipo HMC5883L Magnetómetro
	//ADXL345 acc(1);		//Crea objeto de tipo ADXL345 Acelerómetro
	//ITG3200 gyro(1);	//Crea objeto de tipo ITG3200 Giroscopio

	MPU9150 imu(1);
	AK8975 mag(1);

	/*
	 * Creación de socket que transmitira valores para estimación
	 * de orientación y posición a PC, inálambrico
	 * Envía en buffer:
	 * |6Mag|6Acc|8Gyro|8Time|
	 */
	socket_message IMU_BBB(25423,"192.168.7.2",28);
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
	double xaccel, yaccel, zaccel, temp, xgyro, ygyro, zgyro, xmag,ymag, zmag,ctime;
	long int i=0;

	while (1) {
		i++;

		imu.readMPU();
		mag.readMag();
		stoptimer();
		time = (endt - startt);		// Time calculation (in seconds)
		starttimer();
		ctime=ctime+time;

		memcpy(IMU_BBB.buffer,imu.buffer,imu.buffsize);
		memcpy(&*(IMU_BBB.buffer+14),mag.buffer,mag.buffsize);
		memcpy(&*(IMU_BBB.buffer+14+6),&time,sizeof(double));


		IMU_BBB.write_tcp();


		//for(int i=0; i<14; i++)
		//cout<<hex<<(int)imu.buffer[i]<<"\t";

		/*xmag=((int16_t)(mag.buffer[0] | ((int16_t)mag.buffer[1] << 8)));
		ymag=((int16_t)(mag.buffer[2] | ((int16_t)mag.buffer[3] << 8)));
		zmag=((int16_t)(mag.buffer[4] | ((int16_t)mag.buffer[5] << 8)));


		xaccel=((int16_t)(imu.buffer[1] | ((int16_t)imu.buffer[0] << 8)))/16384.0;
		yaccel=((int16_t)(imu.buffer[3] | ((int16_t)imu.buffer[2] << 8)))/16384.0;
		zaccel=((int16_t)(imu.buffer[5] | ((int16_t)imu.buffer[4] << 8)))/16384.0;

		temp=(((int16_t)(imu.buffer[7] | ((int16_t)imu.buffer[6] << 8)))-521)/340.0+35;
		xgyro=((int16_t)(imu.buffer[9] | ((int16_t)imu.buffer[8] << 8)))/131.0;
		ygyro=((int16_t)(imu.buffer[11] | ((int16_t)imu.buffer[10] << 8)))/131.0;
		zgyro=((int16_t)(imu.buffer[13] | ((int16_t)imu.buffer[12] << 8)))/131.0;


		cout<<ctime<<"\t";
		cout<<xaccel<<"\t";
		cout<<yaccel<<"\t";
		cout<<zaccel<<"\t";
		cout<<temp<<"\t";
		cout<<xgyro<<"\t";
		cout<<ygyro<<"\t";
		cout<<zgyro<<"\t\t";
		cout<<xmag<<"\t";
		cout<<ymag<<"\t";
		cout<<zmag<<"\t";
		cout<<sqrt(pow(xaccel,2)+pow(yaccel,2)+pow(zaccel,2))<<"\t";



		cout<<endl;
*/

		usleep(1000);

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
