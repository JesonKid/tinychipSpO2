/**
 *****************************************************************************
 * @brief   gpio source file.
 *
 * @file   gpio.c
 * @author
 * @date   28/JULY/2020
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
#include "gpio.h"
#include "interrupt.h"


/**
 *******************************************************************************
** \brief GPIO Pinmux adr map
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[u32] Pinmux adr
           [u32] False
 ******************************************************************************/
static tc_pinmux_config_field_t * GPIO_PinMuxAdrMap(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    tc_pinmux_config_field_t * pinMuxAdr = NULL;
    switch(Port)
    {
        case GPIOPortA:
            if(Pin <= GPIOPin1)
            {
                pinMuxAdr = (tc_pinmux_config_field_t *)(PINMUXELSE_BASE_ADDR + (4*Pin));
            }
            else if(Pin <= GPIOPin7)
            {
                pinMuxAdr = (tc_pinmux_config_field_t *)(PINMUX2_7_BASE_ADDR + (4*(Pin- GPIOPin2)));
            }
            else
            {
                return NULL;
            }
            break;
        case GPIOPortB:
            if(Pin <= GPIOPin7)
            {
                pinMuxAdr = (tc_pinmux_config_field_t *)(PINMUXELSE_BASE_ADDR + 0x20 + (4*Pin));
            }
            else
            {
                return NULL;
            }
            break;
        case GPIOPortC:
            if(Pin <= GPIOPin7)
            {
                pinMuxAdr = (tc_pinmux_config_field_t *)(PINMUXELSE_BASE_ADDR + 0x40 + (4*Pin));
            }
            else
            {
                return NULL;
            }
            break;
        case GPIOPortD:
            if(Pin <= GPIOPin1)
            {
                pinMuxAdr = (tc_pinmux_config_field_t *)(PINMUXELSE_BASE_ADDR + 0x60 + (4*Pin));
            }
            else
            {
                return NULL;
            }
            break;
        default:
            break;
    }
    return pinMuxAdr;
}

/**
 *******************************************************************************
** \brief GPIO Pin af set
 **
 ** \param  [u8]Port: PortA,PortB,PortC,PortD
 ** \param  [u8]Pin:  1-7
 ** \param  [u8]Af:  AF0-AF3,数字功能；AF_ANALOG：模拟功能
 ** \retval [u8]TRUE
 **         [u8]FALSE

 ******************************************************************************/
uint8_t GPIO_SetAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin, GPIO_AF_ENUM_E Af)
{
    tc_pinmux_config_field_t * pinMuxCfg = NULL;
    pinMuxCfg = GPIO_PinMuxAdrMap(Port,Pin);

    if(pinMuxCfg != NULL)
    {
        pinMuxCfg->sw_src_sel = Af;
        if(Af == AF_ANALOG)
        {
            pinMuxCfg->sw_ie = DISABLE;
        }
        else
        {
            pinMuxCfg->sw_ie = ENABLE;
        }
        return TRUE;
    }
    return FALSE;
}


/**
 *******************************************************************************
** \brief GPIO clear Alternative function
 **
 ** \param [in]  enPort    IO Port
 ** \param [in]  enPin     IO Port Pin
 ** \retval TRUE
 **         FALSE
 ******************************************************************************/
uint8_t GPIO_ClrAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    tc_pinmux_config_field_t * pinMuxCfg = NULL;
    pinMuxCfg = GPIO_PinMuxAdrMap(Port,Pin);

    if(pinMuxCfg != NULL)
    {
        pinMuxCfg->sw_src_sel = 0;
        return TRUE;
    }

    return FALSE;
}

/**
 *******************************************************************************
** \brief GPIO Get Alternative mode
 **
 ** \param [in]  enPort    IO Port
 ** \param [in]  enPin     IO Port Pin
 ** \retval n/a
 **         FALSE
 ******************************************************************************/
uint8_t GPIO_GetAFMode(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    tc_pinmux_config_field_t * pinMuxCfg = NULL;
    pinMuxCfg = GPIO_PinMuxAdrMap(Port,Pin);

    if(pinMuxCfg != NULL)
    {
        return pinMuxCfg->sw_src_sel;
    }

    return 0xFF;

}


/**
 *******************************************************************************
** \brief GPIO  Register Map
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[u32] GPIO_RegisterMapInfo_T
           [u32] False
 ******************************************************************************/
