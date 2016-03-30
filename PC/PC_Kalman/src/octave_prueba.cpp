#include <iostream>
#include <oct.h>
#include "socketmessage.h"
#include "datasensor.h"


using namespace std;
using namespace socket_msg;
using namespace data_sensor;

int main (void)
{
	socket_message IMU_PC(25433,"192.168.0.2",28);
	IMU_PC.init_tcp_client_socket();
	int count=0;

		double time;
		float xmag,ymag,zmag;
		float xacc,yacc,zacc;
		float temp, xgyro, ygyro, zgyro;
		float sum=0,average=0;
		float ddx,ddy,ddz,p,q,r;//u








  /*
   * LOOP
   */

	while(1)
	{
	IMU_PC.read_tcp();

	/*
	 * Lectura de valores
	 */

	xmag=(int16_t)(IMU_PC.buffer[1] | ((int16_t)IMU_PC.buffer[0] << 8));
	ymag=(int16_t)(IMU_PC.buffer[3] | ((int16_t)IMU_PC.buffer[2] << 8));
	zmag=(int16_t)(IMU_PC.buffer[5] | ((int16_t)IMU_PC.buffer[4] << 8));

	ddx=0.004*(int16_t)((unsigned char)IMU_PC.buffer[6]|(unsigned char)IMU_PC.buffer[7] << 8);
	ddy=0.004*(int16_t)((unsigned char)IMU_PC.buffer[8]|(unsigned char)IMU_PC.buffer[9] << 8);
	ddz=0.004*(int16_t)((unsigned char)IMU_PC.buffer[10]|(unsigned char)IMU_PC.buffer[11] << 8);

	temp=35.0+(((int16_t)((unsigned char)IMU_PC.buffer[12]<<8 | (unsigned char) IMU_PC.buffer[13]))+13200)/280.0;
	xgyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[14]<<8)|((unsigned char)IMU_PC.buffer[15])))/14.375;
	ygyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[16]<<8)|((unsigned char)IMU_PC.buffer[17])))/14.375;
	zgyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[18]<<8)|((unsigned char)IMU_PC.buffer[19])))/14.375;


	/*
	 * Rotación y offset

	p=(ygyro-(0.02827788*temp-0.15901965))*3.1415926/180;
	q=(zgyro-(-0.00097982*temp+0.58196216))*3.1415926/180;
	r=(xgyro-(0.01137447*temp-2.13394702))*3.1415926/180;
	 */

	memcpy(&time,&*(IMU_PC.buffer+20),sizeof(time));

	cout<<time<<"\t";
	cout<<xmag<<"\t"<<ymag<<"\t"<<zmag<<"\t";
	cout<<ddx<<"\t"<<ddy<<"\t"<<ddz<<"\t";
	cout<<temp<<"\t";
	cout<<xgyro<<"\t"<<ygyro<<"\t"<<zgyro<<endl;
	}
	IMU_PC.close_socket();



  return 0;
}
