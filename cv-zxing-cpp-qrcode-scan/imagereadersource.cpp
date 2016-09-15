#include "imagereadersource.h"
#include <zxing/common/IllegalArgumentException.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using std::string;
using std::ostringstream;
using zxing::Ref;
using zxing::ArrayRef;
using zxing::LuminanceSource;


inline char ImageReaderSource::convertPixel(char const* pixel_) const {
    unsigned char const* pixel = (unsigned char const*)pixel_;
    if (comps == 1 || comps == 2) {
        // Gray or gray+alpha
        return pixel[0];
    } if (comps == 3 || comps == 4) {
        // Red, Green, Blue, (Alpha)
        // We assume 16 bit values here
        // 0x200 = 1<<9, half an lsb of the result to force rounding
        return (char)((306 * (int)pixel[0] + 601 * (int)pixel[1] +
                117 * (int)pixel[2] + 0x200) >> 10);
    } else {
        throw zxing::IllegalArgumentException("Unexpected image depth");
    }
}


ImageReaderSource::ImageReaderSource(ArrayRef<char> image_, int width, int height, int comps_)
    : Super(width, height), image(image_), comps(comps_) {}

zxing::Ref<LuminanceSource> ImageReaderSource::create(const cv::Mat & src){
    int width, height;
    int comps = 0;
    zxing::ArrayRef<char> image;
    if(src.empty()){
        throw zxing::IllegalArgumentException("can not open image");
    }
    width = src.cols;
    height = src.rows;
    int channels = src.channels();
    comps = channels;
    cv::Mat dst;
  //  std::cout << "channels = " << channels;
    if(comps == 1 || comps == 2){
        cv::cvtColor(src,dst,cv::COLOR_GRAY2RGBA);
        image = zxing::ArrayRef<char>((char *)dst.data,4 * width * height);

    }else if(comps == 3){
        cv::cvtColor(src,dst,cv::COLOR_RGB2RGBA);
        image = zxing::ArrayRef<char>((char *)dst.data,4 * width * height);

    } else{
        image = zxing::ArrayRef<char>((char *)src.data,4 * width * height);

    }


    if (!image) {
        ostringstream msg;
        throw zxing::IllegalArgumentException(msg.str().c_str());
    }

    return Ref<LuminanceSource>(new ImageReaderSource(image, width, height, comps));
}


Ref<LuminanceSource> ImageReaderSource::create(string const& filename) {
    //  string extension = filename.substr(filename.find_last_of(".") + 1);
    // std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    int width, height;
    int comps = 0;
    zxing::ArrayRef<char> image;

    cv::Mat src = cv::imread(filename);
    if(src.empty()){
        throw zxing::IllegalArgumentException("can not open image");
    }
    width = src.cols;
    height = src.rows;
    int channels = src.channels();
    comps = channels;
    cv::Mat dst;
    if(comps == 1 || comps == 2){
        cv::cvtColor(src,dst,cv::COLOR_GRAY2RGBA);
        image = zxing::ArrayRef<char>((char *)dst.data,4 * width * height);

    }else if(comps == 3){
        cv::cvtColor(src,dst,cv::COLOR_RGB2RGBA);
        image = zxing::ArrayRef<char>((char *)dst.data,4 * width * height);

    } else{
        image = zxing::ArrayRef<char>((char *)src.data,4 * width * height);

    }


    image = zxing::ArrayRef<char>((char *)src.data,4 * width * height);

    if (!image) {
        ostringstream msg;
        msg << "Loading \"" << filename << "\" failed.";
        throw zxing::IllegalArgumentException(msg.str().c_str());
    }

    return Ref<LuminanceSource>(new ImageReaderSource(image, width, height, comps));
}


zxing::ArrayRef<char> ImageReaderSource::getRow(int y, zxing::ArrayRef<char> row) const {
    const char* pixelRow = &image[0] + y * getWidth() * 4;
    if (!row) {
        row = zxing::ArrayRef<char>(getWidth());
    }
    for (int x = 0; x < getWidth(); x++) {
        row[x] = convertPixel(pixelRow + (x * 4));
    }
    return row;
}

/** This is a more efficient implementation. */
zxing::ArrayRef<char> ImageReaderSource::getMatrix() const {
    const char* p = &image[0];
    zxing::ArrayRef<char> matrix(getWidth() * getHeight());
    char* m = &matrix[0];
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            *m = convertPixel(p);
            m++;
            p += 4;
        }
    }
    return matrix;
}
