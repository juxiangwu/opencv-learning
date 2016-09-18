// Project.cpp : Defines the entry point for the console application.
//The StereoMain.cpp file defines the entry point for the stereo vision system tools.
//From the main, the stereo functions for rectification, correlation, and re-projection are called.

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include "StereoGrabber.h"
#include "StereoFunctions.h"
using std::string;
using std::vector;
using namespace std;

#define CALIBRATION 0

#define SHOW_DATA 1	//show and save intermediate results from stereo correlation,
                    //set this to 0 when just producing a point cloud
#define SAVE_DATA 0
#define SHOW_DISTANCE 0 //output point cloud in format to read into dylan's or dan's point cloud viewer
#define SAVE_DISTANCE 1	//saves depth information of each pixel in a seperate file

#define ANALYSIS_MODE 1	//set to 1 to enter analysis mode, where intermediate results are shown
                        //and a correlation variable tool can be used
                        //extra data can still be saved, but is done manually through a button on the tool
#define PI	3.141592654

StereoGrabber* grab = new StereoGrabber();
StereoFunctions* stereoFunc = new StereoFunctions();

//CV_EXTERN_C_FUNCPTR(void(*CvMouseCallback)(int event, int x, int y, int flags, void* param) );
//static void cvSetMouseCallback(const char* window_name, CvMouseCallback* on_mouse, void* param=NULL);
void mouseHandler(int event, int x, int y, int flags, void* param);	//handling mouse events

//functions for handling stereo correlation tools events
void onWindowBarSlide(int pos);
void onTextureBarSlide(int pos);
void onUniquenessBarSlide(int pos);
void onPreFilterSizeBarSlide(int pos);
void onPreFilterCapBarSlide(int pos);
void onSaveDataSlide(int pos);
void onSaveOriginalsSlide(int pos);
void stereoCreateCorrelationControls();
void loadCorrelationParams();
void loadCorrelationParams();

bool fexists(const char *filename);
bool left_mouse = false;

int main()
{
    if(CALIBRATION) stereoFunc->stereoCalibration(grab);

    loadCorrelationParams();
    grab->stereoGrabberInitFrames();
    grab->stereoGrabberGrabFrames();
    stereoFunc->stereoInit(grab);
    std::cout << "begin stereo \n";
    while(1)
    {
        if(ANALYSIS_MODE)	stereoCreateCorrelationControls();

        stereoFunc->stereoCorrelation(grab);
    //	cvSetMouseCallback( "Detect Object", mouseHandler, NULL );
        cvSetMouseCallback("Disparity Map", mouseHandler, NULL);

        //stereoFunc->stereoSavePointCloud();

        grab->stereoGrabberGrabFrames();

        if( cvWaitKey(10) == 27 ) break;
    }

    grab->stereoGrabberStopCam();

    return 0;
}


void loadCorrelationParams()
{
    stereoFunc->stereoDispWindowSize = 33;
    stereoFunc->stereoDispTextureThreshold = 20;
    stereoFunc->stereoDispUniquenessRatio = 15;
    stereoFunc->stereoNumDisparities = 48;
    stereoFunc->threshold = 29;
    stereoFunc->blobArea = 1500;


    stereoFunc->stereoPreFilterSize = 63;
    stereoFunc->stereoPreFilterCap = 63;
    stereoFunc->stereoSavePointCloudValue = 0;
}
bool fexists(const char *filename)
{
    ifstream ifile(filename);
    return true;
}

void onWindowBarSlide(int pos)
{
    stereoFunc->stereoDispWindowSize = cvGetTrackbarPos("SADSize", "Stereo Controls");
    if(stereoFunc->stereoDispWindowSize < 5)
        {	stereoFunc->stereoDispWindowSize = 5;
            stereoFunc->stereoCorrelation(grab);
        }
    else if ( stereoFunc->stereoDispWindowSize%2 == 0)
        {
            stereoFunc->stereoDispWindowSize += 1;
            stereoFunc->stereoCorrelation(grab);
        }
    else stereoFunc->stereoCorrelation(grab);
}

void onTextureBarSlide(int pos)
{
    stereoFunc->stereoDispTextureThreshold = cvGetTrackbarPos("Texture th", "Stereo Controls");
    if(stereoFunc->stereoDispTextureThreshold)
        stereoFunc->stereoCorrelation(grab);
}

