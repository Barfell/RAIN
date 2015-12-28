/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/
//此文件用于包含各种需要包含的头文件
#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <string.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/


/*------stlib---------*/
#include  "stm32f4xx.h"
/*------bsp-----------*/
#include  "bsp.h"
#include  "bsp_init.h"
#include  "bsp_usart.h"
#include  "bsp_ltc2402.h"
#include  "bsp_sst25vf064.h"
#include  "bsp_rtc.h"
#include  "bsp_io_control.h"
#include  "bsp_485.h"
#include  "flash.h"

/*------app-----------*/
#include  "app.h"
#include  "FreqMeasurement.h"
#include  "Tools.h"
#include  "AppMain.h"

/*
*********************************************************************************************************
*                                               SERIAL
*********************************************************************************************************
*/

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
#include  <app_serial.h>
#endif


/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif

