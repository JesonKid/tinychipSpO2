/**
 *****************************************************************************
 * @brief   Watchdog driver source file.
 *
 * @file   wdg.c
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

#include "wdg.h"
#include "interrupt.h"

/*******************************************************************************
* Definitions 私有(仅本源文件内使用)宏、枚举、结构体的定义
******************************************************************************/

/*******************************************************************************
* Variables 所有全局变量(外部，静态，常量，指针)的定义
******************************************************************************/

/******************************************************************************* 
 * Code 所有函数(外部，内部)的定义
 ******************************************************************************/


/********************************************************
@brief  中断处理函数
@param  void
@retval void
*********************************************************/
void __attribute__((weak))Hal_IWDGHandler(void)
{
    printf(" iwdg isr,please define your handler function \r\n");
}

/********************************************************
@brief  中断处理函数
@param  void
@retval void
*********************************************************/
void __attribute__((weak))Hal_WWDGAdvHandler(void)
{
    printf(" adv isr,please define your handler function \r\n");
}

/********************************************************
@brief  中断处理函数
@param  void
@retval void
*********************************************************/
void __attribute__((weak))Hal_WWDGT2Handler(void)
{
    printf(" t2 isr,please define your handler function \r\n");
}

/********************************************************
@brief  中断处理函数
@param  void
@retval void
*********************************************************/
void __attribute__((weak))Hal_WWDGTimeOutHandler(void)
{
    printf(" timeout isr,please define your handler function \r\n");
}

/********************************************************
@brief  IWDG使能函数
@param  iwdg：iwdg寄存器基地址
@retval void
*********************************************************/
void EnableIWDG(T_WDG* iwdg)
{
    iwdg->cnt_max = 0xfff;
    /*set ctrl reg to choose isr handler or reset chip directly*/
    iwdg->ctrl = 0x01;  //0x01 isr
    //iwdg->ctrl = 0x03;//0x03 reset
}

/********************************************************
@brief  WWDG使能函数
@param  wwdg：wwdg寄存器基地址
@retval void
*********************************************************/
void EnableWWDG(T_WDG* wwdg)
{
    wwdg->cnt_max = 0x7F;
    wwdg->wt = 0x6F;
    wwdg->t2 = 0x20;
    wwdg->icr = 0x07;
    /* set ctrl reg to choose isr handler or reset chip directly */
    wwdg->ctrl = 0x0;
    wwdg->ctrl |= 0x01;
}
/********************************************************
@brief  register configure lock
@param
@retval void
*********************************************************/
void IWDG_Lock(void)
{
    IWDG->lock = 0x12345678;
}
/********************************************************
@brief  register configure unlock
@param
@retval void
*********************************************************/
void IWDG_UnLock(void)
{
    IWDG->lock = 0xaaaa5555;
}

/********************************************************
@brief  IWDG_Init
        the overflow time is loadVal / (RC-IN / (clkDiv + 1))
@param  clkDiv the wdg source clk div
        loadVal the wdg overflow value
        mode    the wdg trigger interupt - 0 or reset-1
@retval void
*********************************************************/
void IWDG_Init(uint8_t clkDiv,uint16_t loadVal,uint8_t mode)
{
    //clk enable the wdg
    ACLKRST->clk_en_f.sw_fclk_en_awdg = 0x01;
    //set the prescaler
    ACLKRST->clk_en_f.sw_fclk_div_awdg = clkDiv;

    IWDG->ctrl_f.wdg_rst_en_for_ovf = mode;
    IWDG->cnt_max = loadVal;
    //reset the count
    IWDG->ccr = 0x01;
}
/********************************************************
@brief  IWDG_Enable
@param
@retval void
*********************************************************/
void IWDG_Enable(void)
{
    IWDG->ctrl_f.wdg_enable = 0x01;
}
/********************************************************
@brief  IWDG_Disable
@param
@retval void
*********************************************************/
void IWDG_Disable(void)
{
    IWDG->ctrl_f.wdg_enable = 0x00;
}
/********************************************************
@brief  IWDG_GetITFlag
@param
@retval ITFlag
*********************************************************/
uint8_t IWDG_GetITFlag(uint8_t it_flag)
{
    uint8_t status = 0;
    if(it_flag == WDG_ADV_FLAG)
    {
        status =IWDG->status_f.adv_int;
    }
    else if(it_flag == WDG_IT2_FLAG)
    {
        status =IWDG->status_f.t2_int;
    }
    else if(it_flag == WDG_IOVF_FLAG)
    {
        status =IWDG->status_f.ovf_int;
    }
    return status;
}
/********************************************************
@brief  IWDG_ClearITPending
@param  ITFlag
@retval
*********************************************************/
void IWDG_ClearITPending(uint8_t it_flag)
{
    if(it_flag == WDG_ADV_FLAG)
    {
        IWDG->icr_f.clr_adv_int = 0x01;
    }
    else if(it_flag == WDG_IT2_FLAG)
    {
        IWDG->icr_f.clr_t2_int = 0x01;
    }
    else if(it_flag == WDG_IOVF_FLAG)
    {
        IWDG->icr_f.clr_ovf_int = 0x01;
    }
}
/********************************************************
@brief  IWDG_ResetCnt
        IWDG Feed Dog
@param
@retval
*********************************************************/
void IWDG_ResetCnt(void)
{
    IWDG->ccr = 0x01;
}

