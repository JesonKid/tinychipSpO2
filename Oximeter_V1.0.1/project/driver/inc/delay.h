/**
 *****************************************************************************
 * @brief   delay head file
 *
 * @file   delay.h
 * @author
 * @date   17/JULy/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */
#ifndef __DELAY_H__
#define __DELAY_H__

#include "tcasxxx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define SysTick_CLKSource_HCLK   0
#define SysTick_CLKSource_STCLK  1 


void delay1ms(uint32_t u32Cnt);
void delay100us(uint32_t u32Cnt);
void delay1us(uint32_t u32Cnt);
void SysTickClkSourceConfig(uint32_t source);
void DelayForCommandExeDone(void);


#ifdef __cplusplus
}
#endif
#endif /* __DELAY_H__ */

