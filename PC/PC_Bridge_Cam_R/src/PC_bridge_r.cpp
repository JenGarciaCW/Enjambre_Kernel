#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "ThermalDisplay.cpp"
#include "socketmessage.cpp"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libavformat/avformat.h"
#include "MLX90620_registers.h"


using namespace cv;
using namespace std;
using namespace socket_msg;
using namespace thermal;

char *size_c  ;
double ttime=0.01;
int	tsize = 2320;

double rtime = 0;
int		rsize =0;


void varInitialization(unsigned char calibration_data[]);
void calculate_TA(void);
void calculate_TO();

//Global variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
unsigned int ptat;
int16_t cpix;
int irData[64]; //Contains the raw IR data from the sensor
float temperatures[64]; //Contains the calculated temperatures of each pixel in the array
float Tambient; //Tracks the changing ambient temperature of the sensor
//These are constants calculated from the calibration data stored in EEPROM
//See varInitialize and section 7.3 for more information
int v_th, a_cp, b_cp, tgc, b_i_scale;
float k_t1, k_t2, emissivity;
int a_ij[64], b_ij[64];
//These values are calculated using equation 7.3.3.2
//They are constants and can be calculated using the MLX90620_alphaCalculator sketch
float alpha_ij[64] = {
  1.71613E-8, 1.98971E-8, 2.04791E-8, 1.93150E-8, 1.93150E-8, 2.24000E-8, 2.28074E-8, 2.10612E-8,
  2.16433E-8, 2.42044E-8, 2.45537E-8, 2.33895E-8, 2.30403E-8, 2.67074E-8, 2.72894E-8, 2.45537E-8,
  2.43791E-8, 2.81043E-8, 2.82790E-8, 2.61253E-8, 2.49611E-8, 2.88610E-8, 2.82790E-8, 2.71148E-8,
  2.55432E-8, 2.94431E-8, 2.98506E-8, 2.75223E-8, 2.55432E-8, 2.96177E-8, 3.01998E-8, 2.76969E-8,
  2.59507E-8, 2.96177E-8, 2.98506E-8, 2.75223E-8, 2.59507E-8, 2.92685E-8, 2.96177E-8, 2.75223E-8,
  2.51358E-8, 2.90357E-8, 2.90357E-8, 2.71148E-8, 2.49611E-8, 2.82790E-8, 2.84536E-8, 2.62999E-8,
  2.37970E-8, 2.68820E-8, 2.75223E-8, 2.51358E-8, 2.33895E-8, 2.65327E-8, 2.61253E-8, 2.36224E-8,
  2.20507E-8, 2.45537E-8, 2.51358E-8, 2.22254E-8, 1.97224E-8, 2.29821E-8, 2.26328E-8, 2.10612E-8,
};
unsigned char read_v[132]={0};

unsigned char eepromData[256]= {0xdb,0xdb,0xda,0xdc,0xdd,0xdd,0xde,0xe1,0xdf,0xe0,
		0xe1,0xe0,0xe0,0xe1,0xe1,0xe2,0xe2,0xe2,0xe3,0xe3,0xe4,0xe3,0xe5,0xe4,
		0xe5,0xe5,0xe6,0xe6,0xe8,0xe7,0xe7,0xe9,0xe8,0xe9,0xeb,0xeb,0xeb,0xeb,
		0xeb,0xec,0xec,0xea,0xee,0xee,0xec,0xec,0xed,0xef,0xed,0xee,0xee,0xf2,
		0xee,0xed,0xf0,0xf3,0xed,0xee,0xef,0xf2,0xed,0xee,0xed,0xf1,0x9a,0x9a,
		0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0x9a,0xab,0x9a,0xab,
		0xab,0xab,0xab,0xab,0xab,0xab,0xbc,0xbc,0xab,0xbc,0xbc,0xbc,0xbc,0xbc,
		0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xbc,0xcd,0xbc,0xbc,0xcd,0xbc,
		0xbc,0xbc,0xcd,0xcd,0xde,0xcd,0xbc,0xde,0xcd,0xcd,0xcd,0xde,0xde,0xcd,
		0xde,0xde,0xbc,0xbc,0xbc,0xde,0x00,0x2f,0x39,0x25,0x25,0x5a,0x61,0x43,
		0x4d,0x79,0x7f,0x6b,0x65,0xa4,0xae,0x7f,0x7c,0xbc,0xbf,0x9a,0x86,0xc9,
		0xbf,0xab,0x90,0xd3,0xda,0xb2,0x90,0xd6,0xe0,0xb5,0x97,0xd6,0xda,0xb2,
		0x97,0xd0,0xd6,0xb2,0x89,0xcc,0xcc,0xab,0x86,0xbf,0xc2,0x9d,0x72,0xa7,
		0xb2,0x89,0x6b,0xa1,0x9a,0x6f,0x54,0x7f,0x89,0x57,0x2c,0x64,0x5e,0x43,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xe5,0xe5,0xab,0x32,0x04,0x00,0x09,0x49,0x1a,
		0x00,0x5a,0x83,0x2d,0xb5,0x49,0x28,0x22,0x00,0x80,0x00,0x00,0xff,0x2b,
		0x00,0x16,0x1a,0xbe,0xfa,0x18,0x9d,0x1b,0x49,0x1a,0xff,0x1e,0x74,0x56,
		0xa6,0x20,0x8f,0x43,0x08,0x43,0x00,0x013 };



