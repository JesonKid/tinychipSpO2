/**
 *****************************************************************************
 * @brief   idac driver source file.
 *
 * @file   idac.c
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

#include "idac.h"

/*
 * Trigger IDAC transfer start
 */
void TriggerIdacStart(eIDAC_CHAN_T eCh)
{
    if(eCh == CHAN_IDAC1)
    {
        AON_ANACFG->an_block_f.ctrl_idac1_5_enable = 1;     // enable iadc1
    }
    else if(eCh == CHAN_IDAC2)
    {
        AON_ANACFG->an_block_f.ctrl_idac2_5_enable = 1;     // enable iadc2
    }
}

/*
 * Trigger IDAC transfer stop
 */
void TriggerIdacStop(eIDAC_CHAN_T eCh)
{
    if(eCh == CHAN_IDAC1)
    {
        AON_ANACFG->an_block_f.ctrl_idac1_5_enable = 0;     // disable iadc1
    }
    if(eCh == CHAN_IDAC2)
    {
        AON_ANACFG->an_block_f.ctrl_idac2_5_enable = 0;     // disable iadc2
    }

}

/*
 * Set IDAC Output pin
 */
void SetIdacOutput(eIDAC_OUTPUT_T idac1, eIDAC_OUTPUT_T idac2)
{
    uint32_t mux_in = 0;
    /**     <3:2>:  idac2 output sel bit
            01: output to idac2->PA01
            10: output to idac2->PA03
            <1:0>: idac1 output sel bit
            01: output to idac1->PA00
            10: output to idac1->PA02 */
    if(idac1 == OUTPUT_PA00)
    {
        mux_in |= 0x01;
    }
    else if(idac1 == OUTPUT_PA02)
    {
        mux_in |= 0x02;
    }
    if(idac2 == OUTPUT_PA01)
    {
        mux_in |= (0x01 << 2);
    }
    else if(idac2 == OUTPUT_PA03)
    {
        mux_in |= (0x02 << 2);
    }

    ANAREG_1->reg_idac_mux_in4 = mux_in;
}

/*
 * Set IDAC basic current
 * eCh-- IDAC No. basisCur--basic current (uA)
 *
 */
void SetIdacBasicCurrent(eIDAC_CHAN_T eCh, uint8_t basicCur)
{
    uint32_t ireep = 0;

    if(basicCur > 16)
    {
        return;
    }

    /**Min step:16uA
    <4:0>: ibias sel bit for idac1 basic current; default 0_0001.
    <4>:8uA; <3>:4uA; <2>:2uA; <1>:1uA; <0>:1uA.
    0_0000:0uA; 0_0001:1uA; 0_0010:1uA; 0_0011:2uA ........ 1_1111:16uA. */
    ireep = (basicCur * 2) - 1;

    // bit <5>: module enable signal for idac, 0:disable; 1:enable; default 1.
    ireep |= (0x01 << 5);
    if(eCh == CHAN_IDAC1)
    {
        ANAREG_1->reg_IREEP_6B_top1   = ireep;
    }
    else if(eCh == CHAN_IDAC2)
    {
        ANAREG_1->reg_IREEP_6B_top2   = ireep;
    }
}

/*
 * Set IDAC Input data
 * in1-- IDAC1  in2-- IDAC2 (uA) max: 0x3f
 * IDAC电流输入值        Iout=I_lbs * d2a_idac1_in
 */
void SetIdacIn(uint8_t in1, uint32_t in2)
{
    uint32_t uIdacIn = 0;

    if(in1 > 0x3f || in2 > 0x3f)
    {
        return;
    }

    /* IDAC1IN = in1 IDAC2IN = in2 */
    uIdacIn = in1;
    uIdacIn |= (in2 <<8);
    ANAREG_1->reg_idac_in = uIdacIn;
}


