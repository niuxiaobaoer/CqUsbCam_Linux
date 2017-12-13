/****************************************************************************************\
 * This is the file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        dec 13, 2017                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, dec 13, 2017                                  						*
 *       Add documentation.                                          				*
 *                      															*
 *                                                                                	*
 \***************************************************************************************/

#include <string.h>

#include "MT9M001.h"
#include "../SensorCapbablity.h"


static cq_int32_t MT9M001_WrSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint16_t tempValue= iValue&0xffff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf1, tempValue, tempAddr, data, 2, 100);
}


static cq_int32_t MT9M001_RdSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue[2]={'0'};
	cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf2, 0x0, tempAddr, tempValue, 2, 100);
	iValue = tempValue[0] << 8;
	iValue += tempValue[1];
	return r;
}

static cq_int32_t MT9M001_WrEeprom(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint8_t iValue)
{
	cq_int32_t len=2;
	cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint16_t tempValue= iValue&0xff;
	cyusb_control_write(pUsbHandle,0x40,0xf5,tempValue,tempAddr,data,len,100);
	return len;
}
static cq_int32_t MT9M001_RdEeprom(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint8_t * buffer, cq_int32_t &length)
{
	cq_int32_t transferred=length;
	cq_uint16_t tempAddr= iAddr&0xffff;
    cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf6, 0x0, tempAddr, buffer, transferred, 100);
	return r; 
	
}
static cq_int32_t MT9M001_WrDevID(cyusb_handle *pUsbHandle, cq_uint8_t* chIdBuf, cq_uint32_t &length )
{
	int len=1;
	cq_int32_t r=0;
    //int r= cyusb_control_write(pUsbHandle,0x40,0xd0,0x0,0x0, chIdBuf,len,100);
	length=len;
	return r;
}
static cq_int32_t MT9M001_RdDevID(cyusb_handle *pUsbHandle, cq_uint8_t *chIdBuf, cq_uint32_t &length)
{
	int len=1;
	unsigned char buf[10]={'0'};
    int r= cyusb_control_read(pUsbHandle, 0x40, 0xd1, 0x0, 0x0, buf, len,100);
	memcpy(chIdBuf, buf, len);
	length=len;
	return r;
}

static cq_int32_t MT9M001_WrDevSN(cyusb_handle *pUsbHandle, cq_uint8_t* chSnBuf, cq_uint32_t &length )
{
	int len=4;
    int r= cyusb_control_write(pUsbHandle, 0x40, 0xd2, 0x0, 0x0, chSnBuf, len, 100);
	length=len;
	return r;
}
static cq_int32_t MT9M001_RdDevSN(cyusb_handle *pUsbHandle, cq_uint8_t *chSnBuf, cq_uint32_t &length)
{
	int len=4;
	unsigned char buf[10]={'0'};
    int r= cyusb_control_read(pUsbHandle, 0x40, 0xd3, 0x0, 0x0, buf, len,100);
	memcpy(chSnBuf, buf, len);
	length=len;
	return r;
}
static cq_int32_t MT9M001_WrFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue= iValue&0xff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf3, tempValue, tempAddr, data, 1, 100);
}


static cq_int32_t MT9M001_RdFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue[1]={'0'};
    cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf4, 0x0, tempAddr, tempValue, 1, 100);
	iValue=tempValue[0];
	return r;
}

static cq_int32_t MT9M001_InitSensor(cyusb_handle *pUsbHandle)
{    
   	cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
    return cyusb_control_write(pUsbHandle, 0x40, 0xf0, 0, 0, data, 2, 100);
}


/*
static cq_int32_t MT9M001_SetAnalogGain(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType, const cq_uint32_t chGainType)
{
	cq_int32_t r=0;
	switch(chTrigType) 
	{   
		case TRIGMODE_AUTO:
		{
			switch(chGainType)
			{
				case ANALOGGAIN_1X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x0080);
					break;
				case ANALOGGAIN_2X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x0090);
					break;
				case ANALOGGAIN_4X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x00A0);
					break;
				case ANALOGGAIN_8X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x00B0);
					break;
				default:
					r=-99;
					break;
			}
			break;	
		}
		case TRIGMODE_FPGA:
		{
			switch(chGainType)
			{
				case ANALOGGAIN_1X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x0480);
					break;
				case ANALOGGAIN_2X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x0490);
					break;
				case ANALOGGAIN_4X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x04A0);
					break;
				case ANALOGGAIN_8X:
					r=MT9M001_WrSensorReg(pUsbHandle, 0x30B0,0x04B0);
					break;
				default:
					r=-99;
					break;	
			}
			break;
		}	
		default:
			r=-99;
			break;	
	}
	return r;
}
*/

