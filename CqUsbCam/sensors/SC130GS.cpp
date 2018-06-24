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
#include "SC130GS.h"
#include "../SensorCapbablity.h"





static cq_int32_t SC130GS_WrSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
	cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint16_t tempValue= iValue&0xff;
	return cyusb_control_write(pUsbHandle, 0x40, 0xf1, tempValue, tempAddr, data, 2, 100);
}


static cq_int32_t SC130GS_RdSensorReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint16_t tempAddr= iAddr&0xffff;
	cq_uint8_t tempValue[1]={'0'};
	cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf2,0x0, tempAddr, tempValue, 1, 100);
	iValue = tempValue[0] ;
	return iValue;
}


static cq_int32_t SC130GS_WrFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, const cq_uint32_t iValue)
{
	cq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue= iValue&0xff;
	return cyusb_control_write(pUsbHandle, 0x40, 0xf3, tempValue, tempAddr, data, 1, 100);
}


static cq_int32_t SC130GS_RdFpgaReg(cyusb_handle *pUsbHandle, const cq_uint32_t iAddr, cq_uint32_t &iValue)
{
	cq_uint8_t tempAddr= iAddr&0xff;
	cq_uint8_t tempValue[1]={'0'};
	cq_int32_t r=cyusb_control_read(pUsbHandle, 0x40, 0xf4, 0x0, tempAddr, tempValue, 1, 100);
	iValue=tempValue[0];
	return r;
}
#define FIRMWARE_NEW
static cq_int32_t SC130GS_InitSensor(cyusb_handle *pUsbHandle)
{    
#ifdef FIRMWARE_NEW
	SC130GS_WrSensorReg(pUsbHandle,0x0100,0x00);//0
	SC130GS_WrSensorReg(pUsbHandle,0x0100,0x00);//1
	SC130GS_WrSensorReg(pUsbHandle,0x3036,0x00);//2
	SC130GS_WrSensorReg(pUsbHandle,0x3000,0x0F);//3
	SC130GS_WrSensorReg(pUsbHandle,0x3001,0xFF);//4
	SC130GS_WrSensorReg(pUsbHandle,0x3018,0x7F);//5
	SC130GS_WrSensorReg(pUsbHandle,0x3022,0x10);//6
	SC130GS_WrSensorReg(pUsbHandle,0x302B,0x80);//7
	SC130GS_WrSensorReg(pUsbHandle,0x3030,0x04);//8
	SC130GS_WrSensorReg(pUsbHandle,0x3034,0x25);//9
	SC130GS_WrSensorReg(pUsbHandle,0x3035,0x2A);//10
	SC130GS_WrSensorReg(pUsbHandle,0x3038,0x44);//11
	SC130GS_WrSensorReg(pUsbHandle,0x3039,0x20);//12
	SC130GS_WrSensorReg(pUsbHandle,0x303A,0x25);//13
	SC130GS_WrSensorReg(pUsbHandle,0x303B,0x05);//14
	SC130GS_WrSensorReg(pUsbHandle,0x303C,0x04);//15
	SC130GS_WrSensorReg(pUsbHandle,0x3202,0x00);//16
	SC130GS_WrSensorReg(pUsbHandle,0x3203,0x00);//17
	SC130GS_WrSensorReg(pUsbHandle,0x3205,0x8B);//18
	SC130GS_WrSensorReg(pUsbHandle,0x3206,0x02);//19
	SC130GS_WrSensorReg(pUsbHandle,0x3207,0x04);//20
	SC130GS_WrSensorReg(pUsbHandle,0x3208,0x05);//21
	SC130GS_WrSensorReg(pUsbHandle,0x3209,0x00);//22
	SC130GS_WrSensorReg(pUsbHandle,0x320A,0x04);//23
	SC130GS_WrSensorReg(pUsbHandle,0x320B,0x00);//24
	SC130GS_WrSensorReg(pUsbHandle,0x320C,0x02);//25
	SC130GS_WrSensorReg(pUsbHandle,0x320D,0xF4);//26
	SC130GS_WrSensorReg(pUsbHandle,0x320E,0x02);//27
	SC130GS_WrSensorReg(pUsbHandle,0x320F,0x17);//28
	SC130GS_WrSensorReg(pUsbHandle,0x3210,0x00);//29
	SC130GS_WrSensorReg(pUsbHandle,0x3211,0x0C);//30
	SC130GS_WrSensorReg(pUsbHandle,0x3212,0x00);//31
	SC130GS_WrSensorReg(pUsbHandle,0x3213,0x04);//32
	SC130GS_WrSensorReg(pUsbHandle,0x3300,0x20);//33
	SC130GS_WrSensorReg(pUsbHandle,0x3302,0x0C);//34
	SC130GS_WrSensorReg(pUsbHandle,0x3306,0x28);//35
	SC130GS_WrSensorReg(pUsbHandle,0x3308,0x50);//36
	SC130GS_WrSensorReg(pUsbHandle,0x330A,0x00);//37
	SC130GS_WrSensorReg(pUsbHandle,0x330B,0x78);//38
	SC130GS_WrSensorReg(pUsbHandle,0x330E,0x1A);//39
	SC130GS_WrSensorReg(pUsbHandle,0x3310,0xF0);//40
	SC130GS_WrSensorReg(pUsbHandle,0x3311,0x10);//41
	SC130GS_WrSensorReg(pUsbHandle,0x3319,0xE8);//42
	SC130GS_WrSensorReg(pUsbHandle,0x3333,0x90);//43
	SC130GS_WrSensorReg(pUsbHandle,0x3334,0x30);//44
	SC130GS_WrSensorReg(pUsbHandle,0x3348,0x02);//45
	SC130GS_WrSensorReg(pUsbHandle,0x3349,0xee);//46
	SC130GS_WrSensorReg(pUsbHandle,0x334a,0x02);//47
	SC130GS_WrSensorReg(pUsbHandle,0x334b,0xb0);//48
	SC130GS_WrSensorReg(pUsbHandle,0x335d,0x0a);//49
	SC130GS_WrSensorReg(pUsbHandle,0x3380,0xff);//50
	SC130GS_WrSensorReg(pUsbHandle,0x3382,0xe0);//51
	SC130GS_WrSensorReg(pUsbHandle,0x3383,0x0a);//52
	SC130GS_WrSensorReg(pUsbHandle,0x3384,0xe4);//53
	SC130GS_WrSensorReg(pUsbHandle,0x3400,0x53);//54
	SC130GS_WrSensorReg(pUsbHandle,0x3416,0x31);//55
	SC130GS_WrSensorReg(pUsbHandle,0x3518,0x07);//56
	SC130GS_WrSensorReg(pUsbHandle,0x3519,0xc8);//57
	SC130GS_WrSensorReg(pUsbHandle,0X3620,0x32);//58
	SC130GS_WrSensorReg(pUsbHandle,0x3621,0x18);//59
	SC130GS_WrSensorReg(pUsbHandle,0x3622,0x06);//60
	SC130GS_WrSensorReg(pUsbHandle,0x3623,0x14);//61
	SC130GS_WrSensorReg(pUsbHandle,0x3624,0x1f);//62
	SC130GS_WrSensorReg(pUsbHandle,0x3625,0x00);//63
	SC130GS_WrSensorReg(pUsbHandle,0x3627,0x02);//64
	SC130GS_WrSensorReg(pUsbHandle,0x3630,0x63);//65
	SC130GS_WrSensorReg(pUsbHandle,0x3632,0x74);//66
	SC130GS_WrSensorReg(pUsbHandle,0x3633,0x62);//67
	SC130GS_WrSensorReg(pUsbHandle,0x3634,0xff);//68
	SC130GS_WrSensorReg(pUsbHandle,0x3635,0x44);//69
	SC130GS_WrSensorReg(pUsbHandle,0x3638,0x83);//70
	SC130GS_WrSensorReg(pUsbHandle,0x3639,0x74);//71
	SC130GS_WrSensorReg(pUsbHandle,0x363a,0x24);//72
	SC130GS_WrSensorReg(pUsbHandle,0x363b,0x00);//73
	SC130GS_WrSensorReg(pUsbHandle,0x3640,0x00);//74
	SC130GS_WrSensorReg(pUsbHandle,0x3641,0xc1);//75
	SC130GS_WrSensorReg(pUsbHandle,0x3663,0x88);//76
	SC130GS_WrSensorReg(pUsbHandle,0x3664,0x07);//77
	SC130GS_WrSensorReg(pUsbHandle,0x3d08,0x00);//78
	SC130GS_WrSensorReg(pUsbHandle,0x3e01,0x08);//79
	SC130GS_WrSensorReg(pUsbHandle,0x3e02,0x00);//80
	SC130GS_WrSensorReg(pUsbHandle,0x3e03,0x0b);//81
	SC130GS_WrSensorReg(pUsbHandle,0x3e08,0x03);//82
	SC130GS_WrSensorReg(pUsbHandle,0x3e09,0x20);//83
	SC130GS_WrSensorReg(pUsbHandle,0x3e0e,0x00);//84
	SC130GS_WrSensorReg(pUsbHandle,0x3e0f,0x14);//85
	SC130GS_WrSensorReg(pUsbHandle,0x3e14,0xb0);//86
	SC130GS_WrSensorReg(pUsbHandle,0x3f08,0x04);//87
	SC130GS_WrSensorReg(pUsbHandle,0x4501,0xc0);//88
	SC130GS_WrSensorReg(pUsbHandle,0x4502,0x16);//89
	SC130GS_WrSensorReg(pUsbHandle,0x5000,0x01);//90
	SC130GS_WrSensorReg(pUsbHandle,0x5b00,0x02);//91
	SC130GS_WrSensorReg(pUsbHandle,0x5b01,0x03);//92
	SC130GS_WrSensorReg(pUsbHandle,0x5b02,0x01);//93
	SC130GS_WrSensorReg(pUsbHandle,0x5b03,0x01);//94
	SC130GS_WrSensorReg(pUsbHandle,0x3228,0x60);//95
	SC130GS_WrSensorReg(pUsbHandle,0x0100,0x01);//96
#else
	Sq_uint8_t data[10]={'0'};//no use, just to make firmware happy
	return cyusb_control_write(pUsbHandle, 0x40, 0xf0, 0, 0, data, 2, 100);
#endif //FIRMWARE_NEW
	return 0;
}


