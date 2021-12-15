/**
 *****************************************************************************
 * @brief  I2C source file.
 *
 * @file   I2C.c
 * @author hc
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

#include "tcasxxx.h"
#include "system_tcasxxx.h"
#include "uart.h"
#include "interrupt.h"
#include "dma.h"
#include "i2c.h"
#include "delay.h"
#include "gpio.h"
/*******************************************************************************
* Definitions ˽��(����Դ�ļ���ʹ��)�ꡢö�١��ṹ��Ķ���
******************************************************************************/


/*******************************************************************************
* Variables ����ȫ�ֱ���(�ⲿ����̬��������ָ��)�Ķ���
******************************************************************************/


/*******************************************************************************
 * Code ���к���(�ⲿ���ڲ�)�Ķ���
 ******************************************************************************/

/********************************************************
@brief  i2c���Ÿ���
@param  i2c:i2c�Ĵ�������ַ
@retval void
*********************************************************/
void I2C_ConfigIO(i2cconfig_t *i2ccfg)
{
    if(i2ccfg->i2c == I2C0)
    {
        if (i2ccfg->pinmuxOption == 1)
        {
            GPIO_SetAFMode(GPIOPortC, GPIOPin7, AF1);
            GPIO_SetAFMode(GPIOPortD, GPIOPin0, AF1);
        } else if (i2ccfg->pinmuxOption == 2)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin6, AF3);
            GPIO_SetAFMode(GPIOPortB, GPIOPin7, AF3);
        } else {
            GPIO_SetAFMode(GPIOPortA, GPIOPin0, AF0);
            GPIO_SetAFMode(GPIOPortA, GPIOPin1, AF0);
        }
    }
    else if(i2ccfg->i2c == I2C1)
    {
        if (i2ccfg->pinmuxOption == 1)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin2, AF2);
            GPIO_SetAFMode(GPIOPortB, GPIOPin3, AF2);
        } else {
            GPIO_SetAFMode(GPIOPortC, GPIOPin4, AF3);
            GPIO_SetAFMode(GPIOPortC, GPIOPin5, AF3);
        }
    }

}

/********************************************************
@brief  ��ȡi2cģ��״̬
@param  i2c:i2c�Ĵ�������ַ
        I2C_FLAG:��Ҫ��ȡ��״̬
@retval void
*********************************************************/
int I2C_GetFlagStatus(T_I2C *i2c,uint32_t I2C_FLAG)
{
    int bitStatus = 0;

    if ( (i2c->I2CSTR & I2C_FLAG) != 0)
    {
        bitStatus = 1;
    }
    else
    {
        bitStatus = 0;
    }

    return bitStatus;
}

/********************************************************
@brief  ��ȡi2c�ж�״̬
@param  i2c:i2c�Ĵ�������ַ
        I2C_FLAG:��Ҫ��ȡ��״̬
@retval void
*********************************************************/
int I2C_GetFlagISR(T_I2C *i2c,uint32_t I2C_FLAG)
{
    int bitStatus = 0;

    if ( (i2c->I2CISR & I2C_FLAG) != 0)
    {
        i2c->I2CISR &= I2C_FLAG;
        bitStatus = 1;
    }
    else
    {
        bitStatus = 0;
    }

    return bitStatus;
}

/********************************************************
@brief  ����i2cʱ�ӷ�Ƶ��ʱ��Դѡ��
@param  i2ccfg:i2c���ýṹ��
        div:��Ƶϵ��
        sel:ʱ��Դѡ��
@retval void
*********************************************************/
void SetI2CFclkSource(i2cconfig_t *i2ccfg,uint8_t div,uint8_t sel)
{
    if(i2ccfg->i2c == I2C0)
    {
        // FCLK for SAR, default is using 16M and fclk is enable
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_en_i2c = 1;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_pclk_en_i2c = 1;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_div_i2c = div;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_sel_i2c = sel;
    }
    else if(i2ccfg->i2c == I2C1)
    {
        // FCLK for SAR, default is using 16M and fclk is enable
        SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_en_i2c = 1;
        SYSCTL->I2C1_CLKRST_CTRL_f.sw_pclk_en_i2c = 1;
        SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_div_i2c = div;
        SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_sel_i2c = sel;
    }
}

