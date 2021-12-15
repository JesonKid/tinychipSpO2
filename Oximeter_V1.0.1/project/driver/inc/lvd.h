/**
 *****************************************************************************
 * @brief   lvd header file
 *
 * @file   lvd.h
 * @author
 * @date   25/May/2021
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
#ifndef __LVD_H__
#define __LVD_H__

#include "tcasxxx.h"
#include "gpio.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define             LVD_FCLK_16M                0x0
#define             LVD_FCLK_FMULT              0x1
#define             LVD_FCLK_32K                0x2

#define             LVD_FCLK_DIV1               (1-1)
#define             LVD_FCLK_DIV2               (2-1)
#define             LVD_FCLK_DIV4               (4-1)
#define             LVD_FCLK_DIV8               (8-1)
#define             LVD_FCLK_DIV16              (16-1)
#define             LVD_FCLK_DIV32              (32-1)
#define             LVD_FCLK_DIV64              (64-1)
#define             LVD_FCLK_DIV128             (128-1)
#define             LVD_FCLK_DIV256             (256-1)

#define             LVD_MODE_NONE               0
#define             LVD_MODE_NORMAL             1
#define             LVD_MODE_LOW_POWER          2

#define             LVD_TRIP_VOLT0              (1<<0)              //1.7V
#define             LVD_TRIP_VOLT1              (1<<1)              //1.9V
#define             LVD_TRIP_VOLT2              (1<<2)              //2.1V
#define             LVD_TRIP_VOLT3              (1<<3)              //2.3V
#define             LVD_TRIP_VOLT4              (1<<4)              //2.5V
#define             LVD_TRIP_VOLT5              (1<<5)              //2.7V
#define             LVD_TRIP_VOLT6              (1<<6)              //2.9V
#define             LVD_TRIP_VOLT7              (1<<7)              //3.1V


#define             LVD_VREF_PB01               0x0
#define             LVD_VREF_PB02               0x1
#define             LVD_VREF_PB03               0x2
#define             LVD_VREF_VD5                0x3
#define             LVD_VREF_TEST               0x4
#define             LVD_VREF_GND                0x5

#define             LVD_IT_RISLING              0x1
#define             LVD_IT_FALLING              0x2
#define             LVD_IT_HIGH_LEVEL           0x4

#define             LVD_PORT_PB01               0x1
#define             LVD_PORT_PB02               0x2
#define             LVD_PORT_PB03               0x3

#define             LVD_PORT_MODE               0x5


void LVD_CLKConfig(uint8_t clk_src,uint8_t clk_div);
void LVD_CLKDisable(void);


void LVD_Enable(void);
void LVD_Disable(void);
void LVD_ModeConfig(uint8_t mode);
void LVD_SetTripVolt(uint8_t Volt);
void LVD_SetVrefSel(uint8_t vref);
void LVD_SetFilterTime(uint16_t nTime);
void LVD_ITConfig(uint8_t IT_Mode);
void LVD_PortConfig(uint8_t IO_Port);

#ifdef __cplusplus
}
#endif
#endif /* __LVD_H__ */

