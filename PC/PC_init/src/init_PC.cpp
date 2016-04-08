#include <iostream>
#include <cstdlib>
#include "threadforks.cpp"
#include "socketmessage.cpp"

using namespace socket_msg;
using namespace threads;
using namespace std;

void handShake(unsigned char);
int ping(int dev_s);


socket_message hShake_rec(2251,"192.168.0.1",1);
socket_message hShake_send(2252,"192.168.0.2",1);

//int ping(int dev);



int main(int argc, char *argv[]){
	//receives the number of robots as an argument

	hShake_rec.init_udp_receiver_socket();
	hShake_send.init_udp_sender_socket();
	int n_robots =  atoi(argv[1]);
	stringstream ss_temp;
	threadforks exec_PC[n_robots];
	int i = 0;
	//for(int i=0 ; i<n_robots ;i++)
	//{
		/*MODE 0 : Handshake and connection stablishment*/
		handShake(n_robots+1-i);
		while(!ping(n_robots+1-i));
		usleep(4*1000*1000);

		/*MODE 1 : Running programs comunicating with workstation*/
		exec_PC[i].init_tf(1);

		exec_PC[i].cmd[0]= "/home/kevin/workspace/Enjambre/PC_Bridge_Cam_R/Debug/PC_Bridge_Cam_R";
		ss_temp<<"192.168.0."<<to_string(n_robots+1-i);
		exec_PC[i].from_conn =(char *)ss_temp.str().c_str();
		exec_PC[i].to_conn = "192.168.0.1";
		exec_PC[i].inittf();

	//}

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

	stringstream ss;
	ss<<"192.168.0."<<to_string(dev_s);
	const char *ip = ss.str().c_str();
	sprintf(buffer,"ping -c 3 %s | grep -c ms", ip);
	FILE *p = popen(buffer,"r");
	fgets(c,5,p);
	pclose(p);
	if(strcmp(c,"5\n")==0)
		return 1;
	else
		return 0;


}
