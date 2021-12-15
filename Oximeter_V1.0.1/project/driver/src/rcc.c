/**
*****************************************************************************
* @brief
*
* @file   rcc.c
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
* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/
#include "rcc.h"
#include "delay.h"


#define HCLK_SEL_BITWIDTH  3

/**
 * @description: AHB总线时钟源配置
 * @param {AHB_CLK_SRC_T} src: AHB总线时钟源
 * @return bool: false:配置失败 true:配置成功
 */
bool ahb_clk_config(AHB_CLK_SRC_T src)
{
    AHB_CLK_SRC_T src_ever;
    /* illegal config, return config fail */
    if (src != AHB_LS && src != AHB_HS && src != AHB_MUL)
        return false;
    /* hclk_sel: only low 3 bits valid */
    src_ever = (AHB_CLK_SRC_T)(ACLKRST->hclk_sel & 0x7);
    /* if same as ever,return directly */
    if (src == src_ever)
        return true;

    /* judge differ bits */
    uint8_t _xor = src_ever ^ src;
    for (uint8_t i = 0; i < HCLK_SEL_BITWIDTH; i++)
    {
        if (_xor >> i & 0x1)
        {
            (src_ever >> i & 0x1) ? (src_ever &= ~(1 << i)) : (src_ever |= (1 << i));
            ACLKRST->hclk_sel = src_ever;
            delay1us(64);
        }
    }
    if (src == AHB_MUL)
    {
        ANAREG_1->reg_freq_doubler_top |= ENABLE << ANA_FD_POS;
        AON_ANACFG->an_block_f.ctrl_fd_en_0_enable = ENABLE;
    }
    return true;
}

/**
 * @description: AHB总线分频系数配置
 * @param {AHB_CLK_DIV_T} div: 分频系数
 * @return bool: false:配置失败 true:配置成功
 */
bool ahb_clk_div_config(AHB_CLK_DIV_T div)
{
    if (div > 0xF)
        return false;
    SYSCTL->MO_CLKRST_CTRL_F.SW_HCLK_DIV_M0 = div;
    return true;
}

/**
 * @description: 低速时钟源配置
 * @param {LS_CLK_SRC_T} src 低速时钟源
 * @return bool: false:配置失败 true:配置成功
 */
bool ls_clk_config(LS_CLK_SRC_T src)
{
    if (src != LSE_OSC && src != LSI_RC)
        return false;
    switch (src)
    {
    case LSI_RC:
        ACLKRST->clk_en_f.sw_32k_sel = ACLK_LSI_RC;
        AON_ANACFG->oscrc32k_1p5v_f.ctrl_oscrc32k_2_1 = RC_32K;
        AON_ANACFG->oscrc32k_1p5v_f.ctrl_oscrc32k_0_enable = ENABLE;
        AON_ANACFG->reg_xosc32k_top_f.ctrl_xosc32k_0_enable = DISABLE;
        while (AON_ANACFG->reg_clkready_32k_withDiv2_f.a2d_oscrc32k_ready == false ||
               AON_ANACFG->reg_clkready_32k_withDiv2_f.a2d_xosc32k_ready == true)
            ;
        break;
    case LSE_OSC:
        ACLKRST->clk_en_f.sw_32k_sel = ACLK_LSE_OSC;
        AON_ANACFG->oscrc32k_1p5v_f.ctrl_oscrc32k_2_1 = RC_32K;
        AON_ANACFG->oscrc32k_1p5v_f.ctrl_oscrc32k_0_enable = DISABLE;
        AON_ANACFG->reg_xosc32k_top_f.ctrl_xosc32k_0_enable = ENABLE;
        while (AON_ANACFG->reg_clkready_32k_withDiv2_f.a2d_oscrc32k_ready == true ||
               AON_ANACFG->reg_clkready_32k_withDiv2_f.a2d_xosc32k_ready == false)
            ;
        break;
    }
    return true;
}
/**
 * @description: 高速时钟源配置
 * @param {HS_CLK_SRC_T} src 高速时钟源
 * @return bool: false:配置失败 true:配置成功
 */
