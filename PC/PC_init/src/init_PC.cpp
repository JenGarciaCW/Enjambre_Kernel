#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <sys/types.h>
#include "threadforks.cpp"
#include "socketmessage.cpp"

using namespace socket_msg;
using namespace threads;
using namespace std;

void handShake(unsigned char); //prototype function initial communication
int ping(int dev_s); //ping a device with dev_s as the last digit of the ip


socket_message hShake_rec(2251,"192.168.0.1",1);
socket_message hShake_send(2252,"192.168.0.2",1);

//int ping(int dev);



int main(int argc, char *argv[]){
	stringstream ss;
	char *ip;


	//receives the number of robots as an argument

	/*Initialize sockets for handshake*/
	hShake_rec.init_udp_receiver_socket();
	hShake_send.init_udp_sender_socket();

	int n_robots =  atoi(argv[1]);
	stringstream ss_temp;
	threadforks exec_PC[n_robots];
	double sec, usec;	//variables para cálculo de tiempo
	int i=atoi(argv[2]);
	//for(int i=0 ; i<n_robots ;i++)
	//{

		/*MODE 0 : Handshake and connection stablishment*/
		handShake(n_robots+1-i);
		while(!ping(n_robots+1-i));
		usleep(1*1000*1000);



		ss.str("");
		ss<<"192.168.0."<<to_string(n_robots+1-i);
		ip = (char*)ss.str().c_str();
		cout<<ip<<" "<<2254+n_robots+1-i<<endl;
		socket_message time(2254+n_robots+1-i , ip ,2*sizeof(double) );
		time.init_tcp_client_socket();
		time.read_tcp();
		memcpy(&sec,&*(time.buffer),sizeof(double));
		memcpy(&usec,&*(time.buffer+sizeof(double)),sizeof(double));
		cout<<endl<<sec+usec<<endl;
	//}
		usleep(5000000);

		/*MODE 1 : Running programs comunicating with workstation*/
		exec_PC[i].init_tf(2);

		exec_PC[i].cmd[0]= "/home/kevin/workspace/Enjambre/PC_Bridge_Cam_R/Debug/PC_Bridge_Cam_R";
		exec_PC[i].cmd[1]= "/home/kevin/workspace/Enjambre/PC_Joystick/Debug/PC_Joystick";
		ss_temp<<"192.168.0."<<to_string(n_robots+1-i);
		exec_PC[i].from_conn =(char *)ss_temp.str().c_str();
		exec_PC[i].to_conn = "192.168.0.1";
		exec_PC[i].inittf();

		while(!ping(2));
		exec_PC[i].kill_threads();


	//    pthread_exit(NULL); // close current thread

	return 0;
}

void handShake(unsigned char n){
	hShake_rec.buffer[0]=0;
	hShake_send.buffer[0]=n;

	while(hShake_rec.buffer[0]!=n)
		{
			hShake_send.write_udp();
			hShake_rec.read_udp(0,1);
		}

	hShake_send.buffer[0]=n-1;
	for(int rep = 0 ; rep < 10 ; rep++)
		hShake_send.write_udp();
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