int main(int argc,char *argv[])
{

	cout<<"Communicating from " << argv[1] << " to " << argv[2] <<endl;

	VideoCapture cap;
    Mat image;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.

	Thermal_Display termografo;
	termografo.gensweep();


	 socket_message thermal(332584,argv[1],132); //Creates a socket port, server, client, buffer size
	 thermal.init_tcp_client_socket();

	 std::cout<<"uno";

    socket_message uno(8705,argv[1],80000); //Creates a socket port, server, client, buffer size
	uno.init_tcp_client_socket();

	varInitialization(eepromData);



    FILE *fp;
    FILE *f2;
    f2=fopen("out_history.raw","a");
    int count = 0;

    int size=120;
  	int h=16*size;
  	int v=4*size;
  	int h0=0;
  	int v0=540-v/2;




    fp=fopen("out.raw","w");
	uno.read_tcp();
	memcpy (  &rsize, uno.buffer+sizeof(double),sizeof(int));

	fwrite(uno.buffer+12,rsize,1,fp);
	fclose(fp);

	system(" ffmpeg -nostats -loglevel 0 -f h264 -i out.raw -r 1 -vcodec copy -y out.mp4 ");	 //Llamada al sistema para eliminar link simbólico

	cap.open("out.mp4");
	cap.read(image);

	imshow( "Display window", image );
	waitKey(1);


  	int Temp[h][v];
  	float TempV[h][4];
  	Mat roi = image(Rect(h0,v0,h,v));
  	Mat color(roi.size(),CV_8UC3, Scalar(200,0,0));


    while(1){

    	thermal.read_tcp();

    	memcpy ( read_v, thermal.buffer,132);
    	ptat = ((unsigned int)(read_v[1] << 8) | read_v[0]); //Combine bytes and return

    		 for(int i=0 ; i<64 ; i++ )
    			 irData[i] = (int16_t)(read_v[2*i+3] << 8) | read_v[2*i+2];

    		 cpix = ( (int16_t)(read_v[131] << 8) | read_v[130]);
    		 calculate_TA();
    		 calculate_TO();

    	     memcpy ( termografo.tdata, temperatures,64*sizeof(float));

    		 termografo.convert();

  /*  		 for(int count = 0 ; count < 64 ; count++)
    		 			{


    		 				cout<<(int)termografo.tdata[count] << "\t";
    		 					if(!((count+1)%16))
    		 					cout<<endl;

    		 			}
				cout<<endl;
					cout<<endl;

    			for(int i = 0 ; i<64 ; i++)
    			std::cout<<termografo.tdata[i]<<"\t";
    			 std::cout<<std::endl<<"**************************************************"<<std::endl;

*/
    		// termografo.tamb=Tambient;

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


        fp=fopen("out.raw","w");
        //freopen("out.raw","w",fp);
		uno.read_tcp();
		memcpy (  &rsize, uno.buffer+sizeof(double),sizeof(int));

		fwrite(uno.buffer+12,rsize,1,fp);
		fclose(fp);

		system(" ffmpeg -nostats -loglevel 0 -f h264 -i out.raw -r 1 -vcodec copy -y out.mp4 ");	 //Llamada al sistema para eliminar link simbólico

		cap.open("out.mp4");
    	cap.read(image);

  		flip(image,image,0);
  		flip(image,image,1);
   		addWeighted(color,0.6,roi,1,0.0,roi);

    	imshow( "Display window", image );
		waitKey( 1);

    	cap.release();
	}
    uno.close_socket();
	 cout<<endl<<"Trans_completa"<< "\n";
	 return 0;

}


