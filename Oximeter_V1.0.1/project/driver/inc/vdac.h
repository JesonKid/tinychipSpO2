/**
 *****************************************************************************
 * @brief   vdac header file
 *
 * @file   vdac.h
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
#ifndef __VDAC_H__
#define __VDAC_H__

#include "tcasxxx.h"
#include "trim.h"
#include "gpio.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define VDAC_FIFO_DEPTH         (4)
#define VDAC_OUT_RATIO          (2.412)

typedef enum tagVdacIO
{
    VDAC_PIN_NULL = 0,
    VDAC_PIN1,
    VDAC_PIN2,
} eVDAC_PIN_T;

// vout gain ctrl bit
// 000: x1.0; 001:x1.2; 010:x1.5; 011:x1.6; 100:x1.8; 101:x2.0; 110/111:nil.
typedef enum tagVDAC_GAIN
{
    VDAC_GAIN_0 = 0,        // x1.0
    VDAC_GAIN_1,            // x1.2
    VDAC_GAIN_2,            // x1.5
    VDAC_GAIN_3,            // x1.6
    VDAC_GAIN_4,            // x1.8
    VDAC_GAIN_5,            // x2.0
    VDAC_GAIN_6,            // nill
    VDAC_GAIN_7
} eVDAC_GAIN_T;

/*  <6:4>: vcm sel bit, default 010.
    000:1.2v; 001:1.4v; 010:1.5v; 011:1.6v; 100:1.8v; 101:2.0v; 110:2.5v. 111:nil.
    */
typedef enum tagVDAC_VCMSEL
{
    VDAC_VCMSEL_0 = 0,        // 1.2v
    VDAC_VCMSEL_1,            // 1.4v
    VDAC_VCMSEL_2,            // 1.5v
    VDAC_VCMSEL_3,            // 1.6v
    VDAC_VCMSEL_4,            // 1.8v
    VDAC_VCMSEL_5,            // 2.0v
    VDAC_VCMSEL_6,            // 2.5v
    VDAC_VCMSEL_7             // nil
} eVDAC_VCMSEL_T;


/*
 * Enable VDAC module
 */
void VdacBlockEnable(void);
/*
 * Disable VDAC module
 */
void VdacBlockDisable(void);

/*
 * Config VDAC output IO setting
 * pinNo -- output pin No.
 */
void ConfigVdacOutputPin(eVDAC_PIN_T pinNo);

uint8_t GetVdacFifoValidState(void);
void TriggerVdacStart(void);
void TriggerVdacStop(void);
uint8_t GetVdacFifoFreeNum(void);

/*
 * Config VDAC setting
 * pinNo -- output pin No.
 * eVcm -- vcm sel bit
 * eGain -- vout gain ctrl bit
 */
void VdacConfigure(eVDAC_PIN_T pinNo, eVDAC_VCMSEL_T eVcm, eVDAC_GAIN_T eGain);


void SetVdacFclkSource(uint8_t clkdiv);

void SetVdacOutputValue(uint16_t value);


#ifdef __cplusplus
}
#endif
#endif /* __VDAC_H__ */

