//���ڶ�����֮����л�
/*
1.�ڶ������ʱ��ÿ��������Ҫһ����ʱ����������������������������
2.ֻ���������������ʱ����������л������л����õ����ȼ�������õ����У�
3.��������������̬����һֱִ�и����ȼ����Ǹ���
*/
#include  <includes.h>
/**********************************************************************************************************/
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
int main(void)
{
	OS_ERR  err;
	bsp_Init();//bsp��ʼ��
	OSInit(&err);
/*--------------------------------����һ����ʼ����--------------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,//ָ��������ƿ�
                 (CPU_CHAR     *)"AppTaskStart",//�����ʶ��
                 (OS_TASK_PTR   )AppTaskStart, //����ָ�루��������
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,//�������ȼ�
                 (CPU_STK      *)&AppTaskStartStk[0],//�����ջָ��ջ��
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10,//��ջ��С
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,//��ջ��С
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )4,//ʱ��Ƭ��
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err
							);
	OSStart(&err);
	(void)&err;
	return (0);
}

