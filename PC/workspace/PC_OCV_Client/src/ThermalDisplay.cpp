/*
 * ThermalDisplay.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: kevin
 */

#include "ThermalDisplay.h"
using namespace std;
namespace thermal {

void Thermal_Display::gensweep()
{
	for(int i=0; i<255 ; i++) 		//Color sweep from Blue to Red
	{
		this->sweep[i][0]=255-i;	//Blue
		this->sweep[i][1]=0;		//Green
		this->sweep[i][2]=i;		//Red
	}
	for(int i=255; i<512 ; i++)		//Color sweep from Red to Yellow
	{
		this->sweep[i][0]=0;		//Blue
		this->sweep[i][1]=i-255;	//Gren
		this->sweep[i][2]=255;		//Red
	}
}

void Thermal_Display::convert(char *indata)
{

	string irdata;
	int ircount=0;

	if(indata[ircount]=='B')
			for(int count = 0 ; count < 65 ; count++)
			{
				ircount++;


				while(indata[ircount]!=',')
				{
					irdata.append(1u,indata[ircount]);
					ircount++;
				}


				this->tdata[count]=atof(irdata.c_str());

				if(count==0)
					this->tmax=this->tmin=this->tdata[count];
				else
				{
					if(this->tdata[count]>this->tmax)
						this->tmax=this->tdata[count];
					if(this->tdata[count]<this->tmin)
						this->tmin=this->tdata[count];
				}

				irdata.clear();
				cout<<this->tdata[count]<<endl;
			}
		this->tamb = this->tdata[64];

}

float Thermal_Display::tempcalc(float To,float Tf, int ro, int rf,int r)
{
	return ((Tf-To)/(rf-ro))*(r-ro)+To;
}

int Thermal_Display::colorcalc(float To,float Tf, int ro, int rf,int r)
{
	return round((((Tf-To)/(rf-ro))*(r-ro)+To-this->tmin)*512/(this->tmax-this->tmin));

}

Thermal_Display::Thermal_Display() {
	// TODO Auto-generated constructor stub

}

Thermal_Display::~Thermal_Display() {
	// TODO Auto-generated destructor stub
}

} /* namespace thermal */