bool hs_clk_config(HS_CLK_SRC_T src)
{
    if (src != HSI_RC || src != HSE_OSC)
        return false;
    SYSCTL->SDM_F.SW_16M_SEL = src;
    switch (src)
    {
    case HSI_RC:
        ANAREG_1->reg_oscrc_16m_top = ENABLE << ANA_RC_16M_POS;
        AON_ANACFG->an_block_f.ctrl_crystal_16m_9_enable = DISABLE;
        AON_ANACFG->an_block_f.ctrl_oscrc16m_0_enable = ENABLE;
        while (ANAREG_1->reg_clkready_16m_f.a2d_crystal16m_ready == true ||
               ANAREG_1->reg_clkready_16m_f.a2d_oscrc16m_ready == false)
            ;
        break;
    case HSE_OSC:
        ANAREG_1->reg_oscrc_16m_top = DISABLE << ANA_RC_16M_POS;
        AON_ANACFG->an_block_f.ctrl_crystal_16m_9_enable = ENABLE;
        AON_ANACFG->an_block_f.ctrl_oscrc16m_0_enable = DISABLE;
        while (ANAREG_1->reg_clkready_16m_f.a2d_crystal16m_ready == false ||
               ANAREG_1->reg_clkready_16m_f.a2d_oscrc16m_ready == true)
            ;
        break;
    }
    return true;
}
/**
 * @description: APB总线时钟分频配置
 * @param {APB_CLK_DIV_T} div 分频系数
 * @return bool: false:配置失败 true:配置成功
 */
bool apb_clk_div_config(APB_CLK_DIV_T div)
{
    /*APB总线预分频系数最大3bit = 0x7*/
    if (div > 0x7)
        return false;
    SYSCTL->AHB2APB_CLKRST_CTRL_F.SW_PCLK_DIV_BRIDGE = div;
    return true;
}
/**
 * @description: APB外设时钟使能配置
 * @param {APB_INSTANCE_T} instance： APB外设
 * @param {FunctionalState} en 使能/不使能
 * @return none
 */
void apb_clk_enable(APB_INSTANCE_T instance, FunctionalState en)
{
    switch (instance)
    {
    case APB_SDM:
        SYSCTL->SDM_F.SW_PCLK_EN_SDADC = en;
        break;
    case APB_AON:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_PCLK_EN_AON = en;
        break;
    case APB_GPIO_HS:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_PCLK_EN_TGPIO_HS = en;
        break;
    case APB_ANALOG:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_PCLK_EN_ANAREG = en;
        break;

    case APB_DIVIDER:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_PCLK_EN_DIVIDER = en;
        break;
    case APB_FIC:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_PCLK_EN_FIC = en;
        break;
    case APB_LIC:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_PCLK_EN_LIC = en;
        break;
    case APB_PINMUX:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_PCLK_EN_TPINMUX = en;
        break;

    case APB_LPGPT:
        SYSCTL->LPGPT_CLKRST_CTRL_F.SW_PCLK_EN_LPGPT = en;
        break;
    case APB_UART2:
        SYSCTL->UART2_CLKRST_CTRL_F.SW_PCLK_EN_UART = en;
        break;
    case APB_CRYPTO:
        SYSCTL->CRYPTO_CLKRST_CTRL_F.SW_PCLK_EN_CRYPTO = en;
        break;
    case APB_GPT0:
        SYSCTL->GPT0_CLKRST_CTRL_F.SW_PCLK_EN_GPT = en;
        break;
    case APB_GPT1:
        SYSCTL->GPT1_CLKRST_CTRL_F.SW_PCLK_EN_GPT = en;
        break;
    case APB_GPT2:
        SYSCTL->GPT2_CLKRST_CTRL_F.SW_PCLK_EN_GPT = en;
        break;
    case APB_PWM0:
        SYSCTL->PWM0_CLKRST_CTRL_F.SW_PCLK_EN_PWM = en;
        break;
    case APB_PWM1:
        SYSCTL->PWM1_CLKRST_CTRL_F.SW_PCLK_EN_PWM = en;
        break;
    case APB_PWM2:
        SYSCTL->PWM2_CLKRST_CTRL_F.SW_PCLK_EN_PWM = en;
        break;
    case APB_I2C0:
        SYSCTL->I2C0_CLKRST_CTRL_F.SW_PCLK_EN_I2C = en;
        break;
    case APB_I2C1:
        SYSCTL->I2C1_CLKRST_CTRL_F.SW_PCLK_EN_I2C = en;
        break;
    case APB_SPI0:
        SYSCTL->SPI0_CLKRST_CTRL_F.SW_PCLK_EN_SPI = en;
        break;
    case APB_SPI1:
        SYSCTL->SPI1_CLKRST_CTRL_F.SW_PCLK_EN_SPI = en;
        break;
    case APB_UART0:
        SYSCTL->UART0_CLKRST_CTRL_F.SW_PCLK_EN_UART = en;
        break;
    case APB_UART1:
        SYSCTL->UART1_CLKRST_CTRL_F.SW_PCLK_EN_UART = en;
        break;
    case APB_WDG:
        SYSCTL->WDG_CLKRST_CTRL_F.SW_PCLK_EN_WDG = en;
        break;
    case APB_CP0:
        SYSCTL->CP0_CLKRST_CTRL_F.SW_PCLK_EN_CP = en;
        break;
    case APB_CP1:
        SYSCTL->CP1_CLKRST_CTRL_F.SW_PCLK_EN_CP = en;
        break;
    case APB_LVD:
        SYSCTL->LVD_CLKRST_CTRL_F.SW_PCLK_EN_LVD = en;
        break;
    case APB_SARADC:
        SYSCTL->SARADC_CLKRST_CTRL_F.SW_PCLK_EN_SARADC = en;
        break;
    case APB_VDAC:
        SYSCTL->VDAC_CLKRST_CTRL_F.SW_PCLK_EN_VDAC = en;
        break;
    default:
        break;
    }
}
/**
 * @description: APB外设复位
 * @param {APB_INSTANCE_T} instance： APB外设
 * @param {FunctionalState} en：复位/不复位
 * @return none
 */
