/**
*****************************************************************************
* @brief
*
* @file   op.c
* @author   lt
* @date   2021-10-20
*****************************************************************************
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* Copyright (c) 2021 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/

#include "stdio.h"
#include "system_tcasxxx.h"
#include "op.h"
#include "gpio.h"

/**
 *******************************************************************************
** \brief OPA_VcmEnable
 **
 ** \param [u8]
 ** \retval[void] void
 ******************************************************************************/
void OPA_VcmEnable(void)
{
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vbe = ENABLE;//enable vcm buffer
}

/**
 *******************************************************************************
** \brief OPA_VcmDisable
 **
 ** \param [u8]
 ** \retval[void] void
 ******************************************************************************/
void OPA_VcmDisable(void)
{
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vbe = DISABLE;//disable vcm buffer
}

/**
 *******************************************************************************
** \brief OPA_VcmConfig
 **
 ** \param [u8] Volt:  14 Bit
                         0000_0000_0000_01: 0.2V
                         ......
                         1000_0000_0000_00:1.5V

 ** \retval[void] void
 ******************************************************************************/
void OPA_VcmConfig(uint16_t Volt)
{
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vsb = Volt;
}

/**
 *******************************************************************************
** \brief OP1_Init
 *  [in] nOpx - OP_1,OP_2
 ** Opa_Init_t£º
 ** \param [u8]  opaOutSelect:      OPA_OUT_CHIP
                                    OPA_OUT_PAD

 ** \param [u8]  opaBufferEn:       ENABLE
                                    DISABLE

 ** \param [u8]  opaGainLevel:      OPA_GAIN_LEVEL0
                                    OPA_GAIN_LEVEL1
                                    OPA_GAIN_LEVEL2
                                    OPA_GAIN_LEVEL3
                                    OPA_GAIN_LEVEL4
                                    OPA_GAIN_LEVEL5
                                    OPA_GAIN_LEVEL6
                                    OPA_GAIN_LEVEL7

 ** \param [u8]  opaPosConfig:      OPA_VIP_NONE
                                    OPA_VIP_VREF
                                    OPA_VIP_CHIP
                                    OPA_VIP_PAD

 ** \param [u8]  opaNegConfig:      OPA_VIN_BUFFER
                                    OPA_VIN_PGA
                                    OPA_VIN_CHIP
                                    OPA_VIN_PAD

 ** \param [u8]  opaResConfig:      OPA_RES_GND
                                    OPA_RES_CHIP
                                    OPA_RES_VIN_PAD
                                    OPA_RES_VIP_PAD

 ** \param [u8]  opaVipSelect:      OPA_VIP_VREF_NONE
                                    OPA_VIP_VREF_VCM
                                    OPA_VIP_VREF_IADC
                                    OPA_VIP_VREF_VADC

 ** \param [u8]  opaOutPadEn:       ENABLE
                                    DISABLE

  ** \param [u8]  opaFeedbackEn:    ENABLE
                                    DISABLE

 ** \retval[void] void
 ******************************************************************************/
void OPA_Init(uint8_t nOpx, Opa_Init_t * pInit)
{
    tc_reg_rail_opa_field_t *reg_opa[2] = {&(ANAREG_1->reg_rail_opa_1_f), &(ANAREG_1->reg_rail_opa_2_f)};

    if(nOpx > OP_2)
    {
        return;
    }
    if(pInit->opaVipSelect == OPA_VIP_VREF_VCM)
    {
        reg_opa[nOpx]->reg_rail_opa_svmvc  = 0;
        reg_opa[nOpx]->reg_rail_opa_sivc   = 0;
        reg_opa[nOpx]->reg_rail_opa_svcvc  = 1;
    }
    else if(pInit->opaVipSelect == OPA_VIP_VREF_IADC)
    {
        reg_opa[nOpx]->reg_rail_opa_svmvc  = 0;
        reg_opa[nOpx]->reg_rail_opa_sivc   = 1;
        reg_opa[nOpx]->reg_rail_opa_svcvc  = 0;
    }
    else if(pInit->opaVipSelect == OPA_VIP_VREF_VADC)
    {
        reg_opa[nOpx]->reg_rail_opa_svmvc  = 1;
        reg_opa[nOpx]->reg_rail_opa_sivc   = 0;
        reg_opa[nOpx]->reg_rail_opa_svcvc  = 0;
    }
    else
    {
        reg_opa[nOpx]->reg_rail_opa_svmvc  = 0;
        reg_opa[nOpx]->reg_rail_opa_sivc   = 0;
        reg_opa[nOpx]->reg_rail_opa_svcvc  = 0;
    }

    reg_opa[nOpx]->reg_rail_opa_be        = pInit->opaBufferEn;
    reg_opa[nOpx]->reg_rail_opa_gains     = pInit->opaGainLevel;
    reg_opa[nOpx]->reg_rail_opa_vope      = pInit->opaOutSelect;
    reg_opa[nOpx]->reg_rail_opa_resinsel  = pInit->opaResConfig;
    reg_opa[nOpx]->reg_rail_opa_vns       = pInit->opaNegConfig;
    reg_opa[nOpx]->reg_rail_opa_vps       = pInit->opaPosConfig;
    reg_opa[nOpx]->reg_rail_opa_fre       = pInit->opaFeedbackEn;
    reg_opa[nOpx]->reg_rail_opa_vcvpe     = pInit->opaOutPadEn;

}

