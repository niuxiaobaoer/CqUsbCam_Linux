#ifndef _CQUSBCAM_H_
#define _CQUSBCAM_H_

/****************************************************************************************\
 * This is the main header file for the Caiqi usb camera for Linux/Mac						*
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
#include <stdio.h>
#include <string>
#include <list>

#include "cyusb.h"

#include "Types.h"
#include "ImgFrame.h"
#include "wqueue.h"
#include "DataCapture.h"
#include "DataProcess.h"
#include "tagSensor.h"


 
 class CCqUsbCam
 {
 private:
 	cyusb_handle *m_pUsbHandle;

 	wqueue<CImgFrame*>  *m_pImgQueue;	//
	CDataCapture *m_pDataCap;
	CDataProcess *m_pDataProc;

	list<tagSensor> m_sensorList;
 	tagSensor m_sensorInUse;		
	
	cq_bool_t m_bIsInterfaceClaimed; 		
	cq_bool_t m_bIsCapturing;
	


 public:
 	CCqUsbCam();
	cq_int32_t SelectSensor(string strSensorName);

	cq_int32_t WrSensorReg(const cq_uint32_t iAddr, const cq_uint32_t iValue);
	cq_int32_t RdSensorReg(const cq_uint32_t iAddr, cq_uint32_t & iValue);
	cq_int32_t WrFpgaReg(const cq_uint32_t 	iAddr, const cq_uint32_t iValue);
	cq_int32_t RdFpgaReg(const cq_uint32_t 	iAddr, cq_uint32_t& iValue);

	static cq_int32_t OpenUSB();
	static cq_int32_t CloseUSB();		
	cq_int32_t GetUsbSpeed(cq_uint32_t &chSpeedType);
	cq_int32_t InitSensor();
	cq_int32_t ClaimInterface(const cq_int32_t usbNum);
	cq_int32_t ReleaseInterface();

	cq_int32_t  StartCap(const cq_uint32_t iHeight, const cq_uint32_t iWidth, callback_t CallBackFunc);
	cq_int32_t  StopCap();

	
	cq_int32_t SetAnalogGain(const cq_uint32_t chTrigType, const cq_uint32_t chGainType);
	cq_int32_t SetFpgaTrigFreq(const cq_uint32_t iFreqVal);	
	cq_int32_t SetTrigMode(const cq_uint32_t chTrigType);

	
	cq_int32_t SetExpoValue(const cq_uint32_t iExpoVal);
	cq_int32_t SetGainValue(const cq_uint32_t iGainVal);
	cq_int32_t SetAutoGainExpo(const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo);
	
	
	cq_int32_t SetResolution(const cq_uint32_t chResoluType);
	
	
	cq_int32_t SetMirrorType(const cq_uint32_t chMirrorType);
	cq_int32_t SetBitDepth(const cq_uint32_t chBitDepthType);

	cq_int32_t SendUsbSpeed2Fpga(const cq_uint32_t chSpeedType);

	void GetRecvByteCnt(cq_uint64_t& byteCnt);
	void ClearRecvByteCnt();
	void GetRecvFrameCnt(cq_uint64_t& frameCnt);
	void ClearRecvFrameCnt();

};
 

 
 #endif //_CYUSBCAM_H_
