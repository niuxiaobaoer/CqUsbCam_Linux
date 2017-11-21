

#include "CqU3Cam.h"

int CqU3Cam_WrSensorReg(cyusb_handle *h, const unsigned short iAddr, const unsigned short iValue)
{
    unsigned char data[10]={'0'};//no use, just to make firmware happy
    return cyusb_control_write(h,0x40,0xf1,iValue,iAddr,data,2,100);
}
int CqU3Cam_RdSensorReg(cyusb_handle *h, const unsigned short iAddr, unsigned char *iValue)
{
    return cyusb_control_read(h,0x40,0xf2,0x0,iAddr,iValue,2,100);
}
int CqU3Cam_WrFpgaReg(cyusb_handle *h, const unsigned char iAddr, const unsigned char iValue)
{
    unsigned char data[10]={'0'};//no use, just to make firmware happy
    return cyusb_control_write(h,0x40,0xf3,iValue,iAddr,data,1,100);
}
int CqU3Cam_RdFpgaReg(cyusb_handle *h, const unsigned char iAddr, unsigned char *iValue)
{
    return cyusb_control_read(h,0x40,0xf4,0x0,iAddr,iValue,1,100);
}
int CqU3Cam_InitSensor(cyusb_handle *h)
{
    //unsigned char data[64]={'0'};//no use, just to make firmware happy
    //int r = cyusb_control_write(h,0x40,0xf0,0,0,data,2,3000);
	CqU3Cam_WrSensorReg(h, 0x3028,0x0010);//0		ROW_SPEED = 16
	CqU3Cam_WrSensorReg(h, 0x302A,0x000C);//1		VT_PIX_CLK_DIV = 12   P2   4<=P2<=16
	CqU3Cam_WrSensorReg(h, 0x302C,0x0001);//2		VT_SYS_CLK_DIV = 1    P1   1<=P1<=16
	CqU3Cam_WrSensorReg(h, 0x302E,0x0001);//3		PRE_PLL_CLK_DIV = 2   N    1<=N<=63
	CqU3Cam_WrSensorReg(h, 0x3030,0x0020);//4		PLL_MULTIPLIER = 40   M   32<=M<=255

	CqU3Cam_WrSensorReg(h, 0x3032,0x0000);//5		DIGITAL_BINNING = 0   _BINNING  帧率降一半
	CqU3Cam_WrSensorReg(h, 0x30B0,0x0080);//6		DIGITAL_TEST = 128

	CqU3Cam_WrSensorReg(h, 0x301A,0x00D8);//8		RESET_REGISTER = 216
	CqU3Cam_WrSensorReg(h, 0x301A,0x10DC);//9		RESET_REGISTER = 4316  h10DC    关键寄存器

	CqU3Cam_WrSensorReg(h, 0x3002,0x007C);//10	Y_ADDR_START = 124
	CqU3Cam_WrSensorReg(h, 0x3004,0x0002);//11	X_ADDR_START = 2
	CqU3Cam_WrSensorReg(h, 0x3006,0x034B);//12	Y_ADDR_END = 843
	CqU3Cam_WrSensorReg(h, 0x3008,0x0501);//13	X_ADDR_END = 1281

	CqU3Cam_WrSensorReg(h, 0x300A,0x02FD);//14	FRAME_LENGTH_LINES = 837
	CqU3Cam_WrSensorReg(h, 0x300C,0x056C);//15	LINE_LENGTH_PCK = 1388

	CqU3Cam_WrSensorReg(h, 0x3012,0x0080);//16	COARSE_INTEGRATION_TIME = 252	 h00FC	曝光时间
	CqU3Cam_WrSensorReg(h, 0x3014,0x008D);//17	FINE_INTEGRATION_TIME = 233

	CqU3Cam_WrSensorReg(h, 0x30A6,0x0001);//18	Y_ODD_INC = 1			    SKIP模式

	CqU3Cam_WrSensorReg(h, 0x3040,0x0000);//27	READ_MODE = 0			镜像等

	CqU3Cam_WrSensorReg(h, 0x3064,0x1982);//28	EMBEDDED_DATA_CTRL = 6530    开启输出 两行寄存器值 和  EMBEDDED_DATA  ，如果用AE模式 ，必须得开。图像输出时前两行不读

	CqU3Cam_WrSensorReg(h, 0x3100,0x0003);//30	AE;AG

	CqU3Cam_WrSensorReg(h, 0x305E,0x003C);//29	Total gain

	CqU3Cam_WrSensorReg(h, 0x3046,0x0100);
    return 0;
}
int CqU3Cam_SetAnalogGain_AutoTrig(cyusb_handle* h, const unsigned char gainType)
{
    if(gainType==1)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x0080);
    else if(gainType==2)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x0090);
    else if(gainType==3)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x00A0);
    else if(gainType==4)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x00B0);
    else;

}
int CqU3Cam_SetAnalogGain_FpgaTrig(cyusb_handle* h, const unsigned char gainType)
{
    if(gainType==1)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x0480);
    else if(gainType==2)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x0490);
    else if(gainType==3)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x04A0);
    else if(gainType==4)
        return CqU3Cam_WrSensorReg(h, 0x30B0,0x04B0);
    else;
}