/**
 *******************************************************************************
** \brief OPA_Enable
 **
 ** \param [in] nOpx - OP_1,OP_2
 ** \retval[void] void
 ******************************************************************************/
void OPA_Enable(uint8_t nOpx)
{
    tc_reg_rail_opa_field_t *reg_opa[2] = {&(ANAREG_1->reg_rail_opa_1_f), &(ANAREG_1->reg_rail_opa_2_f)};

    reg_opa[nOpx]->reg_rail_opa_me = ENABLE;
}

/**
 *******************************************************************************
** \brief OPA_Disable
 **
 ** \param [in] nOpx - OP_1,OP_2
 ** \retval[void] void
 ******************************************************************************/
void OPA_Disable(uint8_t nOpx)
{
    tc_reg_rail_opa_field_t *reg_opa[2] = {&(ANAREG_1->reg_rail_opa_1_f), &(ANAREG_1->reg_rail_opa_2_f)};

    reg_opa[nOpx]->reg_rail_opa_me = DISABLE;
}


/**
 *******************************************************************************
** \brief OP1_GainConfig
 ** \param [in] nOpx - OP_1,OP_2
 ** \param [u8] Level:   OPA_GAIN_LEVEL0
                            OPA_GAIN_LEVEL1
                            OPA_GAIN_LEVEL2
                            OPA_GAIN_LEVEL3
                            OPA_GAIN_LEVEL4
                            OPA_GAIN_LEVEL5
                            OPA_GAIN_LEVEL6
                            OPA_GAIN_LEVEL7


 ** \retval[void] void
 ******************************************************************************/
void OPA_GainConfig(uint8_t nOpx, uint8_t Level)
{
    tc_reg_rail_opa_field_t *reg_opa[2] = {&(ANAREG_1->reg_rail_opa_1_f), &(ANAREG_1->reg_rail_opa_2_f)};

    reg_opa[nOpx]->reg_rail_opa_gains = Level;
}



/**
 *******************************************************************************
** \brief OP1_GainConfig
 **
 ** \param [u8] Level:   OP2_VIN_PAD_PC07
                            OP2_VIN_PAD_PC04

 ** \retval[void] void
 ******************************************************************************/

void OP2_VinPadConfig(uint8_t vin_pad)
{
    ANAREG_1->reg_rail_op_mux_in4_f.reg_rail_op_mux_in4_vinsb = vin_pad;
}
/**
 *******************************************************************************
** \brief OP1_GainConfig
 **
 ** \param [u8] Level:   OP2_VIP_PAD_PD00
                            OP2_VIP_PAD_PC03

 ** \retval[void] void
 ******************************************************************************/

void OP2_VipPadConfig(uint8_t vip_pad)
{
    ANAREG_1->reg_rail_op_mux_in4_f.reg_rail_op_mux_in4_vipsb = vip_pad;
}
/**
 *******************************************************************************
** \brief OP1_GainConfig
 **
 ** \param [u8] Level:   OP2_OUT_PAD_PD01
                            OP2_OUT_PAD_PC05

 ** \retval[void] void
 ******************************************************************************/

void OP2_OutPadConfig(uint8_t out_pad)
{
    ANAREG_1->reg_rail_op2_mux_in2_f.reg_rail_op_mux_in2_op2o = out_pad;
}


