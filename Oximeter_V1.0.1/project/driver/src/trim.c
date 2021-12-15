/**
 *****************************************************************************
 * @brief   trim driver source file.
 *
 * @file   trim.c
 * @author
 * @date   15/Jan/2021
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
 
#include "trim.h"
#include "gpio.h"
#include "rtc.h"
#include "delay.h"
#include "tcasxxx.h"
#include "system_tcasxxx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

void LDO15TrimToTarget(uint32_t TrimVal)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    AON_ANACFG->reg_ldo15_top = TrimVal;//0x102F;
}

void LDOLPTrimToTarget(uint32_t TrimVal)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    AON_ANACFG_TRIM->trim_lpldo_value = TrimVal;//0x0D;
}


void LDO3TrimToTarget(uint32_t TrimVal)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    AON_ANACFG->reg_ldo3_top = TrimVal;//0x104F;
    AON_ANACFG->reg_analog_block_enable = 0x2D5F;
}


void VbiasIbiasTrimToTarget(uint32_t TrimVal)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    AON_ANACFG_TRIM->trim_vbias_ibias_value = TrimVal;//0x42166;
}


void BandGapTrimToTarget(uint32_t TrimVal)
{
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    ANAREG_1->reg_bgr = TrimVal;//0x178; 
}


void RC32kTrimToTarget(uint32_t trim_val)
{
    AON_ANACFG_TRIM->trim_oscrc32k_value = (uint32_t)trim_val;
    SystemCoreClockUpdate();
    SysTickClkSourceConfig(SysTick_CLKSource_HCLK);
}


void RC16mTrimToTarget(enumRC16mTrim_t      targetFreq)
{
    ANAREG_TRIM_MISC->trim_oscrc16m_value = (uint32_t)targetFreq;
    SystemCoreClockUpdate();
    SysTickClkSourceConfig(SysTick_CLKSource_HCLK);
}

void RTCThrelTrimToTarget(enumRTCThrelTrim_t ppma0,enumRTCThrelTrim_t ppma1, enumRTCThrelTrim_t ppma2,enumRTCThrelTrim_t ppma3)
{
    RTC->rtccon1_f.cmp_en = 1;//enable rtc threl 
    RTC->rtccon1_f.ext_temp_en = 0;//internal 
    RTC->ppma0 = ppma0;// 0x3B438 #-19400
    RTC->ppma1 = ppma1;// 0x8CB4 #36020
    RTC->ppma2 = ppma2;// 0xD6674 -170380
    RTC->ppma3 = ppma3;// 0x2D5A4 #-76380
    RTC->rtccon1_f.bn_intp_en = 1;//enable bcn 
    RTC->beacon_intp_sel_out = 0;//half an sec bcn ouput
}
void CmpTrimToTarget(enumCMPTrim_t cmp,enumCMPSource_t n_src,enumCMPSource_t p_src,enumCMPDAC_t int_dac_src)
{
    SYSCTL->CP0_CLKRST_CTRL_f.sw_fclk_sel_cp = 0;//16m
    SYSCTL->CP0_CLKRST_CTRL_f.sw_fclk_div_cp = 0;//16m
    AON_ANACFG->power_switch_f.ctrl_powerswitch_4_en6 = 1;    // module enable signal(en6, only ctrl by software)
    AON_ANACFG->reg_vbias_ibias_top |= (1<<0);
    /*set AN channel if used */
    if(n_src <= CMP_AN_CHANNEL_7)
    {
        GPIO_ClrAFMode(GPIOPortA,(GPIO_PIN_ENUM_E)n_src);
        GPIO_SetAFMode(GPIOPortA,(GPIO_PIN_ENUM_E)n_src, AF3);// enable negetive VCIN 
        ANAREG_1->reg_adsar_mux_in = (0x000000FF|(1<<(19+n_src))); //open negetive vcin
    }
    if(p_src <= CMP_AN_CHANNEL_7)
    {
        GPIO_ClrAFMode(GPIOPortA,(GPIO_PIN_ENUM_E)p_src);
        GPIO_SetAFMode(GPIOPortA,(GPIO_PIN_ENUM_E)p_src, AF3);// enable positive VCIN
        ANAREG_1->reg_adsar_mux_in = (0x000000FF|(1<<(19+p_src))); //open positive vcin
    }
    /*set internal dac if used */
    if((n_src == CMP_AN_INT_DAC)||(p_src == CMP_AN_INT_DAC))
    {
        if(CMP0 == cmp)
        {
            AON_ANACFG->reg_comparator_top3 |= ((int_dac_src<<5)|(1<<4)|(3<<0));
            /*set d2a_dac0_din1_1p5v<6:0>=111_1111,make internal dac ouput 1/2 reference.  */
            AON_ANACFG->reg_acomp_dac &= ~(0x7F<<24); //clr cmp0 dac0_din1
            AON_ANACFG->reg_acomp_dac &= ~(0x7F<<16); //clr cmp0 dac0_din2
            AON_ANACFG->reg_acomp_dac |= (0x7F<<24); //set comp0 dac0_din1, 0x0: 0*Vref; [0x40: 0.5*Vref;] 0x7F: 1*Vref.
            AON_ANACFG->reg_acomp_dac |= (0x7F<<16); //ser comp0 dac0_din2, 0x0: 0*Vref; [0x40: 0.5*Vref;] 0x7F: 1*Vref.
        }
        else
        {
             AON_ANACFG->reg_comparator_top3 |= ((int_dac_src<<5)|(1<<4)|(3<<2));
            /*5.set d2a_dac0_din1_1p5v<6:0>=111_1111,make internal dac ouput 1/2 reference.  */
            AON_ANACFG->reg_acomp_dac &= ~(0x7F<<8); //clr cmp1 dac1_din1
            AON_ANACFG->reg_acomp_dac &= ~(0x7F<<0); //clr cmp1 dac1_din2
            AON_ANACFG->reg_acomp_dac |= (0x7F<<8); //set comp0 dac1_din1, 0x0: 0*Vref; [0x40: 0.5*Vref;] 0x7F: 1*Vref.
            AON_ANACFG->reg_acomp_dac |= (0x7F<<0); //ser comp0 dac1_din2, 0x0: 0*Vref; [0x40: 0.5*Vref;] 0x7F: 1*Vref.
        }
    }
    if(CMP0 == cmp)
    {
        ACLKRST->clk_en_f.sw_fclk_en_cp0 = 1;   //  high to enable fclk for comparator0 filter

        GPIO_ClrAFMode(GPIOPortA,GPIOPin4);
        GPIO_SetAFMode(GPIOPortA,GPIOPin4, AF2);//vc0_out
        /*trim comparator value trim_comparator_value<2:0>=7, (Default=0)*/ //cmp current Trim ctrl
        AON_ANACFG_TRIM->trim_comparator_value &= ~(0x7<<0); //comp0 trim val clr
        AON_ANACFG_TRIM->trim_comparator_value |=  (0x0<<0); //comp0 trim vale set
        AON_ANACFG->reg_comparator_top1 &= ~(0x1<<0); // Comp0 disable
        AON_ANACFG->reg_comparator_top1 = 0x37600; //P DAC; N PA7; Vout inventer; High-Speed mode
        AON_ANACFG->reg_comparator_top1 |= ((7<<16)|(p_src<<12)|(n_src<<8));
        AON_ANACFG->reg_comparator_top1 |= (0x1<<0); // Comp0 enable
    }
    else
    {
        ACLKRST->clk_en_f.sw_fclk_en_cp1 = 1;  //  high to enable fclk for comparator1 filter
        GPIO_ClrAFMode(GPIOPortA,GPIOPin4);
        GPIO_SetAFMode(GPIOPortA,GPIOPin5, AF2);//vc1_out
        /*trim comparator value trim_comparator_value<5:3>=7, (Default=0)*/ //cmp current Trim ctrl
        AON_ANACFG_TRIM->trim_comparator_value &= ~(0x7<<3); //comp1 trim val clr
        AON_ANACFG_TRIM->trim_comparator_value |=  (0x0<<3); //comp1 trim vale set
        AON_ANACFG->reg_comparator_top2 &= ~(0x1<<0); // Comp1 disable
        AON_ANACFG->reg_comparator_top2 |= ((7<<16)|(p_src<<12)|(n_src<<8));//select p&n src 
        AON_ANACFG->reg_comparator_top2 |= (0x1<<0); // Comp1 enable
    }
}

