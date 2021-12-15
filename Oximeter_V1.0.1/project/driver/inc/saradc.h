/**
 *****************************************************************************
 * @brief   saradc header file
 *
 * @file   saradc.h
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
#ifndef __SARADC_H__
#define __SARADC_H__

#include "tcasxxx.h"
//#include "sdadc.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define  SARADC_INPUT_WITH_PGA      (0x500)
#define  SARADC_INPUT_NO_PGA        (0x200)

#define SARADC_AIN0_PA00             (0x00)
#define SARADC_AIN1_PA01             (0x01)
#define SARADC_AIN2_PA02             (0x02)
#define SARADC_AIN3_PA03             (0x03)
#define SARADC_AIN4_PA04             (0x04)
#define SARADC_AIN5_PA05             (0x05)
#define SARADC_AIN6_PA06             (0x06)
#define SARADC_AIN7_PA07             (0x07)
#define SARADC_INP_VREFP10           (0x08)
#define SARADC_INP_BGR_REFP12        (0x09)
#define SARADC_INP_RAIL_OP1          (0x0B)
#define SARADC_INP_RAIL_OP2          (0x0C)
#define SARADC_INP_LOCAL_GND         (0x0D)
#define SARADC_INN_VREFP10           (0x08)
#define SARADC_INN_BGR_REFP12        (0x09)
#define SARADC_INN_GND_PAD           (0x0A)
#define SARADC_INN_LOCAL_GND         (0x0B)
#define SARADC_AIN_HIGN_Z            (0xFF)

#define SARADC_SCAN_PAIR_MODE        (0x01)
#define SARADC_SCAN_SEPARATE_MODE    (0x00)

#define SARADC_REF_SELECT_VDD        (0x120 | 0x00)
#define SARADC_REF_SELECT_OFFCHIP    (0x120 | 0x05)
#define SARADC_REF_SELECT_ONCHIP_25V (0x120 | 0x0A)
#define SARADC_REF_SELECT_ONCHIP_15V (0x920 | 0x0A)




void SetSARADCFclkSource(uint8_t div);
void SetSARAdcTriggerNum(enumAdcTriggerNum_t num);
void TriggerSARAdcStart(void);
void SetSARAdcConfig(void);
uint8_t GetSarAdcFifoValidState(void);

#ifdef __cplusplus
}
#endif
#endif /* __SARADC_H__ */

