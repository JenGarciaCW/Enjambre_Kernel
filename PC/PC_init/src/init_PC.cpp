#include <iostream>
#include <cstdlib>
#include "threadforks.cpp"
#include "socketmessage.cpp"

using namespace socket_msg;
using namespace threads;
using namespace std;


socket_message hShake_rec(225421,"192.168.7.1",1);
socket_message hShake_send(225422,"192.168.7.2",1);



void send_n(int n);
//int ping(int dev);

int main(){

	/*MODE 0 : Looking for workstation*/


	int n_robots = 5;

	hShake_rec.init_udp_receiver_socket();
	hShake_send.init_udp_sender_socket();

		send_n(n_robots);





	threadforks G(2);
	G.cmd[0]= "/home/kevin/workspace/Enjambre/TEMPLATE_PC/Debug/TEMPLATE_PC";
	G.cmd[1]= "/home/kevin/workspace/Enjambre/TEMPLATE_PC/Debug/TEMPLATE_PC";

	G.inittf();


			usleep(5.50*1000*1000);
			cout<<"\t\t output status "<<WEXITSTATUS(G.status[0])<<endl;
			G.kill_threads();

			usleep(2.50*1000*1000);
			G.restart_threads();
			G.inittf();

		    pthread_exit(NULL); // close current thread


			return 0;

			threadforks S[3];
			S[1].init_tf(2);

			n_robots--;
			send_n(n_robots);


			hShake_rec.close_socket();
			hShake_send.close_socket();


}
void send_n(int n)
{
	hShake_send.buffer[0]=n;

		if(hShake_send.buffer[0]!=1)
		{
		while(hShake_rec.buffer[0]!=n)
		{
		hShake_send.write_udp();
		hShake_rec.read_udp(0,1);
		}
		}
		else
			hShake_rec.read_udp();

}

/*

int val=0;
int nrobots=5;
for(int i=nrobots+1; i>0 ;i--)
{
	val=ping(i);
	cout<<"Ping to 192.168.0."<<i<< " = "<<val<<endl;
	if(val)break;
}

int ping(int dev)
{
	stringstream current;
	char buffer[50];
	char c[20];

	sprintf(buffer,
		"ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'");
	FILE *x = popen(buffer,"r");
	fgets(c,20,x);
	pclose(x);
	cout<<strcmp(c,"192.168.100.4\n")<<endl;

	cout<<"current ip: "<<c<< endl;

	stringstream ss;
	ss<<"192.168.100.";
	ss<< dev;
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
*/
