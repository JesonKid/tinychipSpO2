/**
*****************************************************************************
* @brief
*
* @file     SegDriver.c
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

#include "SegDriver.h"
#include "DisplayDriver.h"

/**
* @brief 共阳极数码管码值表
*/

uint8_t anode_table[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

/**
* @brief 显示线程
*/

struct pt PT_SpO2;

/**
* @brief 数码管轮显信标
*/

struct pt_sem SEM_POLL;

/**
* @brief 模组状态条枚举
*/

SEG_STATE_T state;

/**
* @brief 模组是否显示心形字样
*/

BOOL isShowHeartSign = TRUE;

/**
* @brief 数码管模组底层初始化
*/

static void lowLevelInit(void)
{
    GPIO_ClrAFMode(LineOnePort, LineOnePin);
    GPIO_ExtModeConfig(LineOnePort, LineOnePin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineTwoPort, LineTwoPin);
    GPIO_ExtModeConfig(LineTwoPort, LineTwoPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineThrPort, LineThrPin);
    GPIO_ExtModeConfig(LineThrPort, LineThrPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);

    GPIO_SetAFMode(LineForPort, LineForPin, AF0);
    GPIO_ExtModeConfig(LineForPort, LineForPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineFivPort, LineFivPin);
    GPIO_ExtModeConfig(LineFivPort, LineFivPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineSixPort, LineSixPin);
    GPIO_ExtModeConfig(LineSixPort, LineSixPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineEitPort, LineEitPin);
    GPIO_ExtModeConfig(LineEitPort, LineEitPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);

    GPIO_ClrAFMode(LineNiPort, LineNiPin);
    GPIO_ExtModeConfig(LineNiPort, LineNiPin, GPIO_PIN_PUSH_PULL, GPIO_PULL_NONE);
    GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
}

/**
* @brief 显示线程启动
*/

void segSetUp(void)
{
    lowLevelInit();
    PT_SEM_INIT(&SEM_POLL, 0);
    PT_INIT(&PT_SpO2);
}

/**
* @brief 1#数码管显示数据加载
*/

static void seg1_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 1#数码管开启显示
*/

static void seg1_show()
{
    GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineOnePort, LineOnePin);
}

/**
* @brief 2#数码管显示数据加载
*/

static void seg2_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 2#数码管开启显示
*/

static void seg2_show()
{
    GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineTwoPort, LineTwoPin);
}

/**
* @brief 3#数码管显示数据加载
*/

static void seg3_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 3#数码管开启显示
*/

static void seg3_show()
{
    GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineThrPort, LineThrPin);
}

/**
* @brief 4#数码管显示数据加载
*/

static void seg4_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 4#数码管开启显示
*/

static void seg4_show()
{
    GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineForPort, LineForPin);
}

/**
* @brief 5#数码管显示数据加载
*/

static void seg5_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 5#数码管开启显示
*/

static void seg5_show()
{
    GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineFivPort, LineFivPin);
}

/**
* @brief 6#数码管显示数据加载
*/

static void seg6_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 6#数码管开启显示
*/

static void seg6_show()
{
    GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineSixPort, LineSixPin);
}

/**
* @brief 7#数码管显示数据加载
*/

static void seg7_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineEitPort, LineEitPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 7#数码管开启显示
*/

static void seg7_show()
{
    GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineSevPort, LineSevPin);
}

/**
* @brief 8#数码管显示数据加载
*/

static void seg8_num_load(uint8_t num, uint8_t poll_index)
{
    switch (poll_index)
    {
    case 0:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineOnePort, LineOnePin);
        }
        break;
    case 1:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineTwoPort, LineTwoPin);
        }
        break;
    case 2:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineThrPort, LineThrPin);
        }
        break;
    case 3:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineForPort, LineForPin);
        }
        break;
    case 4:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineFivPort, LineFivPin);
        }
        break;
    case 5:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSixPort, LineSixPin);
        }
        break;
    case 6:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineSevPort, LineSevPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineSevPort, LineSevPin);
        }
        break;
    case 7:
        if (num >> poll_index & 1)
        {

            GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        }
        else
        {
            GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
            GPIO_ClearBits(LineNiPort, LineNiPin);
        }
        break;
    }
}

/**
* @brief 8#数码管开启显示
*/

static void seg8_show()
{
    GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
    GPIO_SetBits(LineEitPort, LineEitPin);
}

/**
* @brief 数码管显示接口
*/

void segShow(SEG_INDEX_T index, uint8_t num, BOOL isShowDot)
{
    uint8_t res = 0;

    if (num > 0xf)return;
    if (isShowDot)res = anode_table[num] & ~(1 << 7);
    else res = anode_table[num];
    if (num == 0xf)res |= ~(1 << 7);

    switch (index)
    {
    case SEGOne:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg1_num_load(res, i);
        }
        seg1_show();
        break;
    case SEGTwo:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg2_num_load(res, i);
        }
        seg2_show();
        break;
    case SEGThr:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg3_num_load(res, i);
        }
        seg3_show();
        break;
    case SEGFor:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg4_num_load(res, i);
        }
        seg4_show();
        break;
    case SEGFiv:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg5_num_load(res, i);
        }
        seg5_show();
        break;
    case SEGSix:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg6_num_load(res, i);
        }
        seg6_show();
        break;
    case SEGSev:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg7_num_load(res, i);
        }
        seg7_show();
        break;
    case SEGEigt:
        for (uint8_t i = 0; i < 8; i++)
        {
            seg8_num_load(res, i);
        }
        seg8_show();
        break;
    case SEGNine:
        break;

    }
}

