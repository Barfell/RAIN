#ifndef __APP_H_
#define __APP_H_
#include  <includes.h>

#define  APP_CFG_SERIAL_EN                            DEF_DISABLED

//������������л���
//���ȼ�
#define  APP_CFG_TASK_START_PRIO                              2u
#define  APP_CFG_TASK_TASK1_PRIO                              5u
#define  APP_CFG_TASK_TASK2_PRIO                              3u
#define  APP_CFG_TASK_TASK3_PRIO                              6u
#define  APP_CFG_TASK_TASK4_PRIO                              4u

//�����ջ��С
#define  APP_CFG_TASK_START_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK1_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK2_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK3_STK_SIZE                      4048u

//��������
void  AppTaskStart 					(void *p_arg);//��ʼ���񣨹̶��ģ�
void  task1                 (void);//����1
void  task2                 (void);//����2
void  task3                 (void);
void  task4                 (void);


#endif

