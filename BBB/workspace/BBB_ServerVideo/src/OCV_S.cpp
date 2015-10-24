#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <iostream>
#include "sSocket.h"

using namespace std;
using namespace cv;

int main(){


	ServerSocket::sSocket PCsock1(33338);

    VideoCapture vcap(1);
      if(!vcap.isOpened()){
             cout << "Error opening video stream or file" << endl;
             return -1;
      }
      vcap.set(CV_CAP_PROP_FRAME_WIDTH,640);
      vcap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
      Mat frame= Mat(Size(640, 480), CV_8UC3, Scalar(0, 0, 0));
       vcap >> frame;

       PCsock1.initss();

     	namedWindow( "Display2");// Create a window for display.
		 int  imgSize = frame.cols*frame.rows*frame.channels();
		   cout<<imgSize<<endl;


   	while(1)
   	{
   	  	vcap >> frame;

		imshow( "Display2", frame );
		waitKey(1);
		PCsock1.buffer[0]='E';
		PCsock1.buffer[1]='n';
		PCsock1.buffer[2]='J';
		PCsock1.buffer[3]='a';
		PCsock1.buffer[4]='M';
		PCsock1.buffer[5]='b';
		PCsock1.buffer[6]='R';
		PCsock1.buffer[7]='e';
		PCsock1.buffer[8]='E';
		PCsock1.buffer[9]='4';
		PCsock1.buffer[10]='1';
		PCsock1.buffer[11]='0';
		PCsock1.buffer[12]='0';
		PCsock1.buffer[13]='U';
		PCsock1.buffer[14]='N';
		PCsock1.buffer[15]='A';
		PCsock1.buffer[16]='M';





		for(int i=0; i<921600  ; i++)
		{
			*(PCsock1.buffer+i+20)=(uchar)*(frame.data+i);
		}

		PCsock1.writess();
	}


    PCsock1.closess();
  return 0;
}
