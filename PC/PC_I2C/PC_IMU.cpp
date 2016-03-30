#include "socketmessage.cpp"
#include <fstream>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>



using namespace std;
using namespace socket_msg;

int main()
{

	double time, timef=0;



	double xaccel, yaccel, zaccel, temp, xgyro, ygyro, zgyro, xmag,ymag, zmag,ctime;

	socket_message IMU_PC(25423,"192.168.7.2",28);
	IMU_PC.init_tcp_client_socket();

	unlink("./imu_data.txt");

	fstream fs;
	fs.open ("imu_data.txt", std::fstream::in | std::fstream::out | std::fstream::app);






	while(1)
		{
		IMU_PC.read_tcp();

		/*
		 * Lectura de valores
		 */



		xaccel=((int16_t)((unsigned char)IMU_PC.buffer[1] | ((int16_t)(unsigned char)IMU_PC.buffer[0] << 8)))/16384.0;
		yaccel=((int16_t)((unsigned char)IMU_PC.buffer[3] | ((int16_t)(unsigned char)IMU_PC.buffer[2] << 8)))/16384.0;
		zaccel=((int16_t)((unsigned char)IMU_PC.buffer[5] | ((int16_t)(unsigned char)IMU_PC.buffer[4] << 8)))/16384.0;

		temp=(((int16_t)((unsigned char)IMU_PC.buffer[7] | ((int16_t)(unsigned char)IMU_PC.buffer[6] << 8)))-521)/340.0+35;
		xgyro=((int16_t)((unsigned char)IMU_PC.buffer[9] | ((int16_t)(unsigned char)IMU_PC.buffer[8] << 8)))/131.0;
		ygyro=((int16_t)((unsigned char)IMU_PC.buffer[11] | ((int16_t)(unsigned char)IMU_PC.buffer[10] << 8)))/131.0;
		zgyro=((int16_t)((unsigned char)IMU_PC.buffer[13] | ((int16_t)(unsigned char)IMU_PC.buffer[12] << 8)))/131.0;

		xmag=((int16_t)((unsigned char)IMU_PC.buffer[14] | ((int16_t)(unsigned char)IMU_PC.buffer[15] << 8)));
		ymag=((int16_t)((unsigned char)IMU_PC.buffer[16] | ((int16_t)(unsigned char)IMU_PC.buffer[17] << 8)));
		zmag=((int16_t)((unsigned char)IMU_PC.buffer[18] | ((int16_t)(unsigned char)IMU_PC.buffer[19] << 8)));


		memcpy(&time,&*(IMU_PC.buffer+20),sizeof(time));
		timef=time+timef;

		//for(int i=0; i<28; i++)
		//	cout<<hex<<(int)((unsigned char)IMU_PC.buffer[i])<<"\t\t";

		cout<<timef<<"\t";
		cout<<xaccel<<"\t";
		cout<<yaccel<<"\t";
		cout<<zaccel<<"\t";
		cout<<temp<<"\t";
		cout<<xgyro<<"\t";
		cout<<ygyro<<"\t";
		cout<<zgyro<<"\t";
		cout<<xmag<<"\t";
		cout<<ymag<<"\t";
		cout<<zmag<<"\t";
		cout<<sqrt(pow(xaccel,2)+pow(yaccel,2)+pow(zaccel,2))<<"\t";
		cout<<endl;

		fs<<timef<<"\t";
		fs<<xaccel<<"\t";
		fs<<yaccel<<"\t";
		fs<<zaccel<<"\t";
		fs<<temp<<"\t";
		fs<<xgyro<<"\t";
		fs<<ygyro<<"\t";
		fs<<zgyro<<"\t";
		fs<<xmag<<"\t";
		fs<<ymag<<"\t";
		fs<<zmag<<"\t";
		fs<<sqrt(pow(xaccel,2)+pow(yaccel,2)+pow(zaccel,2))<<"\t";
		fs<<endl;

		//fs << timef << "\t" << magnetom[0] << "\t" << magnetom[1] << "\t" << magnetom[2]
		//		   << "\t" << accel[0]  << "\t" << accel[1]  << "\t" << accel[2]
		//		   << "\t" << gyro[0] << "\t" << gyro[1] << "\t" << gyro[2] << "\n";
		}
}
