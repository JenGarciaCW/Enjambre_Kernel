#include <iostream>
#include <oct.h>
#include "socketmessage.h"

using namespace std;
using namespace socket_msg;

int main (void)
{
	/*
	 * Vectores del modelo*
	 */
	float X=0, Y=0, Z=0, u=0, v=0, w=0, phi=0, theta=0, psi=0; //x
	float Xr=0,Yr=0,Zr=0,ur=0,vr=0,wr=0,phir=0,thetar=0,psimag=0;//y
	float g=9.81;//g
	float ddx,ddy,ddz,p,q,r;//u

	double xh=0, yh=0; //Magnitud de proyecciones de medidas del magnetómetro


	  int n = 9;
	  Matrix A = Matrix (n, n);
	  Matrix B = Matrix (n,6);
	  Matrix f = Matrix(9,1);
	  Matrix x_vector = Matrix (n, 1);
	  Matrix xp_vector = Matrix (n, 1);

	  Matrix y_vector = Matrix (7, 1);
	  Matrix e_vector = Matrix (7, 1);
	  Matrix z_vector = Matrix (n, 1);


	  Matrix H = Matrix (n, n);
	  Matrix P = Matrix (n, n);
	  Matrix Pp = Matrix (n, n);
	  Matrix R = Matrix (n, n);
	  Matrix E = Matrix (n, n);
	  Matrix K = Matrix (n, n);
	  Matrix Zm = Matrix (n, n);
 /*
  * Readings
  */
		double time;
		float xmag,ymag,zmag;
		float xacc,yacc,zacc;
		float temp, xgyro, ygyro, zgyro;
		float sum=0,average=0;

		socket_message IMU_PC(25433,"192.168.0.2",28);
		IMU_PC.init_tcp_client_socket();
		int count=0;

/*
 * Data
 */
  x_vector(0,0)=X;
  x_vector(1,0)=Y;
  x_vector(2,0)=Z;
  x_vector(3,0)=u;
  x_vector(4,0)=v;
  x_vector(5,0)=w;
  x_vector(6,0)=phi;
  x_vector(7,0)=theta;
  x_vector(8,0)=psi;

  z_vector(0,0)=Xr;
  z_vector(1,0)=Yr;
  z_vector(2,0)=Zr;
  z_vector(3,0)=ur;
  z_vector(4,0)=vr;
  z_vector(5,0)=wr;
  z_vector(6,0)=phir;
  z_vector(7,0)=thetar;
  z_vector(8,0)=psimag;

  cout<<x_vector<<endl;

  f(0,0)=u*cos(theta)*cos(psi) + cos(psi)*sin(theta)*(w*cos(phi) + v*sin(phi)) + (- v*cos(phi) + w*sin(phi))*sin(psi);
  f(0,1)=cos(psi)*(v*cos(phi) - w*sin(phi)) + u*cos(theta)*sin(psi) + sin(theta)*(w*cos(phi) + v*sin(phi))*sin(psi);
  f(0,2)=- u*sin(theta) + cos(theta)*(w*cos(phi) + v*sin(phi));
  f(0,3)=ddx + r*v - q*w + g*sin(theta);
  f(0,4)=ddy - r*u + p*w - g*cos(theta)*sin(phi);
  f(0,5)=ddz + q*u - p*v - g*cos(theta)*cos(phi);
  f(0,6)=p + r*cos(phi)*tan(theta) + q*sin(phi)*tan(theta);
  f(0,7)=q*cos(phi) - r*sin(phi);
  f(0,8)=1/cos(theta)*(r*cos(phi) + q*sin(phi));



  A(0,0)=0;
  A(0,1)=0;
  A(0,2)=0;
  A(0,3)=cos(theta)*cos(psi);
  A(0,4)=cos(psi)*sin(theta)*sin(phi) - cos(phi)*sin(psi);
  A(0,5)=cos(phi)*cos(psi)*sin(theta) + sin(phi)*sin(psi);
  A(0,6)=cos(psi)*sin(theta)*(v*cos(phi) - w*sin(phi)) + (w*cos(phi) + v*sin(phi))*sin(psi);
  A(0,7)=cos(psi)*(-u * sin(theta) + cos(theta)*(w*cos(phi) + v*sin(phi)));
  A(0,8)=cos(psi)*(-v * cos(phi) + w*sin(phi)) - (u*cos(theta) + sin(theta)*(w*cos(phi) + v*sin(phi)))*sin(psi);

  A(1,0)=0;
  A(1,1)=0;
  A(1,2)=0;
  A(1,3)=cos(theta)*sin(psi);
  A(1,4)=cos(phi)*cos(psi) + sin(theta)*sin(phi)*sin(psi);
  A(1,5)=-cos(psi)*sin(phi) + cos(phi)*sin(theta)*sin(psi);
  A(1,6)=-cos(psi)*(w*cos(phi) + v*sin(phi)) + sin(theta)*(v*cos(phi) - w*sin(phi)) *sin(psi);
  A(1,7)=(-u*sin(theta) + cos(theta)*(w*cos(phi) + v*sin(phi)))*sin(psi);
  A(1,8)=u*cos(theta)*cos(psi) + cos(psi)*sin(theta)*(w*cos(phi) + v*sin(phi)) + (- v*cos(phi) + w*sin(phi))*sin(psi);

  A(2,0)=0;
  A(2,1)=0;
  A(2,2)=0;
  A(2,3)=-sin(theta);
  A(2,4)=cos(theta)*sin(phi);
  A(2,5)=cos(theta)*cos(phi);
  A(2,6)=cos(theta)*(v*cos(phi) - w*sin(phi));
  A(2,7)=-u*cos(theta) - sin(theta)*(w*cos(phi) + v*sin(phi));
  A(2,8)=0;

  A(3,0)=0;
  A(3,1)=0;
  A(3,2)=0;
  A(3,3)=0;
  A(3,4)=r;
  A(3,5)=-q;
  A(3,6)=0;
  A(3,7)=g*cos(theta);
  A(3,8)=0;

  A(4,0)=0;
  A(4,1)=0;
  A(4,2)=0;
  A(4,3)=-r;
  A(4,4)=0;
  A(4,5)=p;
  A(4,6)=-g*cos(theta)*cos(phi);
  A(4,7)=g*sin(theta)*sin(phi);
  A(4,8)=0;

  A(5,0)=0;
  A(5,1)=0;
  A(5,2)=0;
  A(5,3)=q;
  A(5,4)=-p;
  A(5,5)=0;
  A(5,6)=-g*cos(theta)*sin(phi);
  A(5,7)=g*sin(theta)*cos(phi);
  A(5,8)=0;

  A(6,0)=0;
  A(6,1)=0;
  A(6,2)=0;
  A(6,3)=0;
  A(6,4)=0;
  A(6,5)=0;
  A(6,6)=(q*cos(phi) - r*sin(phi))*tan(theta);
  A(6,7)=1/(cos(theta)*cos(theta))*(r*cos(phi) + q*sin(phi));
  A(6,8)=0;

  A(7,0)=0;
  A(7,1)=0;
  A(7,2)=0;
  A(7,3)=0;
  A(7,4)=0;
  A(7,5)=0;
  A(7,6)=-r*cos(phi) - q*sin(phi);
  A(7,7)=0;
  A(7,8)=0;

  A(8,0)=0;
  A(8,1)=0;
  A(8,2)=0;
  A(8,3)=0;
  A(8,4)=0;
  A(8,5)=0;
  A(8,6)=1/cos(theta)*(q*cos(phi) - r*sin(phi));
  A(8,7)=1/cos(theta)*(r*cos(phi) + q*sin(phi))*tan(theta);
  A(8,8)=0;

  B(0,0)=0;
  B(0,1)=0;
  B(0,2)=0;
  B(0,3)=0;
  B(0,4)=0;
  B(0,5)=0;

  B(1,0)=0;
  B(1,1)=0;
  B(1,2)=0;
  B(1,3)=0;
  B(1,4)=0;
  B(1,5)=0;

  B(2,0)=0;
  B(2,1)=0;
  B(2,2)=0;
  B(2,3)=0;
  B(2,4)=0;
  B(2,5)=0;

  B(3,0)=1;
  B(3,1)=0;
  B(3,2)=0;
  B(3,3)=0;
  B(3,4)=-w;
  B(3,5)=v;

  B(4,0)=0;
  B(4,1)=1;
  B(4,2)=0;
  B(4,3)=w;
  B(4,4)=0;
  B(4,5)=-u;

  B(5,0)=0;
  B(5,1)=0;
  B(5,2)=1;
  B(5,3)=-v;
  B(5,4)=u;
  B(5,5)=0;

  B(6,0)=0;
  B(6,1)=0;
  B(6,2)=0;
  B(6,3)=1;
  B(6,4)=sin(phi)*tan(theta);
  B(6,5)=cos(phi)*tan(theta);

  B(7,0)=0;
  B(7,1)=0;
  B(7,2)=0;
  B(7,3)=0;
  B(7,4)=cos(phi);
  B(7,5)=-sin(phi);

  B(8,0)=0;
  B(8,1)=0;
  B(8,2)=0;
  B(8,3)=0;
  B(8,4)=1/cos(theta)*sin(phi);
  B(8,5)=cos(phi)*1/cos(theta);


  H(0,0)=1;
  H(0,1)=0;
  H(0,2)=0;
  H(0,3)=0;
  H(0,4)=0;
  H(0,5)=0;
  H(0,6)=0;
  H(0,7)=0;
  H(0,8)=0;

  H(1,0)=0;
  H(1,1)=1;
  H(1,2)=0;
  H(1,3)=0;
  H(1,4)=0;
  H(1,5)=0;
  H(1,6)=0;
  H(1,7)=0;
  H(1,8)=0;

  H(2,0)=0;
  H(2,1)=0;
  H(2,2)=1;
  H(2,3)=0;
  H(2,4)=0;
  H(2,5)=0;
  H(2,6)=0;
  H(2,7)=0;
  H(2,8)=0;

  H(3,0)=0;
  H(3,1)=0;
  H(3,2)=0;
  H(3,3)=cos(theta)*cos(psi);
  H(3,4)=cos(psi)*sin(theta)*sin(phi) - cos(phi)*sin(psi);
  H(3,5)=cos(psi)*sin(theta)*cos(phi)+sin(psi)*sin(phi);
  H(3,6)=0;
  H(3,7)=0;
  H(3,8)=0;

  H(4,0)=0;
  H(4,1)=0;
  H(4,2)=0;
  H(4,3)=cos(theta)*sin(psi);
  H(4,4)=cos(phi)*cos(psi) + sin(theta)*sin(phi)*sin(psi);
  H(4,5)=sin(psi)*sin(theta)*cos(phi)-cos(psi)*sin(phi);
  H(4,6)=0;
  H(4,7)=0;
  H(4,8)=0;

  H(5,0)=0;
  H(5,1)=0;
  H(5,2)=0;
  H(5,3)=-sin(theta);
  H(5,4)=cos(theta)*sin(phi);
  H(5,5)=cos(theta)*cos(phi);
  H(5,6)=0;
  H(5,7)=0;
  H(5,8)=0;

  H(6,0)=0;
  H(6,1)=0;
  H(6,2)=0;
  H(6,3)=0;
  H(6,4)=0;
  H(6,5)=0;
  H(6,6)=1;
  H(6,7)=0;
  H(6,8)=0;

  H(7,0)=0;
  H(7,1)=0;
  H(7,2)=0;
  H(7,3)=0;
  H(7,4)=0;
  H(7,5)=0;
  H(7,6)=0;
  H(7,7)=1;
  H(7,8)=0;

  H(8,0)=0;
  H(8,1)=0;
  H(8,2)=0;
  H(8,3)=0;
  H(8,4)=0;
  H(8,5)=0;
  H(8,6)=0;
  H(8,7)=0;
  H(8,8)=1;

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
	 */
	p=(ygyro-(0.02827788*temp-0.15901965))*3.1415926/180;
	q=(zgyro-(-0.00097982*temp+0.58196216))*3.1415926/180;
	r=(xgyro-(0.01137447*temp-2.13394702))*3.1415926/180;


	memcpy(&time,&*(IMU_PC.buffer+20),sizeof(time));

	//cout<<time<<"\t";
	  if((theta<1.5 || theta > -1.5) ||(theta>1.6 || theta<-1.6))
	  {
	  f(0,0)=u*cos(theta)*cos(psi) + cos(psi)*sin(theta)*(w*cos(phi) + v*sin(phi)) + (- v*cos(phi) + w*sin(phi))*sin(psi);
	  f(1,0)=cos(psi)*(v*cos(phi) - w*sin(phi)) + u*cos(theta)*sin(psi) + sin(theta)*(w*cos(phi) + v*sin(phi))*sin(psi);
	  f(2,0)=- u*sin(theta) + cos(theta)*(w*cos(phi) + v*sin(phi));
	  f(3,0)=ddx + r*v - q*w + g*sin(theta);
	  f(4,0)=ddy - r*u + p*w - g*cos(theta)*sin(phi);
	  f(5,0)=ddz + q*u - p*v - g*cos(theta)*cos(phi);
	  f(6,0)=p + r*cos(phi)*tan(theta) + q*sin(phi)*tan(theta);
	  f(7,0)=q*cos(phi) - r*sin(phi);
	  f(8,0)=1/cos(theta)*(r*cos(phi) + q*sin(phi));
	  }
	  z_vector(0,0)=z_vector(0,0)+f(0,0)*time;
	  z_vector(1,0)=z_vector(1,0)+f(1,0)*time;
	  z_vector(2,0)=z_vector(2,0)+f(2,0)*time;
	  z_vector(3,0)=z_vector(3,0)+f(3,0)*time;
	  z_vector(4,0)=z_vector(4,0)+f(4,0)*time;
	  z_vector(5,0)=z_vector(5,0)+f(5,0)*time;
	  z_vector(6,0)=z_vector(6,0)+f(6,0)*time;
	  z_vector(7,0)=z_vector(7,0)+f(7,0)*time;
	  z_vector(8,0)=atan2(ymag*cos(phi)+zmag*sin(phi),xmag*cos(theta)-zmag*cos(phi)*sin(theta)+ymag*sin(theta)*sin(phi));

	  phi=z_vector(6,0);
	  theta=z_vector(7,0);
	  psi=z_vector(8,0);

	  cout<<z_vector.transpose()<<f.transpose()<<endl;
	}

	IMU_PC.close_socket();


  /*
   * Prediction
   */

  x_vector=A*x_vector;
  P=A*P*A.transpose();

/*
 * Correction
 */

  e_vector = H * x_vector; //Expectativa de la medición
  E=H*P*H.transpose(); //covarianza de la espectativa

  z_vector=y_vector-e_vector; //inovación
  Zm = R + E;// covarianza de la inovación

  K = P*H.transpose()*Zm.inverse(); //ganancia de Kalman
  x_vector = x_vector + K*z_vector;
  P = P - K*H*P;

  cout<<endl;
  cout<<xp_vector<<endl;
  cout<<P<<endl;
  cout<<endl;

  cout<<Pp<<endl;

  cout<<endl;
  cout<<endl;
  cout<<endl;


  cout << A;
  A.inverse();

  cout<< "inverse";
  cout << A;

  for (octave_idx_type i = 0; i < n; i++)
    for (octave_idx_type j = 0; j < n; j++)
      A(i,j) = (i + 1) * 10 + (j + 1);

  cout << A;

  return 0;
}
