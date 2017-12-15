/****************************************************************************************\
 * This is the main file for the Caiqi usb camera for Linux/Mac						*
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

#include "CqUsbCam.h"
#include "Types.h"
#include "./sensors/AR0135.h"
#include "./sensors/MT9V034.h"
#include <assert.h>



#define ERR_ITF_NOT_CLAIMED 		-0x80
#define ERR_ITF_IS_ALREADY_CLAIMED	-0x81
#define ERR_NULL_FUNC_POINTER		-0x82

#define ERR_IS_CAPTURING		-0x83
#define ERR_IS_NOT_CAPTURING	-0x84

CCqUsbCam::CCqUsbCam()
{
	m_pUsbHandle=NULL;

 	m_pImgQueue=NULL;
	m_pDataCap=NULL;
	m_pDataProc=NULL;

	m_bIsInterfaceClaimed=false; 		
	m_bIsCapturing=false;

	RegisterSensor_AR0135(m_sensorList);
	RegisterSensor_MT9V034(m_sensorList);
}
cq_int32_t  CCqUsbCam::SelectSensor(string strSensorName)
{

	list<tagSensor>::iterator i;      

	if(0==m_sensorList.size())
		return -1;

    for (i = m_sensorList.begin(); i != m_sensorList.end(); ++i)   
    {
		if(strSensorName==(*i).name)
		{
			m_sensorInUse=(*i);
			return 0;
		}
	}
	return -2;
}

cq_int32_t CCqUsbCam::OpenUSB()
{

	cq_int32_t usbCnt=cyusb_open();

	if(usbCnt<0)
		return usbCnt;
	if(usbCnt==0)
	{
		cyusb_close();
		return usbCnt;
	}  

	return usbCnt;

}

cq_int32_t CCqUsbCam::CloseUSB()
{
    cyusb_close();
    return 0;  
}

cq_int32_t CCqUsbCam::ClaimInterface(const cq_int32_t usbNum)
{
	if(true == m_bIsInterfaceClaimed)
		return ERR_ITF_IS_ALREADY_CLAIMED;

	m_pUsbHandle=NULL;
	m_pUsbHandle=cyusb_gethandle(usbNum);
	if(m_pUsbHandle==NULL)
	{
		cyusb_close();
		return -3;
	}

	cq_int32_t r=cyusb_kernel_driver_active(m_pUsbHandle,0);
	if(r!=0)
	{
		cyusb_close();
		return -4;
	}

	r = cyusb_claim_interface(m_pUsbHandle, 0);
	if ( r != 0 )
	{
		cyusb_close();
		return -5;
	}

	m_bIsInterfaceClaimed=true;
	return 0;
}

cq_int32_t CCqUsbCam::ReleaseInterface()
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

	StopCap();//in case StopCap has not been called

    cq_int32_t r = cyusb_release_interface(m_pUsbHandle, 0);
    if(r!=0)
		return -1;

	m_bIsInterfaceClaimed=false;
    return 0;  
	
}

cq_int32_t  CCqUsbCam::InitSensor()
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.InitSensor)
		return ERR_NULL_FUNC_POINTER;

	return m_sensorInUse.InitSensor(m_pUsbHandle);
}


cq_int32_t  CCqUsbCam::StartCap(const cq_uint32_t iHeight, const cq_uint32_t iWidth, callback_t CallBackFunc)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

	if(true ==m_bIsCapturing)
		return ERR_IS_CAPTURING;

	m_pImgQueue=new wqueue<CImgFrame*>;
	m_pDataCap=new CDataCapture(iWidth, iHeight);
	m_pDataProc=new CDataProcess();

	assert(NULL!=m_pImgQueue);
	assert(NULL!=m_pDataCap);
	assert(NULL!=m_pDataProc);


    m_pDataCap->SetUsbHandle(m_pUsbHandle);
    m_pDataCap->SetImgQueue(m_pImgQueue);

	m_pDataProc->SetCallBackFunc(CallBackFunc);
    m_pDataProc->SetImgQueue(m_pImgQueue);


    m_pDataCap->Open();
	m_pDataProc->Open();

    if(NULL==m_sensorInUse.StartCap)
		return ERR_NULL_FUNC_POINTER;
	m_sensorInUse.StartCap(m_pUsbHandle);

	m_bIsCapturing=true;
	return 0;
}


cq_int32_t  CCqUsbCam::StopCap()
{
	if(true!=m_bIsCapturing)
		return ERR_IS_NOT_CAPTURING;

    m_pDataCap->Close();
	m_pDataProc->Close();

	if(NULL!=m_pImgQueue)	delete m_pImgQueue;
	if(NULL!=m_pDataCap)	delete m_pDataCap;
	if(NULL!=m_pDataProc)	delete m_pDataProc;

    if(NULL==m_sensorInUse.StopCap)
		return ERR_NULL_FUNC_POINTER;
	m_sensorInUse.StopCap(m_pUsbHandle);
	
	m_bIsCapturing=false;
	return 0;
}



cq_int32_t CCqUsbCam::GetUsbSpeed(cq_uint32_t &chSpeedType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

	cyusb_device *h_dev = NULL;
	h_dev=libusb_get_device(m_pUsbHandle);
	if(h_dev==NULL)
		return -1;
	chSpeedType=libusb_get_device_speed(h_dev);
	//unsigned char buf[100];
	//memset(buf,0,100);
    //int ret=libusb_get_descriptor(m_pUsbHandle,	/*uint8_t desc_type*/LIBUSB_DT_ENDPOINT, /*uint8_t desc_index*/0, /*unsigned char *data*/buf, /*int length*/100);
    //ret=libusb_get_descriptor(m_pUsbHandle,	/*uint8_t desc_type*/LIBUSB_DT_ENDPOINT, /*uint8_t desc_index*/1, /*unsigned char *data*/buf, /*int length*/100);
    //ret=libusb_get_descriptor(m_pUsbHandle,	/*uint8_t desc_type*/LIBUSB_DT_ENDPOINT, /*uint8_t desc_index*/2, /*unsigned char *data*/buf, /*int length*/100);

	return 0;
}


