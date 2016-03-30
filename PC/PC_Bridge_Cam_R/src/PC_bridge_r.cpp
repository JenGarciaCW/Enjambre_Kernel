#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include "socketmessage.cpp"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libavformat/avformat.h"

using namespace cv;
using namespace std;
using namespace socket_msg;

char *size_c  ;
double ttime=0.01;
int	tsize = 2320;

double rtime = 0;
int		rsize =0;
void clear_file(const char *filename);


int main(int argc,char *argv[])
{

	/*VideoCapture cap("/home/kevin/workspace/Enjambre/PC_Bridge_Cam_R/Debug/out.mp4"); //
    if (!cap.isOpened())  // if not success, exit program
     {
         cout << "Cannot open the video cam" << endl;
         return -1;
     }*/

	VideoCapture cap;
    Mat image;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.




	/*Connection configuration
	      uno.init_udp_sender_socket(); // initialize udp server socket

	uno.write_udp();*/
	//capture_main( data_c, size_c);
	//cout<<*size_c<<"dos";
    //socket_message uno(23322,"192.168.0.2",40000); //Creates a socket port, server, client, buffer size
    socket_message uno(8705,"192.168.0.2",80000); //Creates a socket port, server, client, buffer size

	uno.init_tcp_client_socket();


    FILE *fp;
    FILE *f2;
    f2=fopen("out_history.raw","a");
	//fprintf(stderr, "Force Format %d\n", 2);
    int count = 0;


    while(1){

        fp=fopen("out.raw","w");
		uno.read_tcp();
		memcpy (  &rsize, uno.buffer+sizeof(double),sizeof(int));

		//clear_file("out_history.raw");
		fwrite(uno.buffer+12,rsize,1,fp);
		//fwrite(uno.buffer+12,rsize,1,f2);
		fclose(fp);

		system(" ffmpeg -nostats -loglevel 0 -f h264 -i out.raw -r 1 -vcodec copy -y out.mp4 ");	 //Llamada al sistema para eliminar link simbólico

		cap.open("/home/kevin/workspace/Enjambre/PC_Bridge_Cam_R/Debug/out.mp4");
    	cap.read(image);

    	imshow( "Display window", image );
		waitKey( 10);

    	cap.release();

		//system(" rm out.mp4 ");	 //Llamada al sistema para eliminar link simbólico


		/*  fwrite(uno.buffer+12,rsize,1,stdout);
		fflush(stderr);
		fprintf(stderr, ".");
		fflush(stdout);*/

		//fwrite(p, size, 1, stdout);


	    /*if(image.empty() )                              // Check for invalid input
	    {
	        cout <<  "Could not open or find the image" << std::endl ;
	        return -1;
	    }
	    imshow( "Display window", image );

		// Show our image inside it.

		cap.read(image);
		waitKey( 10);
*/
	}
    uno.close_socket();
	 cout<<endl<<"Trans_completa"<< "\n";
	 return 0;

}


