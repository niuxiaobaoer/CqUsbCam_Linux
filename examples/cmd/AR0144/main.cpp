
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termio.h>

#include <opencv2/highgui/highgui.hpp>

#include "../../../../CqUsbCam_Linux/CqUsbCam/CqUsbCam.h"
#include "../../../../CqUsbCam_Linux/CqUsbCam/SensorCapbablity.h"



#define MAIN_RESOLU_SELECT	 	'a'
#define MAIN_RESOLU_1280_720 			'0'
#define MAIN_RESOLU_1280_960 			'1'
#define MAIN_RESOLU_640_480SKIP 		'2'
#define MAIN_RESOLU_640_480BIN			'3'
#define MAIN_RESOLU_1280_800			'4'

#define MAIN_BITDEPTH_SELECT 		'b'
#define MAIN_BITDEPTH_8		 		'0'
#define MAIN_BITDEPTH_16		 	'1'
#define MAIN_BITDEPTH_L8		 	'2'

#define MAIN_PROCTYPE_SELECT 		'c'
#define MAIN_PROCTYPE_N		 		'0'
#define MAIN_PROCTYPE_X		 		'1'
#define MAIN_PROCTYPE_Y		 		'2'
#define MAIN_PROCTYPE_XY		 	'3'

#define MAIN_CHECK_SPEED 		'd'
#define MAIN_TRIGMODE_AUTO 		'e'
#define MAIN_TRIGMODE_FPGA 		'f'
#define MAIN_FPGA_TRIG_FREQ_SELECT	'g'
#define MAIN_EXPO_VALUE_SELECT		'h'
#define MAIN_GAIN_VALUE_SELECT		'i'
#define MAIN_AUTO_GAIN_EXPO_SELECT	'j'
#define MAIN_ROI			'k'
#define MAIN_CAPTURE			'l'
#define MAIN_ROI_BOX			'm'
#define MAIN_ANALOG_GAIN_AUTO_TRIG	'n'
#define MAIN_ANALOG_GAIN_FPGA_TRIG	'o'
#define MAIN_ANALOG_GAIN_1X			'1'
#define MAIN_ANALOG_GAIN_2X			'2'
#define MAIN_ANALOG_GAIN_4X			'4'
#define MAIN_ANALOG_GAIN_8X			'8'
#define MAIN_ROI_X1			'p'
#define MAIN_ROI_X2			'q'
#define MAIN_ROI_X3			'r'
#define MAIN_ROI_X4			's'
#define MAIN_ROI_X5			't'
#define MAIN_ROI_Y1			'u'
#define MAIN_ROI_Y2			'v'
#define MAIN_ROI_Y3			'w'
#define MAIN_ROI_Y4			'x'
#define MAIN_ROI_Y5			'y'
#define MAIN_ROI_WIDTH			'A'
#define MAIN_ROI_HEIGHT			'B'
#define MAIN_SAVE_VEDIO			'D'
#define MAIN_SAVE_ALL			'E'
#define MAIN_WRITE_SENSOR_REG		'F'
#define MAIN_READ_SENSOR_REG		'G'
#define MAIN_WRITE_FPGA_REG		'H'
#define MAIN_READ_FPGA_REG		'I'
#define MAIN_SELECT_SENSOR		'J'
#define MAIN_EXIT_NORMAL		'z'

string sensor = "AR0144";//	or AR0134
unsigned int g_width=1280;
unsigned int g_height=720;

int g_byteBitDepthNo=1;

volatile bool g_bSave_all = false;
volatile bool g_bSaveVedio = false;
cv::VideoWriter h_vw;

pthread_mutex_t mutexDisp;
pthread_mutex_t mutexCam;

