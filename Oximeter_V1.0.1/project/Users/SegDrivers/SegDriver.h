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
* @brief 数码管模组口线分配
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
* @brief 数码管索引
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
* @brief 状态条枚举
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
* @brief 数码管显示接口
*/

void segShow(SEG_INDEX_T index, uint8_t num, BOOL isShowDot);

/**
* @brief 状态条显示接口
*/

void stateShow(SEG_STATE_T state, BOOL isShowHrtSign);

/**
* @brief 数码管显示线程启动
*/

void segSetUp(void);

/**
* @brief 模组显示线程实体
*/

uint8_t seg_show_thread(struct pt *pt);

/**
* @brief 外部声明
*/

extern struct pt PT_SpO2;
extern struct pt_sem SEM_POLL;
extern SEG_STATE_T state;
extern BOOL isShowHeartSign;

#endif