void apb_rst_enable(APB_INSTANCE_T instance, FunctionalState en)
{
    switch (instance)
    {
    case APB_SDM:
        SYSCTL->SDM_F.SW_RST_SDADC = en;
        break;
    case APB_AON:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_RST_AON = en;
        break;
    case APB_GPIO_HS:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_RST_TGPIO = en;
        break;
    case APB_ANALOG:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_RST_ANAREG = en;
        break;

    case APB_DIVIDER:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_RST_DIVIDER = en;
        break;
    case APB_FIC:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_RST_FIC = en;
        break;
    case APB_LIC:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_RST_LIC = en;
        break;
    case APB_PINMUX:
        SYSCTL->HCLK_PCLK_CTRL_F.SW_RST_TPINMUX = en;
        break;

    case APB_LPGPT:
        SYSCTL->LPGPT_CLKRST_CTRL_F.SW_RST_LPGPT = en;
        break;
    case APB_UART2:
        SYSCTL->UART2_CLKRST_CTRL_F.SW_RST_UART = en;
        break;
    case APB_CRYPTO:
        SYSCTL->CRYPTO_CLKRST_CTRL_F.SW_RST_CRYPTO = en;
        break;
    case APB_GPT0:
        SYSCTL->GPT0_CLKRST_CTRL_F.SW_RST_GPT = en;
        break;
    case APB_GPT1:
        SYSCTL->GPT1_CLKRST_CTRL_F.SW_RST_GPT = en;
        break;
    case APB_GPT2:
        SYSCTL->GPT2_CLKRST_CTRL_F.SW_RST_GPT = en;
        break;
    case APB_PWM0:
        SYSCTL->PWM0_CLKRST_CTRL_F.SW_RST_PWM = en;
        break;
    case APB_PWM1:
        SYSCTL->PWM1_CLKRST_CTRL_F.SW_RST_PWM = en;
        break;
    case APB_PWM2:
        SYSCTL->PWM2_CLKRST_CTRL_F.SW_RST_PWM = en;
        break;
    case APB_I2C0:
        SYSCTL->I2C0_CLKRST_CTRL_F.SW_RST_I2C = en;
        break;
    case APB_I2C1:
        SYSCTL->I2C1_CLKRST_CTRL_F.SW_RST_I2C = en;
        break;
    case APB_SPI0:
        SYSCTL->SPI0_CLKRST_CTRL_F.SW_RST_SPI = en;
        break;
    case APB_SPI1:
        SYSCTL->SPI1_CLKRST_CTRL_F.SW_RST_SPI = en;
        break;
    case APB_UART0:
        SYSCTL->UART0_CLKRST_CTRL_F.SW_RST_UART = en;
        break;
    case APB_UART1:
        SYSCTL->UART1_CLKRST_CTRL_F.SW_RST_UART = en;
        break;
    case APB_WDG:
        SYSCTL->WDG_CLKRST_CTRL_F.SW_RST_WDG = en;
        break;
    case APB_CP0:
        SYSCTL->CP0_CLKRST_CTRL_F.SW_RST_CP = en;
        break;
    case APB_CP1:
        SYSCTL->CP1_CLKRST_CTRL_F.SW_RST_CP = en;
        break;
    case APB_LVD:
        SYSCTL->LVD_CLKRST_CTRL_F.SW_RST_LVD = en;
        break;
    case APB_SARADC:
        SYSCTL->SARADC_CLKRST_CTRL_F.SW_RST_SARADC = en;
        break;
    case APB_VDAC:
        SYSCTL->VDAC_CLKRST_CTRL_F.SW_RST_VDAC = en;
        break;
    default:
        break;
    }
}
/**
 * @description: 外设工作时钟配置
 * @param {PERIPH_INSTANCE_T} instance：外设实例
 * @param {PERIPH_FCLK_SRC_T} src： 工作时钟源
 * @param {PERIPH_FCLK_DIV_T} div： 工作时钟分频系数
 * @return bool: false:配置失败 true:配置成功
 */