void onUniquenessBarSlide(int pos)
{
    stereoFunc->stereoDispUniquenessRatio = cvGetTrackbarPos("Uniqueness", "Stereo Controls");
    if(stereoFunc->stereoDispUniquenessRatio>=0)
        stereoFunc->stereoCorrelation(grab);
}
/*void onPreFilterSizeBarSlide(int pos)
{
    stereoFunc->stereoPreFilterSize = cvGetTrackbarPos("PrFil.Size", "Stereo Controls");
    if(stereoFunc->stereoPreFilterSize>=5)
        if(stereoFunc->stereoPreFilterSize%2!=0)
                stereoFunc->stereoCorrelation(grab);
        else {
                ++(stereoFunc->stereoPreFilterSize);
                stereoFunc->stereoCorrelation(grab);}
    else {
                stereoFunc->stereoPreFilterSize = 5;
                stereoFunc->stereoCorrelation(grab);}

}

void onPreFilterCapBarSlide(int pos)
{
    stereoFunc->stereoPreFilterCap = cvGetTrackbarPos("PrFil.Cap", "Stereo Controls");
    if(stereoFunc->stereoPreFilterCap == 0)
        {	stereoFunc->stereoPreFilterCap = 1;
            stereoFunc->stereoCorrelation(grab);
        }
    else if( stereoFunc->stereoPreFilterCap > 63)
        {	stereoFunc->stereoPreFilterCap = 63;
            stereoFunc->stereoCorrelation(grab);
        }
    else 	stereoFunc->stereoCorrelation(grab);
}*/

void onNumDisparitiesSlide(int pos)
{
    stereoFunc->stereoNumDisparities = cvGetTrackbarPos("Num.Disp", "Stereo Controls");
    while(stereoFunc->stereoNumDisparities%16!=0 || stereoFunc->stereoNumDisparities==0)
        stereoFunc->stereoNumDisparities++;

    stereoFunc->stereoCorrelation(grab);
}
void onSaveDataSlide(int pos)
{
    cvSaveImage("Distance//Img1r.jpeg",stereoFunc->img1r);
    cvSaveImage("Distance//DisparityMap.jpeg",stereoFunc->vdisp);
    stereoFunc->stereoSavePointCloud();
}

void onSaveOriginalsSlide(int pos)
{

    cvSaveImage("Left.jpeg", stereoFunc->img1);
    cvSaveImage("Right.jpeg", stereoFunc->img2);

}

void stereoCreateCorrelationControls()
{
    cvNamedWindow( "Stereo Controls", 0);
    cvResizeWindow("Stereo Controls", 350, 600);
    cvCreateTrackbar("SADSize", "Stereo Controls", &stereoFunc->stereoDispWindowSize,255, onWindowBarSlide);
    cvCreateTrackbar("Texture th", "Stereo Controls", &stereoFunc->stereoDispTextureThreshold,25, onTextureBarSlide);
    cvCreateTrackbar("Uniqueness", "Stereo Controls", &stereoFunc->stereoDispUniquenessRatio,25, onUniquenessBarSlide);
//	cvCreateTrackbar("PrFil.Size", "Stereo Controls", &stereoFunc->stereoPreFilterSize,101, onPreFilterSizeBarSlide);
//	cvCreateTrackbar("PrFil.Cap", "Stereo Controls", &stereoFunc->stereoPreFilterCap,63, onPreFilterCapBarSlide);
    cvCreateTrackbar("Num.Disp", "Stereo Controls", &stereoFunc->stereoNumDisparities,640, onNumDisparitiesSlide);
    cvCreateTrackbar("Save Dist", "Stereo Controls", &stereoFunc->stereoSavePointCloudValue,1, onSaveDataSlide);
//	cvCreateTrackbar("Save.Img", "Stereo Controls", &stereoFunc->stereoSaveOriginal,1, onSaveOriginalsSlide);

    cvCreateTrackbar("Threshold", "Stereo Controls", &stereoFunc->threshold,300,0);
    cvCreateTrackbar("Area", "Stereo Controls", &stereoFunc->blobArea,5000,0);
}
/*
void mouseHandler(int event, int x, int y, int flags, void *param){

    switch(event){
    case CV_EVENT_LBUTTONDOWN:
    //l = cvGet2D(stereoFunc->depthM, x, y);
    printf("Distance to this object is: %f cm \n",(float)cvGet2D(stereoFunc->depthM, x, y).val[0]);
    break;
    }
}
*/
void mouseHandler(int event, int x, int y, int flags, void *param){

    if (event == CV_EVENT_LBUTTONDOWN)
    {
        cout << "x:" << x<< "y:" << y << endl;
        //l = cvGet2D(stereoFunc->depthM, x, y);
        printf("Distance to this object is: %f cm \n", (float)cvGet2D(stereoFunc->depthM, x, y).val[0]);
        left_mouse = true;

        //ÓÃÓÚŽæŽ¢ŽòÓ¡ÍŒÆ¬
        //imwrite(t, image);
        //  imwrite(t1, image1);
        //  num = num++;

    }
    else if (event == CV_EVENT_LBUTTONUP)
    {
        left_mouse = false;
    }
    else if ((event == CV_EVENT_MOUSEMOVE) && (left_mouse == true))
    {
    }
}
