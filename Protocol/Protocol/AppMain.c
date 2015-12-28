#include "AppMain.h"
#include "Tools.h"
#include  <includes.h>

#define APPCOMMUNICATE_PORT USART1
#define APPCOMMUNICATE_PORT_TIMEOUT		20

#define START_SYMBOL					0XFE
#define PROTOCOL_EDITION				0X01
#define DATA_DIRECTION					0X01
#define	MAINCMD_INTERNET 				0X01		//SCT主命令
#define	SUBCMD_DT_SCT	 				0X0001		//使用DT的SCT子命令
#define	SUBCMD_OTHER_SCT	 			0X0002		//使用其他SCT的子命令
#define	FIRST_END_SYMBOL	 			0X0D
#define	SECOND_END_SYMBOL	 			0X0A



enum
{
	ERROR_CMD,
	DT_CMD,
	OTHER_CMD,
};

enum
{
	DT_PROTOCOL,
	DT_UNPROTOCOL,
};


enum
{
	APP_NO_ERROR = 0,
	APP_CRC_ERROR,
	APP_MALLOC_ERROR,
};

typedef struct _tagProtocolInfo
{
	U8 u8ProtocolType;
	U8 u8ErrorCode;

}CProtocolInfo, *PProtocolInfo;

#define ARRAY_SIZEOF(s)		(sizeof(s)-1)
	
//***可调参数区
CONST U8 szDeviceName[] = "15001";
//***可调参数区
CONST U8 szDeviceInfo[] = "SCT";
CONST U8 szErrorInfo1[] = "Flow error";
CONST U8 szErrorInfo2[] = "Command error";
CONST U8 szInfo1[] = "PROTOL";
CONST U8 szInfo2[] = "UNPROTOL";
CONST U8 szEndInfo[] = "\r\n";

CONST U8 szInqueryCmd[] = "DT INFO\r\n";
	
U8 szInquery1Cmd[] = "DT XXXXX INFO\r\n";
U8 szConfigureInfo1[] = "DT xxxxx CONF UNPROTOCOL TYPE\r\n";

U8 szConfigureInfo2[] = "DT xxxxx CONF PROTOCOL TYPE\r\n";
U8 szCmd1[] = "DT XXXXX READ ALL\r\n";
U8 szCmd2[] = "DT XXXXX READ X\r\n";

U8 szCmd3[] = "DT XXXXX TIME xxxxxx\r\n";

U8 szResponse[] = "#SCTXXXXX SUCCESS\r\n";

extern PQueueInfo pUart3QueueInfo;
extern unsigned char finishflag;

extern unsigned char takeflag;
extern char yuliang[1024];
char clearbuffer = 0;


STATIC VOID AppCommunicatePort(USART_TypeDef* USARTx, U8 *pBuf, U16 u16Length)
{	
	UsartSend(USARTx, pBuf, u16Length);
}

STATIC VOID HandleDataPackage(U8 *pBuf, U16 u16Length, PProtocolInfo pProtocolInfo)
{
	U32 *pData;
	U32 u32Length;
	U32 u32TotalPackageNumber = 0;//总的包数
	U32 u32CurrentPackageNumber = 0;//当前包数
	U32 u32CrcValue = 0;
	U16 u16Loop = 0 ;
	U8 *szData;
	u32Length = u16Length + 19;
	
	pData = malloc(u32Length*4);
	
	if(pProtocolInfo->u8ProtocolType == DT_PROTOCOL)
	{
		if(pData != NULL)
		{
			for(u16Loop = 0; u16Loop < u32Length; u16Loop++)
			{
				pData[u16Loop] = pBuf[u16Loop];
			}
			u32CrcValue = SoftGenCrc32(pData, u16Length);
			free(pData);
		}
		else
		{
			pProtocolInfo->u8ErrorCode = APP_MALLOC_ERROR;
		}
		
		szData = malloc(u16Length + 25);
		
		if(pData == NULL)
		{
			pProtocolInfo->u8ErrorCode = APP_MALLOC_ERROR;
		}
		szData[0] = START_SYMBOL;					//FE
		szData[1] = PROTOCOL_EDITION;				//01
		szData[2] = HIBYTE(HIWORD(u32Length));		//长度
		szData[3] = LOBYTE(HIWORD(u32Length));		//长度
		szData[4] = HIBYTE(LOWORD(u32Length));		//长度
		szData[5] = LOBYTE(LOWORD(u32Length));		//长度
		szData[6] = DATA_DIRECTION;					//方向
		szData[7] = MAINCMD_INTERNET;				//主命令
		szData[8] = HIBYTE(SUBCMD_DT_SCT);			//子命令
		szData[9] = LOBYTE(SUBCMD_DT_SCT);			//子命令
		szData[10] = NENCRYPTION_NCOMPRESSION;		//加密情况
		szData[11] = HIBYTE(HIWORD(u32TotalPackageNumber));//包总序号
		szData[12] = LOBYTE(HIWORD(u32TotalPackageNumber));//包总序号
		szData[13] = HIBYTE(LOWORD(u32TotalPackageNumber));//包总序号
		szData[14] = LOBYTE(LOWORD(u32TotalPackageNumber));//包总序号
		szData[15] = HIBYTE(HIWORD(u32CurrentPackageNumber));//当前包序号
		szData[16] = LOBYTE(HIWORD(u32CurrentPackageNumber));//当前包序号
		szData[17] = HIBYTE(LOWORD(u32CurrentPackageNumber));//当前包序号
		szData[18] = LOBYTE(LOWORD(u32CurrentPackageNumber));//当前包序号
		memcpy(szData + 19, pBuf, u16Length);
		
		szData[19 + u16Length] = HIBYTE(HIWORD(u32CrcValue));
		szData[20 + u16Length] = LOBYTE(HIWORD(u32CrcValue));
		szData[21 + u16Length] = HIBYTE(LOWORD(u32CrcValue));
		szData[22 + u16Length] = LOBYTE(LOWORD(u32CrcValue));
		
		szData[23 + u16Length] = FIRST_END_SYMBOL;			//结束符号
		
		szData[24 + u16Length] = SECOND_END_SYMBOL;			//结束符号
	
		
		AppCommunicatePort(APPCOMMUNICATE_PORT, szData, u32Length + 6);
		
		free(szData);
	}
	else
	{
		AppCommunicatePort(APPCOMMUNICATE_PORT, pBuf, u16Length);
	}
}



