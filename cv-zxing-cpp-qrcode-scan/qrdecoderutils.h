#ifndef QRDECODERUTILS_H
#define QRDECODERUTILS_H
#include <opencv2/opencv.hpp>
#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/Result.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/common/HybridBinarizer.h>
#include <exception>
#include <zxing/Exception.h>
#include <zxing/common/IllegalArgumentException.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/DecodeHints.h>

#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/multi/qrcode/QRCodeMultiReader.h>
#include <zxing/multi/ByQuadrantReader.h>
#include <zxing/multi/MultipleBarcodeReader.h>
#include <zxing/multi/GenericMultipleBarcodeReader.h>

#include <vector>

using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;
using  zxing::Ref;
using  zxing::ArrayRef;
using  zxing::LuminanceSource;


class QRDecoderUtils
{
public:
    QRDecoderUtils();

    std::vector<std::string> decodeFromFile(const std::string & file);
    std::vector<std::string> decodeFromMat(const cv::Mat & image);

private:
    std::vector<std::string> read_image(Ref<LuminanceSource> source, bool hybrid, string expected);
    vector<Ref<Result> > decode(Ref<BinaryBitmap> image, DecodeHints hints);
    vector<Ref<Result> > decode_multi(Ref<BinaryBitmap> image, DecodeHints hints);
    string read_expected(string imagefilename);

private:
    bool more;
    bool test_mode;
    bool try_harder;
    bool search_multi;
    bool use_hybrid;
    bool use_global;
    bool verbose;
};

#endif // QRDECODERUTILS_H
