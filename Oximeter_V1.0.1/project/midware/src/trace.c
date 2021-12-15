/**
 *****************************************************************************
 * @brief   trace file.
 *
 * @file    trace.c
 * @author  
 * @date    26/APR/2021
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */
 
#include "trace.h"

#if LOG_USE_UART
volatile uint8_t  g_DebugIndex = 0; 
static uartconfig_t printCfg;


int __attribute__((weak)) fputc(int ch, FILE *f)
{
    if(g_DebugIndex == 1)
    {
        UART_SendOneData(printCfg.uart, (uint8_t)ch);
        while(UART_GetFlagStatus(printCfg.uart, 0x40) == 0);
    }
    return (ch);
}


/********************************************************
@brief  串口打印使能
@param  disable 1- 打开打印；0-关闭打印
@retval void
*********************************************************/
void DebugPrintEnable(uint8_t disable)
{
    g_DebugIndex = disable;
}

/********************************************************
@brief  打印初始化
@param  void
@retval void
*********************************************************/
void PrintInit(T_UART* uart, uint32_t baud, uint8_t pinmuxOpt)
{
    printCfg.uart           = uart,
    printCfg.baudRate       = baud,
    printCfg.wordLength     = UART_WORDLENGTH_8B,
    printCfg.stopBits       = UART_STOPBITS_1,
    printCfg.parity         = UART_PARITY_NONE,
    printCfg.hwFlowCtl      = UART_HWCONTROL_NONE,
    printCfg.txPolarity    = UART_TX_POL_POSITIVE,
    printCfg.rxPolarity    = UART_RX_POL_POSITIVE,
    printCfg.txFifoTriggerLevel = 1;
    printCfg.rxFifoTriggerLevel = 1;
    printCfg.dmaConfig.rxDmaEn = 0;
    printCfg.dmaConfig.txDmaEn = 0;
    printCfg.pinmuxOption = pinmuxOpt;
    ANAREG_TRIM_MISC->trim_oscrc16m_value = ((*(volatile uint32_t *)0x10800d18)&0xFFFF);
    UART_Init(&printCfg);
    DebugPrintEnable(1);
}

#endif

#if LOG_USE_RTT

/********************************************************
*@brief
*@param
*@retval
*********************************************************/
void PrintInit()
{
   //SEGGER_RTT_Init();
}

#endif


