#include "includes.h"
//�����¶�ʹ��
void SPI1_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA5~7���ù������	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//cs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7����Ϊ SPI1

	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
		 
}   


void SPI1_SetSpeed(unsigned char SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 


unsigned char SPI1_ReadWriteByte(unsigned char TxData)
{		
	unsigned char u8charRet;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ		  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	u8charRet = SPI_I2S_ReceiveData(SPI1);//����ͨ��SPIx������յ�����
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


//��ʼ��
void LTC2402Init(void)
{
    SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_128);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);//cs - low
}
//��ȡ����ֵ
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
	    if(  (EOC & 0x80) == 0  )//ת�����
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


//�����¶�
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

