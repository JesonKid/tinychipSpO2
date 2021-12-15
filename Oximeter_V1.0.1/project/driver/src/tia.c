/**
*****************************************************************************
* @brief
*
* @file   tia.c
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
#include "tcasxxx.h"
#include "system_tcasxxx.h"
#include "tia.h"
#include "gpio.h"

void TiaIOConfigure(void)
{
    //使用模拟功能前关闭数字功能
    //跟随测试
    //PC06 out
    GPIO_SetAFMode(GPIOPortC, GPIOPin6, AF1);
}


/*
***********************************************************
Configure PC06 as tia output.Connect OP0_INN together with PC06.
Tia is configured as voltagefollower.
***********************************************************
*/
void TiaConfigureVoltagefollow(void)
{
    //AON_ANACFG->reg_opa_iv = 0x0003F;
    AON_ANACFG->reg_opa_iv_f.opa_iv_stg1 = 0x03;    /*  <1:0> current select of opamp_iv stg1,default 11
                                                        00, 20uA; 01,25uA; 10,30uA; 11,35uA.         */
    AON_ANACFG->reg_opa_iv_f.opa_iv_stg2 = 0x03;    /** <3:2> current select of opamp_iv stg2,default 11
                                                        00, 60uA; 01,105uA; 10,150uA; 11,200uA  */
    AON_ANACFG->reg_opa_iv_f.opa_iv_me = 1;         // module enable

    //0000_0000_0000_0000_0011_1101[19:6] VCM = 0.2V~1.5V   0x103D-->>0.8V
    //AON_ANACFG->reg_vbias_ibias_top = 0x00003D;
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_me = 1;      //module enable
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_svb = 0;     /*  sel vbg bit
                                                                            0: lowpower bgr; 1: high accuracy bgr */
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vbe = 1;     /*  vcm buffer enable   */
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vpbe = 1;    /*  vrefp10 buffer enable   */
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_fbe = 1;     /*  flash buffer enable */
    AON_ANACFG->reg_vbias_ibias_top_f.ctrl_vbias_ibias_top_vge = 1;     /*  vbegen enable */
}


/*
***********************************************************
Configure PC06 as tia output.Connect OP0_INN together with PC06.
Tia is configured as voltagefollower.
***********************************************************
*/
void TiaConfigureComVoltageFromInternal(enumTIAVipOnChipSel_t onchip_sel)
{
    switch (onchip_sel)
    {
        case TIA_OFFCHIP_VCM:
            AON_ANACFG->reg_opa_iv = 0x0003F;
            AON_ANACFG->reg_opa_iv_f.opa_vol_sel = 0;
            break;
        case TIA_BUFFER_OUTPUT:
            AON_ANACFG->reg_opa_iv = 0x0003F;
            AON_ANACFG->reg_opa_iv_f.opa_vol_sel = 1;
            break;
        case TIA_IDAC_VCM:
            AON_ANACFG->reg_opa_iv = 0x0003F;
            AON_ANACFG->reg_opa_iv_f.opa_vol_sel = 2;
            break;
        case TIA_VDAC_VCM:
            AON_ANACFG->reg_opa_iv = 0x0003F;
            AON_ANACFG->reg_opa_iv_f.opa_vol_sel = 3;
            break;
        default:
            break;
    }

    AON_ANACFG->reg_opa_iv_f.opa_out_pgap_en = 1;       //<13> enable opamp output send to pga_p, default 0
    AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgan_en = 1;    //<14> enable opamp positive input send to pga_n, default 0

    //0000_0000_0000_0000_0011_1101[19:6] VCM = 0.2V~1.5V   0x103D-->>0.8V
    AON_ANACFG->reg_vbias_ibias_top = 0x00003D;

    AON_ANACFG_TRIM ->trim_opamp_iv_value = 0x00;
}

/**
 *******************************************************************************
** \brief Tia_Init
 ** Tia_Init_t:
** \param [u8]  tiaStg1Level:       TIA_STG1_LEVEL0
                                    TIA_STG1_LEVEL1
                                    TIA_STG1_LEVEL2
                                    TIA_STG1_LEVEL3

** \param [u8]  tiaStg2Level:       TIA_STG2_LEVEL0
                                    TIA_STG2_LEVEL1
                                    TIA_STG2_LEVEL2
                                    TIA_STG2_LEVEL3

** \param [u8]  tiaPosConfig:       TIA_OUT_CONECT_NONE
                                    TIA_OUT_CONECT_PGA_P
                                    TIA_OUT_CONECT_PGA_N

** \param [u8]  tiaOutConfig:       TIA_OUT_CONECT_NONE
                                    TIA_OUT_CONECT_PGA_P
                                    TIA_OUT_CONECT_PGA_N

** \param [u8]  tiaVipLevel:        TIA_VIP_LEVEL0
                                    TIA_VIP_LEVEL1
                                    TIA_VIP_LEVEL2
                                    TIA_VIP_LEVEL3
                                    TIA_VIP_LEVEL4
                                    TIA_VIP_LEVEL5
                                    TIA_VIP_LEVEL6
                                    TIA_VIP_LEVEL7
                                    TIA_VIP_LEVEL8
                                    TIA_VIP_LEVEL9
                                    TIA_VIP_LEVEL10
                                    TIA_VIP_LEVEL11
                                    TIA_VIP_LEVEL12
                                    TIA_VIP_LEVEL13
                                    TIA_VIP_LEVEL14
                                    TIA_VIP_LEVEL15

** \param [u8]  tiaVipSelect:       TIA_VIP_SELECT_EXTERNAL
                                    TIA_VIP_SELECT_INTERNAL
                                    TIA_VIP_SELECT_IDAC
                                    TIA_VIP_SELECT_VDAC

 ** \param [u8]  tiaHighPowerEn:            ENABLE
                                            DISABLE

 ** \retval[void] void
 ******************************************************************************/