static GPIO_RegisterMapInfo_T GPIO_RegisterMap(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;
    GPIO_RegisterMapInfo.gpio = NULL;
    GPIO_RegisterMapInfo.offset = 0;

    switch(Port)
    {
        case GPIOPortA:
            if(Pin <= GPIOPin1)
            {
                GPIO_RegisterMapInfo.gpio = GPIO_PIN_0_15;
                GPIO_RegisterMapInfo.offset = Pin;
            }
            else if(Pin <= GPIOPin7)
            {
                GPIO_RegisterMapInfo.gpio = GPIO_PIN_2_7;
                GPIO_RegisterMapInfo.offset = Pin - GPIOPin2;
            }
            break;
        case GPIOPortB:
            if(Pin <= GPIOPin7)
            {
                GPIO_RegisterMapInfo.gpio = GPIO_PIN_0_15;
                GPIO_RegisterMapInfo.offset = 8 + Pin;
            }
            break;
        case GPIOPortC:
            if(Pin <= GPIOPin7)
            {
                GPIO_RegisterMapInfo.gpio = GPIO_PIN_16_25;
                GPIO_RegisterMapInfo.offset = Pin;
            }
            break;
        case GPIOPortD:
            if(Pin <= GPIOPin1)
            {
                GPIO_RegisterMapInfo.gpio = GPIO_PIN_16_25;
                GPIO_RegisterMapInfo.offset = 8+Pin;
            }
            break;
        default:
            break;
    }
    return GPIO_RegisterMapInfo;
}

/**
 *******************************************************************************
** \brief GPIO  Config
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \param [u8]  mode: GPIO MODE
 ** \retval[void]
 ******************************************************************************/
void GPIO_Config(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin,GPIO_MODE_E mode)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(mode == GPIO_OUTPUT_MODE)
    {
        GPIO_RegisterMapInfo.gpio->out_en_set = (0x1<<GPIO_RegisterMapInfo.offset);
        return;
    }

    if(mode == GPIO_INPUT_MODE)
    {
        GPIO_RegisterMapInfo.gpio->out_en_clr = (0x1<<GPIO_RegisterMapInfo.offset);
        return;
    }
}

/**
 *******************************************************************************
** \brief GPIO Pin set
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[void] void
 ******************************************************************************/
void GPIO_SetBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(GPIO_RegisterMapInfo.gpio != NULL)
    {
        GPIO_RegisterMapInfo.gpio->data_out = (GPIO_RegisterMapInfo.gpio->data_out | (0x1<<GPIO_RegisterMapInfo.offset));
    }
}

/**
 *******************************************************************************
** \brief GPIO Pin Clear
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[void] void
 ******************************************************************************/

void GPIO_ClearBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(GPIO_RegisterMapInfo.gpio!=NULL)
    {
        GPIO_RegisterMapInfo.gpio->data_out = (GPIO_RegisterMapInfo.gpio->data_out & ~(0x1<<GPIO_RegisterMapInfo.offset));
    }
}

/**
 *******************************************************************************
** \brief GPIO Pin Read
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[u8]  val : 1 high val
                          0 low val
 ******************************************************************************/
uint8_t GPIO_GetBit(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    return (uint8_t)((GPIO_RegisterMapInfo.gpio->data >> GPIO_RegisterMapInfo.offset) & 0x1);
}


/**
 *******************************************************************************
** \brief GPIO Pin toggle
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[u8] void
 ******************************************************************************/

void GPIO_ToggleBits(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(GPIO_RegisterMapInfo.gpio != NULL)
    {
        if(GPIO_RegisterMapInfo.gpio->data_out & (0x01 << GPIO_RegisterMapInfo.offset))     //if the output is high
        {
            GPIO_RegisterMapInfo.gpio->data_out &= ~(0x01 << GPIO_RegisterMapInfo.offset);  //set output low
        }
        else
        {
            GPIO_RegisterMapInfo.gpio->data_out |= (0x01 << GPIO_RegisterMapInfo.offset);   //set output high
        }
    }
}

