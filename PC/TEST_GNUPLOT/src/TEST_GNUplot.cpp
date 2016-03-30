/*#include <stdio.h>

double func ( double x ) {
    if ( x < 0 )
        return 0 ;
    if ( x < 2 )
        return x * x ;

    return 16.0 / ( x * x ) ;
}


 * output will be in 2 coloumns
 * 1 column contains x-axis values
 * 2 column contains y-axis {or f(x) } values corresponding to x-value

int main ( ) {
    double x = 0.0;     // initial value
    double x_max = 15.0;
    double step = 0.01;

    printf ( "# x \t f(x) \n" );

    while ( x <= x_max ) {
        printf ( "%.3f \t %.3f \n", x, func(x) );
        x += step;
    }

    return 0;
}*/


// fstream::open / fstream::close
#include <fstream>      // std::fstream
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
int main () {

	int x=0;
	int limsup=50 , liminf=0;
	double y=0, z=0;
	unlink("./test2.txt");
  fstream fs;
  fstream fs2;

  fs.open ("test2.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

  fs2.open ("config", std::fstream::in | std::fstream::out | std::fstream::trunc);




  struct timespec  tim;


      tim.tv_sec = 0;        /* seconds */
      tim.tv_nsec = 50000000L;       /* nanoseconds */





  while(1)
  {
	x++;
	y=sin(x);
	z=1-y;
  cout<<"hola"<<endl;
  nanosleep(&tim, &tim);

  fs.flush();
  fs <<x<<"\t"<<y<<"\t"<<z<<"\n";

  fs2.flush();

  if(x>50)
  {
	  limsup=x;
  	  liminf=x-50;
  }
  else
  {
	  limsup=50;
	  liminf=0;
  }

  fs2 <<"set xrange ["<<liminf<<":"<<limsup<<"];";

  }
  fs.close();

  return 0;
}

