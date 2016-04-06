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

int ping(int dev, int dev_s);

int main(){



	/*Handshake whit workstation and IP assignation*/
	socket_message hShake_send(225421,"192.168.7.1",1);
	hShake_send.init_udp_sender_socket();
	socket_message hShake_rec(225422,"192.168.7.2",1);
	hShake_rec.init_udp_receiver_socket();

	hShake_rec.read_udp();
	cout<<(int)hShake_rec.buffer[0]<<endl;
	hShake_send.buffer[0]=hShake_rec.buffer[0];

	if(hShake_rec.buffer[0]!=2)
	{

	while(	hShake_send.buffer[0]==hShake_rec.buffer[0]){
	hShake_send.write_udp();
	hShake_rec.read_udp(0,1);
	}
	}
	else
	{
		usleep(10000);
		for(int i = 0; i < 10 ; i++ )
		{
		hShake_send.write_udp();
		}
	}

	hShake_rec.close_socket();
	hShake_send.close_socket();

	/*Assigned IP Configuration*/
	stringstream ss;
	ss<<"ip_assign.sh "<< (int)hShake_send.buffer[0]<<" y";
	const char *ip = ss.str().c_str();
	cout<<ip;
	system(ip);



	return 0;




	socket_message handshake(225425,"192.168.7.1",1);
	handshake.init_tcp_client_socket();

	handshake.read_tcp();
	cout<<handshake.buffer[0];

	handshake.buffer[0]='0';

	cout<<handshake.buffer[0];

	/*MODE 0 : Looking for workstation*/
	//while(!ping(7,1));	//wait to ping workstation





	cout << "end"<<endl;

	return 0;

	threadforks G(3);
	G.cmd[0]= "/root/exec_BBB/BBB_Joystick";
	G.cmd[1]= "/root/exec_BBB/BBB_PWM";
	G.cmd[2]= "/root/exec_BBB/BBB_UART";
	G.inittf();


return 0;

}


int ping(int dev , int dev_s)
{
	stringstream current;
	char buffer[50];
	char c[20];

	stringstream ss;
	ss<<"192.168."<<dev<<"."<<dev_s;
	const char *ip = ss.str().c_str();
	sprintf(buffer,"ping -c 3 %s | grep -c ms", ip);
	FILE *p = popen(buffer,"r");
	fgets(c,5,p);
	pclose(p);

	if(strcmp(c,"5\n")==0){
		return 1;
	}
	else
		return 0;


}