bool periph_fclk_config(PERIPH_INSTANCE_T instance, PERIPH_FCLK_SRC_T src, PERIPH_FCLK_DIV_T div)
{
    if (src >= PERIPH_SRC_MAX)
        return false;
    if (div >= FCLK_DIV_MAX)
        return false;
    switch (instance)
    {
    case PERIPH_SDM:
        SYSCTL->SDM_F.SW_REFCLK_SEL_SDADC = src;
        SYSCTL->SDM_F.SW_REFCLK_DIV_SDADC = div;
        break;
    case PERIPH_RTC:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_SEL_RTC = src;
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_DIV_RTC = div;
        break;
    case PERIPH_LPUART_HS:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_SEL_LPUART_HS = src;
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_DIV_LPUART_HS = div;
        break;
    case PERIPH_SYSTICK:
        SYSCTL->MO_CLKRST_CTRL_F.SW_FCLK_SEL_SYSTICK = src;
        SYSCTL->MO_CLKRST_CTRL_F.SW_FCLK_DIV_SYSTICK = div;
        break;

    case PERIPH_LPGPT:
        SYSCTL->LPGPT_CLKRST_CTRL_F.SW_FCLK_SEL_LPGPT = src;
        SYSCTL->LPGPT_CLKRST_CTRL_F.SW_FCLK_DIV_LPGPT = div;
        break;
    case PERIPH_UART2:
        SYSCTL->UART2_CLKRST_CTRL_F.SW_FCLK_SEL_UART = src;
        SYSCTL->UART2_CLKRST_CTRL_F.SW_FCLK_DIV_UART = div;
        break;
    case PERIPH_CRYPTO:
        SYSCTL->CRYPTO_CLKRST_CTRL_F.SW_FCLK_SEL_CRYPTO = src;
        SYSCTL->CRYPTO_CLKRST_CTRL_F.SW_FCLK_DIV_CRYPTO = div;
        break;
    case PERIPH_GPT0:
        SYSCTL->GPT0_CLKRST_CTRL_F.SW_FCLK_SEL_GPT = src;
        SYSCTL->GPT0_CLKRST_CTRL_F.SW_FCLK_DIV_GPT = div;
        break;
    case PERIPH_GPT1:
        SYSCTL->GPT1_CLKRST_CTRL_F.SW_FCLK_SEL_GPT = src;
        SYSCTL->GPT1_CLKRST_CTRL_F.SW_FCLK_DIV_GPT = div;
        break;
    case PERIPH_GPT2:
        SYSCTL->GPT2_CLKRST_CTRL_F.SW_FCLK_SEL_GPT = src;
        SYSCTL->GPT2_CLKRST_CTRL_F.SW_FCLK_DIV_GPT = div;
        break;
    case PERIPH_PWM0:
        SYSCTL->PWM0_CLKRST_CTRL_F.SW_FCLK_SEL_PWM = src;
        SYSCTL->PWM0_CLKRST_CTRL_F.SW_FCLK_DIV_PWM = div;
        break;
    case PERIPH_PWM1:
        SYSCTL->PWM1_CLKRST_CTRL_F.SW_FCLK_SEL_PWM = src;
        SYSCTL->PWM1_CLKRST_CTRL_F.SW_FCLK_DIV_PWM = div;
        break;
    case PERIPH_PWM2:
        SYSCTL->PWM2_CLKRST_CTRL_F.SW_FCLK_SEL_PWM = src;
        SYSCTL->PWM2_CLKRST_CTRL_F.SW_FCLK_DIV_PWM = div;
        break;
    case PERIPH_I2C0:
        SYSCTL->I2C0_CLKRST_CTRL_F.SW_FCLK_SEL_I2C = src;
        SYSCTL->I2C0_CLKRST_CTRL_F.SW_FCLK_DIV_I2C = div;
        break;
    case PERIPH_I2C1:
        SYSCTL->I2C1_CLKRST_CTRL_F.SW_FCLK_SEL_I2C = src;
        SYSCTL->I2C1_CLKRST_CTRL_F.SW_FCLK_DIV_I2C = div;
        break;
    case PERIPH_SPI0:
        SYSCTL->SPI0_CLKRST_CTRL_F.SW_FCLK_SEL_SPI = src;
        SYSCTL->SPI0_CLKRST_CTRL_F.SW_FCLK_DIV_SPI = div;
        break;
    case PERIPH_SPI1:
        SYSCTL->SPI1_CLKRST_CTRL_F.SW_FCLK_SEL_SPI = src;
        SYSCTL->SPI1_CLKRST_CTRL_F.SW_FCLK_DIV_SPI = div;
        break;
    case PERIPH_UART0:
        SYSCTL->UART0_CLKRST_CTRL_F.SW_FCLK_SEL_UART = src;
        SYSCTL->UART0_CLKRST_CTRL_F.SW_FCLK_DIV_UART = div;
        break;
    case PERIPH_UART1:
        SYSCTL->UART1_CLKRST_CTRL_F.SW_FCLK_SEL_UART = src;
        SYSCTL->UART1_CLKRST_CTRL_F.SW_FCLK_DIV_UART = div;
        break;
    case PERIPH_WDG:
        SYSCTL->WDG_CLKRST_CTRL_F.SW_FCLK_SEL_WDG = src;
        SYSCTL->WDG_CLKRST_CTRL_F.SW_FCLK_DIV_WDG = div;
        break;
    case PERIPH_CP0:
        SYSCTL->CP0_CLKRST_CTRL_F.SW_FCLK_SEL_CP = src;
        SYSCTL->CP0_CLKRST_CTRL_F.SW_FCLK_DIV_CP = div;
        break;
    case PERIPH_CP1:
        SYSCTL->CP1_CLKRST_CTRL_F.SW_FCLK_SEL_CP = src;
        SYSCTL->CP1_CLKRST_CTRL_F.SW_FCLK_DIV_CP = div;
        break;
    case PERIPH_LVD:
        SYSCTL->LVD_CLKRST_CTRL_F.SW_FCLK_SEL_LVD = src;
        SYSCTL->LVD_CLKRST_CTRL_F.SW_FCLK_DIV_LVD = div;
        break;
    case PERIPH_SARADC:
        SYSCTL->SARADC_CLKRST_CTRL_F.SW_FCLK_SEL_SARADC = src;
        SYSCTL->SARADC_CLKRST_CTRL_F.SW_FCLK_DIV_SARADC = div;
        break;
    case PERIPH_VDAC:
        SYSCTL->VDAC_CLKRST_CTRL_F.SW_FCLK_SEL_VDAC = src;
        SYSCTL->VDAC_CLKRST_CTRL_F.SW_FCLK_DIV_VDAC = div;
        break;
    default:
        return false;
    }
    return true;
}
/**
 * @description: 外设工作时钟使能
 * @param {PERIPH_INSTANCE_T} instance：外设实例
 * @param {FunctionalState} en 使能/不使能
 * @return none
 */
