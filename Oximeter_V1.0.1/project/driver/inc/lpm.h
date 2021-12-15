/**
 *****************************************************************************
 * @brief   sleep header file
 *
 * @file   lpm.h
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
#ifndef __LPM_H__
#define __LPM_H__

#include "tcasxxx.h"


/*******************************************************************************
* Definitions：
******************************************************************************/
/* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */
//#define WAKEUP_BY_AON_KEY
//#define WAKEUP_BY_RTC
//#define USING_RC32K
#define USING_RC16M
//#define RC32K_TEST
#define UART_USE_RC32K
//#define WAKEUP_BY_LPUART
//#define WAKEUP_BY_IWDG
//#define IWDG_TEST_MODE

typedef enum SLEEP_MODE
{
    ACTIVE_MODE,
    IDLE_MODE,
    SLEEPWALK_MODE,
    SLEEP_MODE,
    HIBERBNATE_MODE,
    SHUTDOWN_MODE,
    SLEEP_MODE_MAX
}SLEEP_MODE_E;


/*******************************************************************************
* API：外部函数(可加 extern 修饰)的声明
******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

void EnterLpmPmuState(SLEEP_MODE_E state, boolean_t bOnExit);
void SwitchOffModuleClk(void);
void SwitchOffModulePower(void);

#ifdef __cplusplus
}
#endif
#endif /* __LPM_H__ */