STATIC VOID HandleErrorFlowProc(PProtocolInfo pProtocolInfo)
{
	U8 szData[50] = {0};

	sprintf((char *)szData, "%s=%d%s", szErrorInfo1, pProtocolInfo->u8ErrorCode, szEndInfo);
		
	AppCommunicatePort(APPCOMMUNICATE_PORT, szData, strlen((char *)szData));
}

STATIC VOID HandleErrorCmdProc(VOID)
{
	U8 szData[50] = {0};
	U16 u16Length;

	sprintf((char *)szData, "%s", szErrorInfo2);
	
	u16Length = strlen((char *)szData);
	
	sprintf((char *)szData + u16Length, (char *)szEndInfo);
	
	AppCommunicatePort(APPCOMMUNICATE_PORT, szData, strlen((char *)szData));
}

STATIC VOID GetCurrentAppConfigure(PProtocolInfo pProtocolInfo)
{
	U8 szData[100] = {0};
	U16 u16Length;
	U8 szId[50];
	
	read_dev_id(szId);	
	sprintf((char *)szData, "#%s%s=0x%s ", szDeviceInfo, szDeviceName, szId);
	
	u16Length = strlen((char *)szData);

	if(pProtocolInfo->u8ProtocolType == DT_PROTOCOL)
	{
		sprintf((char *)szData + u16Length, (char *)szInfo1);
	}
	else
	{
		sprintf((char *)szData + u16Length, (char *)szInfo2);
	}
	
	u16Length = strlen((char *)szData);
	
	sprintf((char *)szData + u16Length, (char *)szEndInfo);
	
	u16Length = strlen((char *)szData);
		
	AppCommunicatePort(APPCOMMUNICATE_PORT, szData, u16Length);
	
}

VOID HandleAllModule(PProtocolInfo pProtocolInfo)
{
	U8 szData[2048] = {0};

    char timebuf[20]={'\0'};
		
	U8 szId[50];
	
	read_dev_id(szId);

strcat(timebuf,"20"); 
strcat(timebuf,get_time2()); 

sprintf(szData, "#SCT%s=0x%s %s V:%f M1=",szDeviceName, szId,timebuf,get_dev_voltage(get_adc_value()));
strcat(szData,yuliang);
strcat(szData," P1=0 M2=0 P2=0 M3=0 P3=0 M4=0 P4=0 M5=0 P5=0 M6=0 P6=0 M7=0 P7=0 M8=0 P8=0");
strcat((char *)szData,szEndInfo);
sentusartdata(szData);

	clearbuffer = 1;
	//HandleDataPackage(szData, strlen((char *)szData), pProtocolInfo);
}

STATIC VOID HandleIndividualModule(S8 s8Value, PProtocolInfo pProtocolInfo)
{
	U8 u8Value;
	U8 szId[50];
	U8 szData[150] = {0};
	u8Value = s8Value - 0x30;
	
	
	read_dev_id(szId);
	sprintf((char *)szData, "#SCT%s=0x%s %s V:%f M%d=%f P%d= 0 %s", szDeviceName, szId,get_time(),get_dev_voltage(get_adc_value()),GetFreq(u8Value), u8Value, u8Value, szEndInfo);
	HandleDataPackage(szData, strlen((char *)szData), pProtocolInfo);
}


