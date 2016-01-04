//振弦计
#include <includes.h>
unsigned char PulseNUM = 0;//发射脉冲个数

//用于激发振弦vw_tr
void Time3_Config(void)
{   
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	//TIM1_CLOCK= 84M
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);//CH3
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,DISABLE);//关闭，准备好发射脉冲
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//输出固定个数的pwm
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//计时单元
void Time2_Config(void)	   
{
	//84mhz
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 279;		 //   35
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,DISABLE);//一直开启
}
//tim4  vw_rx 捕获ERT
void Time4_Config(void)	   
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 214;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_SelectInputTrigger(TIM4,TIM_TS_ETRF);
	TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x2);//外部时钟引脚,模式2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = 15;
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update); 
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM4->CNT = 0;
	TIM_Cmd(TIM4,DISABLE);//初始化之后是关闭的
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//测量频率
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//计算位移量
double CalDisplacement(double Freq,double T)
{
	double G = 0.016457;
	double TM = 0.000330;
	double TB = 0.415;//Model:4450-3mm
	
	
	double DigitsPRE = 0,DigitsNEXT = 0;
	double Displacement = 0;
	DigitsPRE  = (1175.800000*1175.800000)/1000;
	DigitsNEXT = (Freq*Freq)/1000;
	Displacement = (DigitsNEXT-DigitsPRE)*G + (T - 26.300000)*((DigitsNEXT * TM) +TB)*G;
	return Displacement;
}
//使用前初始化,初始化之后直接使用GetFreq获取频率
void FreqModuleInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|
						RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE,ENABLE);
	/*-----------------------VW_TX PB0---------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);//TIM3_CH3--PWM--pb0(激励源)  tim3
	/*-----------------------VW_TRCN-------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*-----------------------POWER_SW------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*-----------------------VW_SW--------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	/*-----------------------VW_RX PE0--------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_TIM4);//输入
	//tran
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);//CH3
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,DISABLE);//关闭，准备好发射脉冲
	//count
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 279;		 //   35
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);//一直开启
	//measure
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
	TIM_TimeBaseStructure.TIM_Period = 109;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_SelectInputTrigger(TIM4,TIM_TS_ETRF);
	TIM_ETRClockMode2Config(TIM4,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x2);//外部时钟引脚,模式2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update); 
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM4->CNT = 0;
	TIM_Cmd(TIM4,DISABLE);//初始化之后是关闭的
	//nvic
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//测量频率
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//输出固定个数的pwm
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//激励发射一定频率一定数量的脉冲
//freq:频率
//num:个数
void TranPulse(unsigned int freq,unsigned char num)
{
	OutPutFreqSet(freq);
	PulseNUM = num;//脉冲个数
	TIM3->CNT = 0;// 计数清零
	if(num == 0)
	{TIM_Cmd(TIM3,DISABLE);}
	else
	{TIM_Cmd(TIM3,ENABLE);}//开启一次脉冲发射
}

//获取某个通道的频率
double GetFreq(unsigned char ch)
{
	unsigned int f = 0;
	unsigned char times = 0;
	OS_ERR		err;
	SW_VW(ch);//选择通道
	
	
	SW_VW_TRCN(0);//tran
	//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_DLY,&err);
	
		for(f = 400;f<8400;f+=100)
		{
			times = 1000*15/f;
			TranPulse(f,15);
			if(times < 1)
				{OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_DLY,&err);}
			else
				{OSTimeDlyHMSM(0,0,0,times,OS_OPT_TIME_DLY,&err);}
		}
	
	SW_VW_TRCN(1);//recive
	OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_DLY,&err);//停顿，等待激励脉冲消除
	return Measure();
}

double Measure(void)
{
	OS_ERR err;
	unsigned int OverTime = 0; 
	OnFreq = 0;
	TIM_Cmd(TIM4,ENABLE);
	while(measureflag == 0)//等待测量完毕
		{
			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_DLY,&err);
			OverTime++;
			if(OverTime > 250)//300ms超时
			{OverTime = 0;OnFreq = 0;break;}
		}
	measureflag = 0;//测量完毕后清除状态
	TIM_Cmd(TIM4,DISABLE);
	TIM4->CNT = 0;
	return OnFreq;
}

//freq:输入的频率，单位是hz
void OutPutFreqSet(unsigned int freq)
{
	unsigned int N = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//1282hz----84Mhz
	if( (freq>1282) && (freq<84000000))
		{
			N = 42000000/freq;
			TIM_TimeBaseStructure.TIM_Prescaler = N-1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
			TIM_TimeBaseStructure.TIM_Period = 1;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCInitStructure.TIM_Pulse = 1;
		}
//128hz----1282hz
	else if( (freq <= 1282) && (freq > 128) )
		{
			N = 4200000/freq;
			TIM_TimeBaseStructure.TIM_Prescaler = N-1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
			TIM_TimeBaseStructure.TIM_Period = 19;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCInitStructure.TIM_Pulse = 10;
		}
//小于128hz
		else if(freq <= 128)
		{
			N = 42000/freq;
			TIM_TimeBaseStructure.TIM_Prescaler = N-1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
			TIM_TimeBaseStructure.TIM_Period = 1999;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCInitStructure.TIM_Pulse = 1000;
		}
//最大只能是84Mhz
		else if(freq > 84000000)
		{
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;							
			TIM_TimeBaseStructure.TIM_Period = 1;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
			TIM_OCInitStructure.TIM_Pulse = 1;
		}
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
}








