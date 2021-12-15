/**
 *****************************************************************************
 * @brief   sleep driver source file.
 *
 * @file   lmp.c
 * @author
 * @date   17/JULY/2020
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
#include "tcasxxx.h"
#include "lpm.h"
#include "gpio.h"

/*******************************************************************************
* Definitions 私有(仅本源文件内使用)宏、枚举、结构体的定义
******************************************************************************/

/*******************************************************************************
* Variables 所有全局变量(外部，静态，常量，指针)的定义
******************************************************************************/

/******************************************************************************* 
 * Code 所有函数(外部，内部)的定义
 ******************************************************************************/

/********************************************************
@brief  Enter Deep Sleep mode
@param  bOnExit：When set to 1, enter sleep mode (Wait-for-
        Interrupt) automatically when exiting an
        exception handler and returning to thread
        level. When set to 0, this feature is disabled.
@retval void
*********************************************************/
static void EnterLpmDeepSleep(boolean_t bOnExit)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SCB->SCR |= 1u<<bOnExit;
    __WFI();
}


/********************************************************
@brief  Enter Normal Sleep mode
@param  bOnExit：When set to 1, enter sleep mode (Wait-for-
        Interrupt) automatically when exiting an
        exception handler and returning to thread
        level. When set to 0, this feature is disabled.
@retval void
*********************************************************/
static void EnterLpmNormalSleep(boolean_t bOnExit)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    SCB->SCR |= 1u<<bOnExit;
    __WFI();
}


/********************************************************
@brief  Enter Sleep mode
@param  state:睡眠模式
        bOnExit：When set to 1, enter sleep mode (Wait-for-
        Interrupt) automatically when exiting an
        exception handler and returning to thread
        level. When set to 0, this feature is disabled.
@retval void
*********************************************************/
//__attribute__((section("RAMCODE1")))
void EnterLpmPmuState(SLEEP_MODE_E state, boolean_t bOnExit)
{
    switch(state)
    {
        case ACTIVE_MODE:
        case SLEEP_MODE_MAX:
            break;
        case IDLE_MODE:
            EnterLpmNormalSleep(bOnExit);
            break;
        case SLEEPWALK_MODE:
            PMU->sleep_mode = 0x5;
            EnterLpmDeepSleep(bOnExit);
            break;
        case SLEEP_MODE:
            PMU->sleep_mode = 0x6;
            EnterLpmDeepSleep(bOnExit);
            break;
        case HIBERBNATE_MODE:
            PMU->sleep_mode = 0x7;
            PMU->areg[0] = 0xa5a5a5a5;
            EnterLpmDeepSleep(bOnExit);
            break;
        case SHUTDOWN_MODE:
            PMU->sleep_mode = 0x7;
            EnterLpmDeepSleep(bOnExit);
            break;
    }
}

