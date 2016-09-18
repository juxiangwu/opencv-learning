#include <iostream>
#include <opencv2/opencv.hpp>

#include <vtk/vtkImageActor.h>
#include <vtk/vtkImageImport.h>
#include <vtk/vtkImageData.h>
#include <vtk/vtkRenderWindowInteractor.h>
#include <vtk/vtkRenderWindow.h>
#include <vtk/vtkRenderer.h>
#include <vtk/vtkCommand.h>

//Function prototypes
void Ipl2VTK(cv::Mat& src, vtkImageData *dst);


//!Class vtkTimerCallback
/**
 * This class add support for timing events
 */
class vtkTimerCallback : public vtkCommand
{

public:
  vtkTimerCallback(){}
  ~vtkTimerCallback(){}


public:
  static vtkTimerCallback *New()
  {
    vtkTimerCallback *cb = new vtkTimerCallback;
    cb->TimerCount = 0;
    return cb;
  }

  virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eventId,
                       void *vtkNotUsed(callData))
  {

    if (vtkCommand::TimerEvent == eventId)
      {
      ++this->TimerCount;
      }
    cout << this->TimerCount << endl;

    //frame = cvQueryFrame(capture);
    capture >> frame;
   // cv::Mat imageMatrix(frame);
    cv::Mat dist;
    cv::cvtColor(frame,dist,cv::COLOR_BGR2RGB);
    Ipl2VTK(dist, imageData);


    window->Render();

  }

  void SetCapture(cv::VideoCapture & cap)
  {
    capture = cap;
  }

  void SetImageData(vtkImageData *id)
  {
    imageData = id;
  }

  void SetActor(vtkImageActor *act)
  {
    actor = act;
  }

  void SetRenderer(vtkRenderer *rend)
  {

    renderer = rend;
  }

  void SetRenderWindow(vtkRenderWindow *wind)
  {
    window = wind;
  }

private:
  int TimerCount;
  cv::Mat frame;
  cv::VideoCapture capture;
  vtkImageData *imageData;
  vtkImageActor *actor;
  vtkRenderer *renderer;
  vtkRenderWindow *window;
};


#include <vtk/vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
//!Entry point
int main(int argc, char *argv[])
{

  cv::Mat frame ;
  cv::VideoCapture capture(0);

  if(!capture.isOpened())
    {
    std::cout << "Unable to open the device" << std::endl;
    exit(EXIT_FAILURE);
    }

  for(float i=0; i<1; i++)
    {
    capture >> frame;
    if(frame.empty())
      {
      std::cout << "Unable to get frames from the device" << std::endl;
      //cvReleaseCapture(&capture);
      capture.release();
      exit(EXIT_FAILURE);
      }
    }

  vtkImageData *imageData = vtkImageData::New();
  cv::Mat imageMatrix(frame);

  Ipl2VTK(imageMatrix, imageData);

  vtkImageActor *actor = vtkImageActor::New();
  actor->SetInputData(imageData);

  vtkRenderer *renderer = vtkRenderer::New();
  vtkRenderWindow *renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);

  vtkRenderWindowInteractor *renderInteractor = vtkRenderWindowInteractor::New();
  renderInteractor->SetRenderWindow(renderWindow);
  renderInteractor->Initialize();
  renderInteractor->CreateRepeatingTimer(30);

  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1);
  renderer->ResetCamera();

  vtkTimerCallback *timer = vtkTimerCallback::New();
  timer->SetImageData(imageData);
  timer->SetCapture(capture);
  timer->SetActor(actor);
  timer->SetRenderer(renderer);
  timer->SetRenderWindow(renderWindow);
  renderInteractor->AddObserver(vtkCommand::TimerEvent, timer);


  renderInteractor->Start();

 // cvReleaseCapture(&capture);
    capture.release();
  return 0;
}

//!IplImage to vtkImageData
/**
 * Transforms data from cv::Mat to vtkImageData. It assumes
 * that parameters are correctly initialized.
 *
 * \param src OpenCV matrix representing the image data (source)
 *
 * \param dst vtkImageData representing the image (destiny)
 *
 */
void Ipl2VTK(cv::Mat &src, vtkImageData *dst)
{

  vtkImageImport *importer = vtkImageImport::New();

  importer->SetOutput( dst );
  importer->SetDataSpacing( 1, 1, 1 );
  importer->SetDataOrigin( 0, 0, 0 );
  importer->SetWholeExtent(   0, src.size().width-1, 0, src.size().height-1, 0, 0 );
  importer->SetDataExtentToWholeExtent();
  importer->SetDataScalarTypeToUnsignedChar();
  importer->SetNumberOfScalarComponents( src.channels() );
  importer->SetImportVoidPointer( src.data );
  importer->Update();

  return;
}
