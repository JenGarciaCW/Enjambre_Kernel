#include <iostream>
#include <cstdlib>
#include "threadforks.h"

using namespace threads;
using namespace std;

//int ping(int dev);

int main(int argc, char *argv[]){
	/*int val=0;
	int nrobots=1;
	for(int i=nrobots+1; i>0 ;i--)
	{
		val=ping(i);
		cout<<"Ping to 192.168.0."<<i<< " = "<<val<<endl;
		if(val)break;
	}

	cout <<"end";*/


	threadforks G(2);
	G.from_conn = argv[1] ;
	G.to_conn = argv[2];
	G.cmd[0]= "/home/kevin/workspace/Enjambre/PC_Joystick/Debug/PC_Joystick";
	G.cmd[1]= "/home/kevin/workspace/Enjambre/PC_Bridge_Cam_R/Debug/PC_Bridge_Cam_R";
	G.inittf();

return 0;

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