/********************************************************
@brief  在低功耗模式下需要关闭不需要的模块从而使功耗达到最小
@param  void
@retval void
*********************************************************/
void SwitchOffModuleClk(void)
{
    //sw_fclk_en_xx
    //ACLKRST->clk_en &= ~(0x01<< 1 | 0x01<< 2
    //                   | 0x01<< 7 | 0x01<<16
    //                   | 0x01<<17 | 0x01<<18);
    ACLKRST->clk_en_f.sw_fclk_sel_tgpio = 0;    // disable select high speed clock for tgpio
    ACLKRST->clk_en_f.sw_fclk_en_tgpio  = 0;    // disable fclk for tgpio to input detection
    ACLKRST->clk_en_f.sw_fclk_en_awdg  = 0;     // disable fclk for awdg
    ACLKRST->clk_en_f.sw_fclk_en_cp0 = 0;       //disable fclk for comparator0 filter
    ACLKRST->clk_en_f.sw_fclk_en_cp1 = 0;       //disable fclk for comparator1 filter
    ACLKRST->clk_en_f.sw_fclk_en_lvd = 0;       //disable fclk for lvd filter

#if ( !defined UART_USE_RC32K && !defined WAKEUP_BY_LPUART )
    ACLKRST->clk_en_f.sw_pclk_en_auart = 0;
    ACLKRST->clk_en_f.sw_fclk_en_auart = 0;
#endif

#ifndef WAKEUP_BY_RTC
    //sw_fclk_en_rtc
    SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_rtc = 0;
    ACLKRST->clk_en_f.sw_fclk_en_cldr = 0;      // disable fclk for cldr
#endif

#ifndef WAKEUP_BY_AON_KEY
    //sw_fclk_en_agpio
    ACLKRST->clk_en_f.sw_fclk_en_agpio = 0;     // disable fclk for agpio to input detection
#endif
    //sw_refclk_en_sdadc
    SYSCTL->SDM_f.sw_refclk_en_sdadc = 0;
    //sw_fclk_en_auart_hs
    SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
    //SYSCTL->AONTOP_CLKRST_CTRL_f.sw_pclk_en_tgpio_hs = 0; 
    //sw_fclk_en_cp0
    SYSCTL->CP0_CLKRST_CTRL_f.sw_fclk_en_cp  = 0;
    //SYSCTL->CP0_CLKRST_CTRL_f.sw_pclk_en_cp  = 0;
    //sw_fclk_en_cp1
    SYSCTL->CP1_CLKRST_CTRL_f.sw_fclk_en_cp = 0;
    //SYSCTL->CP1_CLKRST_CTRL_f.sw_pclk_en_cp = 0;
    //sw_fclk_en_crypto
    SYSCTL->CRYPTO_CLKRST_CTRL_f.sw_fclk_en_crypto = 0;
    //SYSCTL->CRYPTO_CLKRST_CTRL_f.sw_pclk_en_crypto = 0;
    //sw_fclk_en_gpt0
    SYSCTL->GPT0_CLKRST_CTRL_f.sw_fclk_en_gpt = 0;
    //SYSCTL->GPT0_CLKRST_CTRL_f.sw_pclk_en_gpt = 0;
    //sw_fclk_en_gpt1
    SYSCTL->GPT1_CLKRST_CTRL_f.sw_fclk_en_gpt = 0;
    //SYSCTL->GPT1_CLKRST_CTRL_f.sw_pclk_en_gpt = 0;
    //sw_fclk_en_gpt2
    SYSCTL->GPT2_CLKRST_CTRL_f.sw_fclk_en_gpt = 0;
    //SYSCTL->GPT2_CLKRST_CTRL_f.sw_pclk_en_gpt = 0;
    //sw_fclk_en_i2c0
    SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_en_i2c = 0;
    //SYSCTL->I2C0_CLKRST_CTRL_f.sw_pclk_en_i2c = 0;
    //sw_fclk_en_i2c1
    SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_en_i2c = 0;
    //SYSCTL->I2C1_CLKRST_CTRL_f.sw_pclk_en_i2c = 0;
    //sw_fclk_en_lpgpt
    SYSCTL->LPGPT_CLKRST_CTRL_f.sw_fclk_en_lpgpt = 0;
    //SYSCTL->LPGPT_CLKRST_CTRL_f.sw_pclk_en_lpgpt = 0;
#ifndef WAKEUP_BY_LPUART
    //sw_fclk_en_lpuart
    SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_lpuart_hs = 0;
#endif
    //sw_fclk_en_lvd
    SYSCTL->LVD_CLKRST_CTRL_f.sw_fclk_en_lvd = 0;
    //SYSCTL->LVD_CLKRST_CTRL_f.sw_pclk_en_lvd = 0;
    //sw_fclk_en_psram
    SYSCTL->PSRAM_CLKRST_CTRL_f.sw_fclk_en_psram = 0;
    SYSCTL->PSRAM_CLKRST_CTRL_f.sw_pclk_en_psram = 0;
    SYSCTL->PSRAM_CLKRST_CTRL_f.sw_hclk_en_psram = 0;
    //sw_fclk_en_pwm0
    SYSCTL->PWM0_CLKRST_CTRL_f.sw_fclk_en_pwm = 0;
    //SYSCTL->PWM0_CLKRST_CTRL_f.sw_pclk_en_pwm = 0;
    //sw_fclk_en_pwm1
    SYSCTL->PWM1_CLKRST_CTRL_f.sw_fclk_en_pwm = 0;
    //SYSCTL->PWM1_CLKRST_CTRL_f.sw_pclk_en_pwm = 0;
    //sw_fclk_en_pwm2
    SYSCTL->PWM2_CLKRST_CTRL_f.sw_fclk_en_pwm = 0;
    //SYSCTL->PWM2_CLKRST_CTRL_f.sw_pclk_en_pwm = 0;
    //sw_fclk_en_saradc
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_en_saradc = 0;
    //SYSCTL->SARADC_CLKRST_CTRL_f.sw_pclk_en_saradc = 0;
    //sw_fclk_en_spi0
    SYSCTL->SPI0_CLKRST_CTRL_f.sw_fclk_en_spi = 0;
    //SYSCTL->SPI0_CLKRST_CTRL_f.sw_pclk_en_spi = 0;
    //sw_fclk_en_spi1
    SYSCTL->SPI1_CLKRST_CTRL_f.sw_fclk_en_spi = 0;
    //SYSCTL->SPI1_CLKRST_CTRL_f.sw_pclk_en_spi = 0;
    //sw_fclk_en_systick
    SYSCTL->MO_CLKRST_CTRL_f.sw_fclk_en_systick = 0;
    //sw_fclk_en_uart0
    SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
    //SYSCTL->UART0_CLKRST_CTRL_f.sw_pclk_en_uart = 0;
    //sw_fclk_en_uart1
    SYSCTL->UART1_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
    //SYSCTL->UART1_CLKRST_CTRL_f.sw_pclk_en_uart = 0;
    //sw_fclk_en_vdac
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_fclk_en_vdac = 0;
    //SYSCTL->VDAC_CLKRST_CTRL_f.sw_pclk_en_vdac = 0;
    //sw_fclk_en_wdg
    SYSCTL->WDG_CLKRST_CTRL_f.sw_fclk_en_wdg = 0;
    //SYSCTL->WDG_CLKRST_CTRL_f.sw_pclk_en_wdg = 0;

    //SYSCTL->HCLK_PCLK_CTRL_f.sw_pclk_en_divider = 0;
    //SYSCTL->HCLK_PCLK_CTRL_f.sw_pclk_en_trng = 0;
}