void VDACTrimToTarget(uint32_t TrimValue,enumVDACTrimStyle_t Style,enumVDACTrimTP_t TestPoint)
{
    /*PC01  VDAC OUPUT*/
    GPIO_ClrAFMode(GPIOPortC, GPIOPin1);
    GPIO_SetAFMode(GPIOPortC, GPIOPin1, AF3);
    //SYSCTL->VDAC_CLKRST_CTRL_f.sw_pclk_en_vdac=1;
    AON_ANACFG->reg_vbias_ibias_top = 0x0000003D;
    /*close 16M,or it will pull down the ouput voltage*/
    AON_ANACFG->reg_analog_block_enable = 0x00001D3F;
    /*4:set ctrl_vdac<7:0>= 010_0001   select 1.5V*/
    ANAREG_1->reg_vdac = 0x00000021;
    /*5 pc01 as output pin    */
    ANAREG_1->reg_vdac_mux_in2 = 0x00000001; 
    /*6: d2a_clk_vdac */
    /*7:set trim_vdac<11:0>= 1000_0100_0000 <11:7> Disorder   <6:0>: gain*/
    ANAREG_TRIM_MISC->trim_vdac_value = TrimValue;//0XF40;       //adjust this value to trim
    /* start ouput*/
    ANAREG_1->reg_vdac_conf0 = 0x00000001;
    if(VDAC_GAIN == Style)
    {
        /*gain trim*/
        /* set data to reg«¯d2a_din<12:0>=
        0x0000,
        0x1000,
        0x1FFF*/
        ANAREG_1->reg_vdac_conf2 = TestPoint;
        /*
        test pc01,record the value and then check the Slope K value. 
        if K = 1, keep trim_vdac<6:0> unchanged
        if k < 1, trim_vdac<6:0> +1
        if k > 1, trim_vdac<6:0> -1
        */
    }
    else
    {
        /*Disorder trim*/
        /*set the data to register«¯d2a_din<12:0>=
        10_0000_0000_0000,
        */
        ANAREG_1->reg_vdac_conf2 = TestPoint;//0x2000;
        /*
        test pc01 voltage,record the value 
        if vol = 1.5, keep trim_vdac<11:7> unchanged
        if vol < 1.5, trim_vdac<11:7> +1
        if vol > 1.5, trim_vdac<11:7> -1
        */
    }
}


