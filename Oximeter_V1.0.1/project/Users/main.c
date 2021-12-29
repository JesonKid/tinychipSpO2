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
#include "LightDriver.h"
#include "SegDriver.h"


/*******************************************************************************
* Definitions
******************************************************************************/
/* ˽��(����Դ�ļ���ʹ��)�ꡢö�١��ṹ��Ķ��� */


// Main Procedure
#if defined   ( __ICCARM__   ) /* iar */
int main(void)
#elif defined ( __CC_ARM )     /* keil */
int TCmain(void)
#endif
{
	  delay1ms(3000);
#ifdef LOG
    PrintInit();
#endif
    //Init Interrupt Vector Table
    IC_PowupInit();
    SYS_Hardware_Init();
	  segSetUp();
    SYS_ModuleDrive_Init();
    gSatParam.sysParam.mPulse = 62;
    gSatParam.sysParam.mSpO2 = 98;
    gSatParam.sysParam.mPerfuse = 144;
    
    SYS_Interrupt_Enable();

    while (1)
    {		 			
//			seg_show_thread(&PT_SpO2);
        /* �������� */
        // heartbeat_algorithm();
        // /* ����Ѫ�� */
        // SpO2_algorithm();
    }
}
