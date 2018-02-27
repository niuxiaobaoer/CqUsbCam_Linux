/****************************************************************************************\
 * This is the file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        Feb 4, 2018                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, Feb 4, 2018                                   						*
 *       Add documentation.                                          				*
 *                      															*
 *                                                                                	*
 \***************************************************************************************/
#include "AR0144.h"
#include "../SensorCapbablity.h"



static cq_int32_t AR0144_WrSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint16_t tempValue= iValue&0xffff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf1, tempValue, tempAddr, data, 2, 100);
}


static cq_int32_t AR0144_RdSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint8_t tempValue[2]={'0'};
	cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf2,0x0, tempAddr, tempValue, 2, 100);
	iValue = tempValue[0] << 8;
	iValue += tempValue[1];
	return r;
}


static cq_int32_t AR0144_WrFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue= iValue&0xff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf3, tempValue, tempAddr, data, 1, 100);
}


static cq_int32_t AR0144_RdFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue[1]={'0'};
    cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf4, 0x0, tempAddr, tempValue, 1, 100);
	iValue=tempValue[0];
	return r;
}

#define FIRMWARE_NEW
static cq_int32_t AR0144_InitSensor(cyusb_handle *pUsbHandle)
{ 
#ifdef FIRMWARE_NEW
	AR0144_WrSensorReg(pUsbHandle, 0x302A,0x0006);
	AR0144_WrSensorReg(pUsbHandle, 0x302C,0x0002);
	AR0144_WrSensorReg(pUsbHandle, 0x302E,0x0008);
	AR0144_WrSensorReg(pUsbHandle, 0x3030,0x00C6);
	AR0144_WrSensorReg(pUsbHandle, 0x3036,0x000C);
	AR0144_WrSensorReg(pUsbHandle, 0x3038,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x0038);
	AR0144_WrSensorReg(pUsbHandle, 0x31AE,0x0200);
	AR0144_WrSensorReg(pUsbHandle, 0x3002,0x0028);
	AR0144_WrSensorReg(pUsbHandle, 0x3004,0x0004);
	AR0144_WrSensorReg(pUsbHandle, 0x3006,0x02F7);
	AR0144_WrSensorReg(pUsbHandle, 0x3008,0x0503);
	AR0144_WrSensorReg(pUsbHandle, 0x300A,0x033F);
	AR0144_WrSensorReg(pUsbHandle, 0x300C,0x05D0);
	AR0144_WrSensorReg(pUsbHandle, 0x3012,0x01F2);
	AR0144_WrSensorReg(pUsbHandle, 0x31AC,0x0C0C);
	AR0144_WrSensorReg(pUsbHandle, 0x31C6,0x8000);
	AR0144_WrSensorReg(pUsbHandle, 0x306E,0x9010);
	AR0144_WrSensorReg(pUsbHandle, 0x30A2,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x30A6,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x3082,0x0003);
	AR0144_WrSensorReg(pUsbHandle, 0x3084,0x0003);
	AR0144_WrSensorReg(pUsbHandle, 0x308C,0x0028);
	AR0144_WrSensorReg(pUsbHandle, 0x308A,0x0004);
	AR0144_WrSensorReg(pUsbHandle, 0x3090,0x02F7);
	AR0144_WrSensorReg(pUsbHandle, 0x308E,0x0503);
	AR0144_WrSensorReg(pUsbHandle, 0x30AA,0x02EB);
	AR0144_WrSensorReg(pUsbHandle, 0x303E,0x0678);
	AR0144_WrSensorReg(pUsbHandle, 0x3016,0x02EA);
	AR0144_WrSensorReg(pUsbHandle, 0x30AE,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x30A8,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x3040,0x0000);
	AR0144_WrSensorReg(pUsbHandle, 0x31D0,0x0001);
	AR0144_WrSensorReg(pUsbHandle, 0x301A,0x30DC);
    	AR0144_WrSensorReg(pUsbHandle, 0x3270,0x0100);

    return 0;
#endif // FIRMWARE_NEW
}


