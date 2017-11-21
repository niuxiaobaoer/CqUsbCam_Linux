/****************************************************************************************\
 * This is the implention file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        Oct 12, 2017                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, Oct 12, 2017                                  						*
 *       Add documentation.                                          				*
 *                      															*
 *                                                                                	*
 \***************************************************************************************/

#include "DataProcess.h"


//#define TEST
#ifdef TEST
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

CDataProcess::CDataProcess()
{
    m_bProcess=false;

    pthread_mutex_init(&m_mutexThread, NULL);
    //cv::namedWindow("disp",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
}

void CDataProcess::Open()
{
	pthread_create( &m_pThread, NULL, ThreadAdapter, this );
	m_bProcess=true;
}
void CDataProcess::Close()
{
    m_bProcess=false;

    m_pImgQueue->reset();

    pthread_mutex_lock(&m_mutexThread); 
    /*
	cv::destroyWindow("disp");
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);
    cv::waitKey(1);
	*/
    pthread_mutex_unlock(&m_mutexThread);

}

void CDataProcess::SetImgQueue(wqueue<CImgFrame*>  *pImgQueue)
{
		m_pImgQueue=pImgQueue;
}
void CDataProcess::SetCallBackFunc(callback_t CallBackFunc)
{
	m_CallBackFunc=CallBackFunc;
}

void* CDataProcess::ThreadAdapter(void* __this)
{
	CDataProcess* _this=(CDataProcess*) __this;
	_this->ThreadFunc();	

	return NULL;
}
/*test*****************************************/
#ifdef TEST
int g_flag=0;
void Disp(void* frameData)
{
    //g_pData=(unsigned char*)frameData;
    if(g_flag==1)
        memset(frameData,0,1280*720);
    //frame.data=(unsigned char*)frameData;
    cv::Mat frame(720, 1280, CV_8UC1, (unsigned char*)frameData);


    cv::imshow("disp",frame);
    cv::waitKey(1);
    //cv::waitKey(1);
}
#endif
/*****************************************************/
void CDataProcess::ThreadFunc()
{

	//cv::namedWindow("disp",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);

	CImgFrame* imgfrm;//be careful of this*********************
	
	pthread_mutex_lock(&m_mutexThread);
	
	while(true==m_bProcess)
	{
		imgfrm=m_pImgQueue->remove();
        if(false==m_bProcess)
			break;

		//m_CallBackFunc(imgfrm->m_imgBuf);
#ifdef TEST
		Disp(imgfrm->m_imgBuf);
#else
        m_CallBackFunc(imgfrm->m_imgBuf);
#endif

		usleep(10);	
    }
    pthread_mutex_unlock(&m_mutexThread);

}
