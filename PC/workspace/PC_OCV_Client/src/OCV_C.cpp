#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include "socketmessage.h"
#include "ThermalDisplay.h"


using namespace std;
using namespace cv;
using namespace socket_msg;
using namespace thermal;

int main(){


	char * datos = (char*)"B37.75,36.92,37.50,35.86,38.89,38.97,39.87,35.26,35.47,37.52,37.06,37.12,36.88,37.29,37.75,36.34,37.08,38.63,37.99,36.52,36.55,38.37,36.48,36.19,37.02,37.87,37.47,36.39,36.66,38.10,37.64,36.64,35.88,37.25,35.78,36.08,36.21,37.07,37.25,36.96,36.46,36.85,36.24,36.19,37.18,38.83,37.57,38.30,36.47,35.91,36.39,36.38,35.92,36.01,35.47,36.52,37.84,37.97,37.38,37.77,36.48,36.46,38.79,37.88,39.28,*";


	Thermal_Display termografo;
	termografo.gensweep();
	termografo.convert(datos);



	Mat  img = Mat(Size(320,240), CV_8UC3, Scalar(255, 0, 0));
  	namedWindow( "Display window");// Create a window for display.
	imshow( "Display window", img );
	waitKey();




  	int imsize=img.step*img.rows;

  	int envios=120;
  	while(imsize%envios) envios--;
  	int sizeBuff=imsize/envios;
  	cout<<imsize<<" "<<envios<<" "<<sizeBuff<<endl;

  	socket_message PC_Sock(23222,"192.168.0.1",sizeBuff);
  	PC_Sock.init_udp_receiver_socket();


  	int size=8;
  	int h=16*size;
  	int v=4*size;
  	int h0=0;
  	int v0=0;


  	int Temp[h][v];
  	float TempV[h][4];
  	Mat roi = img(Rect(h0,v0,h,v));
  	Mat color(roi.size(),CV_8UC3, Scalar(200,0,0));

  	for(int reng=0; reng<4 ; reng++)
  	{
  	for(int x=0; x<h/32; x++)
  	{
  		TempV[x][reng]=termografo.tempcalc(termografo.tamb, termografo.tdata[0+reng*16],0,h/32,x);
  	}

  	for(int n=1; n<16; n++)
  	{
  		for(int x=h/32+h/16*(n-1);x<h/32+h/16*(n);x++)
  		{
  		TempV[x][reng]=termografo.tempcalc(termografo.tdata[n-1+reng*16], termografo.tdata[n+reng*16],h/32+h/16*(n-1),h/32+h/16*(n),x);
  		}
  	}

  	for(int x=h-h/32; x<h; x++)
  	{
  		TempV[x][reng]=termografo.tempcalc(termografo.tdata[15+reng*16],termografo.tamb,h-h/32,h,x);
  	}
  	}


  	for(int x=0; x<h ; x++)
  	{
  		for(int y=0; y<v/8; y++)
  		{
  			Temp[x][y]=termografo.colorcalc(termografo.tamb,TempV[x][0],0,v/8,y);

		Vec3b& vec= color.at<Vec3b>(y,x);
			vec[0]=termografo.sweep[Temp[x][y]][0];
		  	vec[1]=termografo.sweep[Temp[x][y]][1];
		  	vec[2]=termografo.sweep[Temp[x][y]][2];
  		}

  		for(int reng=0; reng<3 ; reng++)
  		{
  		for(int y=v/8+v/4*reng; y<v/8+v/4*(reng+1); y++)
  		{
  			Temp[x][y]=termografo.colorcalc(TempV[x][reng],TempV[x][reng+1],v/8+v/4*reng,v/8+v/4*(reng+1),y);

		Vec3b& vec= color.at<Vec3b>(y,x);
			vec[0]=termografo.sweep[Temp[x][y]][0];
		  	vec[1]=termografo.sweep[Temp[x][y]][1];
		  	vec[2]=termografo.sweep[Temp[x][y]][2];
  		}
  		}
  		for(int y=v-v/8; y<v; y++)
  		{
  			Temp[x][y]=termografo.colorcalc(TempV[x][3],termografo.tamb,v-v/8,v,y);

		Vec3b& vec= color.at<Vec3b>(y,x);
			vec[0]=termografo.sweep[Temp[x][y]][0];
		  	vec[1]=termografo.sweep[Temp[x][y]][1];
		  	vec[2]=termografo.sweep[Temp[x][y]][2];
  		}

  	}



   	uchar k=0;
   	int j=0;



   	while(1)
   	{
  		while(j<envios-1)
   		{
   	   	PC_Sock.read_udp();
   		k=(uchar)PC_Sock.buffer[0];
   		j=(int)k;
   		if(j<envios && j>=0)
   		memcpy(&(*(img.data+j*(sizeBuff))),&(*(PC_Sock.buffer))+1,sizeBuff);
   		}


   		addWeighted(color,0.9,roi,1,0.0,roi);
		imshow( "Display window", img );
		j=0;
		if(waitKey(10 )==29) break;
   	}


	PC_Sock.close_socket();                   // Wait for a keystroke in the window/*
       return 0;
}