static cq_int32_t AR0144_SetAnalogGain(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType, const cq_uint32_t chGainType)
{
	cq_int32_t r=0;
	switch(chTrigType) 
	{   
		case TRIGMODE_AUTO:
		{
			switch(chGainType)
			{
				case ANALOGGAIN_1X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x0080);
					break;
				case ANALOGGAIN_2X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x0090);
					break;
				case ANALOGGAIN_4X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x00A0);
					break;
				case ANALOGGAIN_8X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x00B0);
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
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x0480);
					break;
				case ANALOGGAIN_2X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x0490);
					break;
				case ANALOGGAIN_4X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x04A0);
					break;
				case ANALOGGAIN_8X:
					r=AR0144_WrSensorReg(pUsbHandle, 0x30B0,0x04B0);
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


static cq_int32_t AR0144_SetFpgaTrigFreq(cyusb_handle *pUsbHandle, const cq_uint32_t iFreqVal)
{
     cq_int32_t r=AR0144_WrFpgaReg(pUsbHandle, 0x05, iFreqVal);
     return r;
}

static cq_int32_t AR0144_SetTrigMode(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType)
{
	switch(chTrigType)
	{
		case TRIGMODE_AUTO:
			AR0144_WrSensorReg(pUsbHandle, 0x30B0, 0x0080);
			AR0144_WrSensorReg(pUsbHandle, 0x301A, 0x10DC);
			AR0144_WrFpgaReg(pUsbHandle, 0x00, 0x00);
			break;
		case TRIGMODE_FPGA:
			AR0144_WrSensorReg(pUsbHandle, 0x30B0, 0x0480);
			AR0144_WrSensorReg(pUsbHandle, 0x301A, 0x19D8);
			AR0144_WrFpgaReg(pUsbHandle, 0x00, 0x01);
			AR0144_WrFpgaReg(pUsbHandle, 0x05, 0x19);// 0x01 by default
			break;
		default:
			break;
	}
	return 0;

}


static cq_int32_t AR0144_SetExpoValue(cyusb_handle *pUsbHandle, const cq_uint32_t iExpoVal)
{
	cq_int32_t r=AR0144_WrSensorReg(pUsbHandle, 0x3012, iExpoVal);
	return r;

}

static cq_int32_t AR0144_SetGainValue(cyusb_handle *pUsbHandle, const cq_uint32_t iGainVal)
{
    cq_int32_t r=AR0144_WrSensorReg(pUsbHandle, 0x305E, iGainVal);
    return r;
}

static cq_int32_t AR0144_SetAutoGainExpo(cyusb_handle *pUsbHandle, const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo)
{
    if((bIsAutoGain==true)&&(bIsAutoExpo==true))
       return AR0144_WrSensorReg(pUsbHandle, 0x3100, 0x0003);

    if ((bIsAutoGain==true)&&(bIsAutoExpo==false))
       return AR0144_WrSensorReg(pUsbHandle, 0x3100, 0x0002);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==true))
       return AR0144_WrSensorReg(pUsbHandle, 0x3100, 0x0001);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==false))
       return AR0144_WrSensorReg(pUsbHandle, 0x3100, 0x0000);
	return -99;//should never reach here
}



