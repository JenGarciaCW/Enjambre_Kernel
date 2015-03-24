// ----------------------------------------------------------------------------
/**
   File: OCV_S.cpp

   System:	Robot de enjambre para exploración en en estructuras colapsadas
   Component Name: Video
   Status:         Version 0.1
   Language: C++

   License: GNU Public License

   Address:
      AV. UNIVERSIDAD Nº 3000,
      UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO, C.U.,
      DISTRITO FEDERAL, 04510

   Author: Kevin Omar Arias García
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

//#include "opencv2/opencv.hpp"
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      //
#include "v2_0/BlackUART.h"
#include "v2_0/BlackCore.h"
#include "v2_0/BlackCore.cpp"
#include "v2_0/BlackUART.cpp"
#include "socketmessage.h"




using namespace BlackLib;
using namespace socket_msg;

int main(){

	BlackUART motor1(UART4,Baud38400,ParityNo,StopOne,Char8 );
	motor1.open( ReadWrite | NonBlock );


	std::string s="a";
	std::stringstream out;
	int value;

	out << (unsigned char)192;
	std::cout<<out<<std::endl;
	s = out.str();
	motor1.write(s);


      int sizeBuff=2;


/*Video Capture Configuration*/
      socket_message BBBsock(23222,"192.168.0.137",sizeBuff); //address receiver
      BBBsock.init_udp_receiver_socket();

/**/
      while(1)
      {
    	  BBBsock.read_udp();

    	  value=int(((int)BBBsock.buffer[0])/2.0+1);
    	  if(value>127)
		  value=127;

    	  std::cout<<value<<std::endl;
    	  out.str(std::string());
    	  out << (unsigned char)value && 0x0FF;
    	  std::cout<<out<<std::endl;

    	  s = out.str();

  		motor1.write(s);

  		 value=(int)(BBBsock.buffer[1]/2.+129);
  		if(value<128)
  				  value=128;
  		if(value>255)
  			value=255;
  	  std::cout<<value<<std::endl;
  	  out.str(std::string());
  	  out << (unsigned char)value && 0x0FF;
  	  std::cout<<out<<std::endl;

  	  s = out.str();

		motor1.write(s);


      }
    BBBsock.close_socket();
  return 0;
}
