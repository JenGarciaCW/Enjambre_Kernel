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
#include <sys/time.h>
#include <sys/types.h>
#include "threadforks.cpp"
#include "socketmessage.cpp"

using namespace socket_msg;
using namespace threads;
using namespace std;


socket_message hShake_rec(2252,"192.168.0.2",1);
socket_message hShake_send(2251,"192.168.0.1",1);

int ping(int dev_s); //ping a device with dev_s as the last digit of the ip


int  main(){

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

	ss.str("");
	ss<<"192.168.0."<<to_string(curr_ip);
	ip = ss.str().c_str();
	cout<<ip<<endl;
	socket_message time(2254+curr_ip , ip ,2*sizeof(double) );
	time.init_tcp_server_socket();

	struct timeval tp;	//estructura de tiempo que contiene segundos y microsegundos
	double sec, usec;	//variables para cálculo de tiempo


	gettimeofday( &tp, NULL ); //Almacena dato de tiempo en tp



	sec = static_cast<double>( tp.tv_sec ); //obtiene tiempo en segundos
	usec = static_cast<double>( tp.tv_usec )/1E6;//obtiene fracción microsecs
	memcpy(&*(time.buffer),&sec,sizeof(double));
	memcpy(&*(time.buffer+sizeof(double)),&usec,sizeof(double));
	//cout<<endl<<sec+usec<<endl;


	time.write_tcp();
	time.close_socket();


	threadforks G(4);
	G.from_conn =(char *)ss.str().c_str();
	G.to_conn = "192.168.0.1";
	G.cmd[0]= "/root/exec_BBB/BBB_bridge_s_2";
	G.cmd[1]= "/root/exec_BBB/BBB_Joystick";
	G.cmd[2]= "/root/exec_BBB/BBB_PWM";
	G.cmd[3]= "/root/exec_BBB/BBB_UART";
	G.inittf();

	while(!ping(curr_ip-1));
	G.kill_threads();


return 0;

}

int ping(int dev_s)
{
	stringstream current;
	char buffer[50];
	char c[20];

	stringstream sd;
	sd<<"192.168.0."<<to_string(dev_s);
	const char *ip = sd.str().c_str();
	sprintf(buffer,"ping -c 3 %s | grep -c ms", ip);
	FILE *p = popen(buffer,"r");
	fgets(c,5,p);
	pclose(p);
	if(strcmp(c,"5\n")==0)
		return 1;
	else
		return 0;
}