cq_int32_t CCqUsbCam::SetAnalogGain(const cq_uint32_t chTrigType, const cq_uint32_t chGainType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetAnalogGain)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetAnalogGain(m_pUsbHandle, chTrigType, chGainType);
}

cq_int32_t CCqUsbCam::SetFpgaTrigFreq(const cq_uint32_t iFreqVal)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetFpgaTrigFreq)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetFpgaTrigFreq(m_pUsbHandle, iFreqVal);
}

cq_int32_t CCqUsbCam::SetTrigMode(const cq_uint32_t chTrigType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetTrigMode)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetTrigMode(m_pUsbHandle, chTrigType);
}

cq_int32_t CCqUsbCam::SetExpoValue(const cq_uint32_t iExpoVal)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetExpoValue)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetExpoValue(m_pUsbHandle, iExpoVal);
}

cq_int32_t CCqUsbCam::SetGainValue(const cq_uint32_t iGainVal)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetGainValue)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetGainValue(m_pUsbHandle, iGainVal);
}

cq_int32_t CCqUsbCam::SetAutoGainExpo(const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetAutoGainExpo)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetAutoGainExpo(m_pUsbHandle, bIsAutoGain, bIsAutoExpo);
}

cq_int32_t CCqUsbCam::SetResolution(const cq_uint32_t chResoluType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetResolution)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetResolution(m_pUsbHandle, chResoluType);
}

cq_int32_t CCqUsbCam::SetMirrorType(const cq_uint32_t chMirrorType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetMirrorType)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetMirrorType(m_pUsbHandle, chMirrorType);
}

cq_int32_t CCqUsbCam::SetBitDepth(const cq_uint32_t chBitDepthType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SetBitDepth)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SetBitDepth(m_pUsbHandle, chBitDepthType);
}

cq_int32_t CCqUsbCam::SendUsbSpeed2Fpga(const cq_uint32_t chSpeedType)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.SendUsbSpeed2Fpga)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.SendUsbSpeed2Fpga(m_pUsbHandle, chSpeedType);
}

cq_int32_t CCqUsbCam::WrSensorReg(const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.WrSensorReg)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.WrSensorReg(m_pUsbHandle, iAddr, iValue);
}

cq_int32_t CCqUsbCam::RdSensorReg(const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.RdSensorReg)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.RdSensorReg(m_pUsbHandle, iAddr, iValue);
}

cq_int32_t CCqUsbCam::WrFpgaReg(const cq_uint32_t 	iAddr, const cq_uint32_t iValue)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.WrFpgaReg)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.WrFpgaReg(m_pUsbHandle, iAddr, iValue);
}

cq_int32_t CCqUsbCam::RdFpgaReg(const cq_uint32_t 	iAddr, cq_uint32_t &iValue)
{
	if(false == m_bIsInterfaceClaimed)
		return ERR_ITF_NOT_CLAIMED;

    if(NULL==m_sensorInUse.RdFpgaReg)
		return ERR_NULL_FUNC_POINTER;
	return m_sensorInUse.RdFpgaReg(m_pUsbHandle, iAddr, iValue);
}


void CCqUsbCam::GetRecvByteCnt(cq_uint64_t& byteCnt)
{
	byteCnt=m_pDataCap->m_lRecvByteCnt;
}
void CCqUsbCam::ClearRecvByteCnt()
{
	m_pDataCap->m_lRecvByteCnt=0;
}
void CCqUsbCam::GetRecvFrameCnt(cq_uint64_t& frameCnt)
{
	frameCnt=m_pDataCap->m_lRecvFrameCnt;
}
void CCqUsbCam::ClearRecvFrameCnt()
{
	m_pDataCap->m_lRecvFrameCnt=0;
}







