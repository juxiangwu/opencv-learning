#ifndef IMAGEREADERSOURCE_H
#define IMAGEREADERSOURCE_H
#include <zxing/LuminanceSource.h>
#include <opencv2/opencv.hpp>

class ImageReaderSource : public zxing::LuminanceSource {
private:
  typedef LuminanceSource Super;

  const zxing::ArrayRef<char> image;
  const int comps;

  char convertPixel(const char* pixel) const;

public:
  static zxing::Ref<LuminanceSource> create(std::string const& filename);
  static zxing::Ref<LuminanceSource> create(const cv::Mat & src);
  ImageReaderSource(zxing::ArrayRef<char> image, int width, int height, int comps);

  zxing::ArrayRef<char> getRow(int y, zxing::ArrayRef<char> row) const;
  zxing::ArrayRef<char> getMatrix() const;
};


#endif // IMAGEREADERSOURCE_H
