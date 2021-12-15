/**
 *****************************************************************************
 * @brief   Comparator driver source file.
 *
 * @file   cmp.c
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

 #include "cmp.h"

/**
 *******************************************************************************
** \brief Cmp_Init
 **
 ** \param [u8]  cmpPort:               COMPARATOR0
                                        COMPARATOR1

 ** \param [u8]  cmpPosChannel :        CMP_PD00_CHANNEL0
                                        CMP_PD01_CHANNEL1
                                        CMP_PA00_CHANNEL2
                                        CMP_PA01_CHANNEL3
                                        CMP_PA02_CHANNEL4
                                        CMP_PA03_CHANNEL5
                                        CMP_PA06_CHANNEL6
                                        CMP_PA07_CHANNEL7

 ** \param [u8]  cmpNegChannel :        CMP_PD00_CHANNEL0
                                        CMP_PD01_CHANNEL1
                                        CMP_PA00_CHANNEL2
                                        CMP_PA01_CHANNEL3
                                        CMP_PA02_CHANNEL4
                                        CMP_PA03_CHANNEL5
                                        CMP_PA06_CHANNEL6
                                        CMP_PA07_CHANNEL7

 ** \param [u8]  cmpIrqMode:            CMP_IT_NONE
                                        CMP_IT_FALLING
                                        CMP_IT_RISING
                                        CMP_IT_HIGH_LEVEL

** \param [u8]  cmpRevPhaseEn:          0:正向输出
                                        1:反相输出

** \param [u8]  cmpHighSpeedEn:         0：关闭高速模式
                                        1：打开高速模式

** \param [u8]  cmpHysteresisVoltage:       CMP_HYSTERESIS_VOLTAGE_0_0
                                            CMP_HYSTERESIS_VOLTAGE_0_3
                                            CMP_HYSTERESIS_VOLTAGE_4_6
                                            CMP_HYSTERESIS_VOLTAGE_16_0
                                            CMP_HYSTERESIS_VOLTAGE_26_0
                                            CMP_HYSTERESIS_VOLTAGE_33_5

 ** \param [u8]  cmpAnaFilterEn:         0：关闭模拟滤波
                                         1：打开模拟滤波

 ** \param [u8]  cmpAnaFilterTime:       0：模拟滤波时间 5us
                                         1：模拟滤波时间 20us

 ** \param [u8]  cmpDigFilterTime:   数字滤波时间
                                         cmpDigFilterTime / (cmpFclk/cmpFclkDiv)


 ** \retval[void] void
 ******************************************************************************/
void Cmp_Init(uint8_t cmpPort,Cmp_Init_t * pInit)
{
    if(cmpPort == COMPARATOR0)
    {
        AON_ANACFG->reg_acomp0_cfg_f.cfg_cnt_max = pInit->cmpDigFilterTime;
        AON_ANACFG->reg_acomp0_cfg_f.cfg_irq_mode = pInit->cmpIrqMode;

        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0svn = pInit->cmpNegChannel;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0svp = pInit->cmpPosChannel;

        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0he = pInit->cmpHighSpeedEn;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0hs = pInit->cmpHysteresisVoltage;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0oie = pInit->cmpRevPhaseEn;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0le = pInit->cmpAnaFilterEn;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0sl = pInit->cmpAnaFilterTime;

        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cpvne = ENABLE;
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_cp0vpe = ENABLE;
        AON_ANACFG->reg_acomp0_cfg_f.cfg_en = ENABLE;
    }

    if(cmpPort == COMPARATOR1)
    {
        AON_ANACFG->reg_acomp1_cfg_f.cfg_cnt_max = pInit->cmpDigFilterTime;
        AON_ANACFG->reg_acomp1_cfg_f.cfg_irq_mode = pInit->cmpIrqMode;

        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1svn = pInit->cmpNegChannel;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1svp = pInit->cmpPosChannel;

        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1he = pInit->cmpHighSpeedEn;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1hs = pInit->cmpHysteresisVoltage;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1oie = pInit->cmpRevPhaseEn;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1le = pInit->cmpAnaFilterEn;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1sl = pInit->cmpAnaFilterTime;

        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cpvne = ENABLE;
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_cp1vpe = ENABLE;
        AON_ANACFG->reg_acomp1_cfg_f.cfg_en = ENABLE;
    }

    if(pInit->cmpPosChannel <= CMP_PA07_CHANNEL7)
    {
        ANAREG_1->reg_adsar_mux_in_f.adc_sw_input_for_vcin |= (1<<pInit->cmpPosChannel);
        ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_p = 0xF;
    }
    if(pInit->cmpNegChannel <= CMP_PA07_CHANNEL7)
    {
        ANAREG_1->reg_adsar_mux_in_f.adc_sw_input_for_vcin |= (1<<pInit->cmpNegChannel);
        ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_n = 0xF;
    }
    //AON_ANACFG->power_switch_f.ctrl_powerswitch_4_en6 = 1;
    //AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_me = 1;
}
/**
 *******************************************************************************
** \brief Cmp_Enable
 **
** \param [u8]  cmpPort:               COMPARATOR0
                                       COMPARATOR1

** \param [u8]  cmpFclk:               CMP_FCLK_SRC_16M
                                       CMP_FCLK_SRC_FMULT
                                       CMP_FCLK_SRC_32K

** \param [u8]  cmpFclkDiv:            CMP_FCLK_DIV2
                                       CMP_FCLK_DIV4
                                       CMP_FCLK_DIV8
                                       CMP_FCLK_DIV16
                                       CMP_FCLK_DIV32
                                       CMP_FCLK_DIV64
                                       CMP_FCLK_DIV128
                                       CMP_FCLK_DIV256
 ** \retval[void] void
 ******************************************************************************/
