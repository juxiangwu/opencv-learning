#include <ctime>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "opencv2/calib3d/calib3d.hpp"  //findHomography所需头文件
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera \n";
        return -1;
    }
    Mat imgObject = imread( "../../datas/bottle.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    //Mat imgScene = imread( "D:\\opencv_pic\\cat0.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    cv::Mat imgScene;
    char key = -1;
    if( !imgObject.data )
    {
        cout<< " --(!) Error reading images "<<endl;
        cap.release();
        return -1;
    }

    double begin = clock();

    ///-- Step 1: 使用SURF算子检测特征点
    int minHessian = 400;
    Ptr<cv::xfeatures2d::SurfFeatureDetector> detector = cv::xfeatures2d::SurfFeatureDetector::create(minHessian );
    Ptr<cv::xfeatures2d::SurfDescriptorExtractor> extractor = cv::xfeatures2d::SurfDescriptorExtractor::create();
    Mat descriptorsObject, descriptorsScene;
    FlannBasedMatcher matcher;
    while(true){
        cap >> imgScene;
        if(imgScene.empty()){
            continue;
        }
        vector<KeyPoint> keypointsObject, keypointsScene;

        detector->detect( imgObject, keypointsObject );
        detector->detect( imgScene, keypointsScene );
        cout<<"object--number of keypoints: "<<keypointsObject.size()<<endl;
        cout<<"scene--number of keypoints: "<<keypointsScene.size()<<endl;

        ///-- Step 2: 使用SURF算子提取特征（计算特征向量）

        extractor->compute( imgObject, keypointsObject, descriptorsObject );
        extractor->compute( imgScene, keypointsScene, descriptorsScene );

        ///-- Step 3: 使用FLANN法进行匹配
        if(keypointsScene.empty()){
            std::cerr << "not found any matching points\n";
            continue;
        }
        if(descriptorsScene.size() == 0){
            std::cerr << "not found any descript\n";
            continue;
        }
        if(keypointsScene.size() == 0){
            continue;
        }

        vector< DMatch > allMatches;
        matcher.match( descriptorsObject, descriptorsScene, allMatches );
        cout<<"number of matches before filtering: "<<allMatches.size()<<endl;
        if(allMatches.size() == 0){
            continue;
        }
        if(descriptorsScene.empty()){
            continue;
        }
        //-- 计算关键点间的最大最小距离
        double maxDist = 0;
        double minDist = 100;
        for( int i = 0; i < descriptorsObject.rows; i++ )
        {
            double dist = allMatches[i].distance;
            if( dist < minDist )
                minDist = dist;
            if( dist > maxDist )
                maxDist = dist;
        }
        printf("    max dist : %f \n", maxDist );
        printf("    min dist : %f \n", minDist );

        //-- 过滤匹配点，保留好的匹配点（这里采用的标准：distance<3*minDist）
        vector< DMatch > goodMatches;

        for( int i = 0; i < descriptorsObject.rows; i++ )
        {
            if( allMatches[i].distance < 2*minDist )
                goodMatches.push_back( allMatches[i]);
        }


        cout<<"number of matches after filtering: "<< goodMatches.size()<<endl;
        int gms = goodMatches.size();
        std::cout << "gms = " << gms << std::endl;
        if(gms == 0){
            std::cerr << "not found any good mathches.\n";
            continue;
        }
        //-- 显示匹配结果
        Mat resultImg;
        drawMatches( imgObject, keypointsObject, imgScene, keypointsScene,
                     goodMatches, resultImg, Scalar::all(-1), Scalar::all(-1), vector<char>(),
                     DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS //不显示未匹配的点
                     );
        //-- 输出匹配点的对应关系
        for( int i = 0; i < goodMatches.size(); i++ )
            printf( "   good match %d: keypointsObject [%d]  -- keypointsScene [%d]\n", i,
                    goodMatches[i].queryIdx, goodMatches[i].trainIdx );

        ///-- Step 4: 使用findHomography找出相应的透视变换
        vector<Point2f> object;
        vector<Point2f> scene;
        for( size_t i = 0; i < goodMatches.size(); i++ )
        {
            //-- 从好的匹配中获取关键点: 匹配关系是关键点间具有的一 一对应关系，可以从匹配关系获得关键点的索引
            //-- e.g. 这里的goodMatches[i].queryIdx和goodMatches[i].trainIdx是匹配中一对关键点的索引
            object.push_back( keypointsObject[ goodMatches[i].queryIdx ].pt );
            scene.push_back( keypointsScene[ goodMatches[i].trainIdx ].pt );
        }
        Mat H = findHomography( object, scene, CV_RANSAC );

        ///-- Step 5: 使用perspectiveTransform映射点群，在场景中获取目标位置
        std::vector<Point2f> objCorners(4);
        objCorners[0] = cvPoint(0,0);
        objCorners[1] = cvPoint( imgObject.cols, 0 );
        objCorners[2] = cvPoint( imgObject.cols, imgObject.rows );
        objCorners[3] = cvPoint( 0, imgObject.rows );
        std::vector<Point2f> sceneCorners(4);
        perspectiveTransform( objCorners, sceneCorners, H);

        //-- 在被检测到的目标四个角之间划线
        line( resultImg, sceneCorners[0] + Point2f( imgObject.cols, 0), sceneCorners[1] + Point2f( imgObject.cols, 0), Scalar(0, 255, 0), 4 );
        line( resultImg, sceneCorners[1] + Point2f( imgObject.cols, 0), sceneCorners[2] + Point2f( imgObject.cols, 0), Scalar( 0, 255, 0), 4 );
        line( resultImg, sceneCorners[2] + Point2f( imgObject.cols, 0), sceneCorners[3] + Point2f( imgObject.cols, 0), Scalar( 0, 255, 0), 4 );
        line( resultImg, sceneCorners[3] + Point2f( imgObject.cols, 0), sceneCorners[0] + Point2f( imgObject.cols, 0), Scalar( 0, 255, 0), 4 );

        //-- 显示检测结果
        imshow("detection result", resultImg );

        double end = clock();
        cout<<"\nSURF--elapsed time: "<<(end - begin)/CLOCKS_PER_SEC*1000<<" ms\n";

        key = waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
