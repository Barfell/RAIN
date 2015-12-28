#ifndef __APP_H_
#define __APP_H_
#include  <includes.h>

#define  APP_CFG_SERIAL_EN                            DEF_DISABLED

//配置任务的运行环境
//优先级
#define  APP_CFG_TASK_START_PRIO                              2u
#define  APP_CFG_TASK_TASK1_PRIO                              5u
#define  APP_CFG_TASK_TASK2_PRIO                              3u
#define  APP_CFG_TASK_TASK3_PRIO                              6u
#define  APP_CFG_TASK_TASK4_PRIO                              4u

//任务堆栈大小
#define  APP_CFG_TASK_START_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK1_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK2_STK_SIZE                      1024u
#define  APP_CFG_TASK_TASK3_STK_SIZE                      4048u

//任务声明
void  AppTaskStart 					(void *p_arg);//起始任务（固定的）
void  task1                 (void);//任务1
void  task2                 (void);//任务2
void  task3                 (void);
void  task4                 (void);


#endif

