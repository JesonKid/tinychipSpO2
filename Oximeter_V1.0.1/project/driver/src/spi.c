/**
 *****************************************************************************
 * @brief   spi driver source file.
 *
 * @file   spi.c
 * @author hc
 * @date   28/JUL/2020
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


#include "spi.h"
#include "stdio.h"
#include "gpio.h"
/*******************************************************************************
* Definitions：私有(仅本源文件内使用)宏、枚举、结构体的定义
******************************************************************************/
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */


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
@brief  SPI   IO配置函数
@param  ssp： SPI寄存器基地址
@retval void
*********************************************************/
void SpiIOConfig(T_SPI *ssp)
{
    if(ssp == SPI0)
    {
        GPIO_SetAFMode(GPIOPortB, GPIOPin0, AF0);
        GPIO_SetAFMode(GPIOPortB, GPIOPin1, AF0);
        GPIO_SetAFMode(GPIOPortB, GPIOPin2, AF0);
        GPIO_SetAFMode(GPIOPortB, GPIOPin3, AF0);
    }
    else if(ssp == SPI1)
    {
        GPIO_SetAFMode(GPIOPortC, GPIOPin1, AF0);
        GPIO_SetAFMode(GPIOPortC, GPIOPin2, AF0);
        GPIO_SetAFMode(GPIOPortC, GPIOPin3, AF0);
        GPIO_SetAFMode(GPIOPortC, GPIOPin4, AF0);
    }
}

/********************************************************
@brief  SPI Data Size Select
@param  ssp:  SPI寄存器基地址
        size: Data Size
        32 32-bit data
        //2 2-bit data(保留)
        //3 3-bit data（保留）
        4 4-bit data
        5 5-bit data
        6 6-bit data
        7 7-bit data
        8 8-bit data
        9 9-bit data
        10 10-bit data
        11 11-bit data
        12 12-bit data
        13 13-bit data
        14 14-bit data
        15 15-bit data
        16 16-bit data
@retval void
*********************************************************/
void SPI_SetFrameSize(T_SPI *ssp, uint8_t size)
{
    // Only frame sizes of 4 to 16 bits are allowed.
    // Fall-back to default value of 8 bits if the siwe is invalid.

    if ((size < 2 || size > 16)&&(size != 32))
    {
        size = 8;
    }

    if(size == 32)
    {
        size = 1;
    }
    // The DDS field [3:0] of CR0 controls the number of bits
    // transferred in each frame.
    ssp->CR0 &= ~0xF;
    ssp->CR0 |= (size - 1) & 0xF;
}

/********************************************************
@brief  SPI IO配置函数
@param  ssp: SPI寄存器基地址
        CPOL: 极性
        CPHA: 相位
@retval void
*********************************************************/
void SPI_SetCpolCpha(T_SPI* ssp, bool CPOL, bool CPHA)
{
    ssp->CR0 &= ~0xc0;
    ssp->CR0 |= ((CPOL<<6)|(CPHA<<7)&0xc0);
}

/********************************************************
@brief  SPI 帧格式配置
@param  ssp: SPI寄存器基地址
rfr:帧格式
    00: Motorola SPI frame format.
    01: TI synchronous serial frame format.
    10: National Microwire frame format.
    11: Reserved, undefined operation.
@retval void
*********************************************************/
void SPI_SetRfr(T_SPI* ssp,uint8_t rfr)
{
    ssp->CR0 &= ~0x30;
    ssp->CR0 |= ((rfr<<4)&0x30);
}


/********************************************************
@brief  SPI 速率设置函数，rate = FCLK/(CPSDVR*(1+SCR))
@param  ssp: SPI寄存器基地址
        SCR: Serial clock rate
        CPSDVR: Clock prescale divisor，Must be an even number from 2-254,
@retval void
*********************************************************/
void SPI_SetRate(T_SPI *ssp,uint8_t SCR,uint8_t CPSDVR)
{
    /* ssp速率  FSPICLK_FSPICLKOUT = FSPICLK/(CPSDVR*(1+SCR)) = 20000000/(16*(1+9))=0.5M */
    ssp->CR0 &= ~(0x0000ff00);

    ssp->CR0 |= ((SCR<<8)&0x0000ff00);
    ssp->CPSR = CPSDVR&0x000000ff;
}

