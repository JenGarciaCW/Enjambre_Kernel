#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sstream>
#include "threadforks.cpp"
#include "socketmessage.cpp"

using namespace socket_msg;
using namespace threads;
using namespace std;


socket_message hShake_rec(2252,"192.168.0.2",1);
socket_message hShake_send(2251,"192.168.0.1",1);


int main(){

	/*Handshake whit workstation and IP assignation*/
	hShake_send.init_udp_sender_socket();
	hShake_rec.init_udp_receiver_socket();

	stringstream ss_temp;
	int curr_ip;

	hShake_rec.read_udp();
	hShake_send.buffer[0]=hShake_rec.buffer[0];

	while(hShake_send.buffer[0]==hShake_rec.buffer[0] )
	{
			hShake_send.write_udp();
			hShake_rec.read_udp(0,1);
			if(hShake_rec.buffer[0]==2)
				break;
	}

	curr_ip=(int)hShake_send.buffer[0];


	/*Assigned IP Configuration*/
	stringstream ss;
	ss<<"ip_assign.sh "<< to_string(curr_ip)<<" y";
	const char *ip = ss.str().c_str();
	system(ip);



	while(1);
return 0;

}

