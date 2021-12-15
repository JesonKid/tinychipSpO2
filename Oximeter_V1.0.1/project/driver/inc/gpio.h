/**
 *****************************************************************************
 * @brief   gpio head file
 *
 * @file   gpio.h
 * @author
 * @date   28/JULy/2020
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
#ifndef __GPIO_H__
#define __GPIO_H__

#include "tcasxxx.h"
#include "interrupt.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************
 ** \brief GPIO PORT enum
 ******************************************************************************/
typedef enum GPIO_PORT_ENUM
{
    GPIOPortA = 1U,                 ///< GPIO PORT A
    GPIOPortB = 2U,                 ///< GPIO PORT B
    GPIOPortC = 3U,                 ///< GPIO PORT C
    GPIOPortD = 4U,                 ///< GPIO PORT D
} GPIO_PORT_ENUM_E;

/**
 *******************************************************************************
 ** \brief GPIO PIN enum
 ******************************************************************************/
typedef enum GPIO_PIN_ENUM
{
    GPIOPin0  = 0u,                 ///< GPIO PIN0
    GPIOPin1  = 1u,                 ///< GPIO PIN1
    GPIOPin2  = 2u,                 ///< GPIO PIN2
    GPIOPin3  = 3u,                 ///< GPIO PIN3
    GPIOPin4  = 4u,                 ///< GPIO PIN4
    GPIOPin5  = 5u,                 ///< GPIO PIN5
    GPIOPin6  = 6u,                 ///< GPIO PIN6
    GPIOPin7  = 7u,                 ///< GPIO PIN7

} GPIO_PIN_ENUM_E;

typedef enum GPIO_AF_ENUM
{
    AF0 = 1u,
    AF1 = 2u,
    AF2 = 3u,
    AF3 = 4u,
    AF_ANALOG = 5u,
} GPIO_AF_ENUM_E;

typedef struct
{
    T_GPIO *gpio ;
    int offset ;
} GPIO_RegisterMapInfo_T;

typedef enum GPIO_INDEX_ENUM
{

    GPIO_INDEX_0 = 0,
    GPIO_INDEX_1,
    GPIO_INDEX_2,
    GPIO_INDEX_3,
    GPIO_INDEX_4,
    GPIO_INDEX_5,
    GPIO_INDEX_6,
    GPIO_INDEX_7,
    GPIO_INDEX_8,
    GPIO_INDEX_9,
    GPIO_INDEX_10,
    GPIO_INDEX_11,
    GPIO_INDEX_12,
    GPIO_INDEX_13,
    GPIO_INDEX_14,
    GPIO_INDEX_15,
    GPIO_INDEX_16,
    GPIO_INDEX_17,
    GPIO_INDEX_18,
    GPIO_INDEX_19,
    GPIO_INDEX_20,
    GPIO_INDEX_21,
    GPIO_INDEX_22,
    GPIO_INDEX_23,
    GPIO_INDEX_24,
    GPIO_INDEX_25,
    GPIO_INDEX_MAX
} GPIO_INDEX_E;



typedef enum GPIO_MODE
{
    GPIO_INPUT_MODE  = 1,
    GPIO_OUTPUT_MODE
} GPIO_MODE_E;

typedef enum
{
    GPIO_PIN_OPEN_SOURCE     = 1,           // 开源
    GPIO_PIN_OPEN_DRAIN,                    // 开漏
    GPIO_PIN_PUSH_PULL,                     // 推挽
    GPIO_PIN_HIGH_IMPEDANCE,                // 高阻
    GPIO_PIN_MODE_MAX
} GPIO_EXT_MODE_E;

typedef enum
{
    GPIO_IT_ENABLE         = 0x00,
    GPIO_IT_FALLING_EDGE   = 0x01,
    GPIO_IT_RAISING_EDGE   = 0x02,
    GPIO_IT_LOW_LEVEL      = 0x03,
    GPIO_IT_HIGH_LEVEL     = 0x04,
    GPIO_IT_DISABLE        = 0x10,
    GPIO_IT_MODE_MAX
} GPIO_IT_MODE_E;

typedef enum
{
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_PULL_NONE
} GPIO_PUSH_PULL_MODE_E;

typedef enum
{
    GPIO_20MA_PA00,
    GPIO_20MA_PA01,
    GPIO_20MA_PB00,
    GPIO_20MA_PB01,
    GPIO_20MA_PB02,
    GPIO_20MA_PB03
} GPIO_20MA_MODE_E;


typedef struct
{
    uint8_t  GPIO_Port;     /*!< Specifies the GPIO Port to be configured.
                                      This parameter can be any value of @ref GPIO_PORT_ENUM_E */

    uint8_t  GPIO_Pin;      /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_PIN_ENUM_E */

    uint8_t  GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */

    uint8_t  GPIO_Ext_Mode; /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIO_EXT_MODE_E */

    uint8_t  GPIO_Pull_Mode;/*!< Specifies the operating mode for the selected pins.
                                            This parameter can be a value of @ref GPIO_PUSH_PULL_MODE_E */
} GPIO_Init_t;




void GPIO_Config(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin,GPIO_MODE_E mode);
void GPIO_SetBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_ClearBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
uint8_t GPIO_GetBit(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_ToggleBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
uint8_t GPIO_SetAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin, GPIO_AF_ENUM_E Af);
uint8_t GPIO_GetAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_ClrIsrStatus(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void Hal_GPIOIsrHandler(void);
uint8_t GPIO_ClrAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_SwitchOffIe(void);


void GPIO_Init(GPIO_Init_t * pInitSt);
void GPIO_ITConfig(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin, GPIO_IT_MODE_E mode, ISRFunc_T vector);
uint8_t GPIO_GetITStatus(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_ClearITBit(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin);
void GPIO_ExtModeConfig(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin,GPIO_EXT_MODE_E Ext_Mode,GPIO_PUSH_PULL_MODE_E Pull_Mode);
void GPIO_20mA_Mode(GPIO_20MA_MODE_E PortPin,uint8_t En);

#ifdef __cplusplus
}
#endif
#endif /* __GPIO_H__ */
#
