/*
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "socketmessage.h"


using namespace std;
using namespace socket_msg;


int main () {
	  unlink("/root/messPWM");

	  unlink("/root/messUART");

  socket_message JoySock(2211122,"192.168.0.2",4);
  JoySock.init_udp_receiver_socket();


  socket_message JoyPWM("/root/messPWM",2);
  JoyPWM.init_unix_server_socket();

  socket_message JoyUART("/root/messUART",2);
  JoyUART.init_unix_server_socket();

  while(1)
  {
  JoySock.read_udp();

  	cout<<(int)JoySock.buffer[0]<<endl;
  JoyPWM.buffer[0]=JoySock.buffer[0];
  JoyPWM.buffer[1]=JoySock.buffer[1];
  JoyPWM.write_unix();

  JoyUART.buffer[0]=JoySock.buffer[2];
  JoyUART.buffer[1]=JoySock.buffer[3];
  JoyUART.write_unix();


  }
  return 0;
}