/********************************************************
@brief  register configure lock
@param
@retval void
*********************************************************/
void WWDG_Lock(void)
{
    WWDG->lock = 0x12345678;
}
/********************************************************
@brief  register configure unlock
@param
@retval void
*********************************************************/
void WWDG_UnLock(void)
{
    WWDG->lock = 0xaaaa5555;
}

/********************************************************
@brief  WWDG_Enable
@param  WWDG_InitType * pInitSt
        wt : the whindows value
        t2 : the latest feedog chance
        count : the wdg count
        clkSrc : the clock source
        preScaler : the clock prescaler
        Wwdg_Mode : the trigger mode   ----- 0 - interupt 1 - reset mode
@retval void
*********************************************************/

void WWDG_Init(WWDG_InitType * pInitSt)
{
    
    //SYSCTL->WDG_CLKRST_CTRL_f.sw_rst_wdg = 1;
    SYSCTL->WDG_CLKRST_CTRL_f.sw_pclk_en_wdg = 1;
    SYSCTL->WDG_CLKRST_CTRL_f.sw_fclk_en_wdg = 1;
    SYSCTL->WDG_CLKRST_CTRL_f.sw_fclk_sel_wdg = pInitSt->clkSrc;
    SYSCTL->WDG_CLKRST_CTRL_f.sw_fclk_div_wdg = pInitSt->preScaler;

    WWDG->wt = pInitSt->wt;
    WWDG->t2 = pInitSt->t2;
    WWDG->cnt_max = pInitSt->count;
    /* set ctrl reg to choose isr handler or reset chip directly */
    WWDG->ctrl = 0x0;
    WWDG->ctrl_f.wdg_rst_en_for_adv  = pInitSt->Wwdg_Mode.Wwdg_Adv_Mode;
    WWDG->ctrl_f.wdg_rst_en_for_t2   = pInitSt->Wwdg_Mode.Wwdg_T2_Mode;
    WWDG->ctrl_f.wdg_rst_en_for_ovf  = pInitSt->Wwdg_Mode.Wwdg_Ovf_Mode;
    WWDG->ctrl_f.wdg_enable   = 0x01;
    
}
/********************************************************
@brief  WWDG_Enable
@param
@retval void
*********************************************************/
void WWDG_Enable(void)
{
    WWDG->ctrl_f.wdg_enable = 0x01;
}
/********************************************************
@brief  WWDG_Disable
@param
@retval void
*********************************************************/
void WWDG_Disable(void)
{
    WWDG->ctrl_f.wdg_enable = 0x00;
}
/********************************************************
@brief  WWDG_GetITFlag
@param
@retval ITFlag
*********************************************************/
uint8_t WWDG_GetITFlag(uint8_t it_flag)
{
    uint8_t status = 0;
    if(it_flag == WDG_ADV_FLAG)
    {
        status =WWDG->status_f.adv_int;
    }
    else if(it_flag == WDG_IT2_FLAG)
    {
        status =WWDG->status_f.t2_int;
    }
    else if(it_flag == WDG_IOVF_FLAG)
    {
        status =WWDG->status_f.ovf_int;
    }
    return status;
}
/********************************************************
@brief  WWDG_ClearITPending
@param  ITFlag
@retval
*********************************************************/
void WWDG_ClearITPending(uint8_t it_flag)
{
    if(it_flag == WDG_ADV_FLAG)
    {
        WWDG->icr_f.clr_adv_int = 0x01;
    }
    else if(it_flag == WDG_IT2_FLAG)
    {
        WWDG->icr_f.clr_t2_int = 0x01;
    }
    else if(it_flag == WDG_IOVF_FLAG)
    {
        WWDG->icr_f.clr_ovf_int = 0x01;
    }
}


/********************************************************
@brief  WWDG_ResetCnt
        WWDG Feed Dog
@param
@retval
*********************************************************/
void WWDG_ResetCnt(void)
{
    WWDG->ccr = 0x01;
}

