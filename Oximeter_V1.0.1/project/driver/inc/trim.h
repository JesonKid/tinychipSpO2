/**
 *****************************************************************************
 * @file   trim.h
 * @author
 * @date   15/Jan/2021
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
#ifndef __TRIM_H__
#define __TRIM_H__

#include "tcasxxx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum rc16m_trim
{
    RC16M_TRIM_16_DATA = 0xB548,
    RC16M_TRIM_24_DATA = 0x8dB8,
    RC16M_TRIM_27_DATA = 0x85F8,
} enumRC16mTrim_t;
typedef enum rtc_threl_trim
{
    RC32K_PPM0 = 0x3B438,
    RC32K_PPM1 = 0x8CB4,
    RC32K_PPM2 = 0xD6674,
    RC32K_PPM3 = 0x2D5A4,
} enumRTCThrelTrim_t;
    
typedef enum vdac_trim_tp
{
    VDAC_TRIM_TP_0 = 0x0000,
    VDAC_TRIM_TP_1 = 0x1000,
    VDAC_TRIM_TP_2 = 0x1FFF,
} enumVDACTrimTP_t;
typedef enum vdac_trim_style
{
    VDAC_GAIN = 0,
    VDAC_DISORDER = 1,
} enumVDACTrimStyle_t;
typedef enum idac_trim
{
    IDAC0 = 0,
    IDAC1 = 1,
} enumIDACTrim_t;

typedef enum cmp_trim
{
    CMP0 = 0,
    CMP1 = 1,
} enumCMPTrim_t;
typedef enum cmp_source
{
    CMP_AN_CHANNEL_0 = 0,
    CMP_AN_CHANNEL_1 = 1,
    CMP_AN_CHANNEL_2 = 2,
    CMP_AN_CHANNEL_3 = 3,
    CMP_AN_CHANNEL_4 = 4,
    CMP_AN_CHANNEL_5 = 5,
    CMP_AN_CHANNEL_6 = 6,
    CMP_AN_CHANNEL_7 = 7,
    CMP_AN_TEST_INPUT = 8,
    CMP_AN_OPAMP_IV  = 9,
    CMP_AN_VD1AON    = 10,
    CMP_AN_VBEGEN    = 11,
    CMP_AN_INT_DAC   = 12,
} enumCMPSource_t;
typedef enum cmp_dac
{
    CMP_DAC_VD5    = 0,
    CMP_DAC_1V     = 1,
    CMP_DAC_VREFP  = 2,
    CMP_DAC_VD1AON = 3,
    CMP_DAC_UNUSED = 0xFF,
} enumCMPDAC_t;



void LDO15TrimToTarget(uint32_t TrimVal);
void LDOLPTrimToTarget(uint32_t TrimVal);

void LDO3TrimToTarget(uint32_t TrimVal);

void VbiasIbiasTrimToTarget(uint32_t TrimVal);

void BandGapTrimToTarget(uint32_t TrimVal);

void RC32kTrimToTarget(uint32_t trim_val);

void RC16mTrimToTarget(enumRC16mTrim_t      targetFreq);
void RTCThrelTrimToTarget(enumRTCThrelTrim_t ppma0,enumRTCThrelTrim_t ppma1, enumRTCThrelTrim_t ppma2,enumRTCThrelTrim_t ppma3);


void VDACTrimToTarget(uint32_t TrimValue,enumVDACTrimStyle_t Style,enumVDACTrimTP_t TestPoint);

void IDACTrimToTarget(uint32_t TrimValue,enumIDACTrim_t idac);

#ifdef __cplusplus
}
#endif
#endif /* __TRIM_H__ */

