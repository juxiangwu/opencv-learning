#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
    cv::Mat map = imread("/home/jenson/Pictures/f1.jpg");
    double min;
    double max;
    cv::minMaxIdx(map, &min, &max);
    cv::Mat adjMap;
    // expand your range to 0..255. Similar to histEq();
    map.convertTo(adjMap,CV_8UC1, 255 / (max-min), -min);

    // this is great. It converts your grayscale image into a tone-mapped one,
    // much more pleasing for the eye
    // function is found in contrib module, so include contrib.hpp
    // and link accordingly
    cv::Mat falseColorsMap;
    applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_AUTUMN);

    cv::imshow("Out", falseColorsMap);

    imshow("window", map);
    waitKey(0);
    return 0;
}
