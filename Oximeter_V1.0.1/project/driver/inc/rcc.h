/**
*****************************************************************************
* @brief
*
* @file   rcc.h
* @author   lt
* @date   2021-08-18
*****************************************************************************
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* Copyright (c) 2021 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/
#ifndef __RCC_H__
#define __RCC_H__

#include "tcasxxx.h"
#include "stdbool.h"

#define AON_FD_POS        3
#define ANA_FD_POS        1
#define AON_FD_DISABLE    0
#define ANA_FD_DISABLE    0


#define ACLK_LSI_RC       0
#define ACLK_LSE_OSC      1

#define RC_40K            0
#define RC_32K            1

#define ANA_RC_16M_POS    1


/**
* @brief ahb总线时钟源
*/
typedef enum
{
    AHB_LS  = 4,
    AHB_HS  = 2,
    AHB_MUL = 3
} AHB_CLK_SRC_T;

/**
* @brief ahb总线时钟分频系数
*/
typedef enum
{
    AHB_DIV_1  = 0,
    AHB_DIV_2  = 1,
    AHB_DIV_4  = 3,
    AHB_DIV_8  = 7,
    AHB_DIV_16 = 15
} AHB_CLK_DIV_T;

/**
* @brief apb总线时钟分频系数
*/
typedef enum
{
    APB_DIV_1  = 0,
    APB_DIV_2  = 1,
    APB_DIV_4  = 3,
    APB_DIV_8  = 7
} APB_CLK_DIV_T;

/**
* @brief 低速时钟源
*/
typedef enum
{
    LSI_RC  = 0,
    LSE_OSC = 1
} LS_CLK_SRC_T;

/**
* @brief 高速时钟源
*/
typedef enum
{
    HSI_RC  = 0,
    HSE_OSC = 1,
} HS_CLK_SRC_T;

/**
* @brief
*/
typedef enum
{
    PERIPH_SDM = 0,
    PERIPH_RTC,
    PERIPH_LPUART_HS,
    PERIPH_SYSTICK,
    
    PERIPH_LPGPT,
    PERIPH_UART2,
    PERIPH_CRYPTO,
    PERIPH_GPT0,
    PERIPH_GPT1,
    PERIPH_GPT2,
    PERIPH_PWM0,
    PERIPH_PWM1,
    PERIPH_PWM2,
    PERIPH_I2C0,
    PERIPH_I2C1,
    PERIPH_SPI0,
    PERIPH_SPI1,
    PERIPH_UART0,
    PERIPH_UART1,
    PERIPH_WDG,
    PERIPH_CP0,
    PERIPH_CP1,
    PERIPH_LVD,
    PERIPH_SARADC,
    PERIPH_VDAC
} PERIPH_INSTANCE_T;

/**
* @brief 挂载在APB总线的外设
*/
typedef enum
{
    APB_SDM,
    APB_AON,
    APB_GPIO_HS,
    APB_ANALOG,
    APB_DIVIDER,
    APB_FIC,
    APB_LIC,
    APB_PINMUX,

    APB_LPGPT,
    APB_UART2,
    APB_CRYPTO,
    APB_GPT0,
    APB_GPT1,
    APB_GPT2,
    APB_PWM0,
    APB_PWM1,
    APB_PWM2,
    APB_I2C0,
    APB_I2C1,
    APB_SPI0,
    APB_SPI1,
    APB_UART0,
    APB_UART1,
    APB_WDG,
    APB_CP0,
    APB_CP1,
    APB_LVD,
    APB_SARADC,
    APB_VDAC,
} APB_INSTANCE_T;

/**
* @brief 挂载在AHB总线的外设
*/
typedef enum
{
    AHB_FLASH,
    AHB_SRAM,
    AHB_FABRIC
} AHB_INSTANCE_T;

/**
* @brief 外设工作时钟源
*/
typedef enum
{
    PERIPH_SRC_HS  = 0,
    PERIPH_SRC_MUL = 1,
    PERIPH_SRC_LS  = 2,
    PERIPH_SRC_MAX
} PERIPH_FCLK_SRC_T;

/**
* @brief 外设工作时钟分频系数
*/
typedef enum
{
    FCLK_DIV_1 = 0,
    FCLK_DIV_2 = 1,
    FCLK_DIV_4 = 3,
    FCLK_DIV_MAX
} PERIPH_FCLK_DIV_T;



bool ahb_clk_config(AHB_CLK_SRC_T src);
bool ahb_clk_div_config(AHB_CLK_DIV_T div);
bool ls_clk_config(LS_CLK_SRC_T src);
bool hs_clk_config(HS_CLK_SRC_T src);
bool apb_clk_div_config(APB_CLK_DIV_T div);
void apb_clk_enable(APB_INSTANCE_T instance,FunctionalState en);
void apb_rst_enable(APB_INSTANCE_T instance,FunctionalState en);
bool periph_fclk_config(PERIPH_INSTANCE_T instance,PERIPH_FCLK_SRC_T src, PERIPH_FCLK_DIV_T div);
void periph_fclk_enable(PERIPH_INSTANCE_T instance,FunctionalState en);
void apb_auto_clk_gating(FunctionalState en);
void ahb_auto_clk_gating(FunctionalState en);
void ahb2apb_bridge_lpmode(FunctionalState en);


#endif
