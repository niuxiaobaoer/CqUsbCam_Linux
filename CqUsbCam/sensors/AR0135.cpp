/****************************************************************************************\
 * This is the file for the Caiqi usb camera for Linux/Mac						*
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
#include "AR0135.h"
#include "../SensorCapbablity.h"





static cq_int32_t AR0135_WrSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint16_t tempValue= iValue&0xffff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf1, tempValue, tempAddr, data, 2, 100);
}


static cq_int32_t AR0135_RdSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint8_t tempValue[2]={'0'};
	cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf2,0x0, tempAddr, tempValue, 2, 100);
	iValue = tempValue[0] << 8;
	iValue += tempValue[1];
	return r;
}


static cq_int32_t AR0135_WrFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue= iValue&0xff;
    return cyusb_control_write(pUsbHandle, 0x40, 0xf3, tempValue, tempAddr, data, 1, 100);
}


static cq_int32_t AR0135_RdFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue[1]={'0'};
    cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf4, 0x0, tempAddr, tempValue, 1, 100);
	iValue=tempValue[0];
	return r;
}
#define FIRMWARE_NEW
static cq_int32_t AR0135_InitSensor(cyusb_handle *pUsbHandle)
{    
#ifdef FIRMWARE_NEW
    AR0135_WrSensorReg(pUsbHandle, 0x3028,0x0010);//0		ROW_SPEED = 16
	AR0135_WrSensorReg(pUsbHandle, 0x302A,0x000C);//1		VT_PIX_CLK_DIV = 12   P2   4<=P2<=16
	AR0135_WrSensorReg(pUsbHandle, 0x302C,0x0001);//2		VT_SYS_CLK_DIV = 1    P1   1<=P1<=16
	AR0135_WrSensorReg(pUsbHandle, 0x302E,0x0001);//3		PRE_PLL_CLK_DIV = 2   N    1<=N<=63
	AR0135_WrSensorReg(pUsbHandle, 0x3030,0x0020);//4		PLL_MULTIPLIER = 40   M   32<=M<=255

	AR0135_WrSensorReg(pUsbHandle, 0x3032,0x0000);//5		DIGITAL_BINNING = 0   _BINNING  帧率降一半
	AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x0080);//6		DIGITAL_TEST = 128

	AR0135_WrSensorReg(pUsbHandle, 0x301A,0x00D8);//8		RESET_REGISTER = 216
	AR0135_WrSensorReg(pUsbHandle, 0x301A,0x10DC);//9		RESET_REGISTER = 4316  h10DC    关键寄存器

	AR0135_WrSensorReg(pUsbHandle, 0x3002,0x007C);//10	Y_ADDR_START = 124
	AR0135_WrSensorReg(pUsbHandle, 0x3004,0x0002);//11	X_ADDR_START = 2
	AR0135_WrSensorReg(pUsbHandle, 0x3006,0x034B);//12	Y_ADDR_END = 843
	AR0135_WrSensorReg(pUsbHandle, 0x3008,0x0501);//13	X_ADDR_END = 1281

	AR0135_WrSensorReg(pUsbHandle, 0x300A,0x02FD);//14	FRAME_LENGTH_LINES = 837
	AR0135_WrSensorReg(pUsbHandle, 0x300C,0x056C);//15	LINE_LENGTH_PCK = 1388

	AR0135_WrSensorReg(pUsbHandle, 0x3012,0x0080);//16	COARSE_INTEGRATION_TIME = 252	 h00FC	曝光时间
	AR0135_WrSensorReg(pUsbHandle, 0x3014,0x008D);//17	FINE_INTEGRATION_TIME = 233

	AR0135_WrSensorReg(pUsbHandle, 0x30A6,0x0001);//18	Y_ODD_INC = 1			    SKIP模式

	AR0135_WrSensorReg(pUsbHandle, 0x3040,0x0000);//27	READ_MODE = 0			镜像等

	AR0135_WrSensorReg(pUsbHandle, 0x3064,0x1982);//28	EMBEDDED_DATA_CTRL = 6530    开启输出 两行寄存器值 和  EMBEDDED_DATA  ，如果用AE模式 ，必须得开。图像输出时前两行不读

	AR0135_WrSensorReg(pUsbHandle, 0x3100,0x0003);//30	AE;AG

	AR0135_WrSensorReg(pUsbHandle, 0x305E,0x003C);//29	Total gain

	AR0135_WrSensorReg(pUsbHandle, 0x3046,0x0100);
#else
    cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
    return cyusb_control_write(pUsbHandle, 0x40, 0xf0, 0, 0, data, 2, 100);
#endif //FIRMWARE_NEW
    return 0;
}


static cq_int32_t AR0135_SetAnalogGain(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType, const cq_uint32_t chGainType)
{
	cq_int32_t r=0;
	switch(chTrigType) 
	{   
		case TRIGMODE_AUTO:
		{
			switch(chGainType)
			{
				case ANALOGGAIN_1X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x0080);
					break;
				case ANALOGGAIN_2X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x0090);
					break;
				case ANALOGGAIN_4X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x00A0);
					break;
				case ANALOGGAIN_8X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x00B0);
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
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x0480);
					break;
				case ANALOGGAIN_2X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x0490);
					break;
				case ANALOGGAIN_4X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x04A0);
					break;
				case ANALOGGAIN_8X:
					r=AR0135_WrSensorReg(pUsbHandle, 0x30B0,0x04B0);
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


static cq_int32_t AR0135_SetFpgaTrigFreq(cyusb_handle *pUsbHandle, const cq_uint32_t iFreqVal)
{
     cq_int32_t r=AR0135_WrFpgaReg(pUsbHandle, 0x05, iFreqVal);
     return r;
}

static cq_int32_t AR0135_SetTrigMode(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType)
{
	switch(chTrigType)
	{
		case TRIGMODE_AUTO:
			AR0135_WrSensorReg(pUsbHandle, 0x30B0, 0x0080);
			AR0135_WrSensorReg(pUsbHandle, 0x301A, 0x10DC);
			AR0135_WrFpgaReg(pUsbHandle, 0x00, 0x00);
			break;
		case TRIGMODE_FPGA:
			AR0135_WrSensorReg(pUsbHandle, 0x30B0, 0x0480);
			AR0135_WrSensorReg(pUsbHandle, 0x301A, 0x19D8);
			AR0135_WrFpgaReg(pUsbHandle, 0x00, 0x01);
			AR0135_WrFpgaReg(pUsbHandle, 0x05, 0x01);// 0x01 by default
			break;
		default:
			break;
	}
	return 0;

}


static cq_int32_t AR0135_SetExpoValue(cyusb_handle *pUsbHandle, const cq_uint32_t iExpoVal)
{
	cq_int32_t r=AR0135_WrSensorReg(pUsbHandle, 0x3012, iExpoVal);
	return r;

}

static cq_int32_t AR0135_SetGainValue(cyusb_handle *pUsbHandle, const cq_uint32_t iGainVal)
{
    cq_int32_t r=AR0135_WrSensorReg(pUsbHandle, 0x305E, iGainVal);
    return r;
}

static cq_int32_t AR0135_SetAutoGainExpo(cyusb_handle *pUsbHandle, const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo)
{
    if((bIsAutoGain==true)&&(bIsAutoExpo==true))
       return AR0135_WrSensorReg(pUsbHandle, 0x3100, 0x0003);

    if ((bIsAutoGain==true)&&(bIsAutoExpo==false))
       return AR0135_WrSensorReg(pUsbHandle, 0x3100, 0x0002);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==true))
       return AR0135_WrSensorReg(pUsbHandle, 0x3100, 0x0001);

    if ((bIsAutoGain==false)&&(bIsAutoExpo==false))
       return AR0135_WrSensorReg(pUsbHandle, 0x3100, 0x0000);
	return -99;//should never reach here
}



static cq_int32_t AR0135_SetResolution(cyusb_handle *pUsbHandle, const cq_uint32_t chResoluType)
{
	switch(chResoluType)
	{
		case RESOLU_1280_720:
			AR0135_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			AR0135_WrSensorReg(pUsbHandle, 0x3002, 0x007C);
			AR0135_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0135_WrSensorReg(pUsbHandle, 0x3006, 0x034B);
			AR0135_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0135_WrSensorReg(pUsbHandle, 0x300A, 0x02FD);
			AR0135_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0135_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			AR0135_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			AR0135_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			AR0135_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			AR0135_WrFpgaReg(pUsbHandle, 0x03, 0x02);
			AR0135_WrFpgaReg(pUsbHandle, 0x04, 0xd0);
			AR0135_WrFpgaReg(pUsbHandle, 0x06, 0x00);
			break;
		case RESOLU_1280_960:
			AR0135_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			AR0135_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			AR0135_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0135_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			AR0135_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0135_WrSensorReg(pUsbHandle, 0x300A, 0x03FD);
			AR0135_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0135_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			AR0135_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			AR0135_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			AR0135_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			AR0135_WrFpgaReg(pUsbHandle, 0x03, 0x03);
			AR0135_WrFpgaReg(pUsbHandle, 0x04, 0xC0);
			AR0135_WrFpgaReg(pUsbHandle, 0x06, 0x00);
			break;
		case RESOLU_640_480_SKIP:	
			AR0135_WrSensorReg(pUsbHandle, 0x3030, 0x002A);
			AR0135_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			AR0135_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0135_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			AR0135_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0135_WrSensorReg(pUsbHandle, 0x300A, 0x01FB);
			AR0135_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0135_WrSensorReg(pUsbHandle, 0x30A6, 0x0003);
			AR0135_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			AR0135_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			AR0135_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			AR0135_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			AR0135_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			AR0135_WrFpgaReg(pUsbHandle, 0x06, 0x01);					
			break;
		case RESOLU_640_480_BIN:
			AR0135_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			AR0135_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			AR0135_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			AR0135_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			AR0135_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			AR0135_WrSensorReg(pUsbHandle, 0x300A, 0x03FB);
			AR0135_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			AR0135_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			AR0135_WrSensorReg(pUsbHandle, 0x3032, 0x0002);// ¿ªÆôbinningÄ£Êœ

			AR0135_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			AR0135_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			AR0135_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			AR0135_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			AR0135_WrFpgaReg(pUsbHandle, 0x06, 0x00);
		default:
			break;
	}
	return 0;
		

}


static cq_int32_t AR0135_SetMirrorType(cyusb_handle *pUsbHandle, const cq_uint32_t chMirrorType)
{
	if(MIRROR_NORMAL==chMirrorType)	//normal
		return AR0135_WrSensorReg(pUsbHandle, 0x3040, 0x0000);

	if(MIRROR_X==chMirrorType)//X
		return AR0135_WrSensorReg(pUsbHandle, 0x3040, 0x4000);

	if(MIRROR_Y==chMirrorType)//Y
		return AR0135_WrSensorReg(pUsbHandle, 0x3040, 0x8000);

	if(MIRROR_XY==chMirrorType)//XY
		return AR0135_WrSensorReg(pUsbHandle, 0x3040, 0xC000);

	return -99;//should never reach here

}


static cq_int32_t AR0135_SetBitDepth(cyusb_handle *pUsbHandle, const cq_uint32_t chBitDepthType)
{
	if(BITDEPTH_8==chBitDepthType)	//8
		return AR0135_WrFpgaReg(pUsbHandle, 0x7, 0x0);

	if(BITDEPTH_16==chBitDepthType)//16
		return AR0135_WrFpgaReg(pUsbHandle, 0x7, 0x1);

	if(BITDEPTH_16_2==chBitDepthType)//16
		return AR0135_WrFpgaReg(pUsbHandle, 0x7, 0x2);

	return -99;//should never reach here

}


static cq_int32_t AR0135_StartCap(cyusb_handle *pUsbHandle)
{
	AR0135_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	AR0135_WrFpgaReg(pUsbHandle, 0x09, 0x01);
	return 0;	
}

static cq_int32_t AR0135_StopCap(cyusb_handle *pUsbHandle)
{
	AR0135_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	return 0;
}

static cq_int32_t AR0135_SendUsbSpeed2Fpga(cyusb_handle *pUsbHandle, const cq_uint32_t chSpeedType)
{
	AR0135_WrFpgaReg(pUsbHandle, 0x08, chSpeedType);
	return 0;
}



static tagSensor sensor_AR0135=
{
	.name="AR0135",
	.iicAddr=0xf0,
	.regAddrLen=2,
	.regValueLen=2,

	.InitSensor		=	AR0135_InitSensor,
	.WrSensorReg	=	AR0135_WrSensorReg,
	.RdSensorReg	=	AR0135_RdSensorReg,
	.WrFpgaReg		=	AR0135_WrFpgaReg,
	.RdFpgaReg		=	AR0135_RdFpgaReg,
	.SetAnalogGain	=	AR0135_SetAnalogGain,
	.SetFpgaTrigFreq=	AR0135_SetFpgaTrigFreq,
	.SetTrigMode	=	AR0135_SetTrigMode,
	.SetExpoValue	=	AR0135_SetExpoValue,
	.SetGainValue	=	AR0135_SetGainValue,
	.SetAutoGainExpo=	AR0135_SetAutoGainExpo,
	.SetResolution	=	AR0135_SetResolution,
	.SetMirrorType	=	AR0135_SetMirrorType,
	.SetBitDepth	=	AR0135_SetBitDepth,
	.StartCap 		=	AR0135_StartCap,
	.StopCap 		=	AR0135_StopCap,
	.SendUsbSpeed2Fpga=	AR0135_SendUsbSpeed2Fpga
};

void RegisterSensor_AR0135(list<tagSensor>& sensorList)
{
	sensorList.push_back(sensor_AR0135);
}

