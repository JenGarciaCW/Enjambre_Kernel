#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include "socketmessage.cpp"
#include "ThermalDisplay.cpp"


using namespace std;
using namespace cv;
using namespace socket_msg;
using namespace thermal;

int main(){


	char * datos = (char*)"$15.16,16.35,16.06,16.13,15.59,16.26,16.84,16.76,"
			               "16.95,18.08,17.74,16.57,17.36,16.71,17.97,17.35,"
			               "17.31,17.43,17.46,17.34,17.45,18.29,17.48,16.84,"
			               "16.39,18.07,18.82,18.42,17.22,18.10,17.20,17.35,"
			               "18.10,17.42,16.44,17.68,15.73,16.64,17.07,18.07,"
			               "18.32,19.72,18.71,17.97,16.24,19.27,18.61,18.58,"
			               "18.07,15.65,17.68,16.75,17.95,16.71,17.77,16.73,"
			               "18.09,18.22,16.75,16.75,18.91,18.27,16.38,16.86,*";


	Thermal_Display termografo;
	termografo.gensweep();




	Mat  img = Mat(Size(640,480), CV_8UC3, Scalar(255, 0, 0));
  	namedWindow( "Display window");// Create a window for display.
	imshow( "Display window", img );
	VideoWriter ouputVideo;
	ouputVideo.open("vid01.avi",CV_FOURCC('M','P','E','G'),10,Size(640,480),true);
	waitKey();




  	int imsize=img.step*img.rows;

  	int envios=120;
  	while(imsize%envios) envios--;
  	int sizeBuff=imsize/envios;
  	cout<<imsize<<" "<<envios<<" "<<sizeBuff<<endl;

  	socket_message PC_Sock(23222,"192.168.0.1",sizeBuff);
  	PC_Sock.init_udp_receiver_socket();

	//socket_message Thermo(302000,"192.168.0.1",500); //Creación de socket UDP para envío de datos del termógrafo
	//Thermo.init_udp_receiver_socket();


  	int size=14;
  	int h=16*size;
  	int v=4*size;
  	int h0=95;
  	int v0=100;
	uchar k=0;
   	int j=0;



   	while(1)
   	{
   	//Thermo.read_udp();
    //datos=Thermo.buffer;
   	//cout<<datos<<endl;
  	termografo.convert(datos);
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







  		while(j<envios-1)
   		{
   	   	PC_Sock.read_udp();
   		k=(uchar)PC_Sock.buffer[0];
   		j=(int)k;
   		if(j<envios && j>=0)
   		memcpy(&(*(img.data+j*(sizeBuff))),&(*(PC_Sock.buffer))+1,sizeBuff);
   		}

  		flip(img,img,0);
  		flip(img,img,1);

   		//addWeighted(color,0.9,roi,1,0.0,roi);
		imshow( "Display window", img );
		ouputVideo.write(img);
		j=0;
		if(waitKey(10 )==29) break;
   	}


	PC_Sock.close_socket();                   // Wait for a keystroke in the window/*
       return 0;
}
