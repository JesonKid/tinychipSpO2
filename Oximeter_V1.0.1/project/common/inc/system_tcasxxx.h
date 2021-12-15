/**
 *****************************************************************************
 * @brief   system_tcasxxx head file defination.
 *
 * @file   system_tcasxxx.h
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

#ifndef __SYSTEM_TCASXXX_H__
#define __SYSTEM_TCASXXX_H__


#include "base_types.h"

/*******************************************************************************
* Definitions
******************************************************************************/
extern uint32_t SystemCoreClock;


/*******************************************************************************
* API
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

#define CONFIG_SYSTEM_CORE_CLK      HCLK_SRC_FMULT // FMULT double 16M RC frequency

typedef enum system_hclk_src
{
    HCLK_SRC_RC16M,
    HCLK_SRC_XTAL16M,
    HCLK_SRC_FMULT,
    HCLK_SRC_RC32K,
    HCLK_SRC_OSC32K,
    HCLK_SRC_DFLL,
} enumSystemHclkSrc_t;


typedef enum module_fclk_src
{
    FCLK_SRC_16M,
    FCLK_SRC_FMULT,
    FCLK_SRC_32K,
    FCLK_SRC_DFLL
} enumModuleFclkSrc_t;

uint32_t GetFclkVal(void);
void SystemCoreClockUpdate (void);
uint32_t SystemGetHClkFreq(void);
void SystemSetDebugEnable(uint8_t disable);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* __SYSTEM_TCASXXX_H__ */
