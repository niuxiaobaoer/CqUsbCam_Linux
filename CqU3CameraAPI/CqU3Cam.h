
#ifndef CQU3CAM_H
#define CQU3CAM_H

#include "/home/nxb/Desktop/USBCamera/USB30/FX3_SDK_Linux/FX3_SDK_Linux_v1.3.3/cyusb_linux_1.0.4/include/cyusb.h"

int CqU3Cam_WrSensorReg(cyusb_handle *h, const unsigned short iAddr, const unsigned short iValue);
int CqU3Cam_RdSensorReg(cyusb_handle *h, const unsigned short iAddr, unsigned char *iValue);
int CqU3Cam_WrFpgaReg(cyusb_handle *h, const unsigned char iAddr, const unsigned char iValue);
int CqU3Cam_RdFpgaReg(cyusb_handle *h, const unsigned char iAddr, unsigned char *iValue);
int CqU3Cam_InitSensor(cyusb_handle *h);
int CqU3Cam_SetAnalogGain_AutoTrig(cyusb_handle *h, const unsigned char gainType);
int CqU3Cam_SetAnalogGain_FpgaTrig(cyusb_handle* h, const unsigned char gainType);
int CqU3Cam_OpenUSB(cyusb_handle** h);
int CqU3Cam_CloseUSB(cyusb_handle* h);
int CqU3Cam_CheckSpeed(cyusb_handle* h,  unsigned int* speed);
int CqU3Cam_SetAutoTrigMode(cyusb_handle* h);
int CqU3Cam_SetFpgaTrigMode(cyusb_handle* h, const unsigned char freq);
int CqU3Cam_SetFpgaFreq(cyusb_handle* h, const unsigned char freq);
int CqU3Cam_SetExpoValue(cyusb_handle* h, const unsigned short expo);
int CqU3Cam_SetGainValue(cyusb_handle* h, const unsigned short gain);
int CqU3Cam_SetAutoGainExpo(cyusb_handle* h, const bool bIsAutoGain, const bool bIsAutoExpo);
int CqU3Cam_Resolu_1280x720(cyusb_handle* h);
int CqU3Cam_Resolu_1280x960(cyusb_handle* h);
int CqU3Cam_Resolu_640x480Skip(cyusb_handle* h);
int CqU3Cam_Resolu_640x480bin(cyusb_handle* h);
int CqU3Cam_SetProcType(cyusb_handle* h, const unsigned char chProcType);
int CqU3Cam_SetBitDepth(cyusb_handle* h, const unsigned char chBitDepth);
int CqU3Cam_StartCap(cyusb_handle* h);
int CqU3Cam_SendUsbSpeed2Fpga(cyusb_handle* h, const unsigned char speedType);
#endif // CQU3CAM_H