static cq_int32_t MT9M001_SetFpgaTrigFreq(cyusb_handle *pUsbHandle, const cq_uint32_t iFreqVal)
{
     cq_int32_t r=MT9M001_WrFpgaReg(pUsbHandle, 0x05, iFreqVal&0xff);
     return r;
}

static cq_int32_t MT9M001_SetTrigMode(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType)
{
	switch(chTrigType)
	{
		case TRIGMODE_AUTO:
			MT9M001_WrSensorReg(pUsbHandle, 0x1E, 0x0200);
			MT9M001_WrFpgaReg(pUsbHandle, 0x00, 0x00);
			break;
		case TRIGMODE_FPGA:
			MT9M001_WrSensorReg(pUsbHandle, 0x1E, 0x0300);
			MT9M001_WrFpgaReg(pUsbHandle, 0x00, 0x01);// 0x01 by default
			break;
		case TRIGMODE_SOFT:
			MT9M001_WrSensorReg(pUsbHandle, 0x1E, 0x0300);
			MT9M001_WrFpgaReg(pUsbHandle, 0x00, 0x02);
			break;
		case TRIGMODE_SIGNAL:
			MT9M001_WrSensorReg(pUsbHandle, 0x1E, 0x0300);
			MT9M001_WrFpgaReg(pUsbHandle, 0x00, 0x03);
		default:
			break;
	}
	return 0;

}
static cq_int32_t MT9M001_SoftTrig(cyusb_handle* pUsbHandle)
{	 
	  MT9M001_WrFpgaReg(pUsbHandle, 0x0a, 0x00);
	  MT9M001_WrFpgaReg(pUsbHandle, 0x0a, 0x01);
	  return 0;

}

static cq_int32_t MT9M001_SetExpoValue(cyusb_handle *pUsbHandle, const cq_uint32_t iExpoVal)
{
	cq_int32_t r=MT9M001_WrSensorReg(pUsbHandle, 0x09, iExpoVal&0xffff);
	return r;

}

static cq_int32_t MT9M001_SetGainValue(cyusb_handle *pUsbHandle, const cq_uint32_t iGainVal)
{
    cq_int32_t r=MT9M001_WrSensorReg(pUsbHandle, 0x35, iGainVal&0xffff);
    return r;
}
#if 0 //no such function
static cq_int32_t MT9M001_SetAutoGainExpo(cyusb_handle *pUsbHandle, const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo)
{
    if((bIsAutoGain==true)&&(bIsAutoExpo==true))
       return MT9M001_WrSensorReg(pUsbHandle, 0xAF, 0x0003);

    if ((bIsAutoGain==true)&&(bIsAutoExpo==false))
       return MT9M001_WrSensorReg(pUsbHandle, 0xAF, 0x0002);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==true))
       return MT9M001_WrSensorReg(pUsbHandle, 0xAF, 0x0001);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==false))
       return MT9M001_WrSensorReg(pUsbHandle, 0xAF, 0x0000);
	return -99;//should never reach here
}
#endif


static cq_int32_t MT9M001_SetResolution(cyusb_handle *pUsbHandle, const cq_uint32_t chResoluType)
{
	switch(chResoluType)
	{
		case RESOLU_1280_1024:
			MT9M001_WrSensorReg(pUsbHandle,0x01, 0x000C);
			MT9M001_WrSensorReg(pUsbHandle,0x02, 0x0014);
			MT9M001_WrSensorReg(pUsbHandle,0x03, 0x03FF);
			MT9M001_WrSensorReg(pUsbHandle,0x04, 0x04FF);

			MT9M001_WrFpgaReg(pUsbHandle,0x01, 0x05);
			MT9M001_WrFpgaReg(pUsbHandle,0x02, 0x00);
			MT9M001_WrFpgaReg(pUsbHandle,0x03, 0x04);
			MT9M001_WrFpgaReg(pUsbHandle,0x04, 0x00);   					
			break;
		default:
			break;
	}
	return 0;
		

}


static cq_int32_t MT9M001_SetMirrorType(cyusb_handle *pUsbHandle, const cq_uint32_t chMirrorType)
{
	if(MIRROR_NORMAL==chMirrorType)	//normal
		return MT9M001_WrSensorReg(pUsbHandle, 0x20, 0x0000);

	if(MIRROR_X==chMirrorType)//X
		return MT9M001_WrSensorReg(pUsbHandle, 0x20, 0x6000);

	if(MIRROR_Y==chMirrorType)//Y
		return MT9M001_WrSensorReg(pUsbHandle, 0x20, 0x8000);

	if(MIRROR_XY==chMirrorType)//XY
		return MT9M001_WrSensorReg(pUsbHandle, 0x20, 0xC000);

	return -99;//should never reach here

}

