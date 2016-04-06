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

void Thermal_Display::convert()
{

	float tdata_correct[64];

			for(int count = 0 ; count < 64 ; count++)
			{

				if(count==0)
					this->tmax=this->tmin=this->tdata[count];
				else
				{
					if(this->tdata[count]>this->tmax)
						this->tmax=this->tdata[count];
					if(this->tdata[count]<this->tmin)
						this->tmin=this->tdata[count];
				}

				/*cout<<(int)this->tdata[count] << "\t";
					if(!((count+1)%4))
					cout<<endl;*/

			}


		this->tamb=this->tmin;

		int index=0;
		for(int i_corr=15 ; i_corr>=0 ;i_corr--)
		{
			for(int counter=0; counter<4 ; counter++)
			{
				tdata_correct[i_corr+counter*(16)]=this->tdata[index];
				index++;
			}
		}
/*
		for(int reng = 0 ; reng < 4 ; reng++)
			for(int col = 0 ; col < 16 ; col++)
			{
				tdata_correct[reng*16+col]=this->tdata[reng+col*4];
			}

*/
		copy(begin(tdata_correct),end(tdata_correct),begin(this->tdata));
		//this->tdata=tdata_correct;

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
