#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

using namespace cv;

char highpicName[20];
char normalpicName[20];
char lowpicName[20];
Mat mat1, mat2, mat3, dst_mat, tmp_mat;
int highWidth, highHeight;
int normalWidth, normalHeight;
int lowWidth, lowHeight;
IplImage src1, src2, src3, dst_src, tmp_src;
double weight=0.5;


void hdrCale(Mat pic1, Mat pic2, Mat pic3){
    int i, j;
    CvScalar s1, s2, s3;


    src1 = pic1;
    src2 = pic2;
    src3 = pic3;
    dst_src = dst_mat;
    tmp_src = tmp_mat;

    cvCvtColor(&src2, &tmp_src, CV_BGR2GRAY);
    for(i=0; i< normalWidth; i++){
        for(j=0; j<normalHeight; j++){
            s1 = cvGet2D(&src1, i, j);
            s2 = cvGet2D(&tmp_src, i, j);
            s3 = cvGet2D(&src3, i, j);
            weight = 0.5 + (127 - s2.val[0]) * 0.002;
            s3.val[0] = (s1.val[0] * weight) + (s3.val[0] * (1-weight));
            s3.val[1] = (s1.val[1] * weight) + (s3.val[1] * (1-weight));
            s3.val[2] = (s1.val[2] * weight) + (s3.val[2] * (1-weight));
            cvSet2D(&dst_src, i, j, s3);
        }
    }
}


int main(int argc, char *argv[]){
    //    if(argc < 4){
    //        printf("Please input high exposure/normal exposure/low exposure picture!\n");
    //        return -1;
    //    }
    //    memcpy(highpicName, argv[1], sizeof(argv[1]));
    //    memcpy(normalpicName, argv[2], sizeof(argv[2]));
    //    memcpy(lowpicName, argv[3], sizeof(argv[3]));
    //    mat1 = imread(argv[1]);
    //    mat2 = imread(argv[2]);
    //    mat3 = imread(argv[3]);
    VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    char key = -1;
    while(true){
        cap >> mat2;
        if(mat2.empty()){
            continue;
        }
        mat1 = mat2 * 1.5;
        mat3 = mat2 * 0.25;

        highWidth = mat1.rows;
        highHeight = mat1.cols;
        normalWidth = mat2.rows;
        normalHeight = mat2.cols;
        lowWidth = mat3.rows;
        lowHeight = mat3.cols;
        dst_mat = Mat(normalWidth, normalHeight, CV_8UC3, cv::Scalar(0, 0, 0));
        tmp_mat = Mat(normalWidth, normalHeight, CV_8UC1, cv::Scalar(0, 0, 0));

        hdrCale(mat1, mat2, mat3);

        imshow("normal", mat2);
        imshow("HDR", dst_mat);
        imshow("Low",mat3);
        imshow("High",mat1);
        //imwrite("HDR.jpg", dst_mat);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
