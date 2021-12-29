/**
*****************************************************************************
* @brief
*
* @file     SegDriver.h
* @version
* @author
* @date     2021-12-27
*****************************************************************************
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* Copyright (c) 2022 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/

#ifndef __SEG_DRIVER_H__
#define __SEG_DRVIER_H__

#include "pt.h"
#include "pt-sem.h"
#include "DataType.h"
#include "gpio.h"
#include "DisplayDriver.h"

/**
* @brief �����ģ����߷���
*/

#define LineOnePort GPIOPortC
#define LineOnePin GPIOPin1

#define LineTwoPort GPIOPortB
#define LineTwoPin GPIOPin3

#define LineThrPort GPIOPortA
#define LineThrPin GPIOPin1

#define LineForPort GPIOPortB
#define LineForPin GPIOPin5

#define LineFivPort GPIOPortB
#define LineFivPin GPIOPin7

#define LineSixPort GPIOPortB
#define LineSixPin GPIOPin6

#define LineSevPort GPIOPortA
#define LineSevPin GPIOPin0

#define LineEitPort GPIOPortC
#define LineEitPin GPIOPin0

#define LineNiPort GPIOPortB
#define LineNiPin GPIOPin2

/**
* @brief ���������
*/

typedef enum
{
    SEGOne,
    SEGTwo,
    SEGThr,
    SEGFor,
    SEGFiv,
    SEGSix,
    SEGSev,
    SEGEigt,
    SEGNine,
} SEG_INDEX_T;

/**
* @brief ״̬��ö��
*/

typedef enum
{
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
} SEG_STATE_T;

/**
* @brief �������ʾ�ӿ�
*/

void segShow(SEG_INDEX_T index, uint8_t num, BOOL isShowDot);

/**
* @brief ״̬����ʾ�ӿ�
*/

void stateShow(SEG_STATE_T state, BOOL isShowHrtSign);

/**
* @brief �������ʾ�߳�����
*/

void segSetUp(void);

/**
* @brief ģ����ʾ�߳�ʵ��
*/

uint8_t seg_show_thread(struct pt *pt);

/**
* @brief �ⲿ����
*/

extern struct pt PT_SpO2;
extern struct pt_sem SEM_POLL;
extern SEG_STATE_T state;
extern BOOL isShowHeartSign;

#endif