void periph_fclk_enable(PERIPH_INSTANCE_T instance, FunctionalState en)
{
    switch (instance)
    {
    case PERIPH_SDM:
        SYSCTL->SDM_F.SW_REFCLK_EN_SDADC = en;
        break;
    case PERIPH_RTC:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_EN_RTC = en;
        break;
    case PERIPH_LPUART_HS:
        SYSCTL->AONTOP_CLKRST_CTRL_F.SW_FCLK_EN_LPUART_HS = en;
        break;
    case PERIPH_SYSTICK:
        SYSCTL->MO_CLKRST_CTRL_F.SW_FCLK_EN_SYSTICK = en;
        break;

    case PERIPH_LPGPT:
        SYSCTL->LPGPT_CLKRST_CTRL_F.SW_FCLK_EN_LPGPT = en;
        break;
    case PERIPH_UART2:
        SYSCTL->UART2_CLKRST_CTRL_F.SW_FCLK_EN_UART = en;
        break;
    case PERIPH_CRYPTO:
        SYSCTL->CRYPTO_CLKRST_CTRL_F.SW_FCLK_EN_CRYPTO = en;
        break;
    case PERIPH_GPT0:
        SYSCTL->GPT0_CLKRST_CTRL_F.SW_FCLK_EN_GPT = en;
        break;
    case PERIPH_GPT1:
        SYSCTL->GPT1_CLKRST_CTRL_F.SW_FCLK_EN_GPT = en;
        break;
    case PERIPH_GPT2:
        SYSCTL->GPT2_CLKRST_CTRL_F.SW_FCLK_EN_GPT = en;
        break;
    case PERIPH_PWM0:
        SYSCTL->PWM0_CLKRST_CTRL_F.SW_FCLK_EN_PWM = en;
        break;
    case PERIPH_PWM1:
        SYSCTL->PWM1_CLKRST_CTRL_F.SW_FCLK_EN_PWM = en;
        break;
    case PERIPH_PWM2:
        SYSCTL->PWM2_CLKRST_CTRL_F.SW_FCLK_EN_PWM = en;
        break;
    case PERIPH_I2C0:
        SYSCTL->I2C0_CLKRST_CTRL_F.SW_FCLK_EN_I2C = en;
        break;
    case PERIPH_I2C1:
        SYSCTL->I2C1_CLKRST_CTRL_F.SW_FCLK_EN_I2C = en;
        break;
    case PERIPH_SPI0:
        SYSCTL->SPI0_CLKRST_CTRL_F.SW_FCLK_EN_SPI = en;
        break;
    case PERIPH_SPI1:
        SYSCTL->SPI1_CLKRST_CTRL_F.SW_FCLK_EN_SPI = en;
        break;
    case PERIPH_UART0:
        SYSCTL->UART0_CLKRST_CTRL_F.SW_FCLK_EN_UART = en;
        break;
    case PERIPH_UART1:
        SYSCTL->UART1_CLKRST_CTRL_F.SW_FCLK_EN_UART = en;
        break;
    case PERIPH_WDG:
        SYSCTL->WDG_CLKRST_CTRL_F.SW_FCLK_EN_WDG = en;
        break;
    case PERIPH_CP0:
        SYSCTL->CP0_CLKRST_CTRL_F.SW_FCLK_EN_CP = en;
        break;
    case PERIPH_CP1:
        SYSCTL->CP1_CLKRST_CTRL_F.SW_FCLK_EN_CP = en;
        break;
    case PERIPH_LVD:
        SYSCTL->LVD_CLKRST_CTRL_F.SW_FCLK_EN_LVD = en;
        break;
    case PERIPH_SARADC:
        SYSCTL->SARADC_CLKRST_CTRL_F.SW_FCLK_EN_SARADC = en;
        break;
    case PERIPH_VDAC:
        SYSCTL->VDAC_CLKRST_CTRL_F.SW_FCLK_EN_VDAC = en;
        break;
    default:
        break;
    }
}
/**
 * @description: APB总线桥自动时钟门控
 * @param {FunctionalState} en：使能/不使能
 * @return none
 */
void apb_auto_clk_gating(FunctionalState en)
{
    SYSCTL->AHB2APB_CLKRST_CTRL_F.SW_PCLK_AUTOCG_BRIDGE = en;
}
/**
 * @description: AHB总线桥自动时钟门控
 * @param {FunctionalState} en：使能/不使能
 * @return none
 */
void ahb_auto_clk_gating(FunctionalState en)
{
    SYSCTL->AHB2APB_CLKRST_CTRL_F.SW_HCLK_AUTOCG_BRIDGE = en;
}

/**
 * @description: AHB to APB 总线桥低功耗配置
 * @param {FunctionalState} en：ENABLE:SLEEPWALK DISABLE:ACTIVE
 * @return none
 */
void ahb2apb_bridge_lpmode(FunctionalState en)
{
    SYSCTL->AHB2APB_CLKRST_CTRL_F.SW_EN_BRIDGE_LPMODE = en;
}
