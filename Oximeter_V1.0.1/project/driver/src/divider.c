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
* Definitions��˽��(����Դ�ļ���ʹ��)�ꡢö�١��ṹ��Ķ���
******************************************************************************/

/*******************************************************************************
* Variables������ȫ�ֱ���(�ⲿ����̬��������ָ��)�Ķ���
******************************************************************************/

/*******************************************************************************
* Prototypes���ڲ�����(�� static ����)������
******************************************************************************/

/*******************************************************************************
* Code�����к���(�ⲿ���ڲ�)�Ķ���
******************************************************************************/


/********************************************************
@brief  ��������
@param  divider:  ������
        divisor�� ����
        quotient����
        reminder������
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