/**
* @brief 模组状态条显示接口
*/

void stateShow(SEG_STATE_T state, BOOL isShowHrtSign)
{
    switch (state)
    {
    case LEVEL0:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSevPort, LineSevPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_INPUT_MODE);
        break;
    case LEVEL1:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_INPUT_MODE);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineNiPort, LineNiPin);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineEitPort, LineEitPin);
        break;
    case LEVEL2:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineOnePort, LineOnePin);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_INPUT_MODE);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineNiPort, LineNiPin);
        break;
    case LEVEL3:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineOnePort, LineOnePin);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineTwoPort, LineTwoPin);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_INPUT_MODE);
        GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineNiPort, LineNiPin);
        break;
    case LEVEL4:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineOnePort, LineOnePin);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineTwoPort, LineTwoPin);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineThrPort, LineThrPin);
        GPIO_Config(LineForPort, LineForPin, GPIO_INPUT_MODE);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineNiPort, LineNiPin);
        break;
    case LEVEL5:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineOnePort, LineOnePin);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineTwoPort, LineTwoPin);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineThrPort, LineThrPin);
        GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineForPort, LineForPin);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_INPUT_MODE);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineNiPort, LineNiPin);
        break;
    case LEVEL6:
        GPIO_Config(LineOnePort, LineOnePin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineOnePort, LineOnePin);
        GPIO_Config(LineTwoPort, LineTwoPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineTwoPort, LineTwoPin);
        GPIO_Config(LineThrPort, LineThrPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineThrPort, LineThrPin);
        GPIO_Config(LineForPort, LineForPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineForPort, LineForPin);
        GPIO_Config(LineFivPort, LineFivPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineFivPort, LineFivPin);
        GPIO_Config(LineSixPort, LineSixPin, GPIO_INPUT_MODE);
        GPIO_Config(LineEitPort, LineEitPin, GPIO_INPUT_MODE);
        GPIO_Config(LineNiPort, LineNiPin, GPIO_OUTPUT_MODE);
        GPIO_SetBits(LineNiPort, LineNiPin);
        break;
    }
    if (isShowHrtSign)
    {
        GPIO_Config(LineSixPort, LineSixPin, GPIO_OUTPUT_MODE);
        GPIO_ClearBits(LineSixPort, LineSixPin);
    }
}

/**
* @brief 数码管模组显示线程
*/

uint8_t seg_show_thread(struct pt *pt)
{
    static uint8_t hrd = 0;
    static uint8_t ten = 0;
    static uint8_t one = 0;

    PT_BEGIN(pt);

    ten = gSatParam.sysParam.mSpO2 / 10;
    one = gSatParam.sysParam.mSpO2 % 10;

    PT_SEM_WAIT(pt, &SEM_POLL);
    //等到信号
    //显示十位上的数据
    //清屏
    segShow(SEGOne, 0xf, FALSE);
    segShow(SEGOne, ten, FALSE);
    PT_SEM_WAIT(pt, &SEM_POLL);
    segShow(SEGTwo, 0xf, FALSE);
    segShow(SEGTwo, one, FALSE);

    hrd = gSatParam.sysParam.mPulse / 100;
    ten = (gSatParam.sysParam.mPulse % 100) / 10;
    one = (gSatParam.sysParam.mPulse % 100) % 10;
    PT_SEM_WAIT(pt, &SEM_POLL);
    //显示百位上的数据
    if (hrd != 0)
    {
        segShow(SEGThr, 0xf, FALSE);
        segShow(SEGThr, hrd, TRUE);
    }
    else
    {
        //为0不显示
        segShow(SEGThr, 0xf, FALSE);
        segShow(SEGThr, 0xf, TRUE);
    }
    //显示十位上的数据
    PT_SEM_WAIT(pt, &SEM_POLL);
    segShow(SEGFor, 0xf, FALSE);
    segShow(SEGFor, ten, TRUE);
    //显示个位上的数据
    PT_SEM_WAIT(pt, &SEM_POLL);
    segShow(SEGFiv, 0xf, FALSE);
    segShow(SEGFiv, one, TRUE);

    hrd = gSatParam.sysParam.mPerfuse / 100;
    ten = (gSatParam.sysParam.mPerfuse % 100) / 10;
    one = (gSatParam.sysParam.mPerfuse % 100) % 10;
    //显示百位上的数据
    if (hrd != 0)
    {
        PT_SEM_WAIT(pt, &SEM_POLL);
        segShow(SEGSix, 0xf, FALSE);
        segShow(SEGSix, hrd, FALSE);
    }
    //显示十位上的数据
    PT_SEM_WAIT(pt, &SEM_POLL);
    segShow(SEGSev, 0xf, FALSE);
    segShow(SEGSev, ten, FALSE);
    //显示个位上的数据
    PT_SEM_WAIT(pt, &SEM_POLL);
    segShow(SEGEigt, 0xf, FALSE);
    segShow(SEGEigt, one, TRUE);
    PT_SEM_WAIT(pt, &SEM_POLL);
    stateShow(state, isShowHeartSign);
    stateShow(state, isShowHeartSign);
    PT_RESTART(pt);
    PT_END(pt);
}
