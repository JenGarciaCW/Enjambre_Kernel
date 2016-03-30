#include <core/core.hpp>
#include <cv.h>
#include <iostream>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

int main( int argc, char** argv )
{

	cv::VideoCapture vcap(0);
      if(!vcap.isOpened())
      {
    	  std::cout << "Not cameras found" << std::endl;
    	  return -1;

      }


      vcap.set(cv::CAP_PROP_FRAME_WIDTH,1920);
      vcap.set(cv::CAP_PROP_FRAME_HEIGHT,1080);

      cv::Mat image= cv::Mat(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));//240*180

      int imgsize=image.step*image.rows;

      std::vector<cv::Mat> planes;
      int x=200;
      std::vector<cv::Vec3f> circles;
      double res,max=0;

      std::vector<cv::Vec3f>::const_iterator itc;

      char c;


  while(1)
  {
    // cv::Mat image = cv::imread("/home/kevin/Pictures/Webcam/2015-07-23-171929.jpg");
	vcap>>image;
    image=image-cv::Scalar(x,x,x);



    cv::GaussianBlur(image,image,cv::Size(5,5),1);
    cv::split(image,planes);

    cv::HoughCircles(planes[2],circles,CV_HOUGH_GRADIENT,1,1,80,30,1,50);

    for(int i=0;i<circles.size();i++)
    	for(int j=i+1;j<circles.size();j++)
    	{
    		res = cv::sqrt(cv::pow((circles[j][0]-circles[i][0]),2)+cv::pow((circles[j][1]-circles[i][1]),2));
    		//std::cout<<res<<"\t"<<i<<"\t"<<j<<std::endl;
    		if (res>max) max=res;

    	}


    itc= circles.begin();
    while (itc!=circles.end()) {
    cv::circle(planes[2],cv::Point((*itc)[0], (*itc)[1]), // circle centre
    (*itc)[2],
    // circle radius
    cv::Scalar(255), // color
    2);
    // thickness
    std::cout<<(*itc)[0]<< "\t"<< (*itc)[1]<< "\t"<< (*itc)[2]<<std::endl;
    ++itc;
    }

    cv::imshow("image",planes[2]);
    c = cv::waitKey(33);
    if( c == 27 ) break;
    max=0;
  }
}
