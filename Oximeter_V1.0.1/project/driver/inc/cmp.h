/**
 *****************************************************************************
 * @brief   COMPARATOR0 driver header file.
 *
 * @file   cmp.h
 * @author ws
 * @date   27/MAR/2021
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */
#ifndef __CMP_H__
#define __CMP_H__

#include "tcasxxx.h"

/*******************************************************************************
* Definitions：
******************************************************************************/
/* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */

/* 外部全局变量的声明 */


/*******************************************************************************
* API：外部函数(可加 extern 修饰)的声明
******************************************************************************/
#if defined(__cplusplus)
    extern "C" {
#endif /*_cplusplus*/
#define             COMPARATOR0                 0x0
#define             COMPARATOR1                 0x1

#define             CMP_FCLK_SRC_16M            0x0
#define             CMP_FCLK_SRC_FMULT          0x1
#define             CMP_FCLK_SRC_32K            0x2

#define             CMP_FCLK_DIV2               (2-1)
#define             CMP_FCLK_DIV4               (4-1)
#define             CMP_FCLK_DIV8               (8-1)
#define             CMP_FCLK_DIV16              (16-1)
#define             CMP_FCLK_DIV32              (32-1)
#define             CMP_FCLK_DIV64              (64-1)
#define             CMP_FCLK_DIV128             (128-1)
#define             CMP_FCLK_DIV256             (256-1)


#define             CMP_PD00_CHANNEL0            0x0
#define             CMP_PD01_CHANNEL1            0x1
#define             CMP_PA00_CHANNEL2            0x2
#define             CMP_PA01_CHANNEL3            0x3
#define             CMP_PA02_CHANNEL4            0x4
#define             CMP_PA03_CHANNEL5            0x5
#define             CMP_PA06_CHANNEL6            0x6
#define             CMP_PA07_CHANNEL7            0x7

//#define             CMP_CHANNEL_TEST             0x8
#define             CMP_CHANNEL_OPAMP_IV         0x9
#define             CMP_CHANNEL_VD1AON           0xA
#define             CMP_CHANNEL_VBEGEN           0xB
#define             CMP_CHANNEL_VDAC             0xC


#define             CMP_HYSTERESIS_VOLTAGE_0_0   0x0        //0mv
#define             CMP_HYSTERESIS_VOLTAGE_0_3   0x1        //0.3mv
#define             CMP_HYSTERESIS_VOLTAGE_4_6   0x4        //4.6mv
#define             CMP_HYSTERESIS_VOLTAGE_16_0  0x5        //16mv
#define             CMP_HYSTERESIS_VOLTAGE_26_0  0x6        //26mv
#define             CMP_HYSTERESIS_VOLTAGE_33_5  0x7        //33.5mv

#define             CMP_ANALOG_FILTER_5          0x0        //5us
#define             CMP_ANALOG_FILTER_20         0x1        //20us




#define             CMP_IT_NONE                  0x0
#define             CMP_IT_FALLING               0x1
#define             CMP_IT_RISING                0x2
#define             CMP_IT_HIGH_LEVEL            0x4

#define             CMP_VREF_VD5                 0x0
#define             CMP_VREF_1V                  0x1
#define             CMP_VREF_EXT                 0x2
#define             CMP_VREF_VD1AON              0x3

#define             CMP_DAC_DIN1                 0x0
#define             CMP_DAC_DIN2                 0x1





typedef struct
{
    uint8_t cmpPosChannel;
    uint8_t cmpNegChannel;
    uint8_t cmpIrqMode;
    uint8_t cmpRevPhaseEn;
    uint8_t cmpHighSpeedEn;
    uint8_t cmpHysteresisVoltage;
    uint8_t cmpAnaFilterEn;
    uint16_t cmpAnaFilterTime;
    uint16_t cmpDigFilterTime;
}Cmp_Init_t;

void Cmp_Init(uint8_t cmpPort,Cmp_Init_t * pInit);
void Cmp_VrefConfig(uint8_t cmPort,uint8_t channel,uint8_t select,uint8_t ratio);
void Cmp_Disable(uint8_t cmPort);
void Cmp_Enable(uint8_t cmPort);
void Cmp_CLKConfig(uint8_t cmPort,uint8_t cmpFclk,uint8_t cmpFclkDiv);
void Cmp_CLKDisable(uint8_t cmPort);
void Cmp_Trim(uint8_t cmPort,uint8_t value);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif/* __AES_H__ */

