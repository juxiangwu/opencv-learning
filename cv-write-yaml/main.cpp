#include <iostream>
#include <opencv2/opencv.hpp>

#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
    cv::FileStorage fs("test.yaml",cv::FileStorage::WRITE);
    fs << "A" << 5;
    time_t rawtime;
    time(&rawtime);
    fs << "datetime" << asctime(localtime(&rawtime));
    fs.release();
    cv::FileStorage rs("test.yaml",cv::FileStorage::READ);
    int value = rs["A"];
    std::cout <<"A = " <<  value << std::endl;

    rs.release();
    return 0;
}