void IDACTrimToTarget(uint32_t TrimValue,enumIDACTrim_t idac)
{
    /* PA00&PA01 work as Alternative Function IDAC0&IDAC1 */
    if(idac == IDAC1)
    { 
        GPIO_SetAFMode(GPIOPortA, GPIOPin0, AF3);
        
        ANAREG_1->reg_idac_mux_in4 = 0x05; 
        /*Min step 16uA      
        bit<4>:8uA; <3>:4uA; <2>:2uA; <1>:1uA; <0>:1uA    */
        ANAREG_1->reg_IREEP_6B_top1   = 0x0000005f;    
        /* adjust trim register<23:0>*/
        ANAREG_TRIM_MISC->trim_idac1_value = TrimValue;//0x00888884;//adjust this value to trim
        /* gain is 1*/
        ANAREG_1->reg_idac_in = 0x00000101; 
        /* enable DAC1,DAC2 */
        AON_ANACFG->reg_analog_block_enable |= (1<<10);
    }
    else
    {
         GPIO_SetAFMode(GPIOPortA, GPIOPin1, AF3);
        ANAREG_1->reg_idac_mux_in4 = 0x05; 
        /*Min step 16uA      
        bit<4>:8uA; <3>:4uA; <2>:2uA; <1>:1uA; <0>:1uA    */
        ANAREG_1->reg_IREEP_6B_top2   = 0x0000005f;    
        /* adjust trim register<23:0>*/
        ANAREG_TRIM_MISC->trim_idac2_value = TrimValue;//0x00888884;//adjust this value to trim
        /* gain is 1*/
        ANAREG_1->reg_idac_in = 0x00000101; 
        /* enable DAC1,DAC2 */
        AON_ANACFG->reg_analog_block_enable |= (1<<11);
    }
    /*Trim*/
    /*
      1.set d2a_idac1_in_1p5v<5:0>=00_0001,check the ouput current
        if current = 16uA,keep the <2:0>unchanged.
        if current < 16uA,<2:0>+1,else <2:0>-1;
      2.set d2a_idac1_in_1p5v<5:0>=00_0010,check the ouput current
        if current = 32uA,keep the <7:4>unchanged.
        if current < 32uA,<7:4>+1,else <7:4>-1;
      3.set d2a_idac1_in_1p5v<5:0>=00_0100,check the ouput current
        if current = 64uA,keep the <11:8>unchanged.
        if current < 64uA,<11:8>+1,else <11:8>-1;
      4.set d2a_idac1_in_1p5v<5:0>=00_1000,check the ouput current
        if current = 128uA,keep the <15:12>unchanged.
        if current < 128uA,<15:12>+1,else <15:12>-1;
      5.set d2a_idac1_in_1p5v<5:0>=01_0000,check the ouput current
        if current = 256uA,keep the <19:16>unchanged.
        if current < 256uA,<19:16>+1,else <19:16>-1;
      6.set d2a_idac1_in_1p5v<5:0>=10_0000,check the ouput current
        if current = 512uA,keep the <23:20>unchanged.
        if current < 512uA,<23:20>>+1,else <23:20>-1;
    */
}


