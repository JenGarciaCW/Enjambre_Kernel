#include<iostream>
#include<stdlib.h>
#include<cstdint>
#include<fstream>
#include<cmath>
#include <sys/time.h>
#include <sys/types.h>
#include "i2cdevice.h"
#include "socketmessage.h"
#define N 34


using namespace std;
using namespace i2cdevn;
using namespace socket_msg;


struct timeval tp;	//estructura de tiempo que contiene segundos y microsegundos
double sec, usec, startt, endt;	//variables para cálculo de tiempo
double angle , vel;

void starttimer(); /*Función para iniciar conteo de tiempo*/
void stoptimer(); /*Función para detener conteo de tiempo*/
void rotate_arr(float* array, int size );  /*Rotate array function*/
float scalar(float* array1 , float* array2, int size); /*scalar product 2 vect*/
void sum_vs(float* array , int value, int size); /*Add scalar to vector elements*/
float average(float* array , int size); /*Average vector elements*/

int main() {
	double time;
	float b[N] = { 0.000000298910309 ,  0.000001959461095 ,  0.000002847097794,
				  -0.000025211438755 , -0.000195788765758 , -0.000789310970447,
				  -0.002259462041526 , -0.004986784692646 , -0.008656045841657,
				  -0.011429277452822 , -0.009517036757316 ,  0.002063978790339,
				   0.027224499002438 ,  0.065794886477369 ,  0.111565845660295,
				   0.153155814565968 ,  0.178048771809244 ,  0.178048771809244,
				   0.153155814565968 ,  0.111565845660295 ,  0.065794886477369,
				   0.027224499002438 ,  0.002063978790339 , -0.009517036757316,
				  -0.011429277452822 , -0.008656045841657 , -0.004986784692646,
				  -0.002259462041526 , -0.000789310970447 , -0.000195788765758,
				  -0.000025211438755 ,  0.000002847097794 ,  0.000001959461095,
				   0.000000298910309 };

	float x_n[N], y_n[N], vel_n[N], vel_n_b[N], x_n_b[N];
	float vel;

	//AS5048 one(0x042,2,3);	//Crea objeto de tipo AS5048, sensor de rotación
	AS5048 two(0x042,2,3);
	//HMC5883L mag(1);	//Crea objeto de tipo HMC5883L Magnetómetro
	//ADXL345 acc(1);		//Crea objeto de tipo ADXL345 Acelerómetro
	//ITG3200 gyro(1);	//Crea objeto de tipo ITG3200 Giroscopio

	/*
	 * Creación de socket que transmitira valores para estimación
	 * de orientación y posición a PC, inálambrico
	 * Envía en buffer:
	 * |6Mag|6Acc|8Gyro|8Time|
	 */
	//socket_message IMU_BBB(25433,"192.168.0.2",28);
	//IMU_BBB.init_tcp_server_socket();

	/*
	 * Creación de socket que transmitirá valores para control de
	 * motores laterales, interno
	 * Envía en buffer
	 * |2Enc1|2Enc2|4Time|
	 */
	//socket_message CTL_ENCO("/root/messENC",2*sizeof(int)+sizeof(float));
	//CTL_ENCO.init_unix_server_socket();

	starttimer();

	angle = two.readang();

	for(int i = 0 ; i<N ; i++)
	{
		angle = two.readang();
		x_n[i]=angle;
		y_n[i]=angle;
		vel_n[i]=0;
		vel_n_b[i]=0;

	}

	while (1) {



		angle = two.readang();
		/*Lee datos del cronómetro y los copia al buffer*/
		stoptimer();
		time = (endt - startt);		// Time calculation (in seconds)
		starttimer();


		rotate_arr(x_n,N);
		rotate_arr(y_n,N);

		x_n[N-1]=angle;
		y_n[N-1]=scalar(b,x_n,N);

		if(x_n[N-1]-x_n[N-2]>250)
		{
			sum_vs(y_n,360,N);
			sum_vs(x_n,360,N);
			x_n[N-1]=angle;
		}
		else if(x_n[N-2]-x_n[N-1]>250)
		{
			sum_vs(y_n,-360,N);
			sum_vs(x_n,-360,N);
			x_n[N-1]=angle;
		}

		rotate_arr(vel_n,N);
		rotate_arr(vel_n_b,N);

		vel = (y_n[N-1]-y_n[N-2])/time ;

		vel_n[N-1]=vel;
		vel=average(vel_n,N);

		vel_n_b[N-1]=vel;
		vel=average(vel_n_b,N);


		cout<<"\t"<<angle<<"\t"<<time<<"\t"<<vel<<"\t"<<endl;
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

void rotate_arr(float* array, int size )
{
	for(int i = 1 ; i < size ; i++)
	{
		array[i-1]=array[i];
		array[i]=0;
	}
}

float scalar(float* in1 , float* in2, int size)
{
	float out=0;

	for(int i = 1 ; i < size ; i++)
	{
		out=out+in1[i]*in2[i];
	}
	return out;
}

void sum_vs(float* array , int value, int size)
{
	for(int i = 1 ; i < size ; i++)
	{
		array[i]=array[i]+value;
	}
}

float average(float* array , int size)
{
	float average=0;

	for(int i = 1 ; i < size ; i++)
	{
		average=average+array[i];
	}
	average=average/size;
	return average;
}