void Disp(void* frameData)
{
	pthread_mutex_lock(&mutexDisp);
	cv::Mat frame(g_height, g_width, (g_byteBitDepthNo==1? CV_8UC1: CV_16UC1), (unsigned char*)frameData);	
	cv::imshow("disp",frame);
	cv::waitKey(10);

	if(g_bSave_all)
	{

		char* strName =(char*) malloc(100);
		if(1)
		{
			int iFileIndex=1;
			do
			{
				sprintf(strName, g_byteBitDepthNo == 1?"V_%d.bmp": "V_%d.png",iFileIndex);
				++iFileIndex;
			} while (access(strName,0)==0);

#if 1
			std::string cvfilename(strName);

			cv::vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(0);    // 无压缩png.
			//sprintf(fileName, "Picture %d.png", ++countCamera); //生成文件名
			//cv::Mat pImgMat(pImg, 0);  //把IplImage转换为Mat
			//imwrite(fileName, pImgMat, compression_params);  //保存图片</span>


			cv::imwrite(cvfilename,frame, g_byteBitDepthNo == 1 ? compression_params: std::vector<int>());
			//g_bSnap=false;
#endif
		}
		free(strName);
	}
	if (g_bSaveVedio == true)
		h_vw.write(frame);//save video operation

	pthread_mutex_unlock(&mutexDisp);

}



CCqUsbCam cam0, *pCamInUse;


unsigned short hex2dec(char *hex)

{

	unsigned short  number=0;

	char *p=hex;

	for(p=hex;*p;++p)
	{
		if((hex[p-hex]<='z')&&(hex[p-hex]>='a'))
			hex[p-hex]=hex[p-hex]-32;
		number=number*16+(hex[p-hex]>='A'?hex[p-hex]-'A'+10:hex[p-hex]-'0');
	}

	return number;

}

void checkspeed()
{
	unsigned int speed = 0;
	cam0.GetUsbSpeed(speed);
	if(speed==LIBUSB_SPEED_SUPER)
	{
		printf("USB 3.0 device found on cam0!\n");
		cam0.SendUsbSpeed2Fpga(LIBUSB_SPEED_SUPER);
	}
	else if(speed==LIBUSB_SPEED_HIGH)
	{
		printf("USB 2.0 device found on cam0!\n");
		cam0.SendUsbSpeed2Fpga(LIBUSB_SPEED_HIGH);
	}
	else
	{
		printf("Device speed unknown on cam0!\n");
	}
}

void timerFunction(int sig)
{

	unsigned long iByteCntPerSec=0;
	unsigned long iFrameCntPerSec=0;

	pthread_mutex_lock(&mutexCam);

	cam0.GetRecvByteCnt(iByteCntPerSec);
	cam0.ClearRecvByteCnt();
	cam0.GetRecvFrameCnt(iFrameCntPerSec);
	cam0.ClearRecvFrameCnt();

	printf("cam0: %ld Fps     %0.4f MBs\n", iFrameCntPerSec, float(iByteCntPerSec)/1024.0/1024.0);
	//system("gnome-terminal -e  sudo echo  hahah >./text");
	/***************************************************************/
	alarm(1);

	pthread_mutex_unlock(&mutexCam);

}