static cq_int32_t SC130GS_SetAnalogGain(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType, const cq_uint32_t chGainType)
{
	cq_int32_t r=0;
	switch(chTrigType) 
	{   
		case TRIGMODE_AUTO:
			{
				switch(chGainType)
				{
					case ANALOGGAIN_1X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x0080);
						break;
					case ANALOGGAIN_2X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x0090);
						break;
					case ANALOGGAIN_4X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x00A0);
						break;
					case ANALOGGAIN_8X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x00B0);
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
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x0480);
						break;
					case ANALOGGAIN_2X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x0490);
						break;
					case ANALOGGAIN_4X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x04A0);
						break;
					case ANALOGGAIN_8X:
						r=SC130GS_WrSensorReg(pUsbHandle, 0x30B0,0x04B0);
						break;
					default:
						r=-99;
						break;	
				}
				break;
			}	
		default:
			switch(chGainType)
			{
				case ANALOGGAIN_1X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x03);
					break;
				case ANALOGGAIN_2X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x07);
					break;
				case ANALOGGAIN_4X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x0F);
					break;
				case ANALOGGAIN_8X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x1F);
					break;
				case ANALOGGAIN_16X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x3F);
					break;
				case ANALOGGAIN_32X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0x7F);
					break;
				case ANALOGGAIN_64X:
					r=SC130GS_WrSensorReg(pUsbHandle, 0x3E08,0xFF);
					break;
				default:
					r=-99;
					break;	
			}
			break;	
	}
	return r;
}