void Cmp_CLKConfig(uint8_t cmPort,uint8_t cmpFclk,uint8_t cmpFclkDiv)
{
    if(cmPort == COMPARATOR0)
    {
        SYSCTL->CP0_CLKRST_CTRL_f.sw_fclk_sel_cp = cmpFclk;
        SYSCTL->CP0_CLKRST_CTRL_f.sw_fclk_div_cp = cmpFclkDiv;
        ACLKRST->clk_en_f.sw_fclk_en_cp0 = ENABLE;
    }
    else if(cmPort == COMPARATOR1)
    {
        SYSCTL->CP1_CLKRST_CTRL_f.sw_fclk_sel_cp = cmpFclk;
        SYSCTL->CP1_CLKRST_CTRL_f.sw_fclk_div_cp = cmpFclkDiv;
        ACLKRST->clk_en_f.sw_fclk_en_cp1 = ENABLE;
    }
}
/**
 *******************************************************************************
** \brief Cmp_Enable
 **
** \param [u8]  cmpPort:               COMPARATOR0
                                       COMPARATOR1
 ** \retval[void] void
 ******************************************************************************/
void Cmp_CLKDisable(uint8_t cmPort)
{
    if(cmPort == COMPARATOR0)
    {
        ACLKRST->clk_en_f.sw_fclk_en_cp0 = DISABLE;
    }
    else if(cmPort == COMPARATOR1)
    {
        ACLKRST->clk_en_f.sw_fclk_en_cp1 = DISABLE;
    }
}

/**
 *******************************************************************************
** \brief Cmp_Enable
 **
** \param [u8]  cmpPort:               COMPARATOR0
                                       COMPARATOR1
 ** \retval[void] void
 ******************************************************************************/
void Cmp_Enable(uint8_t cmPort)
{
    if(cmPort == COMPARATOR0)
    {
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_me = ENABLE;
    }
    else if(cmPort == COMPARATOR1)
    {
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_me = ENABLE;
    }
}
/**
 *******************************************************************************
** \brief Cmp_Disable
 **
 ** \param [u8]  cmpPort:               COMPARATOR0
                                        COMPARATOR1
 ** \retval[void] void
 ******************************************************************************/
void Cmp_Disable(uint8_t cmPort)
{
    if(cmPort == COMPARATOR0)
    {
        AON_ANACFG->reg_comparator_top1_f.comp0_ctrl_me = DISABLE;
    }
    else if(cmPort == COMPARATOR1)
    {
        AON_ANACFG->reg_comparator_top2_f.comp1_ctrl_me = DISABLE;
    }
}

/**
 *******************************************************************************
** \brief Cmp_VrefConfig
 **
 ** \param [u8]  cmpPort:               COMPARATOR0
                                        COMPARATOR1

 ** \param [u8]  channel:               CMP_VREF_VD5
                                        CMP_VREF_1V
                                        CMP_VREF_EXT
                                        CMP_VREF_VD1AON

 ** \param [u8]  select:                CMP_DAC_DIN1
                                        CMP_DAC_DIN2

 ** \param [u8]  ratio:                 dac_out = vref*(din<6>*1/2 +  din<5>*1/4 +  .....+ din<0>*1/128)

 ** \retval[void] void
 ******************************************************************************/

void Cmp_VrefConfig(uint8_t cmPort,uint8_t channel,uint8_t select,uint8_t ratio)
{
    if(cmPort == COMPARATOR0)
    {
        AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp0db = 1;
        if(select == CMP_DAC_DIN1)
        {
            AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp0ds = 0;
            AON_ANACFG->reg_acomp_dac_f.dac0_din1 = ratio;
        }
        else if(select == CMP_DAC_DIN2)
        {
            AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp0ds = 1;
            AON_ANACFG->reg_acomp_dac_f.dac0_din2 = ratio;
        }
    }
    else if(cmPort == COMPARATOR1)
    {
        AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp1db = 1;
        if(select == CMP_DAC_DIN1)
        {
            AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp1ds = 0;
            AON_ANACFG->reg_acomp_dac_f.dac1_din1 = ratio;
        }
        else if(select == CMP_DAC_DIN2)
        {
            AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_cp1ds = 1;
            AON_ANACFG->reg_acomp_dac_f.dac1_din2 = ratio;
        }
    }
    AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_me = 1;
    AON_ANACFG->reg_comparator_top3_f.comp2_ctrl_sdv = channel;
}

/**
 *******************************************************************************
** \brief Cmp_Trim
 **
 ** \param [u8]  cmpPort:               COMPARATOR0
                                        COMPARATOR1
 ** \param [u8]  value:                 trimvalue 3bit
 ** \retval[void] void
 ******************************************************************************/

void Cmp_Trim(uint8_t cmPort,uint8_t value)
{
    if(cmPort == COMPARATOR0)
    {
        AON_ANACFG_TRIM->trim_comparator_value_f.trim_cp0_1p5v = value;
    }
    else if(cmPort == COMPARATOR1)
    {
        AON_ANACFG_TRIM->trim_comparator_value_f.trim_cp1_1p5v = value;
    }

}