//From the 256 bytes of EEPROM data, initialize
void varInitialization(unsigned char calibration_data[])
{
  v_th = (int16_t)(256 * calibration_data[VTH_H] + calibration_data[VTH_L]);
  k_t1 = (int16_t)(256 * calibration_data[KT1_H] + calibration_data[KT1_L])/ 1024.0; //2^10 = 1024
  k_t2 = (int16_t)(256 * calibration_data[KT2_H] + calibration_data[KT2_L])/ 1048576.0; //2^20 = 1,048,576
  emissivity = (256 * calibration_data[CAL_EMIS_H] + calibration_data[CAL_EMIS_L]) / 32768.0;
  a_cp = calibration_data[CAL_ACP];
  if(a_cp > 127) a_cp -= 256; //These values are stored as 2's compliment. This coverts it if necessary.
  b_cp = calibration_data[CAL_BCP];
  if(b_cp > 127) b_cp -= 256;
  tgc = calibration_data[CAL_TGC];
  if(tgc > 127) tgc -= 256;
  b_i_scale = calibration_data[CAL_BI_SCALE];
  for(int i = 0 ; i < 64 ; i++)
  {
    //Read the individual pixel offsets
    a_ij[i] = calibration_data[i];
    if(a_ij[i] > 127) a_ij[i] -= 256; //These values are stored as 2's compliment. This coverts it if necessary.
    //Read the individual pixel offset slope coefficients
    b_ij[i] = calibration_data[0x40 + i]; //Bi(i,j) begins 64 bytes into EEPROM at 0x40
    if(b_ij[i] > 127) b_ij[i] -= 256;
  }
}

//Gets the latest PTAT (package temperature ambient) reading from the MLX
//Then calculates a new Tambient
//Many of these values (k_t1, v_th, etc) come from varInitialization and EEPROM reading
//This has been tested to match example 7.3.2

void calculate_TA(void)
{
	Tambient = (-k_t1 + sqrt(pow(k_t1,2) - (4 * k_t2 * (v_th - (float)ptat)))) / (2*k_t2) + 25; //it's much more simple now, isn't it? :)
}


//Calculate the temperatures seen for each pixel
//Relies on the raw irData array
//Returns an 64-int array called temperatures
void calculate_TO()
{
  float v_ir_off_comp;
  float v_ir_tgc_comp;
  float v_ir_comp;


  //Calculate the offset compensation for the one compensation pixel
  //This is a constant in the TO calculation, so calculate it here.
  float v_cp_off_comp = (float)cpix - (a_cp + (b_cp/pow(2, b_i_scale)) * (Tambient - 25));

  for (int i = 0 ; i < 64 ; i++)
  {
    v_ir_off_comp = irData[i] - (a_ij[i] + (float)(b_ij[i]/pow(2, b_i_scale)) * (Tambient - 25)); //#1: Calculate Offset Compensation
    v_ir_tgc_comp = v_ir_off_comp - ( ((float)tgc/32) * v_cp_off_comp); //#2: Calculate Thermal Gradien Compensation (TGC)
    v_ir_comp = v_ir_tgc_comp / emissivity; //#3: Calculate Emissivity Compensation
    temperatures[i] = sqrt( sqrt( (v_ir_comp/alpha_ij[i]) + pow(Tambient + 273.15, 4) )) - 273.15;
  }
}