static cq_int32_t SC130GS_SetFpgaTrigFreq(cyusb_handle *pUsbHandle, const cq_uint32_t iFreqVal)
{
	cq_int32_t r=SC130GS_WrFpgaReg(pUsbHandle, 0x05, iFreqVal&0xff);
	return r;
}

static cq_int32_t SC130GS_SetTrigMode(cyusb_handle *pUsbHandle, const cq_uint32_t chTrigType)
{
	switch(chTrigType)
	{
		case TRIGMODE_AUTO:
			SC130GS_WrSensorReg(pUsbHandle, 0x3234, 0x30);
			SC130GS_WrFpgaReg(pUsbHandle, 0x00, 0x00);
			break;
		case TRIGMODE_FPGA:
			SC130GS_WrSensorReg(pUsbHandle, 0x3234, 0xB0);
			SC130GS_WrFpgaReg(pUsbHandle, 0x00, 0x01);
			SC130GS_WrFpgaReg(pUsbHandle, 0x05, 0x01);// 0x01 by default
			break;
		default:
			break;
	}
	return 0;

}


static cq_int32_t SC130GS_SetExpoValue(cyusb_handle *pUsbHandle, const cq_uint32_t iExpoVal)
{
	cq_int32_t r=SC130GS_WrSensorReg(pUsbHandle, 0x3E01, iExpoVal >> 8 & 0XFF);
	r=SC130GS_WrSensorReg(pUsbHandle, 0x3E02, iExpoVal & 0XFF);
	return r;

}

