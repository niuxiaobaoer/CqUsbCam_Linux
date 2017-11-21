#ifndef _DATAPROCESS_H_
#define _DATAPROCESS_H_

/****************************************************************************************\
 * This is the header file for the Caiqi usb camera for Linux/Mac					*
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
 

#include "ImgFrame.h"
#include "wqueue.h"
#include "Types.h"



#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class CDataProcess
{


public:
	CDataProcess();



	void Open();
	void Close();

	void SetImgQueue(wqueue<CImgFrame*>  *pImgQueue);
	void SetCallBackFunc(callback_t CallBackFunc);	

	void ThreadFunc();
	static void* ThreadAdapter(void* __this);




	
private:
	pthread_mutex_t m_mutexThread;
	pthread_t m_pThread; 
	
 	wqueue<CImgFrame*>  *m_pImgQueue;	//

	cq_bool_t m_bProcess;
	callback_t m_CallBackFunc;
	
};


 #endif //_DATAPROCESS_H_
