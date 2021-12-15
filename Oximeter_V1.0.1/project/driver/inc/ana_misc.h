/**
 *****************************************************************************
 * @brief   ana misc header file
 *
 * @file   saradc.h
 * @author
 * @date   8/Aug/2021
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
#ifndef __ANA_MISC_H__
#define __ANA_MISC_H__

#include "tcasxxx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


#define VBIAS_VCM_SEL_NONE      0x0000      // do not output
#define VBIAS_VCM_SEL_02V       0x0001      // 0.2V
#define VBIAS_VCM_SEL_03V       0x0002      // 0.3V
#define VBIAS_VCM_SEL_04V       0x0004      // 0.4V
#define VBIAS_VCM_SEL_05V       0x0008      // 0.5V
#define VBIAS_VCM_SEL_06V       0x0010      // 0.6V
#define VBIAS_VCM_SEL_07V       0x0020      // 0.7V
#define VBIAS_VCM_SEL_08V       0x0040      // 0.8V
#define VBIAS_VCM_SEL_09V       0x0080      // 0.9V
#define VBIAS_VCM_SEL_10V       0x0100      // 1.0V
#define VBIAS_VCM_SEL_11V       0x0200      // 1.1V
#define VBIAS_VCM_SEL_12V       0x0400      // 1.2V
#define VBIAS_VCM_SEL_13V       0x0800      // 1.3V
#define VBIAS_VCM_SEL_14V       0x1000      // 1.4V
#define VBIAS_VCM_SEL_15V       0x2000      // 1.5V


/*
 * Configure PGA module enable
 */
void PGA_EnableModule(void);
/*
 * Configure PGA module disable
 */
void PGA_DisableModule(void);

/*
 * Configure BGR module enable
 */
void BGR_EnableModule(void);

/*
 * Configure BGR module Disable
 */
void BGR_DisableModule(void);

/*
 * Configure vbias_ibias module enable
 */
void Vbias_Enable(void);

/*
 * Configure vbias_ibias module disable
 */
void Vbias_Disable(void);

void Vbias_SelVcm(uint16_t vcm_sel);

void LDO3_Enable(void);
void LDO3_Disable(void);

void XOSC32K_Enable(void);
void XOSC32K_Disable(void);

void XOSC16M_Enable(void);
void XOSC16M_Disable(void);




#ifdef __cplusplus
}
#endif
#endif /* __ANA_MISC_H__ */


