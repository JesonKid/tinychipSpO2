/**
 *****************************************************************************
 * @brief   Timer driver source file.
 *
 * @file   timer.c
 * @author
 * @date   28/JUN/2020
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

#include "timer.h"
#include "interrupt.h"
#include "gpio.h"


void TimerStart(T_TIMER* timer)
{
    timer->cfg |= 0x01;
}

void TimerStop(T_TIMER* timer)
{
    timer->cfg &= 0x1FE;
}

/**
******************************************************************************
    ** \brief  WEAK Timer isr handler
    ** @param  void
    ** \retval void
    **
******************************************************************************/

void __attribute__((weak)) Hal_Timer0Handler(void)
{
    printf("timer0 interrupt,no handler function defined/r/n");
}
void __attribute__((weak))Hal_Timer1Handler(void)
{
    printf("timer1 interrupt....\r\n");
}
void __attribute__((weak))Hal_Timer2Handler(void)
{
    printf("timer2 interrupt....\r\n");
}
void __attribute__((weak))Hal_Timer3Handler(void)
{
    printf("timer3 interrupt....\r\n");
}
void __attribute__((weak))Hal_Timer4Handler(void)
{                            
    printf("timer4 interrupt....\r\n");
}
void __attribute__((weak))Hal_Timer5Handler(void)
{
    printf("timer5 interrupt....\r\n");

}
void __attribute__((weak))Hal_LptimerHandler(void)
{
    printf("lptimer interrupt....\r\n");
}

/**
******************************************************************************
    ** \brief  Timer Interrupt set function
    ** @param  T_TIMER: Timer0~5
    ** \retval void
    **
******************************************************************************/
void SetTimerIsr(T_TIMER* Timer)
{
    Timer->sw_force_con = 1;
    Timer->latched_cnt = 0;
    Timer->prescale = 9;
    /*time = period * repeat  */
    Timer->period = 20000;
    Timer->repeat_value = 1000;
    Timer->cfg = 0x01;
}


/**
******************************************************************************
    ** \brief  cal external pulse
    ** @param  T_TIMER
    ** \retval void
    **
******************************************************************************/
void CalExtPulse(T_TIMER* Timer)
{

    if(Timer == TIMER0)
    {
         PINMUX2_7->PA_03 |= (1<<8);
    }
    else if(Timer == TIMER1)
    {
        PINMUX2_7->PA_04 |= (1<<8);
    }
    else if(Timer == TIMER2)
    {
        PINMUX2_7->PA_05 |= (1<<8);
    }
    else
    {
        printf("error basic timer!");
        return;
    }
    Timer->prescale = 0;
    Timer->cfg = 0x5;
}

/**
******************************************************************************
    ** \brief  software force bk
    ** @param  advTimer: Timer3 Timer4  Timer5
    ** \retval Void
    **
******************************************************************************/

void SoftwareForceBK(T_TIMER* advTimer)
{
    advTimer->sw_force_con |= 0x02; 
}

/**
******************************************************************************
    ** \brief  adv timer sync start or stop
    ** @param  cmd: 0:stop, bit0:TIME3, bit1:TIME4, bit2:TIME5
    ** \retval void
    **
*****************************************************************************
*/
void SyncStartStop(uint8_t cmd)
{
    if(cmd == 0)
    {
        ((T_SYSCTL *)SYSCTL_BASE_ADDR)->ext_pwm_en = 0x00;
    }
    else
    {
        ((T_SYSCTL *)SYSCTL_BASE_ADDR)->ext_pwm_en = cmd;
    }
}

void ADT_StartTimer(timerconfig_t *timerConfig)
{
    timerConfig->timer->cfg |= 0x01;
}

void ADT_StopTimer(timerconfig_t *timerConfig)
{
    timerConfig->timer->cfg &= ~0x01;
}

/**
******************************************************************************
    ** \brief  set four match point PWM out
    ** @param  timerConfig:timerÅäÖÃ½á¹¹Ìå
        retval void
******************************************************************************/
void SetTimer(timerconfig_t *timerConfig)
{
	/*
    if(timerConfig->timer == TIMER0)
    {
        GPIO_SetAFMode(GPIOPortA, GPIOPin3, AF0, 0);
    }
    else if(timerConfig->timer == TIMER1)
    {
        GPIO_SetAFMode(GPIOPortA, GPIOPin4, AF0, 0);
    }
    else if(timerConfig->timer == TIMER2)
    {
        GPIO_SetAFMode(GPIOPortA, GPIOPin5, AF0, 0);
    }
    else if(timerConfig->timer == TIMER3)
    {
        if(timerConfig->channel&0x01)
            GPIO_SetAFMode(GPIOPortC, GPIOPin3, AF2, 0);
        if(timerConfig->channel&0x02)
            GPIO_SetAFMode(GPIOPortC, GPIOPin4, AF2, 0);
    }
    else if(timerConfig->timer == TIMER4)
    {
        if(timerConfig->channel&0x01)
            GPIO_SetAFMode(GPIOPortA, GPIOPin0, AF1, 0);
        if(timerConfig->channel&0x02)
            GPIO_SetAFMode(GPIOPortA, GPIOPin1, AF1, 0);
    }
    else if(timerConfig->timer == TIMER5)
    {
        if(timerConfig->channel&0x01)
            GPIO_SetAFMode(GPIOPortD, GPIOPin0, AF0, 0);
        if(timerConfig->channel&0x02)
            GPIO_SetAFMode(GPIOPortD, GPIOPin1, AF0, 0);
    }
    else
    {
        printf("error timer!");
        return;
    }
	*/
	
    timerConfig->timer->prescale     = timerConfig->prescale;
    timerConfig->timer->period       = timerConfig->period;
    timerConfig->timer->repeat_value = timerConfig->repeatVal;


    timerConfig->timer->matcha_op   = timerConfig->matchAOp;//0x924;//100 100 100 100
    timerConfig->timer->matcha0_val = timerConfig->matchA0;
    timerConfig->timer->matcha1_val = timerConfig->matchA1;
    timerConfig->timer->matcha2_val = timerConfig->matchA2;
    timerConfig->timer->matcha3_val = timerConfig->matchA3;

    timerConfig->timer->matchb_op   = timerConfig->matchBOp;//0x924;//100 100 100 100
    timerConfig->timer->matchb0_val = timerConfig->matchB0;
    timerConfig->timer->matchb1_val = timerConfig->matchB1;
    timerConfig->timer->matchb2_val = timerConfig->matchB2;
    timerConfig->timer->matchb3_val = timerConfig->matchB3;
    timerConfig->timer->delta       = timerConfig->delta;


    timerConfig->timer->cfg = ((timerConfig->mode&0x7)<<1)|((timerConfig->wave&0x1)<<4)|((timerConfig->matchnum&0x3)<<5)|((timerConfig->ece&0x1)<<7);
}