int CqU3Cam_OpenUSB(cyusb_handle** h)
{
    int r=cyusb_open();
    if(r<0)
        return -1;
    else if(r==0)
    {
        cyusb_close();
        return -2;
    }
    int usbNum=0;//s_usbNum.toInt();

    *h=NULL;


    *h=cyusb_gethandle(usbNum);
#define TEST
#ifdef TEST
		if(*h==NULL)
    		*h=cyusb_gethandle(usbNum+1);
#endif
    if(*h==NULL)
    {
        cyusb_close();
        return -3;
    }
    r=cyusb_kernel_driver_active(*h,0);
    if(r!=0)
    {
        cyusb_close();
        return -4;
    }
    r = cyusb_claim_interface(*h, 0);
    if ( r != 0 )
    {
        cyusb_close();
        return -5;
    }

    return 0;
	}
int CqU3Cam_CloseUSB(cyusb_handle* h)
{
    int r = cyusb_release_interface(h, 0);
    if(r!=0)
        return -1;
    cyusb_close();
    return 0;  
}
int CqU3Cam_CheckSpeed(cyusb_handle* h,  unsigned int* speed)
{
		cyusb_device *h_dev = NULL;
    h_dev=libusb_get_device(h);
    if(h_dev==NULL)
			return -1;
    *speed=libusb_get_device_speed(h_dev);
		
		return 0;
}
int CqU3Cam_SetAutoTrigMode(cyusb_handle* h)
{
		CqU3Cam_WrSensorReg(h, 0x30B0, 0x0080);
		CqU3Cam_WrSensorReg(h, 0x301A, 0x10DC);
		CqU3Cam_WrFpgaReg(h, 0x00, 0x00);
		return 0;
}
int CqU3Cam_SetFpgaTrigMode(cyusb_handle* h, const unsigned char freq)
{
	  CqU3Cam_WrSensorReg(h, 0x30B0, 0x0480);
	  CqU3Cam_WrSensorReg(h, 0x301A, 0x19D8);
	  CqU3Cam_WrFpgaReg(h, 0x00, 0x01);
	  CqU3Cam_WrFpgaReg(h, 0x05, freq&0xff);
	  return 0;
}
int CqU3Cam_SetFpgaFreq(cyusb_handle* h, const unsigned char freq)
{
     CqU3Cam_WrFpgaReg(h, 0x05, freq & 0xff);
     return 0;
}
int CqU3Cam_SetExpoValue(cyusb_handle* h, const unsigned short expo)
{
		CqU3Cam_WrSensorReg(h, 0x3012, expo);
		return 0;
}
int CqU3Cam_SetGainValue(cyusb_handle* h, const unsigned short gain)
{
    CqU3Cam_WrSensorReg(h, 0x305E, gain);
    return 0;
}
int CqU3Cam_SetAutoGainExpo(cyusb_handle* h, const bool bIsAutoGain, const bool bIsAutoExpo)
{
    if((bIsAutoGain==true)&&(bIsAutoExpo==true))
       CqU3Cam_WrSensorReg(h, 0x3100, 0x0003);
    else if ((bIsAutoGain==true)&&(bIsAutoExpo==false))
       CqU3Cam_WrSensorReg(h, 0x3100, 0x0002);
    else if ((bIsAutoGain==false)&&(bIsAutoExpo==true))
       CqU3Cam_WrSensorReg(h, 0x3100, 0x0001);
    else if ((bIsAutoGain==false)&&(bIsAutoExpo==false))
       CqU3Cam_WrSensorReg(h, 0x3100, 0x0000);
    else;
    	
    return 0;
}
int CqU3Cam_Resolu_1280x720(cyusb_handle* h)
{
    CqU3Cam_WrSensorReg(h,0x3030, 0x0020);
    CqU3Cam_WrSensorReg(h,0x3002, 0x007C);
    CqU3Cam_WrSensorReg(h,0x3004, 0x0002);
    CqU3Cam_WrSensorReg(h,0x3006, 0x034B);
    CqU3Cam_WrSensorReg(h,0x3008, 0x0501);
    CqU3Cam_WrSensorReg(h,0x300A, 0x02FD);
    CqU3Cam_WrSensorReg(h,0x300C, 0x056C);
    CqU3Cam_WrSensorReg(h,0x30A6, 0x0001);
    CqU3Cam_WrSensorReg(h,0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

    CqU3Cam_WrFpgaReg(h,0x01, 0x05);
    CqU3Cam_WrFpgaReg(h,0x02, 0x00);
    CqU3Cam_WrFpgaReg(h,0x03, 0x02);
    CqU3Cam_WrFpgaReg(h,0x04, 0xd0);
    CqU3Cam_WrFpgaReg(h,0x06, 0x00);
    
    return 0;
}
int CqU3Cam_Resolu_1280x960(cyusb_handle* h)
{
    CqU3Cam_WrSensorReg(h, 0x3030, 0x0020);
    CqU3Cam_WrSensorReg(h, 0x3002, 0x0004);
    CqU3Cam_WrSensorReg(h, 0x3004, 0x0002);
    CqU3Cam_WrSensorReg(h, 0x3006, 0x03C3);
    CqU3Cam_WrSensorReg(h, 0x3008, 0x0501);
    CqU3Cam_WrSensorReg(h, 0x300A, 0x03FD);
    CqU3Cam_WrSensorReg(h, 0x300C, 0x056C);
    CqU3Cam_WrSensorReg(h, 0x30A6, 0x0001);
    CqU3Cam_WrSensorReg(h, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

    CqU3Cam_WrFpgaReg(h, 0x01, 0x05);
    CqU3Cam_WrFpgaReg(h, 0x02, 0x00);
    CqU3Cam_WrFpgaReg(h, 0x03, 0x03);
    CqU3Cam_WrFpgaReg(h, 0x04, 0xC0);
    CqU3Cam_WrFpgaReg(h, 0x06, 0x00);
    
    return 0;
}
int CqU3Cam_Resolu_640x480Skip(cyusb_handle* h)
{
    CqU3Cam_WrSensorReg(h, 0x3030, 0x002A);
    CqU3Cam_WrSensorReg(h, 0x3002, 0x0004);
    CqU3Cam_WrSensorReg(h, 0x3004, 0x0002);
    CqU3Cam_WrSensorReg(h, 0x3006, 0x03C3);
    CqU3Cam_WrSensorReg(h, 0x3008, 0x0501);
    CqU3Cam_WrSensorReg(h, 0x300A, 0x01FB);
    CqU3Cam_WrSensorReg(h, 0x300C, 0x056C);
    CqU3Cam_WrSensorReg(h, 0x30A6, 0x0003);
    CqU3Cam_WrSensorReg(h, 0x3032, 0x0000);// ¹Ø±ÕbiningÄ£Êœ

    CqU3Cam_WrFpgaReg(h, 0x01, 0x02);
    CqU3Cam_WrFpgaReg(h, 0x02, 0x80);
    CqU3Cam_WrFpgaReg(h, 0x03, 0x01);
    CqU3Cam_WrFpgaReg(h, 0x04, 0xE0);
    CqU3Cam_WrFpgaReg(h, 0x06, 0x01);
    
    return 0;
}
int CqU3Cam_Resolu_640x480bin(cyusb_handle* h)
{
    CqU3Cam_WrSensorReg(h, 0x3030, 0x0020);
    CqU3Cam_WrSensorReg(h, 0x3002, 0x0004);
    CqU3Cam_WrSensorReg(h, 0x3004, 0x0002);
    CqU3Cam_WrSensorReg(h, 0x3006, 0x03C3);
    CqU3Cam_WrSensorReg(h, 0x3008, 0x0501);
    CqU3Cam_WrSensorReg(h, 0x300A, 0x03FB);
    CqU3Cam_WrSensorReg(h, 0x300C, 0x056C);
    CqU3Cam_WrSensorReg(h, 0x30A6, 0x0001);
    CqU3Cam_WrSensorReg(h, 0x3032, 0x0002);// ¿ªÆôbinningÄ£Êœ

    CqU3Cam_WrFpgaReg(h, 0x01, 0x02);
    CqU3Cam_WrFpgaReg(h, 0x02, 0x80);
    CqU3Cam_WrFpgaReg(h, 0x03, 0x01);
    CqU3Cam_WrFpgaReg(h, 0x04, 0xE0);
    CqU3Cam_WrFpgaReg(h, 0x06, 0x00);
    
    return 0;
}
int CqU3Cam_SetProcType(cyusb_handle* h, const unsigned char chProcType)
{
		if(0==chProcType)	//normal
			CqU3Cam_WrSensorReg(h, 0x3040, 0x0000);
		else if(1==chProcType)//X
			CqU3Cam_WrSensorReg(h, 0x3040, 0x4000);
		else if(2==chProcType)//Y
			CqU3Cam_WrSensorReg(h, 0x3040, 0x8000);
		else if(3==chProcType)//XY
			CqU3Cam_WrSensorReg(h, 0x3040, 0xC000);
		else;
			
		return 0;
}
int CqU3Cam_SetBitDepth(cyusb_handle* h, const unsigned char chBitDepth)
{
		if(0==chBitDepth)	//8
			CqU3Cam_WrFpgaReg(h, 0x7, 0x0);
		else if(1==chBitDepth)//16
			CqU3Cam_WrFpgaReg(h, 0x7, 0x1);
		else if(2==chBitDepth)//16
			CqU3Cam_WrFpgaReg(h, 0x7, 0x2);
		else;
			
		return 0;
}
int CqU3Cam_StartCap(cyusb_handle* h)
{
	CqU3Cam_WrFpgaReg(h, 0x09, 0x00);
	CqU3Cam_WrFpgaReg(h, 0x09, 0x01);
	return 0;
}
int CqU3Cam_SendUsbSpeed2Fpga(cyusb_handle* h, const unsigned char speedType)
{
	CqU3Cam_WrFpgaReg(h, 0x08, speedType);
	return 0;
}
