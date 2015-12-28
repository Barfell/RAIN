/*
*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                          (c) Copyright 2004-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/LIB is provided in source form to registered licensees ONLY.  It is 
*               illegal to distribute this source code to any third party unless you receive 
*               written permission by an authorized Micrium representative.  Knowledge of 
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest 
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

//---------配置一些使用到的库----------------------------------------------------------------------------
/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  LIB_CFG_MODULE_PRESENT
#define  LIB_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                          uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  LIB_MEM_CFG_ARG_CHK_EXT_EN     DEF_ENABLED
                                                        /*   DEF_DISABLED     Argument check DISABLED                   */
                                                        /*   DEF_ENABLED      Argument check ENABLED                    */

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN    DEF_ENABLED
                                                        /*   DEF_DISABLED     Assembly-optimized function(s) DISABLED   */
                                                        /*   DEF_ENABLED      Assembly-optimized function(s) ENABLED    */

#define  LIB_MEM_CFG_ALLOC_EN           DEF_ENABLED
                                                        /*   DEF_DISABLED     Memory allocation DISABLED                */
                                                        /*   DEF_ENABLED      Memory allocation ENABLED                 */



//------------定义堆大小--------------------------------------------------------------------------------------------------
#define  LIB_MEM_CFG_HEAP_SIZE          23u * 1024u     /* Configure Heap Memory Size                                   */


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                  /* End of lib cfg module include.                               */

