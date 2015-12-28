//关于多任务之间的切换
/*
1.在多任务的时候，每个任务需要一个延时函数或者其他动作来阻塞本任务，
2.只有在阻塞本任务的时候进行任务切换才能有机会让低优先级的任务得到运行，
3.假如任务不在阻塞态，会一直执行高优先级的那个。
*/
#include  <includes.h>
/**********************************************************************************************************/
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
int main(void)
{
	OS_ERR  err;
	bsp_Init();//bsp初始化
	OSInit(&err);
/*--------------------------------创建一个开始任务--------------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,//指向任务控制块
                 (CPU_CHAR     *)"AppTaskStart",//任务标识串
                 (OS_TASK_PTR   )AppTaskStart, //任务指针（任务名）
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,//任务优先级
                 (CPU_STK      *)&AppTaskStartStk[0],//任务堆栈指针栈底
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10,//堆栈大小
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,//堆栈大小
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )4,//时间片？
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err
							);
	OSStart(&err);
	(void)&err;
	return (0);
}