//同步时间------------------------------------------------------------------------
STATIC VOID HandleTIME(unsigned char* s8Value, PProtocolInfo pProtocolInfo)
{
//	unsigned char year;
//	unsigned char mounth;
//	unsigned char date;
//	unsigned char hour;
//	unsigned char min;
//	unsigned char second;
//	
//	unsigned char s_year[3];
//	unsigned char s_mounth[3];
//	unsigned char s_date[3];
//	unsigned char s_hour[3];
//	unsigned char s_min[3];
//	unsigned char s_second[3];
//	
//	strcat(s_year,*(s8Value+14));
//	strcat(s_year,*(s8Value+14));
//	
//	strcat(s_mounth,*(s8Value+14));
//	strcat(s_mounth,*(s8Value+14));
//	
//	strcat(s_date,*(s8Value+14));
//	strcat(s_date,*(s8Value+14));
//	
//	strcat(s_hour,*(s8Value+14));
//	strcat(s_hour,*(s8Value+14));
//	
//	strcat(s_min,*(s8Value+14));
//	strcat(s_min,*(s8Value+14));
//	
//	strcat(s_second,*(s8Value+14));
//	strcat(s_second,*(s8Value+14));
//	
//	year = atoi(s_year);
//	mounth = atoi(s_mounth);
//	date = atoi(s_date);
//	min = atoi(s_min);
//	second = atoi(s_second);
//	hour = atoi(s_hour);
//	
//	set_time(year,mounth,date,0,RTC_H12_PM,hour,min,second);
	printf("SCT15001 SUCCESS \r\n");
	
}


STATIC U8 HandleAppCmdProc(U8 *pBuf, U16 u16Length, PProtocolInfo pProtocolInfo);

STATIC VOID HandleProtocolData(U8 *pBuf, U16 u16Length, PProtocolInfo pProtocolInfo)
{	
	
	U16 u16Loop;
	
	U32 *pCheckData;
	
	U32 u32CrcValue, u32CheckLength, u32CrcCheckValue;
	
	u32CheckLength = u16Length - 25;

	pCheckData = malloc(u32CheckLength*4);
	
	if(pCheckData != NULL)
	{
		for(u16Loop = 0; u16Loop < u32CheckLength; u16Loop++)
		{
			pCheckData[u16Loop] = pBuf[u16Loop + 19];
		}
		u32CrcValue = SoftGenCrc32(pCheckData, u32CheckLength);
		
		free(pCheckData);
		
		u32CrcCheckValue = MAKELONG(MAKEWORD(pBuf[u16Length-3], pBuf[u16Length-4]), MAKEWORD(pBuf[u16Length-5], pBuf[u16Length-6]));
	
		if(u32CrcValue == u32CrcCheckValue)
		{
			U32 u32Length = 0;
			U8 *pData = malloc(u32Length - 19) ;
		
			u32Length = MAKELONG(MAKEWORD(pBuf[5], pBuf[4]), MAKEWORD(pBuf[3], pBuf[2]));
			
			if(pData != NULL)
			{
				memcpy(pData, pBuf + 19, u32Length - 19);

				HandleAppCmdProc(pData, u32Length - 19, pProtocolInfo);
				
				free(pData);
			}
			else
			{
				pProtocolInfo->u8ErrorCode = APP_MALLOC_ERROR;
			}

			
		}
		else
		{
			pProtocolInfo->u8ErrorCode = APP_CRC_ERROR;
		}
	}
	else
	{
		pProtocolInfo->u8ErrorCode = APP_MALLOC_ERROR;
	}
	
	
}