static cq_int32_t AR0144_SetResolution(cyusb_handle *pUsbHandle, const cq_uint32_t chResoluType)
{
	switch(chResoluType)
	{
		case RESOLU_1280_720:
			//1280*720
			AR0144_WrSensorReg(pUsbHandle,0x3002, 0x0028);		//Y_ADDR_START = 40
			AR0144_WrSensorReg(pUsbHandle,0x3004, 0x0004);		//X_ADDR_START = 4
			AR0144_WrSensorReg(pUsbHandle,0x3006, 0x02F7);		//Y_ADDR_END = 759
			AR0144_WrSensorReg(pUsbHandle,0x3008, 0x0503);		//X_ADDR_END = 1283
			AR0144_WrSensorReg(pUsbHandle,0x300A, 0x02EB);		//FRAME_LENGTH_LINES = 747
			AR0144_WrSensorReg(pUsbHandle,0x300C, 0x0678);		//LINE_LENGTH_PCK = 1656
			AR0144_WrSensorReg(pUsbHandle,0x3012, 0x02EA);		//COARSE_INTEGRATION_TIME = 746


			AR0144_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			AR0144_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			AR0144_WrFpgaReg(pUsbHandle, 0x03, 0x02);
			AR0144_WrFpgaReg(pUsbHandle, 0x04, 0xd0);
			AR0144_WrFpgaReg(pUsbHandle, 0x06, 0x00);
			break;
		case RESOLU_1280_800:
			//1280*800
			AR0144_WrSensorReg(pUsbHandle,0x3002, 0x0000);		//Y_ADDR_START = 0
			AR0144_WrSensorReg(pUsbHandle,0x3004, 0x0004);		//X_ADDR_START = 4
			AR0144_WrSensorReg(pUsbHandle,0x3006, 0x031F);		//Y_ADDR_END = 799
			AR0144_WrSensorReg(pUsbHandle,0x3008, 0x0503);		//X_ADDR_END = 1283
			AR0144_WrSensorReg(pUsbHandle,0x300A, 0x033B);		//FRAME_LENGTH_LINES = 827
			AR0144_WrSensorReg(pUsbHandle,0x300C, 0x05D8);		//LINE_LENGTH_PCK = 1496
			AR0144_WrSensorReg(pUsbHandle,0x3012, 0x033A);		//COARSE_INTEGRATION_TIME = 826

			AR0144_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			AR0144_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			AR0144_WrFpgaReg(pUsbHandle, 0x03, 0x03);
			AR0144_WrFpgaReg(pUsbHandle, 0x04, 0x20);
			AR0144_WrFpgaReg(pUsbHandle, 0x06, 0x00);
			break;
		case RESOLU_640_480_SKIP:	
			AR0144_WrSensorReg(pUsbHandle, 0x3030, 0x002A);
			AR0144_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			AR0144_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0144_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			AR0144_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0144_WrSensorReg(pUsbHandle, 0x300A, 0x01FB);
			AR0144_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0144_WrSensorReg(pUsbHandle, 0x30A6, 0x0003);
			AR0144_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			AR0144_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			AR0144_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			AR0144_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			AR0144_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			AR0144_WrFpgaReg(pUsbHandle, 0x06, 0x01);					
			break;
		case RESOLU_640_480_BIN:
			AR0144_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			AR0144_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			AR0144_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0144_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			AR0144_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0144_WrSensorReg(pUsbHandle, 0x300A, 0x03FB);
			AR0144_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0144_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			AR0144_WrSensorReg(pUsbHandle, 0x3032, 0x0002);// ¿ªÆôbinningÄ£Êœ

			AR0144_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			AR0144_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			AR0144_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			AR0144_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			AR0144_WrFpgaReg(pUsbHandle, 0x06, 0x00);
		default:
			break;
	}
	return 0;
		

}


static cq_int32_t AR0144_SetMirrorType(cyusb_handle *pUsbHandle, const cq_uint32_t chMirrorType)
{
	if(MIRROR_NORMAL==chMirrorType)	//normal
		return AR0144_WrSensorReg(pUsbHandle, 0x3040, 0x0000);

	if(MIRROR_X==chMirrorType)//X
		return AR0144_WrSensorReg(pUsbHandle, 0x3040, 0x4000);

	if(MIRROR_Y==chMirrorType)//Y
		return AR0144_WrSensorReg(pUsbHandle, 0x3040, 0x8000);

	if(MIRROR_XY==chMirrorType)//XY
		return AR0144_WrSensorReg(pUsbHandle, 0x3040, 0xC000);

	return -99;//should never reach here

}


