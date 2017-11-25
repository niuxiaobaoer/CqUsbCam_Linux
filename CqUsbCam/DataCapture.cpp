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

#include "DataCapture.h"
#include <unistd.h>
#include <stdio.h>
#include "Types.h"


CDataCapture::CDataCapture(const cq_uint32_t iWidth, const cq_uint32_t iHeight)
{

    m_iCount=0;

    m_pInData=NULL;
    m_pOutData=NULL;
    m_pReadBuff=NULL;

    m_bCapture=false;

    m_lRecvByteCnt=0;
    m_lRecvFrameCnt=0;

	m_iWidth=iWidth;
	m_iHeight=iHeight;


    cq_int32_t ret =pthread_mutex_init(&m_mutexThread, NULL);
    if(ret!=0)
        printf("pthread_mutex_init failed");

}

cq_int32_t CDataCapture::Open()
{
	m_iCount=0;		//数据计数器

	m_pOutData=NULL;
	try
	{
		m_pOutData=new cq_uint8_t[m_iWidth*m_iHeight+512];
	}
	catch(const bad_alloc& e)
	{
		printf("Alloc mem exception, try agian\n");
		return -1;
	}

	m_pInData=NULL;
	try
	{
		m_pInData=new cq_uint8_t[(m_iWidth*m_iHeight+512)*4];
	}
	catch(const bad_alloc& e)
	{
		printf("Alloc mem exception, try agian\n");
		return -1;
	}

	m_pReadBuff=NULL;
	try
	{
		m_pReadBuff=new cq_uint8_t[m_iWidth*m_iHeight+512];
	}
	catch(const bad_alloc& e)
	{
		printf("Alloc mem exception, try agian\n");
		return -1;
	}

	m_pInputframe=NULL;
	try
	{
		m_pInputframe=new CImgFrame(m_iWidth,m_iHeight,0);//added by qbc
	}
	catch(const bad_alloc& e)
	{
		printf("Alloc mem exception, try agian\n");
		return -1;
	}
	

	memset(m_pInData,0,(m_iWidth*m_iHeight+512)*4*sizeof(cq_byte_t));
	memset(m_pOutData,0,(m_iWidth*m_iHeight+512)*sizeof(cq_byte_t));
	memset(m_pReadBuff,0,(m_iWidth*m_iHeight+512)*sizeof(cq_byte_t));
	
	//memset(m_pInputframe,0,sizeof(CImgFrame));

	/*int ret = */pthread_create( &m_pThread, NULL, ThreadAdapter, this );

	m_bCapture=true;

	return 0;
}
cq_int32_t CDataCapture::Close()
{
    if(m_bCapture==false)
        return -1;

	m_bCapture=false;

    pthread_mutex_lock(&m_mutexThread);

    if(m_pInputframe!=NULL)
    {
        delete m_pInputframe;//added byq qbc
        m_pInputframe=NULL;
    }
    if(m_pOutData!=NULL)
    {
        delete[] m_pOutData;
        m_pOutData=NULL;
    }
    if(m_pInData!=NULL)
    {
        delete[] m_pInData;
        m_pInData=NULL;
    }
    if(m_pReadBuff!=NULL)
    {
        delete[] m_pReadBuff;
        m_pInData=NULL;
    }
    pthread_mutex_unlock(&m_mutexThread);

    return 0;
}
void* CDataCapture::ThreadAdapter(void* __this)
{
	CDataCapture* _this=(CDataCapture*) __this;
	_this->ThreadFunc();	

	return NULL;
}

void CDataCapture::ThreadFunc()
{
	cq_int32_t transferred=0;
		
	libusb_device * dev = libusb_get_device(m_pUsbHandle);
	libusb_config_descriptor *config_descriptor_in; 
	libusb_get_config_descriptor(dev, 0, &config_descriptor_in);
	//printf("endpt addr is %d\n", config_descriptor_in->interface->altsetting->endpoint->bEndpointAddress);
/*	
	unsigned char chData[2]={'0','0'};
	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x0, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x1, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x2, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x3, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x4, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x5, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x6, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);

	libusb_get_descriptor(m_pUsbHandle,LIBUSB_DT_ENDPOINT, 0x7, chData, 2);
	printf("chData[0]=%x, chData[1]=%x\n", chData[0], chData[1]);
*/
	unsigned char endpoint=(0x01==config_descriptor_in->interface->altsetting->endpoint->bEndpointAddress?0x81:0x86);

	pthread_mutex_lock(&m_mutexThread);
	while (true==m_bCapture)
	{

        cyusb_bulk_transfer(m_pUsbHandle, endpoint/*3.0 0x81, 2.0 0x86*/, m_pReadBuff, m_iWidth*m_iHeight+512, &transferred,100);
        if(transferred>0)
		{
		    Input(m_pReadBuff,transferred);
		    m_lRecvByteCnt+=transferred;
		}
		usleep(1);//deleted by qbc
	}
	pthread_mutex_unlock(&m_mutexThread);

}
cq_int32_t CDataCapture::Input(const cq_uint8_t* lpData, const cq_uint32_t dwSize )
{

    cq_uint32_t iBytes=0;
    iBytes=dwSize+m_iCount;//m_iCount上一次拷贝剩余数据
    cq_bool_t b_header=false/*, b_imu=false*/;
    cq_uint32_t datalen=m_iWidth*m_iHeight+16;// 16 added by qbc

    memcpy(m_pInData+m_iCount,lpData,dwSize);

    for(cq_uint32_t i=0;i<iBytes;++i)
    {
        
        if ((i + datalen) >= iBytes)
        {
            m_iCount = iBytes - i;
            memcpy(m_pInData, m_pInData + i, m_iCount);
            return 0;
        }

        if(m_pInData[i]==0x33&&m_pInData[i+1]==0xcc&&m_pInData[i+14]==0x22&&m_pInData[i+15]==0xdd&&b_header==false)
        {
            i=i+16;
            memcpy(m_pOutData,m_pInData+i,datalen);          

            memcpy(m_pInputframe->m_imgBuf,m_pOutData,m_iWidth*m_iHeight);

            m_pImgQueue->add(m_pInputframe);

            m_lRecvFrameCnt++;

            usleep(1);
            }

    }
    return 0;
}

void CDataCapture::SetImgQueue(wqueue<CImgFrame*>  *pImgQueue)
{
	m_pImgQueue=pImgQueue;
}
void CDataCapture::SetUsbHandle(cyusb_handle *pUsbHandle)
{
	m_pUsbHandle=pUsbHandle;
}