void GPIO_SwitchOffIe(void)
{
    PINMUXELSE->PA_00 &= ~(0X01 << 19);
    PINMUXELSE->PA_01 &= ~(0X01 << 19);
    PINMUXELSE->PB_00 &= ~(0X01 << 19);
    PINMUXELSE->PB_01 &= ~(0X01 << 19);
    PINMUXELSE->PB_02 &= ~(0X01 << 19);
    PINMUXELSE->PB_03 &= ~(0X01 << 19);
    PINMUXELSE->PB_04 &= ~(0X01 << 19);
    PINMUXELSE->PB_05 &= ~(0X01 << 19);
    PINMUXELSE->PB_06 &= ~(0X01 << 19);
    PINMUXELSE->PB_07 &= ~(0X01 << 19);
    PINMUXELSE->PC_00 &= ~(0X01 << 19);
    PINMUXELSE->PC_01 &= ~(0X01 << 19);
    PINMUXELSE->PC_02 &= ~(0X01 << 19);
    PINMUXELSE->PC_03 &= ~(0X01 << 19);
    PINMUXELSE->PC_04 &= ~(0X01 << 19);
    PINMUXELSE->PC_05 &= ~(0X01 << 19);
    PINMUXELSE->PC_06 &= ~(0X01 << 19);
    PINMUXELSE->PC_07 &= ~(0X01 << 19);

    PINMUX2_7->PA_02 &= ~(0X01 << 19);
    PINMUX2_7->PA_03 &= ~(0X01 << 19);
    PINMUX2_7->PA_04 &= ~(0X01 << 19);
    PINMUX2_7->PA_05 &= ~(0X01 << 19);
    PINMUX2_7->PA_06 &= ~(0X01 << 19);
    PINMUX2_7->PA_07 &= ~(0X01 << 19);
}

/**
 *******************************************************************************
** \brief GPIO  GPIO_Init
 **               confingure the gpio interupt
 ** \param [u8]  GPIO_Init_t
                   Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin:  1-19
 ** \param [u8]  GPIO_Mode:      GPIO_INPUT_MODE  ,
                                 GPIO_OUTPUT_MODE,

 ** \param [u8]  GPIO_Ext_Mode:     GPIO_PIN_OPEN_SOURCE,
                                    GPIO_PIN_OPEN_DRAIN,
                                    GPIO_PIN_PULL_UP,
                                    GPIO_PIN_PULL_DOWN,
                                    GPIO_PIN_HIGH_IMPDENCE,
                                    GPIO_PIN_MODE_MAX
 ** \retval[void]
 ******************************************************************************/
void GPIO_Init(GPIO_Init_t * pInitSt)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;
    tc_pinmux_config_field_t * pinConfig = NULL;


    GPIO_RegisterMapInfo = GPIO_RegisterMap((GPIO_PORT_ENUM_E)pInitSt->GPIO_Port,(GPIO_PIN_ENUM_E)pInitSt->GPIO_Pin);
    pinConfig = (tc_pinmux_config_field_t *)GPIO_PinMuxAdrMap((GPIO_PORT_ENUM_E)pInitSt->GPIO_Port,(GPIO_PIN_ENUM_E)pInitSt->GPIO_Pin);

    /* GPIO Direction Setting */
    if((GPIO_RegisterMapInfo.gpio != NULL)&&(pinConfig != NULL))
    {
        if(pInitSt->GPIO_Mode == GPIO_OUTPUT_MODE)
        {
            GPIO_RegisterMapInfo.gpio->out_en_set = (0x1<<GPIO_RegisterMapInfo.offset);
        }
        else if(pInitSt->GPIO_Mode == GPIO_INPUT_MODE)
        {
            GPIO_RegisterMapInfo.gpio->out_en_clr = (0x1<<GPIO_RegisterMapInfo.offset);
        }
        else
        {

        }

        if(pInitSt->GPIO_Ext_Mode == GPIO_PIN_OPEN_SOURCE)
        {
            pinConfig->open_source = 1;
            pinConfig->open_drain  = 0;
        }

        if(pInitSt->GPIO_Ext_Mode == GPIO_PIN_OPEN_DRAIN)
        {
            pinConfig->open_source = 0;
            pinConfig->open_drain  = 1;
        }

        if(pInitSt->GPIO_Ext_Mode == GPIO_PIN_PUSH_PULL)
        {
            pinConfig->open_source = 0;
            pinConfig->open_drain  = 0;
        }

        if(pInitSt->GPIO_Pull_Mode == GPIO_PULL_UP)
        {
            pinConfig->sw_pull_sel = 1;
            pinConfig->sw_pullup   = 1;
            pinConfig->sw_pulldown  = 0;
        }

        if(pInitSt->GPIO_Pull_Mode == GPIO_PULL_DOWN)
        {
            pinConfig->sw_pull_sel = 1;
            pinConfig->sw_pullup   = 0;
            pinConfig->sw_pulldown  = 1;
        }

        if(pInitSt->GPIO_Pull_Mode == GPIO_PULL_NONE)
        {
            pinConfig->sw_pull_sel = 0;
            pinConfig->sw_pullup   = 0;
            pinConfig->sw_pulldown  = 0;
        }

        if(pInitSt->GPIO_Ext_Mode == GPIO_PIN_HIGH_IMPEDANCE)
        {
            GPIO_RegisterMapInfo.gpio->out_en_set &= ~(0x1<<GPIO_RegisterMapInfo.offset);
            GPIO_RegisterMapInfo.gpio->out_en_clr &= ~(0x1<<GPIO_RegisterMapInfo.offset);
        }
    }
}


