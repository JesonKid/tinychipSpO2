/**
 *****************************************************************************
 * @brief   Device Peripheral Access Layer Header File.
 *          This file contains all the peripheral register's definitions, bits
 *          definitions and memory mapping for TC80xx chip.
 *
 * @file   interrupt.h
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
#ifndef __TCINTERRUPT_H__
#define __TCINTERRUPT_H__

#include "tcasxxx.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


#define NUM_XIC_MODULE              3
#define NUM_FIC_INT                28     /* number of interrupts in FIC0 */
#define NUM_LIC_INT                24     /* number of interrupts in LIC0 */
#define NUM_AIC_INT                16     /* number of interrupts in AIC0 */

typedef void ( *ISRFunc_T )(void);



void XIC_SetVector(IRQn_Type IRQn, ISRFunc_T vector);
void XIC_EnableIRQ(IRQn_Type IRQn, ISRFunc_T vector);
void XIC_DisableIRQ(IRQn_Type IRQn);
void IC_PowupInit(void);
void IC_PowupDeinit(void);
void IC_EnableNVIC(void);
void IC_DisableNVIC(void);



#ifdef __cplusplus
}
#endif
#endif /* __TCINTERRUPT_H__ */
