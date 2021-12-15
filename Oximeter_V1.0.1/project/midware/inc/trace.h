/**
 *****************************************************************************
 * @brief   Tiemr driver header.
 *
 * @file   timer.h
 * @author
 * @date   28/JUN/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __TRACE_H__
#define __TRACE_H__

#include "tcasxxx.h"
#include "system_tcasxxx.h"
#include "uart.h"

  
#define LOG_USE_RTT    1
#define LOG_USE_UART   0

#if (LOG_USE_RTT && LOG_USE_UART)
#error "only one option permit!"
#endif

#if (LOG_USE_RTT==0 && LOG_USE_UART==0)
#undef LOG
#else
#define LOG
#endif



#if LOG_USE_UART

/********************************************************
@brief  打印初始化
@param  void
@retval void
*********************************************************/
void PrintInit(T_UART* uart, uint32_t baud, uint8_t pinmuxOption);

/********************************************************
@brief  串口打印使能
@param  disable 1- 打开打印；0-关闭打印
@retval void
*********************************************************/
void DebugPrintEnable(uint8_t disable);

#endif

#if LOG_USE_RTT
#include "SEGGER_RTT.h"

#define printf(...) SEGGER_RTT_printf(0,__VA_ARGS__)
void PrintInit(void);

#endif

#endif //__TRACE_H__

