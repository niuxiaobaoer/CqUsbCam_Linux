#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "../../CqUsbCam.h"
#include "../../Types.h"
#include "../../ImgFrame.h"

#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


CCqUsbCam aa;
CCqUsbCam aa2;


//cv::Mat frame(720, 1280, CV_8UC1);
//cv::Mat frame2(720, 1280, CV_8UC1);
unsigned char* g_pData=NULL;
unsigned char* g_pData2=NULL;

int g_flag=0;
pthread_mutex_t m_mutexThread;
void DDisp(void* frameData)
{
    pthread_mutex_lock(&m_mutexThread);
    //g_pData=(unsigned char*)frameData;
    if(g_flag==1)
        memset(frameData,0,1280*720);
    //frame.data=(unsigned char*)frameData;
    cv::Mat frame(720, 1280, CV_8UC1, (unsigned char*)frameData);


    cv::imshow("disp",frame);
    cv::waitKey(1);
    //cv::waitKey(1);
    pthread_mutex_unlock(&m_mutexThread);
}
void DDisp2(void* frameData)
{
    pthread_mutex_lock(&m_mutexThread);
    //g_pData2=(unsigned char*)frameData;
    if(g_flag==1)
        memset(frameData,0,1280*720);
    //frame2.data=(unsigned char*)frameData;
    cv::Mat frame2(720, 1280, CV_8UC1, (unsigned char*)frameData);


    cv::imshow("disp2",frame2);
    cv::waitKey(1);
    pthread_mutex_unlock(&m_mutexThread);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cq_int32_t ret =pthread_mutex_init(&m_mutexThread, NULL);
    if(ret!=0)
        printf("pthread_mutex_init failed");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    cv::namedWindow("disp",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    cv::namedWindow("disp2",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
    aa.StartCap(720,1280,DDisp);
    aa2.StartCap(720,1280,DDisp2);
}

void MainWindow::on_pushButton_2_clicked()
{
    aa.StopCap();
    aa2.StopCap();
    cv::destroyWindow("disp");
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);


    cv::destroyWindow("disp2");
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    cq_int32_t usbCnt=0;

    usbCnt=CCqUsbCam::OpenUSB();
    if(usbCnt<0)
    {
       QString str= QString(libusb_error_name(usbCnt));
       return;
    }
    aa.ClaimInterface(0);
    aa2.ClaimInterface(1);

}

void MainWindow::on_pushButton_4_clicked()
{
    cq_uint32_t chSpeedType=0;

    aa.InitSensor();
    aa2.InitSensor();
}

void MainWindow::on_pushButton_5_clicked()
{
    on_pushButton_2_clicked();
    aa.ReleaseInterface();
    aa2.ReleaseInterface();
    int ret=CCqUsbCam::CloseUSB();

    if(0!=ret)
    {
       ui->statusBar->showMessage( QString(libusb_error_name(ret)));
           return;
    }
    ret=9;

}

void MainWindow::on_pushButton_6_clicked()//752*480
{

}

void MainWindow::on_pushButton_7_clicked()//640*480
{
    //aa.SetResolution(RESOLU_1280_720);
}

void MainWindow::on_pushButton_8_clicked()
{
    int aaaaa=aa.SelectSensor("AR0135");
    aaaaa=aa2.SelectSensor("AR0135");
    aaaaa=4;
}

void MainWindow::on_pushButton_set_g_flag_clicked()
{
    g_flag=1;
}

void MainWindow::on_pushButton_clear_g_flag_clicked()
{
    g_flag=0;
}