/*
static cq_int32_t MT9M001_SetBitDepth(cyusb_handle *pUsbHandle, const cq_uint32_t chBitDepthType)
{
	if(BITDEPTH_8==chBitDepthType)	//8
		return MT9M001_WrFpgaReg(pUsbHandle, 0x7, 0x0);

	if(BITDEPTH_16==chBitDepthType)//16
		return MT9M001_WrFpgaReg(pUsbHandle, 0x7, 0x1);

	if(BITDEPTH_16_2==chBitDepthType)//16
		return MT9M001_WrFpgaReg(pUsbHandle, 0x7, 0x2);

	return -99;//should never reach here

}

*/
static cq_int32_t MT9M001_StartCap(cyusb_handle *pUsbHandle)
{
	MT9M001_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	MT9M001_WrFpgaReg(pUsbHandle, 0x09, 0x01);
	return 0;	
}

static cq_int32_t MT9M001_StopCap(cyusb_handle *pUsbHandle)
{
	MT9M001_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	return 0;
}

static cq_int32_t MT9M001_SendUsbSpeed2Fpga(cyusb_handle *pUsbHandle, const cq_uint32_t chSpeedType)
{
	MT9M001_WrFpgaReg(pUsbHandle, 0x08, chSpeedType);
	return 0;
}


/*
static tagSensor sensor_MT9M001=
{
	.name="MT9M001",
	.iicAddr=0xf0,
	.regAddrLen=1,
	.regValueLen=2,

	.InitSensor		=	NULL,
	.WrSensorReg	=	MT9M001_WrSensorReg,
	.RdSensorReg	=	MT9M001_RdSensorReg,
	.WrFpgaReg		=	MT9M001_WrFpgaReg,
	.RdFpgaReg		=	MT9M001_RdFpgaReg,
	.SetAnalogGain	=	NULL,
	.SetFpgaTrigFreq=	MT9M001_SetFpgaTrigFreq,
	.SetTrigMode	=	MT9M001_SetTrigMode,
	.SetExpoValue	=	MT9M001_SetExpoValue,
	.SetGainValue	=	MT9M001_SetGainValue,
	.SetAutoGainExpo=	MT9M001_SetAutoGainExpo,
	.SetResolution	=	MT9M001_SetResolution,
	.SetMirrorType	=	MT9M001_SetMirrorType,
	.SetBitDepth	=	NULL,
	.StartCap 		=	MT9M001_StartCap,
	.StopCap 		=	MT9M001_StopCap,
	.SendUsbSpeed2Fpga=	MT9M001_SendUsbSpeed2Fpga,

	.WrEeprom		=	MT9M001_WrEeprom,
	.RdEeprom		=	MT9M001_RdEeprom,
	.WrDevID		=	MT9M001_WrDevID,
	.RdDevID		=	MT9M001_RdDevID,
	.WrDevSN		=	MT9M001_WrDevSN,
	.RdDevSN		=	MT9M001_RdDevSN,
	.SoftTrig		=	MT9M001_SoftTrig
};
*/
static tagSensor sensor_MT9M001=
{
	"MT9M001",
	0xf0,
	1,
	2,

	MT9M001_InitSensor,
	MT9M001_WrSensorReg,
	MT9M001_RdSensorReg,
	MT9M001_WrFpgaReg,
	MT9M001_RdFpgaReg,
	NULL,
	MT9M001_SetFpgaTrigFreq,
	MT9M001_SetTrigMode,
	MT9M001_SetExpoValue,
	MT9M001_SetGainValue,
	NULL,
	MT9M001_SetResolution,
	MT9M001_SetMirrorType,
	NULL,
	MT9M001_StartCap,
	MT9M001_StopCap,
	MT9M001_SendUsbSpeed2Fpga,

	MT9M001_WrEeprom,
	MT9M001_RdEeprom,
	MT9M001_WrDevID,
	MT9M001_RdDevID,
	MT9M001_WrDevSN,
	MT9M001_RdDevSN,
	MT9M001_SoftTrig
};
void RegisterSensor_MT9M001(list<tagSensor>& sensorList)
{
	sensorList.push_back(sensor_MT9M001);

}