/********************************************************
@brief  SPI 初始化函数
@param  ssp: SPI寄存器基地址
        config: spi 初始化配置结构体
@retval void
*********************************************************/
void SPI_Init(SPI_Config config)
{
    /* 配置引脚 */
    // SpiIOConfig(config.spi);  /* 需根据具体IO寄存器重新配置 */

    /* ssp速率  FSPICLK_FSPICLKOUT = FSPICLK/(CPSDVR*(1+SCR)) = 16000000/(2*(1+0))=8M */
    // Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 7.
    config.spi->CR0 = 0x00000007;
    SPI_SetRfr(config.spi,config.frf);
    SPI_SetFrameSize(config.spi, config.frame_size);
    SPI_SetCpolCpha(config.spi,config.CPOL,config.CPHA);

    // SPICPSR clock prescale register, master mode, minimum divisor is 0x02.
    config.spi->CPSR = 2;

    for (int i = 0; i < FIFOSIZE; i++)
    {
        // Clear the RxFIFO.
        uint8_t Dummy = config.spi->DR;
        UNUSED(Dummy);
    }

    config.spi->IMSC  = config.imsc;/* 配置中断使能位 */
    config.spi->DMACR = (config.dmaConfig.txDmaEn<<1)|(config.dmaConfig.rxDmaEn);/* DMA使能配置 */

    // Device select as master, SPI Enabled.
    if (config.loopback)    /* loopback模式 */
    {
        config.spi->CR1 = SPICR1_LBM | SPICR1_SSE;
    }
    else
    {
        if (config.slave)   /* 从机模式 */
        {
            // Slave mode.
            if ( config.spi->CR1 & SPICR1_SSE )
            {
                // The slave bit can't be set until SSE bit is zero.
                config.spi->CR1 &= ~SPICR1_SSE;
            }
            // Enable slave bit first.
            config.spi->CR1 = SPICR1_MS;
            // Enable SPI.
            config.spi->CR1 |= SPICR1_SSE;
        }
        else            /* 主机模式 */
        {
            // Master mode.
            config.spi->CR1 = SPICR1_SSE;
        }
    }
    // Set SPIINMS registers to enable interrupts.
    // Enable all error related interrupts.
}

/********************************************************
@brief  SPI  get status registe flag
@param  spi: SPI寄存器基地址
        SPI_FLAG: 需要获取的状态
@retval 状态值
*********************************************************/
int SPI_GetFlagStatus(T_SPI *spi,uint16_t SPI_FLAG)
{
    int bitStatus = 0;
    if ( (spi->SR & SPI_FLAG) != 0)
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
@brief  SPI主机发送
@param  spi: SPI寄存器基地址
        buffer: 需要发送的数据BUFF
        size：发送的数据长度
@retval void
*********************************************************/
void SPI_SendData_Host(T_SPI *spi, uint32_t *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        while ((spi->SR & (SPISR_TNF|SPISR_BSY)) != SPISR_TNF) continue;/* 一个字节一个字节发送，不会产生发送中断 */
        spi->DR = *buffer;
        buffer++;
    }
    while((spi->SR & (SPISR_BSY)) == SPISR_BSY);
    /*
    while ((spi->SR & (SPISR_BSY|SPISR_RNE)) == SPISR_RNE)
    {
        uint32_t Dummy = spi->DR;
        UNUSED(Dummy);
    }
    */
}

/********************************************************
@brief  SPI从机发送
@param  spi: SPI寄存器基地址
        buffer: 需要发送的数据BUFF
        size：发送的数据长度
@retval void
*********************************************************/
void SPI_SendData_Slave(T_SPI *spi, uint32_t *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        while ((spi->SR & (SPISR_TNF)) != SPISR_TNF) continue;
        {
            spi->DR = *buffer;
            buffer++;
        }
    }

    while ((spi->SR & (SPISR_BSY|SPISR_RNE)) == SPISR_RNE)
    {
        uint32_t Dummy = spi->DR;
        UNUSED(Dummy);
    }

}

/********************************************************
@brief  SPI主机接收
@param  spi: SPI寄存器基地址
        buffer: 接收数据BUFF
        size：接收数据长度
@retval void
*********************************************************/
void SPI_ReceiveData_Host(T_SPI *spi, uint32_t *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        while ((spi->SR & (SPISR_TNF)) != SPISR_TNF) continue;
        spi->DR = 0xFF;
        while ((spi->SR & (SPISR_BSY|SPISR_RNE)) != SPISR_RNE) continue;
        *buffer = spi->DR;
        buffer++;
    }
}

/********************************************************
@brief  SPI从机接收
@param  spi: SPI寄存器基地址
        buffer: 接收数据BUFF
        size：接收数据长度
@retval void
*********************************************************/
void SPI_ReceiveData_Slave(T_SPI *spi, uint32_t *buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        while (!(spi->SR & SPISR_RNE)) continue;
        *buffer = spi->DR;
        buffer++;
    }
}

/********************************************************
@brief  spi DMA 接收函数
@param  config: SPI配置结构体
        buffer: 存放接收数据的BUFF
        Len:    接收的数据长度
@retval
*********************************************************/
void SPI_Receive_DMAConfig(SPI_Config *config,uint32_t *buffer, uint16_t len)
{
    PeripheralsRxDmaConfig((uint32_t*)buffer,\
                           (uint32_t*)&(config->spi->DR),\
                           len,\
                           config->dmaConfig.rxChannel,\
                           config->dmaConfig.rxRequest,\
                           config->dmaConfig.rxDalgn,\
                           config->dmaConfig.rxWidth,
                           config->dmaConfig.rxBurst,\
                           config->dmaConfig.rxEndIrq);
}


/********************************************************
@brief  spi DMA 发送函数
@param  config: SPI配置结构体
        buffer: 需要发送的数据BUFF
        Len：    发送的数据长度
@retval
*********************************************************/
void SPI_Send_DMAConfig(SPI_Config *config,uint32_t *buffer, uint16_t len)
{
    PeripheralsTxDmaConfig((uint32_t*)&(config->spi->DR),\
                           (uint32_t*)buffer,\
                           len,\
                           config->dmaConfig.txChannel,\
                           config->dmaConfig.txRequest,\
                           config->dmaConfig.txDalgn,\
                           config->dmaConfig.txWidth,\
                           config->dmaConfig.txBurst,\
                           config->dmaConfig.txEndIrq);
}




