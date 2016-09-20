#include <iostream>
#include <vector>
using namespace std;
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

int main(int argc, char *argv[])
{
    cv::VideoCapture left_cap(0);
    if(!left_cap.isOpened()){
        std::cerr << "cannot open left camera.\n";
        return -1;
    }

    cv::VideoCapture right_cap(1);
    if(!right_cap.isOpened()){
        std::cerr << "cannot open right camera.\n";
        return -1;
    }

    cv::FileStorage fs("disparity-stereobm-config.yaml",cv::FileStorage::READ);


    cv::Mat left_frame;
    cv::Mat right_frame;
    char key = -1;



    cv::Mat img_disparity_16s_r,img_disparity_16s_g,img_disparity_16s_b;//cv::Mat( imgLeft.rows, imgLeft.cols, CV_16S );
    cv::Mat img_disparity_8u_r,img_disparity_8u_g,img_disparity_8u_b ;//= Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );
    int ndisparities = fs["disparityRange"];//= 16*5;   /**< Range of disparity */
    int sad_window_size = fs["disparityWindowSize"]; /**< Size of the block window. Must be odd */
    cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create( ndisparities, sad_window_size );
    fs.release();
    std::vector<cv::Mat> channels_left,channels_right;
    //std::vector<cv::Mat> output_channels;
    while(true){
        left_cap >> left_frame;
        right_cap >> right_frame;

        if(left_frame.empty() || right_frame.empty()){
            continue;
        }

        if(img_disparity_8u_r.empty()){
            img_disparity_8u_r = cv::Mat(left_frame.rows,left_frame.cols,CV_8UC1);
        }

        if(img_disparity_8u_g.empty()){
            img_disparity_8u_g = cv::Mat(left_frame.rows,left_frame.cols,CV_8UC1);
        }

        if(img_disparity_8u_b.empty()){
            img_disparity_8u_b = cv::Mat(left_frame.rows,left_frame.cols,CV_8UC1);
        }


        if(img_disparity_16s_r.empty()){
            img_disparity_16s_r = cv::Mat(left_frame.rows,left_frame.cols,CV_16S);
        }

        if(img_disparity_16s_g.empty()){
            img_disparity_16s_g = cv::Mat(left_frame.rows,left_frame.cols,CV_16S);
        }
        if(img_disparity_16s_b.empty()){
            img_disparity_16s_b = cv::Mat(left_frame.rows,left_frame.cols,CV_16S);
        }

        cv::split(left_frame,channels_left);
        cv::split(right_frame,channels_right);
//        cv::Mat left_gray,right_gray;

//        cv::cvtColor(left_frame,left_gray,cv::COLOR_RGB2GRAY);
//        cv::cvtColor(right_frame,right_gray,cv::COLOR_RGB2GRAY);

//        cv::blur(left_gray,left_gray,cv::Size(5,5));
//        cv::blur(right_gray,right_gray,cv::Size(5,5));

        sbm->compute( channels_left[0], channels_right[0], img_disparity_16s_r );
        sbm->compute( channels_left[1], channels_right[1], img_disparity_16s_g );
        sbm->compute( channels_left[2], channels_right[2], img_disparity_16s_b );

        double minVal_r,minVal_g,minVal_b; double maxVal_r,maxVal_g,maxVal_b;

        cv::minMaxLoc( img_disparity_16s_r, &minVal_r, &maxVal_r );
        cv::minMaxLoc( img_disparity_16s_g, &minVal_g, &maxVal_g );
        cv::minMaxLoc( img_disparity_16s_b, &minVal_b, &maxVal_b );

        img_disparity_16s_r.convertTo( img_disparity_8u_r, CV_8UC1, 255/(maxVal_r - minVal_r));
        img_disparity_16s_g.convertTo( img_disparity_8u_g, CV_8UC1, 255/(maxVal_g - minVal_g));
        img_disparity_16s_b.convertTo( img_disparity_8u_b, CV_8UC1, 255/(maxVal_b - minVal_b));

        std::vector<cv::Mat> disparit_channels;
        disparit_channels.push_back(img_disparity_8u_r);
        disparit_channels.push_back(img_disparity_8u_g);
        disparit_channels.push_back(img_disparity_8u_b);
        cv::Mat output_disparity;
        cv::merge(disparit_channels,output_disparity);
        cv::cvtColor(output_disparity,output_disparity,cv::COLOR_RGB2GRAY);
        cv::imshow("video:left",left_frame);
        cv::imshow("video:right",right_frame);
        cv::imshow("video:disparity",output_disparity);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
