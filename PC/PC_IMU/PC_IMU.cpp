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
	float temp=0;
	float accel[3],gyro[3],magnetom[3];//




	socket_message IMU_PC(25433,"192.168.0.2",28);
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

		magnetom[0]=(int16_t)(IMU_PC.buffer[1] | ((int16_t)IMU_PC.buffer[0] << 8));
		magnetom[1]=-1*(int16_t)(IMU_PC.buffer[3] | ((int16_t)IMU_PC.buffer[2] << 8));
		magnetom[2]=-1*(int16_t)(IMU_PC.buffer[5] | ((int16_t)IMU_PC.buffer[4] << 8));

		accel[0]=(int16_t)((unsigned char)IMU_PC.buffer[6]|(unsigned char)IMU_PC.buffer[7] << 8);
		accel[1]=(int16_t)((unsigned char)IMU_PC.buffer[8]|(unsigned char)IMU_PC.buffer[9] << 8);
		accel[2]=(int16_t)((unsigned char)IMU_PC.buffer[10]|(unsigned char)IMU_PC.buffer[11] << 8);

		temp=35.0+(((int16_t)((unsigned char)IMU_PC.buffer[12]<<8 | (unsigned char) IMU_PC.buffer[13]))+13200)/280.0;
		gyro[0] =-1*(float)((int16_t)(((unsigned char)IMU_PC.buffer[14]<<8)|((unsigned char)IMU_PC.buffer[15])));
		gyro[1] =-1*(float)((int16_t)(((unsigned char)IMU_PC.buffer[16]<<8)|((unsigned char)IMU_PC.buffer[17])));
		gyro[2] =-1*(float)((int16_t)(((unsigned char)IMU_PC.buffer[18]<<8)|((unsigned char)IMU_PC.buffer[19])));

		memcpy(&time,&*(IMU_PC.buffer+20),sizeof(time));
		timef=time+timef;
		fs << timef << "\t" << magnetom[0] << "\t" << magnetom[1] << "\t" << magnetom[2]
				   << "\t" << accel[0]  << "\t" << accel[1]  << "\t" << accel[2]
				   << "\t" << gyro[0] << "\t" << gyro[1] << "\t" << gyro[2] << "\n";
		}
}
