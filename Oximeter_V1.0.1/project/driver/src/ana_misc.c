/**
 *****************************************************************************
 * @brief   Flash driver source file.
 *
 * @file   ana_misc.c
 * @author
 * @date   8/AUG/2021
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
#include "stdio.h"
#include "tcasxxx.h"
#include "ana_misc.h"

/*
 * Configure PGA module enable
 */
void PGA_EnableModule(void)
{
    AON_ANACFG->an_block_f.pga_ctrl_7_enable = 1;
}

/*
 * Configure PGA module disable
 */
void PGA_DisableModule(void)
{
    AON_ANACFG->an_block_f.pga_ctrl_7_enable = 0;
}

/*
 * Configure PGA module enable
 */
void BGR_EnableModule(void)
{
    AON_ANACFG->an_block_f.bgr_ctrl_6_enable = 1;
}

/*
 * Configure PGA module Disable
 */
void BGR_DisableModule(void)
{
    AON_ANACFG->an_block_f.bgr_ctrl_6_enable = 0;
}


/*
 * Configure vbias_ibias module enable
 */
void Vbias_Enable(void)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3C;
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_me = 1;
}

/*
 * Configure vbias_ibias module disable
 */
void Vbias_Disable(void)
{
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_me = 0;
}


/**
 *******************************************************************************
** \brief Vbias_SelVcm
 *  [in]  vcm_sel:           VBIAS_VCM_SEL_NONE            // do not output
                             VBIAS_VCM_SEL_02V             // 0.2V
                             VBIAS_VCM_SEL_03V             // 0.3V
                             VBIAS_VCM_SEL_04V             // 0.4V
                             VBIAS_VCM_SEL_05V             // 0.5V
                             VBIAS_VCM_SEL_06V             // 0.6V
                             VBIAS_VCM_SEL_07V             // 0.7V
                             VBIAS_VCM_SEL_08V             // 0.8V
                             VBIAS_VCM_SEL_09V             // 0.9V
                             VBIAS_VCM_SEL_10V             // 1.0V
                             VBIAS_VCM_SEL_11V             // 1.1V
                             VBIAS_VCM_SEL_12V             // 1.2V
                             VBIAS_VCM_SEL_13V             // 1.3V
                             VBIAS_VCM_SEL_14V             // 1.4V
                             VBIAS_VCM_SEL_15V             // 1.5V

** \retval[void] void
******************************************************************************/
void Vbias_SelVcm(uint16_t vcm_sel)
{
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vsb = vcm_sel;
}

/**
 *******************************************************************************
** \brief LDO3_Enable
** \retval[void] void
******************************************************************************/
void LDO3_Enable(void)
{
    AON_ANACFG->reg_ldo3_top = 0x103F;     // LDO OUTPUT 3V
}

void LDO3_Disable(void)
{
    AON_ANACFG->reg_ldo3_top_f.ctrl_ldo3_0_enable = DISABLE;
}


void XOSC32K_Enable(void)
{
    AON_ANACFG->reg_xosc32k_top_f.ctrl_xosc32k_0_enable = ENABLE;
}

void XOSC32K_Disable(void)
{
    AON_ANACFG->reg_xosc32k_top_f.ctrl_xosc32k_0_enable = DISABLE;
}

void XOSC16M_Enable(void)
{
    ANAREG_1->reg_crystal_16m_top |= (0x01 << 9);
    AON_ANACFG->an_block_f.ctrl_crystal_16m_9_enable = 1;
}

void XOSC16M_Disable(void)
{
    ANAREG_1->reg_crystal_16m_top &= ~(0x01 << 9);
    AON_ANACFG->an_block_f.ctrl_crystal_16m_9_enable = 0;
}


