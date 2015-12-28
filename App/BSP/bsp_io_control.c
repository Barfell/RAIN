#include <includes.h>



void bsp_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|
						   RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|
						   RCC_AHB1Periph_GPIOE,ENABLE);  
	/*-----------------------SW_POWER------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2;//12v��5V
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*-----------------------SW_TRCN------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10;//����ͽ���ѡ��
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*-----------------------SW_VW--------------------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC,&GPIO_InitStructure);//PC4 PC5 PE8----ͨ��ѡ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void SW_12V(unsigned char onOroff)
{
	if(onOroff == 1 )
		{GPIO_SetBits(GPIOC,GPIO_Pin_1);}
	else
		{GPIO_ResetBits(GPIOC,GPIO_Pin_1);}
}
void SW_5V(unsigned char onOroff)
{
	if(onOroff == 1 )
		{GPIO_SetBits(GPIOC,GPIO_Pin_2);}
	else
		{GPIO_ResetBits(GPIOC,GPIO_Pin_2);}
}
void SW_VW_TRCN(unsigned char onOroff)
{
	if(onOroff == 1 )
		{GPIO_SetBits(GPIOB,GPIO_Pin_10);}
	else
		{GPIO_ResetBits(GPIOB,GPIO_Pin_10);}
}





//ͨ��ѡ��
void SW_VW(unsigned char onOroff)
{
	switch(onOroff)
	{
		case 2:	GPIO_SetBits(GPIOE,GPIO_Pin_8);//001
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				break;
		case 3:	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//010
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
				GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				break;
		case 4:	GPIO_SetBits(GPIOE,GPIO_Pin_8);//011
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
				GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				break;
		case 5:	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//100
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				GPIO_SetBits(GPIOC,GPIO_Pin_5);
				break;
		case 6:	GPIO_SetBits(GPIOE,GPIO_Pin_8);//101
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				GPIO_SetBits(GPIOC,GPIO_Pin_5);
				break;
		case 7:	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//110
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
				GPIO_SetBits(GPIOC,GPIO_Pin_5);
				break;
		case 8:	GPIO_SetBits(GPIOE,GPIO_Pin_8);//111
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
				GPIO_SetBits(GPIOC,GPIO_Pin_5);
				break;
		case 1:	GPIO_ResetBits(GPIOE,GPIO_Pin_8);//000
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				GPIO_ResetBits(GPIOC,GPIO_Pin_5);
				break;
		default:break;
	}
}







void yuliang_gpio(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);  
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource0);

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;//Pd0
    EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