STATIC U8 HandleAppCmdProc(U8 *pBuf, U16 u16Length, PProtocolInfo pProtocolInfo)
{
	U8 u8Ret = 0;
		
	if((pBuf[0] == 'D') && (pBuf[1] == 'T'))
	{
		u8Ret = DT_CMD;
		if((memcmp(pBuf, szInqueryCmd, ARRAY_SIZEOF(szInqueryCmd)) == 0) && (u16Length == ARRAY_SIZEOF(szInqueryCmd)))
		{
			GetCurrentAppConfigure(pProtocolInfo);
		}
		else
		{
			if((memcmp(pBuf, szConfigureInfo1, ARRAY_SIZEOF(szConfigureInfo1)) == 0) && (u16Length == ARRAY_SIZEOF(szConfigureInfo1)))
			{
				pProtocolInfo->u8ProtocolType = DT_UNPROTOCOL;
				AppCommunicatePort(APPCOMMUNICATE_PORT, szResponse, ARRAY_SIZEOF(szResponse));		
			}
			else if((memcmp(pBuf, szConfigureInfo2, ARRAY_SIZEOF(szConfigureInfo2)) == 0) && (u16Length == ARRAY_SIZEOF(szConfigureInfo2)))
			{
				pProtocolInfo->u8ProtocolType = DT_PROTOCOL;
				AppCommunicatePort(APPCOMMUNICATE_PORT, szResponse, ARRAY_SIZEOF(szResponse));
			}
			
			else if((memcmp(pBuf, szCmd1, ARRAY_SIZEOF(szCmd1)) == 0) && (u16Length == ARRAY_SIZEOF(szCmd1)))
			{
				HandleAllModule(pProtocolInfo);
			}
			else if((memcmp(pBuf, szCmd2, ARRAY_SIZEOF(szCmd2)) == 0) && (u16Length == ARRAY_SIZEOF(szCmd2)))
			{
				HandleIndividualModule(pBuf[14], pProtocolInfo);
			}
			
			else if((memcmp(pBuf, szInquery1Cmd, ARRAY_SIZEOF(szInquery1Cmd)) == 0) && (u16Length == ARRAY_SIZEOF(szInquery1Cmd)))
			{
				GetCurrentAppConfigure(pProtocolInfo);
			}
			
			
			//同步时间---------------------------------------------------------------------------
			else if( memcmp(pBuf, szCmd3, 12) == 0) 
			{
				HandleTIME(pBuf, pProtocolInfo);
			}
			
			else
			{
				u8Ret = ERROR_CMD;
			}
		}
	}
	else if((pBuf[0] == 0xFE) && (pBuf[1] == 0X01))
	{
		HandleProtocolData(pBuf, u16Length, pProtocolInfo);
	}
		
	else if((pBuf[0] == 'O') && (pBuf[1] == 'T'))
	{		
		u8Ret = OTHER_CMD;
	}
	else
	{
		u8Ret = ERROR_CMD;
	}
	
	return u8Ret;
	
}

STATIC VOID HandleOtherCmdProc(U8 *pBuf, U16 u16Length)
{
	//TODO  透传数据给其他外界模块
	
}

STATIC PQueueInfo AppConfigure(USART_TypeDef* USARTx)
{
	PQueueInfo pQueueInfo;
	pQueueInfo = pUart3QueueInfo;
	return pQueueInfo;
}




VOID AppMain(VOID)
{
	U16 u16Length;
	U8 *szData;
	U8 u8Ret;
	PQueueInfo pQueueInfo;
	PProtocolInfo pProtocolInfo = malloc(sizeof(CProtocolInfo));
	if(pProtocolInfo == NULL)
	{
		printf("e");
	}
	szData = malloc(USER_UARTSIZE);
	if(szData == NULL)
	{
		printf("e");
	}

	
	pProtocolInfo->u8ProtocolType = DT_UNPROTOCOL;//默认DT无协议
	
	pProtocolInfo->u8ErrorCode = APP_NO_ERROR;//默认无错误
	
	pQueueInfo = AppConfigure(APPCOMMUNICATE_PORT);

	/*修改机号*/
	memcpy((char *)(szInquery1Cmd+3), szDeviceName, ARRAY_SIZEOF(szDeviceName));
	memcpy((char *)(szConfigureInfo1+3), szDeviceName, ARRAY_SIZEOF(szDeviceName));
	memcpy((char *)(szConfigureInfo2+3), szDeviceName, ARRAY_SIZEOF(szDeviceName));
	memcpy((char *)(szCmd1+3), szDeviceName, ARRAY_SIZEOF(szDeviceName));
	memcpy((char *)(szCmd2+3), szDeviceName, ARRAY_SIZEOF(szDeviceName));
	memcpy((char *)(szResponse+4), szDeviceName, ARRAY_SIZEOF(szDeviceName));
		if(GetQueueLength(pQueueInfo))
		{ 
			if(finishflag == 1)//一个命令传输结束
			{
				finishflag = 0;
				ReadUsartData(APPCOMMUNICATE_PORT, szData, &u16Length);//读取数据
				//UsartSend(APPCOMMUNICATE_PORT, szData, u16Length);
				u8Ret = HandleAppCmdProc(szData, u16Length, pProtocolInfo);//处理对应的命令
				switch(u8Ret)
				{
					case ERROR_CMD:
						HandleErrorCmdProc();
						break;
					
					case OTHER_CMD:
						HandleOtherCmdProc(szData, u16Length);
						break;
					default:
						break;
				}
				
				if(pProtocolInfo->u8ErrorCode != APP_NO_ERROR)
				{
					HandleErrorFlowProc(pProtocolInfo);
					pProtocolInfo->u8ErrorCode = APP_NO_ERROR;
				}
			}
		}
		
		free(pProtocolInfo);
		free(szData);
}