void Tia_Init(Tia_Init_t * pInit)
{

    if(pInit->tiaPosConfig == TIA_VIP_CONECT_PGA_P)
    {
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgan_en = 0;
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgap_en = 1;
    }
    else if(pInit->tiaPosConfig == TIA_VIP_CONECT_PGA_N)
    {
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgan_en = 1;
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgap_en = 0;
    }
    else
    {
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgan_en = 0;
        AON_ANACFG->reg_opa_iv_f.opa_pos_in_pgap_en = 0;
    }

    if(pInit->tiaPosConfig == TIA_OUT_CONECT_PGA_P)
    {
        AON_ANACFG->reg_opa_iv_f.opa_out_pgan_en = 0;
        AON_ANACFG->reg_opa_iv_f.opa_out_pgap_en = 1;
    }
    else if(pInit->tiaPosConfig == TIA_OUT_CONECT_PGA_N)
    {
        AON_ANACFG->reg_opa_iv_f.opa_out_pgan_en = 1;
        AON_ANACFG->reg_opa_iv_f.opa_out_pgap_en = 0;
    }
    else
    {
        AON_ANACFG->reg_opa_iv_f.opa_out_pgan_en = 0;
        AON_ANACFG->reg_opa_iv_f.opa_out_pgap_en = 0;
    }

    if(AON_ANACFG->reg_opa_iv_f.opa_vol_sel == TIA_VIP_SELECT_INTERNAL)
    {
        AON_ANACFG->reg_opa_iv_f.opa_output_vol = pInit->tiaVipLevel;
    }

    AON_ANACFG->reg_opa_iv_f.opa_vol_sel = pInit->tiaVipSelect;
    AON_ANACFG->reg_opa_iv_f.opa_iv_stg1 = pInit->tiaStg1Level;
    AON_ANACFG->reg_opa_iv_f.opa_iv_stg2 = pInit->tiaStg2Level;
    AON_ANACFG->reg_opa_iv_f.opa_iv_high_en = pInit->tiaHighPowerEn;
    AON_ANACFG->reg_opa_iv_f.opa_out_rail_en = pInit->tiaOutSelect;

}


/**
 *******************************************************************************
** \brief Tia_Enable
 **
 ** \param [u8]
 **
 ** \retval[void] void
 ******************************************************************************/

void Tia_Enable(void)
{
    AON_ANACFG->reg_opa_iv_f.opa_iv_me = 1;
}


/**
 *******************************************************************************
** \brief Tia_Disable
 **
 ** \param [u8]
 **
 ** \retval[void] void
 ******************************************************************************/

void Tia_Disable(void)
{
    AON_ANACFG->reg_opa_iv_f.opa_iv_me = 0;
}


/**
 *******************************************************************************
** \brief Tia_PosVrefConfig
 **         when AON_ANACFG->reg_opa_iv_f.opa_vol_sel is TIA_VIP_SELECT_INTERNAL can use this interface

 ** \param [u8]  Level:      TIA_VIP_LEVEL0
                             TIA_VIP_LEVEL1
                             TIA_VIP_LEVEL2
                             TIA_VIP_LEVEL3
                             TIA_VIP_LEVEL4
                             TIA_VIP_LEVEL5
                             TIA_VIP_LEVEL6
                             TIA_VIP_LEVEL7
                             TIA_VIP_LEVEL8
                             TIA_VIP_LEVEL9
                             TIA_VIP_LEVEL10
                             TIA_VIP_LEVEL11
                             TIA_VIP_LEVEL12
                             TIA_VIP_LEVEL13
                             TIA_VIP_LEVEL14
                             TIA_VIP_LEVEL15
 **
 ** \retval[void] void
 ******************************************************************************/
void Tia_PosVrefConfig(uint8_t Level)
{
    AON_ANACFG->reg_opa_iv_f.opa_output_vol = Level;
}
