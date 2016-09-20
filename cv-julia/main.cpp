#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

#define IMG_8UB(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x)]
#define IMG_8UG(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x) + 1]
#define IMG_8UR(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x) + 2]
#define IMG8U(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[(x)]

CvPoint pt1 = {-1,-1};//两个用来记录鼠标点击和放松的点
CvPoint pt2 = {-1,-1};
IplImage* fractal;
IplImage* fcopy;//用来画矩形
int width = 600;//图像宽和高
int height = 400;
double XMax = 2.5;//复平面的最大坐标
double XMin = -2.5;
double YMax = 2.5;
double YMin = -2.5;

#define MAX_COLOR 256//用来记录配色，毕竟黑白的不是那么好看
int B[MAX_COLOR];
int G[MAX_COLOR];
int R[MAX_COLOR];

struct complex_t//复数
{
    double real;
    double img;
};

char* name[100];//即时输出观察时候的图片的名字
int fileIndex = 0;//标号

char* itoa(int value)
{
    char* tmp = (char*)calloc(11,sizeof(char));
    char reverse[15];
    int index = 0;
    do
    {
        reverse[index++] = value % 10 + '0';
        value /= 10;
    }while(value);
    int size = index - 1;
    while(index--)
    {
        tmp[size - index] = reverse[index];
    }
    tmp[++size] = '.';
    tmp[++size] = 'j';
    tmp[++size] = 'p';
    tmp[++size] = 'g';
    tmp[++size] = '\0';
    return tmp;
}
void initFileName()//直接用数字命名图片了
{
    for(int i = 0;i < 100;i++)
        name[i] = itoa(i);
}

void initColor()//这里需要说明下，虽然写的是bgr，但是这里是当作HSV来初始化的，然后图片绘制完成后直接转下空间即可
{
    for(int i = 0;i < MAX_COLOR;i++)
    {
        B[i] = i * 4 % 256;
        G[i] = 0.7 * 255.0;
        R[i] = 255.0 * (1.0 - i / 255.0 * i / 255.0 / 1.2);
    }
}

void drawPic()
{
    double deltaX = (XMax - XMin) / width;
    double deltaY = (YMax - YMin) / height;
    int max_iterations = 256;//最大迭代次数
    double max_size = 4.0;//这里是2的平方，为什么？建议看看维基百科的介绍~
    for(int row = 0;row < height;row++)
    {
        for(int col = 0;col < width;col++)
        {
            int color = 0;
            complex_t c,z;
            z.real = 0;//这里是Mandelbrot集，下面被注释掉的是Julia集
            z.img = 0;
            c.real  = XMin + col * deltaX;
            c.img = YMin + row * deltaY;
            //z.real = XMin + col * deltaX;
            //z.img = YMin + row * deltaY;
            //c.real = 0.285;
            //c.img = 0.01;

            while((color < max_iterations) && ((z.img * z.img + z.real * z.real) < max_size))
            {
                double tmp = z.real * z.real - z.img * z.img + c.real;
                z.img = z.img * z.real + z.real * z.img + c.img;
                z.real = tmp;
                color++;
            }
            if(color >= max_iterations)
                color = 255;
            color %= MAX_COLOR;
            IMG_8UB(fractal,col,row) = B[color];
            IMG_8UG(fractal,col,row) = G[color];
            IMG_8UR(fractal,col,row) = R[color];
        }
    }
    cvCvtColor(fractal,fractal,CV_HSV2BGR);//将HSV空间转换为BGR，方便显示
    cvShowImage("Fractal",fractal);
    cvSaveImage(name[fileIndex++],fractal);//记录查看过程

    cvWaitKey(0);
}

void on_mouse( int event, int x, int y, int flags, void *param  =  NULL)
{
    if( !fractal)
        return;

    if( event == CV_EVENT_LBUTTONDOWN)
    {
        pt1 = cvPoint(x,y);
    }
    else if(  event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )
    {
        pt2 = cvPoint(x,y);
        int dx = abs(pt2.x - pt1.x);
        int dy = abs(pt2.y - pt1.y);

        if(pt1.x > 0 && pt1.y > 0
                && pt2.x > 0 && pt2.y > 0
                && dx > 5 && dy > 5)
        {
            cvSaveImage(name[fileIndex++],fcopy);//记录查看过程
            double DX = XMax - XMin;//接下来转换放大的坐标
            double DY = YMax - YMin;
            double offX = DX / width;
            double offY = DY / height;
            if(pt1.x < pt2.x)
            {
                XMax = offX * pt2.x + XMin;
                XMin = offX * pt1.x + XMin;
            }
            else
            {
                XMax = offX * pt1.x + XMin;
                XMin = offX * pt2.x + XMin;
            }
            if(pt1.y < pt2.y)
            {
                YMax = offY * pt2.y + YMin;
                YMin = offY * pt1.y + YMin;
            }
            else
            {
                YMax = offY * pt1.y + YMin;
                YMin = offY * pt2.y + YMin;
            }
            pt1 = cvPoint(-1,-1);//注意重新赋值，否则鼠标移动时还会进来，造成重复计算
            pt2 = cvPoint(-1,-1);
            printf("XMax:%.15lf XMin:%.15lf\nYMax:%.15lf YMin:%.15lf\n",XMax,XMin,YMax,YMin);
            drawPic();//重新绘制分形
        }
    }
    else if(event == CV_EVENT_RBUTTONDOWN)//右键取消放大，重新选取放大区域
    {
        pt1 = cvPoint(-1,-1);
        pt2 = cvPoint(-1,-1);
    }
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )//在选取过程中画矩形
    {
        pt2 = cvPoint(x,y);
        if(pt1.x > 0 && pt1.y > 0
                && pt2.x > 0 && pt2.y > 0
                && abs(pt2.x - pt1.x) > 5
                && abs(pt2.y - pt1.y) > 5)
        {
            cvCopy(fractal,fcopy);
            cvRectangle(fcopy,pt1,pt2,cvScalar(255,255,255));
            cvShowImage( "Fractal", fcopy);
        }
    }
}

int main(int argc, char *argv[])
{
    cvNamedWindow("Fractal",1);
    cvSetMouseCallback( "Fractal", on_mouse, 0 );//注册鼠标事件
    fractal = cvCreateImage(cvSize(width,height),8,3);
    fcopy = cvCreateImage(cvSize(width,height),8,3);
    initFileName();//初始化
    initColor();

    drawPic();

    cvReleaseImage(&fractal);
    cvReleaseImage(&fcopy);
    cvDestroyWindow("Fractal");
    for(int i = 0;i < 100;i++)
        free(name[i]);
    return 0;
}
