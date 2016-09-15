#include "qrdecoderutils.h"

#include "imagereadersource.h"
#include <fstream>
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
#include <zxing/Result.h>

using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;
using zxing::Ref;
using zxing::ArrayRef;
using zxing::LuminanceSource;
using zxing::String;

QRDecoderUtils::QRDecoderUtils()
{
    more = false;
    test_mode = false;
    try_harder = false;
    search_multi = false;
    use_hybrid = false;
    use_global = false;
    verbose = false;
}

std::vector<std::string> QRDecoderUtils::decodeFromFile(const std::string & file){
    Ref<LuminanceSource> irs = ImageReaderSource::create(file);
    std::vector<std::string> res = this->read_image(irs,false,"");
    return res;

}

std::vector<std::string> QRDecoderUtils::decodeFromMat(const cv::Mat & image){
    Ref<LuminanceSource> irs = ImageReaderSource::create(image);
    std::vector<std::string> res = this->read_image(irs,false,"");
    return res;
}

vector<Ref<Result> > QRDecoderUtils::decode(Ref<BinaryBitmap> image, DecodeHints hints) {
    Ref<Reader> reader(new MultiFormatReader);
    return vector<Ref<Result> >(1, reader->decode(image, hints));
}

vector<Ref<Result> > QRDecoderUtils::decode_multi(Ref<BinaryBitmap> image, DecodeHints hints) {
    MultiFormatReader delegate;
    GenericMultipleBarcodeReader reader(delegate);
    return reader.decodeMultiple(image, hints);
}


std::vector <std::string> QRDecoderUtils::read_image(Ref<LuminanceSource> source, bool hybrid, string expected) {
    vector<Ref<Result> > results;
    string cell_result;
    std::vector<std::string> final_result;
    int res = -1;

    try {
        Ref<Binarizer> binarizer;
        if (hybrid) {
            binarizer = new HybridBinarizer(source);
        } else {
            binarizer = new GlobalHistogramBinarizer(source);
        }
        DecodeHints hints(DecodeHints::DEFAULT_HINT);
        hints.setTryHarder(try_harder);
        Ref<BinaryBitmap> binary(new BinaryBitmap(binarizer));
        if (search_multi) {
            results = decode_multi(binary, hints);
        } else {
            results = decode(binary, hints);
        }
        res = 0;
    } catch (const ReaderException& e) {
        cell_result = "zxing::ReaderException: " + string(e.what());
        res = -2;
    } catch (const zxing::IllegalArgumentException& e) {
        cell_result = "zxing::IllegalArgumentException: " + string(e.what());
        res = -3;
    } catch (const zxing::Exception& e) {
        cell_result = "zxing::Exception: " + string(e.what());
        res = -4;
    } catch (const std::exception& e) {
        cell_result = "std::exception: " + string(e.what());
        res = -5;
    }

    int fsize = results.size();
    for(int i = 0;i < fsize;i++){
       final_result.push_back(results[i]->getText()->getText());
    }

    return final_result;
}

string QRDecoderUtils::read_expected(string imagefilename) {
    string textfilename = imagefilename;
    string::size_type dotpos = textfilename.rfind(".");

    textfilename.replace(dotpos + 1, textfilename.length() - dotpos - 1, "txt");
    ifstream textfile(textfilename.c_str(), ios::binary);
    textfilename.replace(dotpos + 1, textfilename.length() - dotpos - 1, "bin");
    ifstream binfile(textfilename.c_str(), ios::binary);
    ifstream *file = 0;
    if (textfile.is_open()) {
        file = &textfile;
    } else if (binfile.is_open()) {
        file = &binfile;
    } else {
        return std::string();
    }
    file->seekg(0, ios_base::end);
    size_t size = size_t(file->tellg());
    file->seekg(0, ios_base::beg);

    if (size == 0) {
        return std::string();
    }

    char* data = new char[size + 1];
    file->read(data, size);
    data[size] = '\0';
    string expected(data);
    delete[] data;

    return expected;
}
