/**
 *****************************************************************************
 * @brief   divider source file.
 *
 * @file   divider.c
 * @author hc
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
#include "system_tcasxxx.h"
#include "delay.h"
#include "divider.h"

/*******************************************************************************
* Definitions：私有(仅本源文件内使用)宏、枚举、结构体的定义
******************************************************************************/

/*******************************************************************************
* Variables：所有全局变量(外部，静态，常量，指针)的定义
******************************************************************************/

/*******************************************************************************
* Prototypes：内部函数(即 static 修饰)的声明
******************************************************************************/

/*******************************************************************************
* Code：所有函数(外部，内部)的定义
******************************************************************************/


/********************************************************
@brief  除法函数
@param  divider:  被除数
        divisor： 除数
        quotient：商
        reminder：余数
@retval void
*********************************************************/
void DividerFuction(uint32_t dividend,uint32_t divisor,uint32_t *quotient,uint32_t *reminder)
{
    DIVIDER->dividend = dividend;
    DIVIDER->divisor = divisor;
   // DelayForCommandExeDone();
    *quotient = DIVIDER->quotient;
    *reminder = DIVIDER->reminder;
}




