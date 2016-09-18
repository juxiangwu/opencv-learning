//StereoGrabber.h is the header file which contains the definition 
//for variables and functions used to obtain images for processing.
#ifndef __STEREO_GRABBER_H__
#define __STEREO_GRABBER_H__
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#define WIDTH 352
#define HEIGHT 288
struct StereoGrabber{
	
	void stereoGrabberInitFrames();
	void stereoGrabberGrabFrames();	//grab a frame from the stream
	void stereoGrabberStopCam();
	IplImage* imageLeft;
	IplImage* imageRight;
};

#endif
