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
#include "threadforks.h"

using namespace threads;
using namespace std;

int ping(int dev);

int main(){
	/*int val=0;
	int nrobots=1;
	for(int i=nrobots+1; i>0 ;i--)
	{
		val=ping(i);
		cout<<"Ping to 192.168.0."<<i<< " = "<<val<<endl;
		if(val)break;
	}

	cout <<"end";*/


	threadforks G(5);
	G.cmd[0]= "/root/code/BBB_Joystick";
	G.cmd[1]= "/root/code/BBB_PWM";
	G.cmd[2]= "/root/code/BBB_UART";
	G.cmd[3]= "/root/code/BBB_UART2";
	G.cmd[4]= "/root/code/Video";
	G.inittf();


return 0;

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
	ss<<"192.168.0.";
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