/********************************************************
@brief  ��ȡi2cʱ��ֵ
@param  i2ccfg:i2c���ýṹ��
@retval ʱ��ֵ
*********************************************************/
uint32_t GetI2CFclkSource(i2cconfig_t *i2ccfg)
{
    if(i2ccfg->i2c == I2C0)
    {
        if(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_sel_i2c == 0)
        {
            return (16000000/(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
        else if(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_sel_i2c == 1)
        {
            return (32000000/(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
        else if(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_sel_i2c == 2)
        {
            return (32000/(SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
    }
    else if(i2ccfg->i2c == I2C1)
    {
        if(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_sel_i2c == 0)
        {
            return (16000000/(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
        else if(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_sel_i2c == 1)
        {
            return (32000000/(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
        else if(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_sel_i2c == 2)
        {
            return (32000/(SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_div_i2c+1));
        }
    }
    return 16000000;
}

/********************************************************
@brief  i2c��ʼ��
@param  i2ccfg:i2c���ýṹ��
@retval void
*********************************************************/
void I2C_Init(i2cconfig_t *i2ccfg)
{
    uint32_t scl = ((GetI2CFclkSource(i2ccfg)/3400000)-8-2*((i2ccfg->i2c->I2CTPR>>24)&0xf))/2;
    I2C_ConfigIO(i2ccfg);
    if(i2ccfg->rate==2) /* high mode 2Mbit/s */
    {
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_en_i2c = 1;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_pclk_en_i2c = 1;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_div_i2c = 0;
        SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_sel_i2c = 1;// fmult 32M
        i2ccfg->i2c->I2CTPR = 0x00440000;
        scl = ((32000000/2000000)-8-2*((i2ccfg->i2c->I2CTPR>>24)&0xf))/2;
        i2ccfg->i2c->I2CTPR |= scl|(scl<<8); /* speed = fclk_i2c/(sclh + 1  +sclh + 1 + SPIKE_FILTER_CNUM + 2 + 1 + SPIKE_FILTER_CNUM + 2 + 1)*/
    }
    else if(i2ccfg->rate==1) /* fast mode 400kbit/s  */
    {
        SetI2CFclkSource(i2ccfg,0,0);
        i2ccfg->i2c->I2CTPR = 0x01440000;
        scl = ((GetI2CFclkSource(i2ccfg)/400000)-8-2*((i2ccfg->i2c->I2CTPR>>24)&0xf))/2;
        i2ccfg->i2c->I2CTPR |= scl|(scl<<8); /* speed = fclk_i2c/(sclh + 1  +sclh + 1 + SPIKE_FILTER_CNUM + 2 + 1 + SPIKE_FILTER_CNUM + 2 + 1)*/
    }
    else /* standard mode 100kbit/s  */
    {
        SetI2CFclkSource(i2ccfg,0,0);
        i2ccfg->i2c->I2CTPR = 0x01440000;
        scl = ((GetI2CFclkSource(i2ccfg)/100000)-8-2*((i2ccfg->i2c->I2CTPR>>24)&0xf))/2;
        i2ccfg->i2c->I2CTPR |= scl|(scl<<8); /* speed = fclk_i2c/(sclh + 1  +sclh + 1 + SPIKE_FILTER_CNUM + 2 + 1 + SPIKE_FILTER_CNUM + 2 + 1)*/
    }

    if(i2ccfg->slaveen == 1)/* slave set */
    {
        if(i2ccfg->slaveaddrmode)   //10-bit address mode
            i2ccfg->i2c->I2CSAR = (1<<12)|(i2ccfg->slaveaddr);
        else
            i2ccfg->i2c->I2CSAR = (1<<12)|(i2ccfg->slaveaddr<<1);
    }
    
    i2ccfg->i2c->I2CIER = (i2ccfg->icen&0x3ffff);           /* �ж�ʹ������ */
    i2ccfg->i2c->I2CIMR = ((~(i2ccfg->icen))&0x3ffff);      /* �ж�MASK */

    i2ccfg->i2c->I2CMCR = 0x00000009|(i2ccfg->dmaConfig.rxDmaEn<<5)|(i2ccfg->dmaConfig.txDmaEn<<4)|(i2ccfg->slaveen<<8)|(i2ccfg->tx_fifo_threshold << 16)|(i2ccfg->rx_fifo_threshold <<20);    //enable i2c 

}

/********************************************************
@brief  i2c���ͺ���
@param  i2ccfg:i2c���ýṹ��
        data�������͵�����BUFF
        Tslaveaddr:�ӻ���ַ
        size���������ݴ�С
        Timeout:��ʱʱ�� us
@retval 0���ɹ� 1��ʧ��
*********************************************************/
uint8_t I2C_SendData(i2cconfig_t *i2ccfg,uint8_t *data,uint16_t Tslaveaddr,uint16_t size,int32_t Timeout)
{
    /* start */
    uint16_t len = size;
    if(i2ccfg->slaveen == 0)
    {
        if(i2ccfg->slaveaddrmode)
            i2ccfg->i2c->I2CSCR =0x0018000|(Tslaveaddr)|((size - 1) << 23);    /* start = 1 restart = 1 target_rwn = 0 */
        else
            i2ccfg->i2c->I2CSCR =0x0018000|(Tslaveaddr<<1)|((size - 1) << 23); /* start = 1 restart = 1 target_rwn = 0 */
    }
    else
    {
        i2ccfg->i2c->I2CSCR = ((size - 1) << 23);
    }

    /* send data */
    while(len--)
        i2ccfg->i2c->I2CTDR = *data++;

    /* wait till size byte transmit */
    while((i2ccfg->i2c->I2CSTR&I2C_I2CSTR_BUSY) == 1)
    {
        if (--Timeout == 0x00)
        {
            if(i2ccfg->slaveen == 0)
            {
                /* Generate stop */
                i2ccfg->i2c->I2CSCR |= 1<<17;
            }
            return 1;
        }
        delay1us(1);
    }
    if(i2ccfg->slaveen == 0)
    {
        /* Generate stop */
        i2ccfg->i2c->I2CSCR |= 1<<17;
    }

    /* Return 0, everything ok */
    return 0;
}

/********************************************************
@brief  i2c���պ���
@param  i2ccfg:i2c���ýṹ��
        Tslaveaddr:�ӻ���ַ
        rvdata:�������ݴ洢��
        size���������ݳ���
        Timeout:��ʱʱ�� us
@retval 0���ɹ� 1��ʧ��
*********************************************************/
uint8_t I2C_ReceiveData(i2cconfig_t *i2ccfg,uint16_t Tslaveaddr,uint8_t* rvdata,uint16_t size,int32_t Timeout)
{
    uint16_t i = 0;
    if(i2ccfg->slaveen == 0)
    {
        if(i2ccfg->slaveaddrmode)
            i2ccfg->i2c->I2CSCR = 0x000C000|(Tslaveaddr)| ((size - 1) << 23); /* start = 1 target_rwn = 1 */
        else
            i2ccfg->i2c->I2CSCR = 0x000C000|(Tslaveaddr<<1)| ((size - 1) << 23); /* start = 1 target_rwn = 1 */
    }
    else
    {
        i2ccfg->i2c->I2CSCR = 0x005C000|((size - 1) << 23); /* ACK */
    }


    while(i < size)
    {
        if(i2ccfg->i2c->I2CFSR&0x1f00)
        {
            rvdata[i] = i2ccfg->i2c->I2CRDR;
            i++;
        }
        else
        {
            if (--Timeout == 0x00)
            {
                return 1;
            }
            delay1us(1);
        }
    }
    return 0;
}

/********************************************************
@brief  i2c ����DMA���ú���
@param  i2ccfg:i2c���ýṹ��
        pu8Buffer:��������BUFF
        len���������ݳ���
@retval void
*********************************************************/
void I2C_TxDmaConfig(i2cconfig_t *i2cConfig,uint32_t Slave_addr,uint8_t *pu8Buffer,uint16_t len)
{
    PeripheralsTxDmaConfig((uint32_t*)(&(i2cConfig->i2c->I2CTDR)),\
                          (uint32_t*)pu8Buffer,\
                          len,\
                          i2cConfig->dmaConfig.txChannel,\
                          i2cConfig->dmaConfig.txRequest,\
                          i2cConfig->dmaConfig.txDalgn,\
                          i2cConfig->dmaConfig.txWidth,\
                          i2cConfig->dmaConfig.txBurst,\
                          i2cConfig->dmaConfig.txEndIrq );

    if(i2cConfig->slaveen == 0)
    {
        if(i2cConfig->slaveaddrmode)
            i2cConfig->i2c->I2CSCR =0x0018000|(Slave_addr)|((len - 1) << 23);    /* start = 1 restart = 1 target_rwn = 0 */
        else
            i2cConfig->i2c->I2CSCR =0x0018000|(Slave_addr<<1)|((len - 1) << 23); /* start = 1 restart = 1 target_rwn = 0 */
    }
    else
    {
        i2cConfig->i2c->I2CSCR = ((len - 1) << 23);
    }
}

/********************************************************
@brief  i2c ����DMA���ú���
@param  i2ccfg:i2c���ýṹ��
        pu8Buffer:��������BUFF
        len���������ݳ���
@retval void
*********************************************************/
void I2C_RxDmaConfig(i2cconfig_t *i2cConfig,uint32_t Slave_addr,uint8_t *pu8Buffer,uint16_t len)
{
    if(i2cConfig->slaveen == 0)
    {
        if(i2cConfig->slaveaddrmode)
            i2cConfig->i2c->I2CSCR = 0x000C000|(Slave_addr)| ((len - 1) << 23); /* start = 1 target_rwn = 1 */
        else
            i2cConfig->i2c->I2CSCR = 0x000C000|(Slave_addr<<1)| ((len - 1) << 23); /* start = 1 target_rwn = 1 */
    }
    else{
        i2cConfig->i2c->I2CSCR = 0x005C000|((len - 1) << 23); /* ACK */
    }
    PeripheralsRxDmaConfig((uint32_t*)pu8Buffer,\
                          ((uint32_t*)(&(i2cConfig->i2c->I2CTDR))),\
                          len,\
                          i2cConfig->dmaConfig.rxChannel,\
                          i2cConfig->dmaConfig.rxRequest,\
                          i2cConfig->dmaConfig.rxDalgn,\
                          i2cConfig->dmaConfig.rxWidth,\
                          i2cConfig->dmaConfig.rxBurst,\
                          i2cConfig->dmaConfig.rxEndIrq);
}
