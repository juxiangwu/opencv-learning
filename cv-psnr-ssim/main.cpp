#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


double getPSNR(const Mat& I1, const Mat& I2){
    Mat s1;
    absdiff(I1, I2, s1);
    s1.convertTo(s1, CV_32F);
    s1 = s1.mul(s1);
    Scalar s = sum(s1);
    double sse = s.val[0] + s.val[1] + s.val[2];
    if(sse <= 1e-10)
        return 0;
    else{
        double mse = sse/(double)(I1.channels()*I1.total());
        double psnr = 10.0*log10(255*255/mse);
        return psnr;
    }

}

Scalar getMSSIM(const Mat& i1, const Mat& i2){
    const double C1=6.5025, C2 = 58.5225;
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);
    i2.convertTo(I2, d);
    Mat I2_2 = I2.mul(I2);     // I2^2
    Mat I1_2 = I1.mul(I1);      //I1^2
    Mat I1_I2 = I1.mul(I2);     // I1*I2

    Mat mu1, mu2;
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2 ,Size(11, 11), 1.5);

    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11,11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11,11), 1.5);
    sigma12 -= mu1_mu2;

    Mat t1, t2, t3;
    t1 = 2*mu1_mu2 + C1;
    t2 = 2*sigma12 + C2;
    t3 = t1.mul(t2);

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);

    Mat ssim_map;
    divide(t3, t1, ssim_map);
    Scalar mssim = mean(ssim_map);
    return mssim;
}


int main(){
    Mat i1 = imread("../../datas/f1.jpg");
    Mat i2 = imread("../../datas/f1.jpg");
    if(!i1.data || !i2.data){
        cout << "图片路径有误!" << endl;
        return -1;
    }

    cout << "PSNR: " << getPSNR(i1, i2) << endl;
    Scalar result = getMSSIM(i1, i2);
    if(i2.channels() == 3)
        cout<< "SSIM: " << (result.val[0]+ result.val[1]+result.val[2])/3 << endl;
    else cout << "SSIM: " << result.val[0] << endl;

    Mat i11, i22;
    cvtColor(i1, i11, COLOR_BGR2YUV);
    cvtColor(i2, i22, COLOR_BGR2YUV);
    vector<Mat> mv1, mv2;
    split(i11, mv1);
    split(i22, mv2);
    cout << "Y 分量PSNR: " << getPSNR(mv1[0], mv2[0]) << endl;
    cout << "Y 分量SSIM: " << getMSSIM(mv1[0], mv2[0]).val[0] << endl;
    return 0;
}
