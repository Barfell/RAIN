#include  <includes.h>

/***************************************************************/
//������ƿ�
static  OS_TCB   AppTaskTASK1TCB;
static  OS_TCB   AppTaskTASK2TCB;
static  OS_TCB   AppTaskTASK3TCB;
//�����ջ
static  CPU_STK  AppTaskTASK1Stk[APP_CFG_TASK_TASK1_STK_SIZE];
static  CPU_STK  AppTaskTASK2Stk[APP_CFG_TASK_TASK2_STK_SIZE];
static  CPU_STK  AppTaskTASK3Stk[APP_CFG_TASK_TASK3_STK_SIZE];

/***************************************************************/
PQueueInfo pUart3QueueInfo;

char yuliang[1024];
unsigned char takeflag =0;
int yuliangcishu=0;
extern unsigned char yuliang_buf[20];
extern char clearbuffer;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ����
void  AppTaskStart (void *p_arg)
{
	OS_ERR  err;
	CPU_Init();//��ʼ��cpu�������жϣ�����ʱ�����
	BSP_Tick_Init();//���õδ�ʱ��
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
				 
	//����������������ɾ���Լ�
    OSTaskDel((OS_TCB*)0,&err);
}




















///////////////////////////////////////////////////////////////////////////////////////////////////////////
//�û�����1
void task1(void)
{
	OS_ERR      err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task2(void)
{
	OS_ERR      err;
    while(1)
    {
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
    }
}

 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task3(void)
{
	OS_ERR      err;
	
	pUart3QueueInfo = malloc(sizeof(CQueueInfo));//�����ڴ�
	memset(pUart3QueueInfo, 0, sizeof(CQueueInfo));

	
	bsp_io_init();//IO��
	UsartConfig();//������������
	SW_12V(1);//��Դ
	SW_5V(1);//������Դ�����ڼ��ز�Z
	FreqModuleInit();//��Ƶ��ģ���ʼ��
	LTC2402Init();
	yuliang_gpio();
	voltage_adc_init();
	
				
	GetFreq(1);
	SW_VW(1);
	while(1)
	{
		AppMain();
		
		if(takeflag == 1)
		{
			takeflag =0;
				
			strcat(yuliang,yuliang_buf);
			
			yuliangcishu++;
			if(yuliangcishu >70)
				{yuliangcishu = 0;memset(yuliang,0,1024);}
		}
		
		if(clearbuffer == 1)
		{
			clearbuffer =0;
			takeflag = 0;
			yuliangcishu = 0;
			memset(yuliang,0,1024);
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_DLY,&err);//10������һ���Ƿ������
	}
}













