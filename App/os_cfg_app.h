//�ں����������,�жϹ�����������ͳ������ȡ�����
//����user�ļ�������Ϊ�����޸�
#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

//һЩϵͳ����
/*
************************************************************************************************************************
*                                                      CONSTANTS
************************************************************************************************************************
*/
//��������С���ж϶�ջ��С
                                                            /* --------------------- MISCELLANEOUS ------------------ */
#define  OS_CFG_MSG_POOL_SIZE            100u               /* Maximum number of messages                             */
#define  OS_CFG_ISR_STK_SIZE             256u               /* Stack size of ISR stack (number of CPU_STK elements)   */
#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY  10u               /* Stack limit position in percentage to empty            */

//��������
                                                            /* ---------------------- IDLE TASK --------------------- */
#define  OS_CFG_IDLE_TASK_STK_SIZE       128u               /* Stack size (number of CPU_STK elements)                */

//�жϷ�������
                                                            /* ------------------ ISR HANDLER TASK ------------------ */
#define  OS_CFG_INT_Q_SIZE                10u               /* Size of ISR handler task queue                         */
#define  OS_CFG_INT_Q_TASK_STK_SIZE      128u               /* Stack size (number of CPU_STK elements)                */


//��̬����                                                            /* ------------------- STATISTIC TASK ------------------- */
#define  OS_CFG_STAT_TASK_PRIO     (OS_CFG_PRIO_MAX - 2u)   /* Priority                                               */
#define  OS_CFG_STAT_TASK_RATE_HZ         10u               /* Rate of execution (10 Hz Typ.)                         */
#define  OS_CFG_STAT_TASK_STK_SIZE       128u               /* Stack size (number of CPU_STK elements)                */


//                                                            /* ------------------------ TICKS ----------------------- */
#define  OS_CFG_TICK_RATE_HZ            1000u               /* Tick rate in Hertz (10 to 1000 Hz)                     */
#define  OS_CFG_TICK_TASK_PRIO             1u               /* Priority                                               */
#define  OS_CFG_TICK_TASK_STK_SIZE       128u               /* Stack size (number of CPU_STK elements)                */
#define  OS_CFG_TICK_WHEEL_SIZE           16u               /* Number of 'spokes' in tick wheel                       */


                                                            /* ----------------------- TIMERS ----------------------- */
#define  OS_CFG_TMR_TASK_PRIO      (OS_CFG_PRIO_MAX - 2u)   /* Priority                                               */
#define  OS_CFG_TMR_TASK_RATE_HZ          10u               /* Rate for timers (10 Hz Typ.)                           */
#define  OS_CFG_TMR_TASK_STK_SIZE        128u               /* Stack size (number of CPU_STK elements)                */
#define  OS_CFG_TMR_WHEEL_SIZE            17u               /* Number of 'spokes' in timer wheel                      */

#endif
