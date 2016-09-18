#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <vector>

using namespace cv;
using namespace std;

int main() {

  cv::VideoCapture cap(0);
  if(!cap.isOpened()){
      std::cerr << "cannot open camera \n";
      return 0;
  }
  Mat frame, background, foreground ;
  Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2(50,100,false); //bg(50,100,false);

  bg->setNMixtures(3);

  vector<vector<Point> > contours;

  char key = -1;
  while(true){
      cap >> frame;
      cv::Mat contoursFrame = frame.clone();
      if(frame.empty()){
          std::cerr << "captured empty frame \n";
          continue;
      }

      bg->apply(frame,foreground);

      //bg.operator()(frame,foreground);
      bg->getBackgroundImage(background);
      erode(foreground,foreground,Mat());
      dilate(foreground,foreground,Mat());

      findContours(foreground,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
      drawContours(contoursFrame,contours,-1,Scalar(0,255,0),2);

      cv::imshow("src",frame);
      cv::imshow("foreground",foreground);
      cv::imshow("background",background);
      cv::imshow("contours",contoursFrame);

      cv::Scalar value = cv::sum(foreground);
      std::cout << "foreground sum value:" << value[0] << std::endl;
      if(value[0] > 500){
          cv::imwrite("capture.png",frame);
          std::cout << "something or someone invating\n";
      }
      key = cv::waitKey(10);
      if(key == 27){
          break;
      }
  }


    return 0;
}