static cq_int32_t SC130GS_SetGainValue(cyusb_handle *pUsbHandle, const cq_uint32_t iGainVal)
{
	cq_int32_t r=SC130GS_WrSensorReg(pUsbHandle, 0x305E, iGainVal);
	return r;
}

static cq_int32_t SC130GS_SetAutoGainExpo(cyusb_handle *pUsbHandle, const cq_bool_t bIsAutoGain, const cq_bool_t bIsAutoExpo)
{
	if((bIsAutoGain==true)&&(bIsAutoExpo==true))
		return SC130GS_WrSensorReg(pUsbHandle, 0x3E03, 0x00);

	if ((bIsAutoGain==true)&&(bIsAutoExpo==false))
		return SC130GS_WrSensorReg(pUsbHandle, 0x3E03, 0x09);

	if ((bIsAutoGain==false)&&(bIsAutoExpo==true))
		return SC130GS_WrSensorReg(pUsbHandle, 0x3E03, 0x0A);

	if ((bIsAutoGain==false)&&(bIsAutoExpo==false))
		return SC130GS_WrSensorReg(pUsbHandle, 0x3E03, 0x0B);
	return -99;//should never reach here
}



static cq_int32_t SC130GS_SetResolution(cyusb_handle *pUsbHandle, const cq_uint32_t chResoluType)
{
	switch(chResoluType)
	{
		case RESOLU_1280_720:
			SC130GS_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			SC130GS_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			SC130GS_WrFpgaReg(pUsbHandle, 0x03, 0x04);
			SC130GS_WrFpgaReg(pUsbHandle, 0x04, 0x00);
			break;
		case RESOLU_1280_960:
			SC130GS_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			SC130GS_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			SC130GS_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			SC130GS_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			SC130GS_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			SC130GS_WrSensorReg(pUsbHandle, 0x300A, 0x03FD);
			SC130GS_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			SC130GS_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			SC130GS_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			SC130GS_WrFpgaReg(pUsbHandle, 0x01, 0x05);
			SC130GS_WrFpgaReg(pUsbHandle, 0x02, 0x00);
			SC130GS_WrFpgaReg(pUsbHandle, 0x03, 0x03);
			SC130GS_WrFpgaReg(pUsbHandle, 0x04, 0xC0);
			SC130GS_WrFpgaReg(pUsbHandle, 0x06, 0x00);
			break;
		case RESOLU_640_480_SKIP:	
			SC130GS_WrSensorReg(pUsbHandle, 0x3030, 0x002A);
			SC130GS_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			SC130GS_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			SC130GS_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			SC130GS_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			SC130GS_WrSensorReg(pUsbHandle, 0x300A, 0x01FB);
			SC130GS_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			SC130GS_WrSensorReg(pUsbHandle, 0x30A6, 0x0003);
			SC130GS_WrSensorReg(pUsbHandle, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

			SC130GS_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			SC130GS_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			SC130GS_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			SC130GS_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			SC130GS_WrFpgaReg(pUsbHandle, 0x06, 0x01);					
			break;
		case RESOLU_640_480_BIN:
			SC130GS_WrSensorReg(pUsbHandle, 0x3030, 0x0020);
			SC130GS_WrSensorReg(pUsbHandle, 0x3002, 0x0004);
			SC130GS_WrSensorReg(pUsbHandle, 0x3004, 0x0002);
			SC130GS_WrSensorReg(pUsbHandle, 0x3006, 0x03C3);
			SC130GS_WrSensorReg(pUsbHandle, 0x3008, 0x0501);
			SC130GS_WrSensorReg(pUsbHandle, 0x300A, 0x03FB);
			SC130GS_WrSensorReg(pUsbHandle, 0x300C, 0x056C);
			SC130GS_WrSensorReg(pUsbHandle, 0x30A6, 0x0001);
			SC130GS_WrSensorReg(pUsbHandle, 0x3032, 0x0002);// ¿ªÆôbinningÄ£Êœ

			SC130GS_WrFpgaReg(pUsbHandle, 0x01, 0x02);
			SC130GS_WrFpgaReg(pUsbHandle, 0x02, 0x80);
			SC130GS_WrFpgaReg(pUsbHandle, 0x03, 0x01);
			SC130GS_WrFpgaReg(pUsbHandle, 0x04, 0xE0);
			SC130GS_WrFpgaReg(pUsbHandle, 0x06, 0x00);
		default:
			break;
	}
	return 0;


}


static cq_int32_t SC130GS_SetMirrorType(cyusb_handle *pUsbHandle, const cq_uint32_t chMirrorType)
{
	if(MIRROR_NORMAL==chMirrorType)	//normal
		return SC130GS_WrSensorReg(pUsbHandle, 0x3221, 0x00);

	if(MIRROR_X==chMirrorType)//X
		return SC130GS_WrSensorReg(pUsbHandle, 0x3221, 0x06);

	if(MIRROR_Y==chMirrorType)//Y
		return SC130GS_WrSensorReg(pUsbHandle, 0x3221, 0x60);

	if(MIRROR_XY==chMirrorType)//XY
		return SC130GS_WrSensorReg(pUsbHandle, 0x3221, 0x66);

	return -99;//should never reach here

}


static cq_int32_t SC130GS_SetBitDepth(cyusb_handle *pUsbHandle, const cq_uint32_t chBitDepthType)
{
	if(BITDEPTH_8==chBitDepthType)	//8
		return SC130GS_WrFpgaReg(pUsbHandle, 0x7, 0x0);

	if(BITDEPTH_16==chBitDepthType)//16
		return SC130GS_WrFpgaReg(pUsbHandle, 0x7, 0x1);

	if(BITDEPTH_16_2==chBitDepthType)//16
		return SC130GS_WrFpgaReg(pUsbHandle, 0x7, 0x2);

	return -99;//should never reach here

}


static cq_int32_t SC130GS_StartCap(cyusb_handle *pUsbHandle)
{
	SC130GS_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	SC130GS_WrFpgaReg(pUsbHandle, 0x09, 0x01);
	return 0;	
}

static cq_int32_t SC130GS_StopCap(cyusb_handle *pUsbHandle)
{
	SC130GS_WrFpgaReg(pUsbHandle, 0x09, 0x00);
	return 0;
}

static cq_int32_t SC130GS_SendUsbSpeed2Fpga(cyusb_handle *pUsbHandle, const cq_uint32_t chSpeedType)
{
	SC130GS_WrFpgaReg(pUsbHandle, 0x08, chSpeedType);
	return 0;
}



static tagSensor sensor_SC130GS=
{
	.name="SC130GS",
	.iicAddr=0xf0,
	.regAddrLen=2,
	.regValueLen=1,

	.InitSensor		=	SC130GS_InitSensor,
	.WrSensorReg	=	SC130GS_WrSensorReg,
	.RdSensorReg	=	SC130GS_RdSensorReg,
	.WrFpgaReg		=	SC130GS_WrFpgaReg,
	.RdFpgaReg		=	SC130GS_RdFpgaReg,
	.SetAnalogGain	=	SC130GS_SetAnalogGain,
	.SetFpgaTrigFreq=	SC130GS_SetFpgaTrigFreq,
	.SetTrigMode	=	SC130GS_SetTrigMode,
	.SetExpoValue	=	SC130GS_SetExpoValue,
	.SetGainValue	=	SC130GS_SetGainValue,
	.SetAutoGainExpo=	SC130GS_SetAutoGainExpo,
	.SetResolution	=	SC130GS_SetResolution,
	.SetMirrorType	=	SC130GS_SetMirrorType,
	.SetBitDepth	=	SC130GS_SetBitDepth,
	.StartCap 		=	SC130GS_StartCap,
	.StopCap 		=	SC130GS_StopCap,
	.SendUsbSpeed2Fpga=	SC130GS_SendUsbSpeed2Fpga
};

void RegisterSensor_SC130GS(list<tagSensor>& sensorList)
{
	sensorList.push_back(sensor_SC130GS);
}