static cq_int32_t AR0144_SetBitDepth(cyusb_handle *pUsbHandle, const cq_uint32_t chBitDepthType)
{
	if(BITDEPTH_8==chBitDepthType)	//8
		return AR0144_WrFpgaReg(pUsbHandle, 0x7, 0x0);

	if(BITDEPTH_16==chBitDepthType)//16
		return AR0144_WrFpgaReg(pUsbHandle, 0x7, 0x1);

	if(BITDEPTH_16_2==chBitDepthType)//16
		return AR0144_WrFpgaReg(pUsbHandle, 0x7, 0x2);

	return -99;//should never reach here

}


static cq_int32_t AR0144_StartCap(cyusb_handle *pUsbHandle)
{
	AR0144_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	AR0144_WrFpgaReg(pUsbHandle, 0x09, 0x01);
	return 0;	
}

static cq_int32_t AR0144_StopCap(cyusb_handle *pUsbHandle)
{
	AR0144_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	return 0;
}

static cq_int32_t AR0144_SendUsbSpeed2Fpga(cyusb_handle *pUsbHandle, const cq_uint32_t chSpeedType)
{
	AR0144_WrFpgaReg(pUsbHandle, 0x08, chSpeedType);
	return 0;
}


/*
static tagSensor sensor_AR0144=
{
	.name="AR0144",
	.iicAddr=0xf0,
	.regAddrLen=2,
	.regValueLen=2,

	.InitSensor		=	AR0144_InitSensor,
	.WrSensorReg	=	AR0144_WrSensorReg,
	.RdSensorReg	=	AR0144_RdSensorReg,
	.WrFpgaReg		=	AR0144_WrFpgaReg,
	.RdFpgaReg		=	AR0144_RdFpgaReg,
	.SetAnalogGain	=	AR0144_SetAnalogGain,
	.SetFpgaTrigFreq=	AR0144_SetFpgaTrigFreq,
	.SetTrigMode	=	AR0144_SetTrigMode,
	.SetExpoValue	=	AR0144_SetExpoValue,
	.SetGainValue	=	AR0144_SetGainValue,
	.SetAutoGainExpo=	AR0144_SetAutoGainExpo,
	.SetResolution	=	AR0144_SetResolution,
	.SetMirrorType	=	AR0144_SetMirrorType,
	.SetBitDepth	=	AR0144_SetBitDepth,
	.StartCap 		=	AR0144_StartCap,
	.StopCap 		=	AR0144_StopCap,
	.SendUsbSpeed2Fpga=	AR0144_SendUsbSpeed2Fpga
};
*/

static tagSensor sensor_AR0144=
{
		.name="AR0144",
	.iicAddr=0xf0,
	.regAddrLen=2,
	.regValueLen=2,

	.InitSensor		=	AR0144_InitSensor,
	.WrSensorReg	=	AR0144_WrSensorReg,
	.RdSensorReg	=	AR0144_RdSensorReg,
	.WrFpgaReg		=	AR0144_WrFpgaReg,
	.RdFpgaReg		=	AR0144_RdFpgaReg,
	.SetAnalogGain	=	AR0144_SetAnalogGain,
	.SetFpgaTrigFreq=	AR0144_SetFpgaTrigFreq,
	.SetTrigMode	=	AR0144_SetTrigMode,
	.SetExpoValue	=	AR0144_SetExpoValue,
	.SetGainValue	=	AR0144_SetGainValue,
	.SetAutoGainExpo=	AR0144_SetAutoGainExpo,
	.SetResolution	=	AR0144_SetResolution,
	.SetMirrorType	=	AR0144_SetMirrorType,
	.SetBitDepth	=	AR0144_SetBitDepth,
	.StartCap 		=	AR0144_StartCap,
	.StopCap 		=	AR0144_StopCap,
	.SendUsbSpeed2Fpga=	AR0144_SendUsbSpeed2Fpga

};

void RegisterSensor_AR0144(list<tagSensor>& sensorList)
{
	sensorList.push_back(sensor_AR0144);
}


