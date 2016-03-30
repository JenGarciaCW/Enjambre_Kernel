// ----------------------------------------------------------------------------
/**
   File: OCV_S.cpp

   System:	Robot de enjambre para exploraci������n en en estructuras colapsadas
   Component Name: Video
   Status:         Version 0.1
   Language: C++

   License: GNU Public License

   Address:
      AV. UNIVERSIDAD N������ 3000,
      UNIVERSIDAD NACIONAL AUT������NOMA DE M������XICO, C.U.,
      DISTRITO FEDERAL, 04510

   Author: Kevin Omar Arias Garc������a
   E-Mail: kevinoag@gmail.com

   Description: This code beside sSocket.cpp and sSocket.h
   are intended to stream and manage video captured remotely.

   Limitations: You must have installed OpenCV2 libraries

   Function: 1) bla bla bla
             2) bla bla bla
             3) bla bla bla


   Platform Dependencies: Linux/Intel

   Compiler Options: -lm

   Revisions:
   Date         Author       	Description
   2014.02.05   KevinArias	 	Added dynamic video transfer adjustable to user defined resolution
**/
// ----------------------------------------------------------------------------

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "socketmessage.cpp"


using namespace std;
using namespace cv;
using namespace socket_msg;


int main(){

/*Video Capture Configuration*/

	VideoCapture vcap(-1);			//Crea objeto para lectura de camara disponible
      if(!vcap.isOpened())
      {
    	  cout << "Not cameras found" << endl;
    	  return -1;
      }

      vcap.set(CAP_PROP_FRAME_WIDTH,640);
      vcap.set(CAP_PROP_FRAME_HEIGHT,480);

/* OpenCV processsing information */

      Mat frame; //Matrix size, 8 bit 3 colors, begins in zero
      vcap>>frame;

      /*Connection configuration*/
            int imgsize=frame.step*frame.rows;						//
            int envios=120;
            while(imgsize%envios) envios--; //reduce numero de envios hasta que sea divisible el tamano de imagen sin residuo
            int sizeBuff=imgsize/envios; //define el tamano del buffer
            cout<<imgsize<<" "<<envios<<" "<<sizeBuff<<endl;


/*Connection configuration*/
      socket_message BBBsock(23222,"192.168.0.1",sizeBuff); //Creates a socket port, server, client, buffer size
      BBBsock.init_udp_sender_socket(); // initialize udp server socket









/**/
      while(1)
      {
    	usleep(500000);
   	  	vcap >> frame;

   	    for(int i=0;i<envios;i++)
   	    {
   	    	usleep(10);
		memcpy(&(*(BBBsock.buffer))+1,&(*(frame.data+i*sizeBuff)),sizeBuff);
   	  	BBBsock.buffer[0]=(uchar)i;
   	  	BBBsock.write_udp();
   	    }
      }

  return 0;
}
