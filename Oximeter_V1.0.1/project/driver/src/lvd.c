/**
 *****************************************************************************
 * @brief   lvd driver source file.
 *
 * @file   lvd.c
 * @author
 * @date   25/May/2021
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

#include "lvd.h"

/**
 *******************************************************************************
** \brief LVD_Enable
 **
** \param [u8] IO_Port:  LVD_PORT_PB01
                            LVD_PORT_PB02
                            LVD_PORT_PB03

 ** \retval[void] void
 ******************************************************************************/
void LVD_PortConfig(uint8_t IO_Port)
{
    tc_pinmux_config_field_t * pinconfig = NULL;

    if(IO_Port == LVD_PORT_PB01)
    {
        pinconfig = (tc_pinmux_config_field_t *)&PINMUXELSE->PB_01;
    }
    if(IO_Port == LVD_PORT_PB02)
    {
        pinconfig = (tc_pinmux_config_field_t *)&PINMUXELSE->PB_02;
    }
    if(IO_Port == LVD_PORT_PB03)
    {
        pinconfig = (tc_pinmux_config_field_t *)&PINMUXELSE->PB_03;
    }

    if(pinconfig != NULL)
    {
        pinconfig->sw_src_sel = LVD_PORT_MODE;
        pinconfig->sw_ie = DISABLE;
    }

}
/**
 *******************************************************************************
** \brief LVD_CLKConfig
 **
** \param [u8] clk_src:     LVD_FCLK_16M
                               LVD_FCLK_FMULT
                               LVD_FCLK_32K

 ** \param [u8] clk_div:    LVD_FCLK_DIV1
                               LVD_FCLK_DIV2
                               LVD_FCLK_DIV4
                               LVD_FCLK_DIV8
                               LVD_FCLK_DIV16
                               LVD_FCLK_DIV32
                               LVD_FCLK_DIV64
                               LVD_FCLK_DIV128
                               LVD_FCLK_DIV256

 ** \retval[void] void
 ******************************************************************************/
void LVD_CLKConfig(uint8_t clk_src,uint8_t clk_div)
{
    SYSCTL->LVD_CLKRST_CTRL_f.sw_fclk_div_lvd = clk_div;
    SYSCTL->LVD_CLKRST_CTRL_f.sw_fclk_sel_lvd = clk_src;
    SYSCTL->LVD_CLKRST_CTRL_f.sw_fclk_en_lvd = ENABLE;
}
/**
 *******************************************************************************
** \brief LVD_CLKDisable
 **
** \param [u8]:

 ** \retval[void] void
 ******************************************************************************/
void LVD_CLKDisable(void)
{
    SYSCTL->LVD_CLKRST_CTRL_f.sw_fclk_en_lvd = DISABLE;
}

/**
 *******************************************************************************
** \brief LVD_Enable
 **
** \param [u8]:

 ** \retval[void] void
 ******************************************************************************/
void LVD_Enable(void)
{
    AON_ANACFG->reg_lvd_cfg_f.cfg_en = ENABLE;
}
/**
 *******************************************************************************
** \brief LVD_Disable
 **
** \param [u8]:

 ** \retval[void] void
 ******************************************************************************/
void LVD_Disable(void)
{
    AON_ANACFG->reg_lvd_cfg_f.cfg_en = DISABLE;
}
/**
 *******************************************************************************
** \brief LVD_ModeConfig
 **
** \param [u8]  mode:       LVD_MODE_NONE
                            LVD_MODE_NORMAL
                            LVD_MODE_LOW_POWER
 ** \retval[void] void
 ******************************************************************************/
void LVD_ModeConfig(uint8_t mode)
{
    if(mode == LVD_MODE_NORMAL)
    {
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_lp_enable = ENABLE;
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_enable = ENABLE;
    }
    else if(mode == LVD_MODE_LOW_POWER)
    {
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_lp_enable = ENABLE;
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_enable = DISABLE;
    }
    else
    {
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_lp_enable = DISABLE;
        AON_ANACFG->reg_lvd_top_f.ctrl_lvd_enable = DISABLE;
    }
}

/**
 *******************************************************************************
** \brief LVD_SetTripVolt
 **
** \param [u8] Volt:    LVD_TRIP_VOLT0
                           LVD_TRIP_VOLT1
                           LVD_TRIP_VOLT2
                           LVD_TRIP_VOLT3
                           LVD_TRIP_VOLT4
                           LVD_TRIP_VOLT5
                           LVD_TRIP_VOLT6
                           LVD_TRIP_VOLT7

 ** \retval[void] void
 ******************************************************************************/
void LVD_SetTripVolt(uint8_t Volt)
{
    AON_ANACFG->reg_lvd_top_f.ctrl_lvd_trip_volt = Volt;
}

/**
 *******************************************************************************
** \brief LVD_SetVrefSel
 **
** \param [u8]vref:     LVD_VREF_PB01
                        LVD_VREF_PB02
                        LVD_VREF_PB03
                        LVD_VREF_VD5
                        LVD_VREF_TEST
                        LVD_VREF_GND

 ** \retval[void] void
 ******************************************************************************/
void LVD_SetVrefSel(uint8_t vref)
{
    AON_ANACFG->reg_lvd_top_f.ctrl_lvd_vref_sel = vref;
}
/**
 *******************************************************************************
** \brief LVD_SetFilterTime
 **
** \param [u8] nTime:   FilterTime = nTime / Fclk

 ** \retval[void] void
 ******************************************************************************/

void LVD_SetFilterTime(uint16_t nTime)
{
    AON_ANACFG->reg_lvd_cfg_f.cfg_cnt_max = nTime;
}
/**
 *******************************************************************************
** \brief LVD_SetFilterTime
 **
** \param [u8] IT_Mode:     LVD_IT_RISLING
                               LVD_IT_FALLING
                               LVD_IT_HIGH_LEVEL

 ** \retval[void] void
 ******************************************************************************/
void LVD_ITConfig(uint8_t IT_Mode)
{
    AON_ANACFG->reg_lvd_cfg_f.cfg_irq_mode = IT_Mode;
}


