/**
 *****************************************************************************
 * @brief   idac header file
 *
 * @file   idac.h
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
#ifndef __IDAC_H__
#define __IDAC_H__

#include "tcasxxx.h"
#include "trim.h"
#include "gpio.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define IDAC_FIFO_DEPTH      (4)

typedef enum idac_channel
{
    CHAN_IDAC1 = 1,
    CHAN_IDAC2 = 2,
} eIDAC_CHAN_T;

// IDAC output pin setup
typedef enum
{
    OUTPUT_NONE = 0,
    OUTPUT_PA00,
    OUTPUT_PA01,
    OUTPUT_PA02,
    OUTPUT_PA03
}eIDAC_OUTPUT_T;


/*
 * Trigger IDAC transfer start
 */
void TriggerIdacStart(eIDAC_CHAN_T eCh);

/*
 * Trigger IDAC transfer stop
 */
void TriggerIdacStop(eIDAC_CHAN_T eCh);


/*
 * Set IDAC Output pin
 */
void SetIdacOutput(eIDAC_OUTPUT_T idac1, eIDAC_OUTPUT_T idac2);

/*
 * Set IDAC basic current
 * eCh-- IDAC No. basisCur--basic current (uA)
 * 
 */
void SetIdacBasicCurrent(eIDAC_CHAN_T eCh, uint8_t basicCur);

/*
 * Set IDAC Input data
 * in1-- IDAC1  in2-- IDAC2 (uA)
 * 
 */
void SetIdacIn(uint8_t in1, uint32_t in2);


#ifdef __cplusplus
}
#endif
#endif /* __IDAC_H__ */

