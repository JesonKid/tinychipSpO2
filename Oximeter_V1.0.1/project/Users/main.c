/**
*****************************************************************************
* @brief
*
* @file   main.c
* @author   lt
* @date   2021-11-11
*****************************************************************************
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* Copyright (c) 2021 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/
#include "DataType.h"
#include "SystemDriver.h"
#include "Spo2Interface.h"
#include "trace.h"
#include "interrupt.h"
#include "wdg.h"
#include "delay.h"


/*******************************************************************************
* Definitions
******************************************************************************/
/* 私有(仅本源文件内使用)宏、枚举、结构体的定义 */


// Main Procedure
#if defined   ( __ICCARM__   ) /* iar */
int main(void)
#elif defined ( __CC_ARM )     /* keil */
int TCmain(void)
#endif
{
#ifdef LOG
    PrintInit();
#endif
    //Init Interrupt Vector Table
    IC_PowupInit();

    SYS_Hardware_Init();
    SYS_ModuleDrive_Init();

//    IWDG_Init(WDG_PRESCALER_32, 0xFFF, 0);  //4秒
//    IWDG_ClearITPending(WDG_IOVF_FLAG);

    SYS_Interrupt_Enable();

    while (1)
    {
        //SpO2 Frequency procedure
        //SatSpectrumThread(&gSatCfg);
        /* 喂狗 */
//        IWDG_ResetCnt();
			  
        /* 计算心率 */
//        heartbeat_algorithm();
        /* 喂狗 */
//        IWDG_ResetCnt();
        /* 计算血氧 */
//        SpO2_algorithm();
			printf("%d\n",gSatSamp.RedAC);
			delay1ms(1);
    }
}
