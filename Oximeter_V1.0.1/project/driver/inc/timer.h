/**
 *****************************************************************************
 * @brief   Tiemr driver header.
 *
 * @file   timer.h
 * @author
 * @date   28/JUN/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "tcasxxx.h"

  /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
 extern "C"
 {
#endif

 /******************************************************************************* 
  * Definitions 
  ******************************************************************************/
 
 /* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */



 typedef struct _timer_config
 {
    T_TIMER* timer;     /* 寄存器基地址 */
    uint8_t  mode;
                        /*
                        000：简单计时器
                        001：输入脉冲计数
                        010：输入采样
                        011：输出比较
                        100：单脉冲模式
                        101：独立PWM模式
                        110：互补PWM模式
                        111：带死区的互补PWM模式
                        只有高级定时器支持 101/110/111 模式
                        */
    uint8_t  wave;
                        /*
                        0：边缘对齐模式
                        1：中心对齐模式
                        */
    uint8_t  matchnum;
                        /*
                        00：match point 0 被使用
                        01：match point 0-1被使用
                        10：match point 0-2被使用
                        11：match point 0-3被使用
                        */
    uint8_t  ece;
                        /*
                        0：使用bit0来使能gpt
                        1：使用外部寄存器来使能gpt，只有在高级定时器中有效
                        */
    uint8_t  channel;    /* bit0 = 1表示使用通道A         bit1 = 1表示使用通道B 其他位无效 */
                        
    uint32_t prescale;  /* 定时器分频系数 */
    uint32_t period;    /* 计数周期 */
    uint32_t repeatVal;
                        /*
                        非单脉冲模式下，设置为0；单脉冲模式下，波形在达到repeat val次数时中止。
                        它不会影响timer的周期，但是影响中断发生时间，中断在第repeat val个match点产生
                        */
    uint16_t matchAOp;
                        /*
                        11：9表示MATCHx3_OP       8：6表示MATCHx2_OP      5：3表示MATCHx1_OP      2：0表示MATCHx1_OP
                        MATCHxx_OP 000:当cnt = matcha3_val时，输出高电平
                        001:当cnt = matcha3_val时，输出低电平
                        010:当cnt = matcha3_val时，翻转电平
                        011:无输出
                        100：当cnt=0，输出低电平，当cnt = matcha3_val时，翻转电平；
                        101：当cnt=0；输出高电平，当cnt = matcha3_val时，翻转电平
                        110：输出高电平
                        111：输出低电平
                        仅高级定时器有效
                        */
    uint32_t matchA0;   /* 匹配值0 */
    uint32_t matchA1;   /* 匹配值1 */
    uint32_t matchA2;   /* 匹配值2 */
    uint32_t matchA3;   /* 匹配值3 */
    
    uint16_t matchBOp;
    uint32_t matchB0;
    uint32_t matchB1;
    uint32_t matchB2;
    uint32_t matchB3;
    int32_t  delta;
 }timerconfig_t;



 /******************************************************************************* 
  * API 
  ******************************************************************************/
 
 /* 外部函数(可加 extern 修饰)的声明 */


void Hal_Timer0Handler(void);
void Hal_Timer1Handler(void);
void Hal_Timer2Handler(void);
void Hal_Timer3Handler(void);
void Hal_Timer4Handler(void);
void Hal_Timer5Handler(void);
void Hal_LptimerHandler(void);

void ADT_StartTimer(timerconfig_t *timerConfig);
void ADT_StopTimer(timerconfig_t *timerConfig);

void SetTimerIsr(T_TIMER* Timer);
void CalExtPulse(T_TIMER* Timer);
void SoftwareForceBK(T_TIMER* advTimer);
void SyncStartStop(uint8_t mode);
void SetTimer(timerconfig_t *timerConfig);

#ifdef __cplusplus
}
#endif
#endif /* __TIMER_H__ */

