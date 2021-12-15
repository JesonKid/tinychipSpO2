/**
 *****************************************************************************
 * @brief   tia driver header.
 *
 * @file    tia.h
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

#ifndef __TIA_H
#define __TIA_H

#include "tcasxxx.h"

#define             TIA_STG1_LEVEL0                 0x0             //20uA
#define             TIA_STG1_LEVEL1                 0x2             //30uA
#define             TIA_STG1_LEVEL2                 0x1             //25uA
#define             TIA_STG1_LEVEL3                 0x3             //35uA


#define             TIA_STG2_LEVEL0                 0x0             //60uA
#define             TIA_STG2_LEVEL1                 0x1             //105uA
#define             TIA_STG2_LEVEL2                 0x2             //150uA
#define             TIA_STG2_LEVEL3                 0x3             //200uA

#define             TIA_VIP_LEVEL0                  0x0             //0.0V
#define             TIA_VIP_LEVEL1                  0x1             //0.1V
#define             TIA_VIP_LEVEL2                  0x2             //0.2V
#define             TIA_VIP_LEVEL3                  0x3             //0.3V
#define             TIA_VIP_LEVEL4                  0x4             //0.4V
#define             TIA_VIP_LEVEL5                  0x5             //0.5V
#define             TIA_VIP_LEVEL6                  0x6             //0.6V
#define             TIA_VIP_LEVEL7                  0x7             //0.7V
#define             TIA_VIP_LEVEL8                  0x8             //0.8V
#define             TIA_VIP_LEVEL9                  0x9             //0.9V
#define             TIA_VIP_LEVEL10                 0xA             //1.0V
#define             TIA_VIP_LEVEL11                 0xB             //1.1V
#define             TIA_VIP_LEVEL12                 0xC             //1.2V
#define             TIA_VIP_LEVEL13                 0xD             //1.3V
#define             TIA_VIP_LEVEL14                 0xE             //1.4V
#define             TIA_VIP_LEVEL15                 0xF             //1.5V

#define             TIA_VIP_CONECT_NONE             0x0
#define             TIA_VIP_CONECT_PGA_P            0x1
#define             TIA_VIP_CONECT_PGA_N            0x2

#define             TIA_OUT_CONECT_NONE             0x0
#define             TIA_OUT_CONECT_PGA_P            0x1
#define             TIA_OUT_CONECT_PGA_N            0x2

#define             TIA_OUT_SELECT_PAD              0x0
#define             TIA_OUT_SELECT_CHIP             0x1



#define             TIA_VIP_SELECT_EXTERNAL         0x0
#define             TIA_VIP_SELECT_INTERNAL         0x1
#define             TIA_VIP_SELECT_IDAC             0x2
#define             TIA_VIP_SELECT_VDAC             0x3


typedef enum tia_viponchip_sel
{
    TIA_OFFCHIP_VCM,
    TIA_BUFFER_OUTPUT,
    TIA_IDAC_VCM,
    TIA_VDAC_VCM
} enumTIAVipOnChipSel_t;

typedef struct
{
    uint8_t tiaStg1Level;
    uint8_t tiaStg2Level;
    uint8_t tiaPosConfig;
    uint8_t tiaOutConfig;
    //uint8_t tiaNegConfig;
    uint8_t tiaVipLevel;
    uint8_t tiaVipSelect;
    uint8_t tiaOutSelect;
    uint8_t tiaHighPowerEn;
} Tia_Init_t;

/*
***********************************************************
Tia IO configure PC06 OUT,PIN OP0_INP tia INP INPUT£¬OP0_INN tia INNINPUT
***********************************************************
*/

void TiaIOConfigure(void);
void TiaConfigureVoltagefollow(void);
void TiaConfigureComVoltageFromInternal(enumTIAVipOnChipSel_t onchip_sel);
void Tia_Init(Tia_Init_t * pInit);
void Tia_Enable(void);
void Tia_Disable(void);
void Tia_PosVrefConfig(uint8_t Level);

#endif


