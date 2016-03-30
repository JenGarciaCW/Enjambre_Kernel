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

using namespace threads;
using namespace std;

int ping(int dev, int dev_s);

int main(){

	/*MODE 0 : Looking for workstation*/
	while(!ping(7,1));




	cout <<"end"<<endl;

	return 0;

	threadforks G(5);
	G.cmd[0]= "/root/code/BBB_Joystick";
	G.cmd[1]= "/root/code/BBB_PWM";
	G.cmd[2]= "/root/code/BBB_UART";
	G.cmd[3]= "/root/code/BBB_UART2";
	G.cmd[4]= "/root/code/Video";
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
