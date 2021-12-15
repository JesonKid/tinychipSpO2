/**
 *****************************************************************************
 * @brief   saradc driver source file.
 *
 * @file   saradc.c
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

#include "saradc.h"
#include "delay.h"
/* 
 * Set SARAdc Fclk Source  RC16M
 */
void SetSARADCFclkSource(uint8_t div)
{
    // FCLK for SAR, default is using 16M and fclk is enable
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_en_saradc = 1;
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_pclk_en_saradc = 1;
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_div_saradc = div;
	SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_sel_saradc = 0;
}

/*
 * Set SARAdc trigger num after once start
 * Fix me:ADC_TRIGGER_NUM_256 work fail
 */
void SetSARAdcTriggerNum(enumAdcTriggerNum_t num)
{
    ANAREG_1->reg_adc_conf1 &=~(0x7 << 5);
    ANAREG_1->reg_adc_conf1 |= (num << 5);
}

/*
 * Trigger SARAdc transfer
 */
void TriggerSARAdcStart(void)
{
    ANAREG_1->reg_saradc_ctrl |= 0x01;
    DelayForCommandExeDone();
    ANAREG_1->reg_saradc_ctrl &= ~0x01;
}

/*
 * Configure SARAdc
 */
void SetSARAdcConfig(void)
{
    ANAREG_1->reg_adsar_top    = 0x120;
    //ctrl_sar_8_enable + ctrl_adsar_chargepump_0_enable
    AON_ANACFG->reg_analog_block_enable &= ~(0x01 << 1); // disable sdadc
    AON_ANACFG->reg_analog_block_enable |= ((0x01 << 7) | (0x01 << 5));
}


/*
 * Get SarADC fifo state
 */
uint8_t GetSarAdcFifoValidState()
{
    return (ANAREG_1->reg_adc_fifo_state_f.adcsar_fifo_entry_valid);
}


