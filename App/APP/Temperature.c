//温度计
#include "includes.h"
//测量温度使用
void SPI1_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA5~7复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//cs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7复用为 SPI1

	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
		 
}   


void SPI1_SetSpeed(unsigned char SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 


unsigned char SPI1_ReadWriteByte(unsigned char TxData)
{		
	unsigned char u8charRet;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位		  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
	u8charRet = SPI_I2S_ReceiveData(SPI1);//返回通过SPIx最近接收的数据
	return  u8charRet;	
}


unsigned char LTC2402_TEST_EOC(void)
{
	OS_ERR      err;
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_DLY,&err);
	return LTC2402_ReadByte();
}


unsigned char LTC2402_ReadByte(void)
{
	unsigned char u8charValue;
	u8charValue = SPI1_ReadWriteByte(0XFF);    
	return u8charValue;
}


//初始化
void LTC2402Init(void)
{
    SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);//cs - low
}
//获取电阻值
double LTC2402_GetResistance(unsigned char channel)
{
    unsigned char u8charresbuf[4]={'\0'};
    unsigned char EOC = 0;
    u32 u32resbuf = 0;
    double convertionresult = 0;
	double current = 0;
	double RES = 0;
	double ch1 = 0;
	double ch0 = 0;
	unsigned char i = 0;
	
	
	
	SW_VW(channel);
    for(i = 0;i < 6;i++)
    {
    	EOC = LTC2402_TEST_EOC();
	    if(  (EOC & 0x80) == 0  )//转换完毕
    	{
			u8charresbuf[0] = EOC;
	        u8charresbuf[1] = LTC2402_ReadByte();
	        u8charresbuf[2] = LTC2402_ReadByte();
	        u8charresbuf[3] = LTC2402_ReadByte();
			u32resbuf = ((u8charresbuf[0]<<24)+(u8charresbuf[1]<<16)+(u8charresbuf[2]<<8)+(u8charresbuf[3]<<0));
	        u32resbuf = u32resbuf&0x0FFFFFF0;           
	        u32resbuf = u32resbuf >>4;
			convertionresult = (((double)u32resbuf)/16777216.0)*LTC2402_ReferenceVoltage;
			if(  EOC & 0x40  )	/*channel 1 */
				{ch1 = convertionresult;ch1=0;	}
			else				/*channel 0 */
				{ch0 = convertionresult;		}
			u8charresbuf[0] = 0;
			u8charresbuf[1] = 0;
			u8charresbuf[2] = 0;
			u8charresbuf[3] = 0;
    	}
    }
    if(ch0 > ch1)
		{RES = ch0 - ch1;}
	else
		{RES = ch1 - ch0;}
	current = (LTC2402_ReferenceVoltage - RES)/LTC2402_ReferenceResistance;
	RES = RES/current;
    return RES;    
}


//计算温度
double GetTemperature(double NTCRes)
{
	double fTem;
	if(NTCRes>PT_Min && NTCRes < PT_Max)
		{
			fTem = (sqrt((PT_A*PT_ZeroTemRes)*(PT_A*PT_ZeroTemRes)-4*PT_B*PT_ZeroTemRes*(PT_ZeroTemRes-NTCRes))-PT_A*PT_ZeroTemRes)/2/PT_B/PT_ZeroTemRes;
		}
	else if(NTCRes>NTC_Min && NTCRes < NTC_Max)
		{
			fTem = (1/((NTC_A) + (NTC_B*(log(NTCRes))) + (NTC_C*(log(NTCRes))*(log(NTCRes))*(log(NTCRes))))) - 273.2;
		}
	else
		{
			//printf("Get resistance Error");
			fTem = 0;
		}
	return(fTem);
}

