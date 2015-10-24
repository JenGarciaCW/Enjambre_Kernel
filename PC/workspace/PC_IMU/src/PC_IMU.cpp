#include<iostream>
#include<math.h>
#include<fstream>

#include"socketmessage.h"

using namespace std;
using namespace socket_msg;

int main(){
	//ofstream file;
	//file.open("data2.csv",ofstream::out);

	double time;
	float xmag,ymag,zmag;
	float xacc,yacc,zacc;
	float temp, xgyro, ygyro, zgyro;
	float sum=0,average=0;


	/*Valores iniciales*/

	double psi=0, theta=1, phi=0; //Ángulos de Euler
	double dpsi=0, dtheta=0, dphi=0;//Velocidades Ángulos de Euler

	double p=0, q=0, r=0; //Velocidades angulares medidas en la IMU

	double dx=0, dy=0, dz=0; //Velocidades lineales
	double xh=0, yh=0; //Magnitud de proyecciones de medidas del magnetómetro



	return 0;

	socket_message IMU_PC(25433,"192.168.0.2",28);
	IMU_PC.init_tcp_client_socket();
	int count=0;



	while(count<5000)
	{
	IMU_PC.read_tcp();

	/*
	 * Lectura de valores
	 */


	xmag=(int16_t)(IMU_PC.buffer[1] | ((int16_t)IMU_PC.buffer[0] << 8));
	ymag=(int16_t)(IMU_PC.buffer[3] | ((int16_t)IMU_PC.buffer[2] << 8));
	zmag=(int16_t)(IMU_PC.buffer[5] | ((int16_t)IMU_PC.buffer[4] << 8));

	xacc=0.004*(int16_t)((unsigned char)IMU_PC.buffer[6]|(unsigned char)IMU_PC.buffer[7] << 8);
	yacc=0.004*(int16_t)((unsigned char)IMU_PC.buffer[8]|(unsigned char)IMU_PC.buffer[9] << 8);
	zacc=0.004*(int16_t)((unsigned char)IMU_PC.buffer[10]|(unsigned char)IMU_PC.buffer[11] << 8);

	temp=35.0+(((int16_t)((unsigned char)IMU_PC.buffer[12]<<8 | (unsigned char) IMU_PC.buffer[13]))+13200)/280.0;
	xgyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[14]<<8)|((unsigned char)IMU_PC.buffer[15])))/14.375;
	ygyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[16]<<8)|((unsigned char)IMU_PC.buffer[17])))/14.375;
	zgyro =(float)((int16_t)(((unsigned char)IMU_PC.buffer[18]<<8)|((unsigned char)IMU_PC.buffer[19])))/14.375;

	/*
	 * Rotación y offset
	 */
	p=(ygyro-(0.02827788*temp-0.15901965));
	p=p*3.14159264/180;
	q=-(zgyro-(-0.00097982*temp+0.58196216));
	q=q*3.14159264/180;
	r=-(xgyro-(0.01137447*temp-2.13394702));
	r=r*3.14159264/180;


	memcpy(&time,&*(IMU_PC.buffer+20),sizeof(time));

	//file << xmag << "\t" << ymag << "\t" << zmag<<"\t"<<xacc<<"\t"<<yacc<<"\t"<<zacc<<"\t"<<temp<<"\t"<<xgyro<<"\t"<<ygyro<<"\t"<<zgyro<<"\t"<<time<<"\n";

	//cout << xmag << "\t" << ymag << "\t" << zmag<<"\t"<<xacc<<"\t"<<yacc<<"\t"<<zacc<<"\t"<<temp<<"\t"<<xgyro<<"\t"<<ygyro<<"\t"<<zgyro<<"\t"<<time;




	/*Velocidades ángulos de Euler*/

	dpsi=p+q*(sin(phi)*tan(theta))+r*(cos(phi)*tan(theta));
	dtheta=q*(cos(phi))-r*(sin(phi));
	dphi=q*(sin(phi)/sin(theta))+r*(cos(phi)/sin(theta));

	psi=psi+dpsi*time;
	theta=theta+dtheta*time;
	phi=phi+dphi*time;

	count ++;
	sum=sum+p;
	average=sum/count;
	cout<<average<<p<<endl;
	//cout<<p<<"\t"<<q<<"\t"<<r<<endl;
	//cout<<"\t"<<dpsi<<"\t"<<q<<"\t"<<r;//<<"\t"<<dpsi<<"\t"<<time<<"\t"<<p<<"\t"<<q<<"\t"<<r<<endl;
	//cout<<phi/3.1415*180<<"\t"<<theta/3.1415*180<<"\t"<<psi/3.1415*180<<endl;


	}
	//file.close();
	IMU_PC.close_socket();




	return 0;
}
