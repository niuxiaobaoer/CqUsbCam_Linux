#include <QApplication>
#include "mainwindow.h"


#include "../../CqUsbCam.h"
#include "../../Types.h"
#include "../../ImgFrame.h"

#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