/**
 *******************************************************************************
** \brief GPIO  GPIO_ITConfig
 **               confingure the gpio interupt
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \param [u8]  mode: IT Mode
 ** \param [u8] ISRFunc_T vector
 ** \retval[void]
 ******************************************************************************/
void GPIO_ITConfig(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin, GPIO_IT_MODE_E mode, ISRFunc_T vector)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(GPIO_RegisterMapInfo.gpio != NULL)
    {
        if((mode & 0xF0) == 0x00)
        {
            if(mode == GPIO_IT_FALLING_EDGE)
            {
                GPIO_RegisterMapInfo.gpio->int_type_set |= (1<<GPIO_RegisterMapInfo.offset);
                GPIO_RegisterMapInfo.gpio->int_pol_clr |= (1<<GPIO_RegisterMapInfo.offset);
            }
            else if(mode == GPIO_IT_RAISING_EDGE)
            {
                GPIO_RegisterMapInfo.gpio->int_type_set |= (1<<GPIO_RegisterMapInfo.offset);
                GPIO_RegisterMapInfo.gpio->int_pol_set |= (1<<GPIO_RegisterMapInfo.offset);
            }
            else if(mode == GPIO_IT_LOW_LEVEL)
            {
                GPIO_RegisterMapInfo.gpio->int_type_clr |= (1<<GPIO_RegisterMapInfo.offset);
                GPIO_RegisterMapInfo.gpio->int_pol_clr |= (1<<GPIO_RegisterMapInfo.offset);
            }
            else if(mode == GPIO_IT_HIGH_LEVEL)
            {
                GPIO_RegisterMapInfo.gpio->int_type_clr |= (0x1<<GPIO_RegisterMapInfo.offset);
                GPIO_RegisterMapInfo.gpio->int_pol_set |= (0x1<<GPIO_RegisterMapInfo.offset);
            }

            GPIO_RegisterMapInfo.gpio->int_en_set |= (0x1<<GPIO_RegisterMapInfo.offset);
            if(GPIO_RegisterMapInfo.gpio == GPIO_PIN_2_7)
            {
                XIC_EnableIRQ(AIC_Aon_Gpio_IRQn, vector);
            }
            else
            {
                XIC_EnableIRQ(AIC_Top_Gpio_IRQn, vector);
            }
        }
        else
        {
            GPIO_RegisterMapInfo.gpio->int_en_clr |= (0x1<<GPIO_RegisterMapInfo.offset);

            if(GPIO_RegisterMapInfo.gpio == GPIO_PIN_2_7)
            {
                XIC_DisableIRQ(AIC_Aon_Gpio_IRQn);
            }
            else
            {
                XIC_DisableIRQ(AIC_Top_Gpio_IRQn);
            }
        }
    }
}

/**
 *******************************************************************************
** \brief GPIO  GPIO_GetITStatus
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[void] 1 IRQ Bit is set
                    0 IRQ Bit not set
 ******************************************************************************/
uint8_t GPIO_GetITStatus(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    return (GPIO_RegisterMapInfo.gpio->int_status & (1<<GPIO_RegisterMapInfo.offset)?1:0);
}


/**
 *******************************************************************************
** \brief GPIO  Clearr Isr Status
 **
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \retval[void]
 ******************************************************************************/
void GPIO_ClearITBit(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    if(GPIO_RegisterMapInfo.gpio != NULL)
    {
        GPIO_RegisterMapInfo.gpio->int_status  |= (0x01<<GPIO_RegisterMapInfo.offset);
    }
}

