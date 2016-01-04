#include  <includes.h>

/***************************************************************/
//任务控制块
static  OS_TCB   AppTaskTASK1TCB;
static  OS_TCB   AppTaskTASK2TCB;
static  OS_TCB   AppTaskTASK3TCB;
//任务堆栈
static  CPU_STK  AppTaskTASK1Stk[APP_CFG_TASK_TASK1_STK_SIZE];
static  CPU_STK  AppTaskTASK2Stk[APP_CFG_TASK_TASK2_STK_SIZE];
static  CPU_STK  AppTaskTASK3Stk[APP_CFG_TASK_TASK3_STK_SIZE];

/***************************************************************/
PQueueInfo pUart3QueueInfo;//串口缓冲
/***************************************************************/


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//开始任务
void  AppTaskStart (void *p_arg)
{
	OS_ERR  err;
	CPU_Init();//初始化cpu，禁用中断，开启时间戳等
	BSP_Tick_Init();//设置滴答定时器
/*------------------------------*/		 
	OSTaskCreate((OS_TCB       *)&AppTaskTASK1TCB,              
                 (CPU_CHAR     *)"task1",
                 (OS_TASK_PTR   )task1, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK1_PRIO,
                 (CPU_STK      *)&AppTaskTASK1Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK1_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK1_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
/*------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskTASK2TCB,              
                 (CPU_CHAR     *)"task2",
                 (OS_TASK_PTR   )task2, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK2_PRIO,
                 (CPU_STK      *)&AppTaskTASK2Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK2_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK2_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);  
/*-------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskTASK3TCB,              
                 (CPU_CHAR     *)"task3",
                 (OS_TASK_PTR   )task3, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK3_PRIO,
                 (CPU_STK      *)&AppTaskTASK3Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK3_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK3_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	//创建好其他任务后就删除自己
    OSTaskDel((OS_TCB*)0,&err);
}









///////////////////////////////////////////////////////////////////////////////////////////////////////////
//用户任务1
void task1(void)
{
	OS_ERR      err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task2(void)
{
	OS_ERR      err;
    while(1)
    {
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err);
		//printf("Current Time:%s\r\n",get_time());
    }
}





 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task3(void)
{
	char DEVICE_ID[50];
	OS_ERR      err;
	pUart3QueueInfo = malloc(sizeof(CQueueInfo));//申请内存.串口缓冲
	memset(pUart3QueueInfo, 0, sizeof(CQueueInfo));
	
	printf("-----SCT15001-Version 0.0------\r\n          @Copyright SZDT Technology.\r\n");
	
	bsp_io_init();//IO口
	printf("Uart Init                 Ok.\r\n");
	printf("Control IO Init           Ok.\r\n");
	SW_12V(1);//电源
	SW_5V(1);//正负电源，用于检测回波
	printf("Power Control IO Init     Ok.\r\n");
/*********************************************************/
	read_dev_id(DEVICE_ID);
	printf("CHIP ID: %s\r\n",DEVICE_ID);
	voltage_adc_init();//内部电压
	printf("Inner ADC Init            Ok.\r\n");
	printf("Inner ADC Value:          %f v\r\n",get_dev_voltage(get_adc_value()));
	rain_gpio();//雨量GPIO初始化
	printf("RainCounter Init          Ok.\r\n");
	FreqModuleInit();//测频率模块初始化
	printf("FreqModule Init           Ok.\r\n");
	LTC2402Init();//温度采集初始化
	printf("LTC2402 Init              Ok.\r\n");
	printf("RTC Configration......\r\n");
	RTC_Config(inner);
	printf("RTC Configed In Inner Clock Mode.\r\n");
	printf("Current Time:<-%s->\r\n",get_time2());
	printf("Board All Moudles Init Done.\r\n");
/********************************************************/
	GetFreq(1);//获取频率
	
	
	
	while(1)
	{
		AppMain();//假如有串口数据
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_DLY,&err);
	}
}