int main(int argc, char *argv[])
{
	cq_int32_t ret;
	ret =pthread_mutex_init(&mutexDisp, NULL);
	if(ret!=0)
		printf("pthread_mutex_init failed");
	ret =pthread_mutex_init(&mutexCam, NULL);
	if(ret!=0)
		printf("pthread_mutex_init failed");

	cam0.SelectSensor(sensor);

	int usbCnt=CCqUsbCam::OpenUSB();
	printf("%d usb device(s) found!\n", usbCnt);
	if(usbCnt<=0)
	{
		printf("exiting ...\n");
		return -1;
	}
	cam0.ClaimInterface(0);

	checkspeed();

	cam0.InitSensor();

	pCamInUse=&cam0;

	while(1)
	{
		printf("Please input your choice ...\n");
		printf("\
				\'a\':	Select resolution\n\
				\'c\':	Select proc type\n\
				\'d\':	Check speed\n\
				\'e\':	Set auto trig mode\n\
				\'f\':	Set fpga trig mode\n\
				\'g\':	Set fpga trig frequency\n\
				\'h\':	Set expo value\n\
				\'i\':	Set gain value\n\
				\'j\':	Set auto-gain-expo function\n\
				\n\
				\'l\':	Begin to capture\n\
				\n\
				\'D\':	Sve vedio\n\
				\'E\':	Save all\n\
				\'F\':	Write sensor\n\
				\'G\':	Read sensor\n\
				\n\
				\'z\':	Exit\n"\
		      );
		char ch=getchar();
		getchar();
		printf("Your choice is %c\n", ch);
		switch(ch)
		{
			case MAIN_RESOLU_SELECT:
				{
					printf("Please input your choice ...\n");
					printf("\
							\'0\':	1280x720\n\
							\'4\':	1280x800\n"\
					      );
					char ch=getchar();
					getchar();	
					printf("Your choice is %c\n", ch);
					switch(ch)
					{
						case MAIN_RESOLU_1280_720:
							pCamInUse->SetResolution(RESOLU_1280_720);
							g_width=1280;
							g_height=720;
							break;
						case MAIN_RESOLU_1280_800:
							pCamInUse->SetResolution(RESOLU_1280_800);
							g_width=1280;
							g_height=800;
							break;
						default:
							printf("Bad inut ...");
					}
					break;
				}
			case MAIN_PROCTYPE_SELECT:
				{
					printf("Please input your choice ...\n");
					printf("\
							\'0\':	Normal\n\
							\'1\':	X mirror\n\
							\'2\':	Y mirror\n\
							\'3\':	XY mirror\n"\
					      );

					char ch=getchar();
					getchar();
					printf("Your choice is %c", ch);
					switch(ch)
					{
						case MAIN_PROCTYPE_N:
							pCamInUse->SetMirrorType(MIRROR_NORMAL);
							break;
						case MAIN_PROCTYPE_X:
							pCamInUse->SetMirrorType(MIRROR_X);
							break;
						case MAIN_PROCTYPE_Y:
							pCamInUse->SetMirrorType(MIRROR_Y);
							break;
						case MAIN_PROCTYPE_XY:
							pCamInUse->SetMirrorType(MIRROR_XY);
							break;
						default:
							printf("Bad inut ...\n");
					}
					break;
				}
			case MAIN_CHECK_SPEED:
				{
					unsigned int speed = 0;
					pCamInUse->GetUsbSpeed(speed);
					if(speed==LIBUSB_SPEED_SUPER)
					{
						printf("USB 3.0 device found!\n");
						pCamInUse->SendUsbSpeed2Fpga(USB_SPEED_SUPER);
					}
					else if(speed==LIBUSB_SPEED_HIGH)
					{
						printf("USB 2.0 device found!\n");
						pCamInUse->SendUsbSpeed2Fpga(USB_SPEED_HIGH);
					}
					else
					{
						printf("Device speed unknown!\n");
					}
					break;
				}
			case MAIN_TRIGMODE_AUTO:
				pCamInUse->SetTrigMode(TRIGMODE_AUTO);
				break;
			case MAIN_TRIGMODE_FPGA:
				pCamInUse->SetTrigMode(TRIGMODE_FPGA);
				break;
			case MAIN_FPGA_TRIG_FREQ_SELECT:
				{
					printf("Please input the trig frequency (Dec, 0~45), make sure the device is in FPGA trig mode\n");
					char str[10];
					memset(str,0,sizeof(str));
					fgets(str, 9,stdin);
					unsigned int freq=atoi(str);
					printf("Your input is %d \n", freq);
					pCamInUse->SetFpgaTrigFreq(freq);	
					break;				
				}
			case MAIN_AUTO_GAIN_EXPO_SELECT:
				{
					printf("Please input your choice ...\n");
					printf("\
							\'0\':	Manual 	gain, Manual 	expo\n\
							\'1\':	Manual 	gain, Auto 	expo\n\
							\'2\':	Auto	gain, Manual 	expo\n\
							\'3\':	Auto 	gain, Auto	expo\n"\
					      );
					char ch=getchar();
					getchar();
					printf("Your choice is %c\n", ch);
					switch(ch)
					{
						case '0':
							pCamInUse->SetAutoGainExpo(false, false);
							break;
						case '1':
							pCamInUse->SetAutoGainExpo(false, true);
							break;
						case '2':
							pCamInUse->SetAutoGainExpo(true, false);
							break;
						case '3':
							pCamInUse->SetAutoGainExpo(true, true);
							break;
						default:
							printf("Bad inut ...\n");					
					}
					break;
				}

			case MAIN_EXPO_VALUE_SELECT:
				{
					printf("Please input the expo value (Dec, 0~65536)\n");
					char str[10];
					memset(str,0,sizeof(str));
					fgets(str,9,stdin);
					unsigned int expoValue=atoi(str);
					printf("Your input is %d \n", expoValue);
					pCamInUse->SetExpoValue(expoValue);
					break;
				}
			case MAIN_GAIN_VALUE_SELECT:
				{
					printf("Please input the gain value (Dec, 0~64)\n");
					char str[10];
					memset(str,0,sizeof(str));
					fgets(str,9,stdin);
					unsigned int gainValue=atoi(str);
					printf("Your input is %d \n", gainValue);
					pCamInUse->SetGainValue(gainValue);
					break;
				}		
			case MAIN_CAPTURE:
				{
					cv::namedWindow("disp",CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
					cam0.StartCap(g_height,  (g_byteBitDepthNo==1? g_width: g_width*2), Disp);
					signal(SIGALRM, timerFunction);
					alarm(1);

					printf("Press any key to stop capturing\n");


					getchar();


					pthread_mutex_lock(&mutexCam);
					alarm(0);
					cam0.StopCap();
					pthread_mutex_unlock(&mutexCam);

					pthread_mutex_lock(&mutexDisp);
					cv::destroyWindow("disp");
					cv::waitKey(1);
					cv::waitKey(1);
					cv::waitKey(1);
					cv::waitKey(1);
					pthread_mutex_unlock(&mutexDisp);
					break;
				}
			case MAIN_SAVE_VEDIO:
				{
					printf("Please input your choice ...\n");
					printf("\
							\'0\':	Not save\n\
							\'1\':	Save\n"\
					      );
					char ch=getchar();
					getchar();
					printf("Your choice is %c \n", ch);
					if('0'==ch)
						g_bSaveVedio =  false;
					else if('1'==ch)
					{
						std::string filename = "videofile.avi";
						cv::Size videosize = cv::Size(g_byteBitDepthNo == 1? g_width: g_width/2,  g_height);
						h_vw.open(filename,CV_FOURCC('F', 'L', 'V', '1') /*CV_FOURCC_DEFAULT*/, 15, videosize, 0);
						//h_vw.open(filename, -1, 15.0, videosize, 1);
						if (!h_vw.isOpened())
							printf("create vedio file failed");
						printf("Start to save vedio ...");

						g_bSaveVedio = true;
					}
					else;


					break;

				}
			case MAIN_SAVE_ALL:
				{
					printf("Please input your choice ...\n");
					printf("\
							\'0\':	Not save\n\
							\'1\':	Save all\n"\
					      );
					char ch=getchar();
					getchar();
					printf("Your choice is %c \n", ch);
					if('0'==ch)
						g_bSave_all = false;
					else if('1'==ch)
						g_bSave_all = true;
					else;


					break;

				}
			case MAIN_WRITE_SENSOR_REG:
				{
					printf("Please input the reg address (Hex. Just number, no \'0x\' needed))\n");
					char str[10];
					memset(str,0,sizeof(str));
					fgets(str,9,stdin);
					unsigned int iDecRegAddr=hex2dec(str);
					printf("Your input is %x \n", iDecRegAddr);
					printf("Please input the reg value (Hex. Just number, no \'0x\' needed))\n");
					memset(str,0,sizeof(str));
					fgets(str,9,stdin);
					unsigned int iDecRegValue=hex2dec(str);
					printf("Your input is %x \n", iDecRegValue);
					pCamInUse->WrSensorReg(iDecRegAddr, iDecRegValue);
					break;
				}
			case MAIN_READ_SENSOR_REG:
				{
					printf("Please input the reg address (Hex. Just number, no \'0x\' needed))\n");
					char str[10];
					memset(str,0,sizeof(str));
					fgets(str,9,stdin);
					unsigned int iDecRegAddr=hex2dec(str);
					printf("Your input is %x \n", iDecRegAddr);
					unsigned int iValue=0;
					pCamInUse->RdSensorReg(iDecRegAddr, iValue);
					printf("reg value is %04x\n", iValue&0xffff);
					break;
				}
			case MAIN_EXIT_NORMAL:
				cam0.ReleaseInterface();
				CCqUsbCam::CloseUSB();
				printf("Exiting ...\n");
				exit(0);
				break;
			default:
				printf("Bad inut ...\n");
		}

	}



	return 0;

}