/********************************************************
@brief  在低功耗模式下需要关闭不需要的模块电源从而使功耗达到最小
@param  void
@retval void
*********************************************************/
void SwitchOffModulePower(void)
{
    AON_ANACFG->reg_lvd_top &= ~(0x01<<8);
    //comp0_ctrl_0_enable
    AON_ANACFG->reg_comparator_top1 &= ~(0x01<<0);
    //comp1_ctrl_0_enable
    AON_ANACFG->reg_comparator_top2 &= ~(0x01<<0);
    //ctrl_ldo3_0_enable
    AON_ANACFG->reg_ldo3_top &= ~(0x01<<0);
    //ctrl_xosc32k_0_enable
    AON_ANACFG->reg_xosc32k_top &= ~(0x01<<0);
    //only keep RC16M alive
    AON_ANACFG->reg_analog_block_enable = 0x1;
    //en6 powerswitch control
    AON_ANACFG->reg_power_switch_top &= ~(0x1 << 4);
    //power down detect function contrl, high active
    AON_ANACFG->reg_power_switch_top |= (0x1<<7);
    AON_ANACFG->reg_clk_deglitch_2500n |= 0x01;
    //power down TIA
    AON_ANACFG->reg_opa_iv &= ~(0x1 << 4);
    //trim lplado to lowest level,maybe 1.37V or so
    AON_ANACFG_TRIM->trim_lpldo_value = 0x01;
    GPIO_SwitchOffIe();
}