/**
 *******************************************************************************
** \brief GPIO_ExtModeConfig
 **       Caution:the GPIO_PIN_HIGH_IMPDENCE will disable the gpio out and in mode
 ** \param [u8]  Port: PortA,PortB,PortC,PortD,PortE
 ** \param [u8]  Pin: 1-19
 ** \param [u8]  GPIO_EXT_MODE_E:     GPIO_PIN_OPEN_SOURCE
                                      GPIO_PIN_OPEN_DRAIN
                                      GPIO_PIN_PUSH_PULL
                                      GPIO_PIN_HIGH_IMPDENCE
                 GPIO_PUSH_PULL_MODE_E
                                      GPIO_PULL_UP,
                                      GPIO_PULL_DOWN,
                                      GPIO_PULL_NONE
 ** \retval[void]
 ******************************************************************************/
void GPIO_ExtModeConfig(GPIO_PORT_ENUM_E Port, GPIO_PIN_ENUM_E Pin,GPIO_EXT_MODE_E Ext_Mode,GPIO_PUSH_PULL_MODE_E Pull_Mode)
{
    GPIO_RegisterMapInfo_T GPIO_RegisterMapInfo ;

    tc_pinmux_config_field_t * pinConfig = (tc_pinmux_config_field_t *)GPIO_PinMuxAdrMap(Port,Pin);
    GPIO_RegisterMapInfo = GPIO_RegisterMap(Port,Pin);

    /* GPIO Extral Mode Setting */

    if((GPIO_RegisterMapInfo.gpio != NULL)&&(pinConfig != NULL))
    {
        if(Ext_Mode == GPIO_PIN_OPEN_SOURCE)
        {
            pinConfig->open_source = 1;
            pinConfig->open_drain  = 0;
        }

        if(Ext_Mode == GPIO_PIN_OPEN_DRAIN)
        {
            pinConfig->open_source = 0;
            pinConfig->open_drain  = 1;
        }

        if(Ext_Mode == GPIO_PIN_PUSH_PULL)
        {
            pinConfig->open_source = 0;
            pinConfig->open_drain  = 0;
        }

        if(Ext_Mode == GPIO_PIN_HIGH_IMPEDANCE)
        {
            GPIO_RegisterMapInfo.gpio->out_en_set &= ~(0x1<<GPIO_RegisterMapInfo.offset);
            GPIO_RegisterMapInfo.gpio->out_en_clr &= ~(0x1<<GPIO_RegisterMapInfo.offset);
        }

        if(Pull_Mode == GPIO_PULL_UP)
        {
            pinConfig->sw_pull_sel = 1;
            pinConfig->sw_pullup   = 1;
            pinConfig->sw_pulldown  = 0;
        }

        if(Pull_Mode == GPIO_PULL_DOWN)
        {
            pinConfig->sw_pull_sel = 1;
            pinConfig->sw_pullup   = 0;
            pinConfig->sw_pulldown  = 1;
        }
        if(Pull_Mode == GPIO_PULL_NONE)
        {
            pinConfig->sw_pull_sel = 0;
            pinConfig->sw_pullup   = 0;
            pinConfig->sw_pulldown  = 0;
        }
    }
}

/**
 *******************************************************************************
** \brief GPIO  GPIO_20mA_Mode
 **
 ** \param [u8]  PGPIO_20MA_MODE_E：
                                     GPIO_20MA_PA00,
                                     GPIO_20MA_PA01,
                                     GPIO_20MA_PB00,
                                     GPIO_20MA_PB01,
                                     GPIO_20MA_PB02,
                                     GPIO_20MA_PB03
    \param [u8]    En
 ** \retval[void]
 ******************************************************************************/
void GPIO_20mA_Mode(GPIO_20MA_MODE_E PortPin,uint8_t En)
{
    tc_pinmux_ma30en_field_t *pConfig =  &PINMUXELSE->SW_MA30_EN_f;

    if(PortPin == GPIO_20MA_PA00)
    {
        pConfig->sw_ma30_pa00_en = En;
    }
    if(PortPin == GPIO_20MA_PA01)
    {
        pConfig->sw_ma30_pa01_en = En;
    }
    if(PortPin == GPIO_20MA_PB00)
    {
        pConfig->sw_ma30_pb00_en = En;
    }
    if(PortPin == GPIO_20MA_PB01)
    {
        pConfig->sw_ma30_pb01_en = En;
    }
    if(PortPin == GPIO_20MA_PB02)
    {
        pConfig->sw_ma30_pb02_en = En;
    }
    if(PortPin == GPIO_20MA_PB03)
    {
        pConfig->sw_ma30_pb03_en = En;
    }

}

