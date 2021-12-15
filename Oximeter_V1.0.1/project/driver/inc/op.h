/**
 *****************************************************************************
 * @brief   OP driver header.
 *
 * @file    OP.h
 * @author
 * @date   28/JUN/2020
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

#ifndef __OP_H__
#define __OP_H__

#include "tcasxxx.h"


#define OP_1        0
#define OP_2        1

#define             OPA_VIP_VREF_NONE           0x0
#define             OPA_VIP_VREF_VCM            0x1
#define             OPA_VIP_VREF_IADC           0x2
#define             OPA_VIP_VREF_VADC           0x3

/*
    in phase: 000-111£ºx1.21,x1.45,x1.94,x2.9,x4.85,x8.72,x16.5,x32
    inverse: 000-111£ºx0.25,x0.5,x1,x2,x4,x8,x16,x32
*/
#define             OPA_GAIN_LEVEL0             0x0
#define             OPA_GAIN_LEVEL1             0x1
#define             OPA_GAIN_LEVEL2             0x2
#define             OPA_GAIN_LEVEL3             0x3
#define             OPA_GAIN_LEVEL4             0x4
#define             OPA_GAIN_LEVEL5             0x5
#define             OPA_GAIN_LEVEL6             0x6
#define             OPA_GAIN_LEVEL7             0x7


#define             OPA_VIP_NONE                0x0
#define             OPA_VIP_VREF                0x1
#define             OPA_VIP_CHIP                0x2
#define             OPA_VIP_PAD                 0x3

#define             OPA_VIN_BUFFER              0x0
#define             OPA_VIN_PGA                 0x1
#define             OPA_VIN_CHIP                0x2
#define             OPA_VIN_PAD                 0x3

#define             OPA_OUT_CHIP                0x0
#define             OPA_OUT_PAD                 0x1


#define             OPA_RES_GND                 0x0
#define             OPA_RES_CHIP                0x1
#define             OPA_RES_VIN_PAD             0x2
#define             OPA_RES_VIP_PAD             0x3

#define             OP2_VIN_PAD_PC07            0x1
#define             OP2_VIN_PAD_PC04            0x2

#define             OP2_VIP_PAD_PD00            0x1
#define             OP2_VIP_PAD_PC03            0x2

#define             OP2_OUT_PAD_PD01            0x1
#define             OP2_OUT_PAD_PC05            0x2




typedef struct
{
    uint8_t opaOutSelect;
    uint8_t opaBufferEn;
    uint8_t opaGainLevel;
    uint8_t opaPosConfig;
    uint8_t opaNegConfig;
    uint8_t opaResConfig;
    uint8_t opaVipSelect;
    uint8_t opaOutPadEn;
    uint8_t opaFeedbackEn;
} Opa_Init_t;





void OPA_VcmEnable(void);
void OPA_VcmDisable(void);
void OPA_VcmConfig(uint16_t Volt);

/**
 *******************************************************************************
** \brief OP1_Init
 *  [in] nOpx - OP_1,OP_2
 ** Opa_Init_t£º
 ** \param [u8]  opaOutSelect:      OPA_OUT_CHIP
                                    OPA_OUT_PAD

 ** \param [u8]  opaBufferEn:       ENABLE
                                    DISABLE

 ** \param [u8]  opaGainLevel:      OPA_GAIN_LEVEL0
                                    OPA_GAIN_LEVEL1
                                    OPA_GAIN_LEVEL2
                                    OPA_GAIN_LEVEL3
                                    OPA_GAIN_LEVEL4
                                    OPA_GAIN_LEVEL5
                                    OPA_GAIN_LEVEL6
                                    OPA_GAIN_LEVEL7

 ** \param [u8]  opaPosConfig:      OPA_VIP_NONE
                                    OPA_VIP_VREF
                                    OPA_VIP_CHIP
                                    OPA_VIP_PAD

 ** \param [u8]  opaNegConfig:      OPA_VIN_BUFFER
                                    OPA_VIN_PGA
                                    OPA_VIN_CHIP
                                    OPA_VIN_PAD

 ** \param [u8]  opaResConfig:      OPA_RES_GND
                                    OPA_RES_CHIP
                                    OPA_RES_VIN_PAD
                                    OPA_RES_VIP_PAD

 ** \param [u8]  opaVipSelect:      OPA_VIP_VREF_NONE
                                    OPA_VIP_VREF_VCM
                                    OPA_VIP_VREF_IADC
                                    OPA_VIP_VREF_VADC

 ** \param [u8]  opaOutPadEn:       ENABLE
                                    DISABLE

  ** \param [u8]  opaFeedbackEn:    ENABLE
                                    DISABLE

 ** \retval[void] void
 ******************************************************************************/
void OPA_Init(uint8_t nOpx, Opa_Init_t * pInit);

/**
 *******************************************************************************
** \brief OPA_Enable
 **
 ** \param [in] nOpx - OP_1,OP_2
 ** \retval[void] void
 ******************************************************************************/
void OPA_Enable(uint8_t nOpx);

/**
 *******************************************************************************
** \brief OPA_Disable
 **
 ** \param [in] nOpx - OP_1,OP_2
 ** \retval[void] void
 ******************************************************************************/
void OPA_Disable(uint8_t nOpx);

/**
 *******************************************************************************
** \brief OP1_GainConfig
 ** \param [in] nOpx - OP_1,OP_2
 ** \param [u8] Level:   OPA_GAIN_LEVEL0
                            OPA_GAIN_LEVEL1
                            OPA_GAIN_LEVEL2
                            OPA_GAIN_LEVEL3
                            OPA_GAIN_LEVEL4
                            OPA_GAIN_LEVEL5
                            OPA_GAIN_LEVEL6
                            OPA_GAIN_LEVEL7


 ** \retval[void] void
 ******************************************************************************/
void OPA_GainConfig(uint8_t nOpx, uint8_t Level);


void OP2_VinPadConfig(uint8_t vin_pad);
void OP2_VipPadConfig(uint8_t vip_pad);
void OP2_OutPadConfig(uint8_t out_pad);




#endif      //#ifndef __OP_H__



