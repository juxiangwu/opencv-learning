#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    char key = -1;
    vector<Mat> images;
    images.resize(3);
    vector<float> times;
    times.push_back(0.5);
    times.push_back(1.0);
    times.push_back(0.5);
    // loadExposureSeq(argv[1], images, times);

    Mat response;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
    Ptr<MergeMertens> merge_mertens = createMergeMertens();

    Mat hdr;
    cv::Mat src;
    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        cv::Mat high_exposure = src * 1.5;
        cv::Mat low_exposure = src * 0.25;
        images[0] = high_exposure;
        images[1] = src;
        images[2] = low_exposure;

        calibrate->process(images, response, times);

        merge_debevec->process(images, hdr, times, response);

        Mat ldr;
        Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
        tonemap->process(hdr, ldr);

        Mat fusion;
        merge_mertens->process(images, fusion);

        cv::imshow("src",src);
        cv::imshow("fusion",fusion * 255);
        cv::imshow("ldr",ldr * 255);
        cv::imshow("hdr",hdr);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

//    imwrite("fusion.png", fusion * 255);
//    imwrite("ldr.png", ldr * 255);
//    imwrite("hdr.hdr", hdr);

    return 0;
}
