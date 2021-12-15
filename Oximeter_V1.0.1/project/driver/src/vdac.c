/**
 *****************************************************************************
 * @brief   vdac driver source file.
 *
 * @file   vdac.c
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

#include "vdac.h"

/*
 * Enable VDAC module
 */
void VdacBlockEnable()
{
    AON_ANACFG->an_block_f.ctrl_vdac_0_enable = 1;   // enable vadc
}

/*
 * Disable VDAC module
 */
void VdacBlockDisable()
{
    AON_ANACFG->an_block_f.ctrl_vdac_0_enable = 0;   // disable vadc
}

/*
 * Get VDAC fifo state
 */
uint8_t GetVdacFifoFreeNum()
{
    return (ANAREG_1->reg_adc_fifo_state_f.vdac_fifo_free);
}

/*
 * Trigger VDAC transfer start
 */
void TriggerVdacStart()
{
    ANAREG_1->reg_vdac_conf0 = 0x00000001;
}

/*
 * Trigger VDAC transfer start
 */
void TriggerVdacStop()
{
    ANAREG_1->reg_vdac_conf0 = 0x00000000;
}

/*
 * Config VDAC output IO setting
 * pinNo -- output pin No.
 */
void ConfigVdacOutputPin(eVDAC_PIN_T pinNo)
{
    if(pinNo == VDAC_PIN1)
    {
        GPIO_ClrAFMode(GPIOPortC, GPIOPin1);
        GPIO_SetAFMode(GPIOPortC, GPIOPin1, AF3);
    }
    else if(pinNo == VDAC_PIN2)
    {
        GPIO_ClrAFMode(GPIOPortC, GPIOPin2);
        GPIO_SetAFMode(GPIOPortC, GPIOPin2, AF3);
    }
    /*close 16M.  XTALH and VDAC mux*/
    AON_ANACFG->an_block_f.ctrl_crystal_16m_9_enable = 0;
    AON_ANACFG->an_block_f.sdm_ctrl_7_enable = 0;
    AON_ANACFG->an_block_f.pga_ctrl_7_enable = 0;
    AON_ANACFG->an_block_f.ctrl_sar_8_enable = 0;
}

/*
 * Config VDAC setting
 * pinNo -- output pin No.
 * eVcm -- vcm sel bit
 * eGain -- vout gain ctrl bit
 */
void VdacConfigure(eVDAC_PIN_T pinNo, eVDAC_VCMSEL_T eVcm, eVDAC_GAIN_T eGain)
{
    /*1:pc01 output    2:pc02 output*/
    if (pinNo == VDAC_PIN1)
    {
        ANAREG_1->reg_vdac_mux_in2 = 0x00000001;    // output to PC01
    }
    else if (pinNo == VDAC_PIN2)
    {
        ANAREG_1->reg_vdac_mux_in2 = 0x00000002;    // output to PC02
    }
    else
    {
        ANAREG_1->reg_vdac_mux_in2 = 0x00000000;
    }

    /*0x51 : 2V*1   0x21: 1.5V*  0x11: 1.4V  0x01: 1.2V*/
    ANAREG_1->reg_vdac_f.vout_gain_ctrl = eGain;
    ANAREG_1->reg_vdac_f.vadc_vcm_sel = eVcm;

    /*0000_0000_0000_00: 0.2V*/
    AON_ANACFG->reg_vbias_ibias_top = 0x0000003D;
}

/*
 * Config VDAC Fclk max 1Mhz
 */
void SetVdacFclkSource(uint8_t clkdiv)
{
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_rst_vdac = 1;
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_pclk_en_vdac = 0;
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_fclk_en_vdac = 0;
    __NOP();
    __NOP();
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_fclk_div_vdac = clkdiv;
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_pclk_en_vdac = 1;
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_fclk_en_vdac = 1;
    SYSCTL->VDAC_CLKRST_CTRL_f.sw_rst_vdac = 0;
}

/*
 * Set VDAC output value
 * value-- VDAC output data
 */
void SetVdacOutputValue(uint16_t value)
{ 
    /* ouput value*/
    ANAREG_1->reg_vdac_conf2 = value;   // <12:0> Vdac fifo wr
}



