/**
 *****************************************************************************
 * @brief   uart driver source file.
 *
 * @file   uart.c
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
#include "trim.h"
#include "gpio.h"

/*******************************************************************************
* Definitions：私有(仅本源文件内使用)宏、枚举、结构体的定义
******************************************************************************/

/*******************************************************************************
* Variables：所有全局变量(外部，静态，常量，指针)的定义
******************************************************************************/
uint8_t  uartBuffer[UARTBUFSIZE];
uint32_t uartCount = 0;
uint32_t uartStatus = 0;
uint8_t  uartTxEmpty = 1;



/*******************************************************************************
* Prototypes：内部函数(即 static 修饰)的声明
******************************************************************************/

/*******************************************************************************
* Code：所有函数(外部，内部)的定义
******************************************************************************/

/********************************************************
@brief  设置UART时钟源
@param  uart：uart寄存器基地址
        clkSrc：时钟源选择
@retval void
*********************************************************/
void SetUartFclkSource(T_UART *uart, enumModuleFclkSrc_t clkSrc)
{
    if(uart == UART0)
    {
        SYSCTL->UART0_CLKRST_CTRL_f.sw_rst_uart = 1;
        SYSCTL->UART0_CLKRST_CTRL_f.sw_pclk_en_uart = 0;
        SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
        __NOP();
        __NOP();
        SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_sel_uart = (uint8_t)clkSrc;
        SYSCTL->UART0_CLKRST_CTRL_f.sw_pclk_en_uart = 1;
        SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_en_uart = 1;
        SYSCTL->UART0_CLKRST_CTRL_f.sw_rst_uart = 0;
    }else if(uart == UART1)
    {
        SYSCTL->UART1_CLKRST_CTRL_f.sw_rst_uart = 1;
        SYSCTL->UART1_CLKRST_CTRL_f.sw_pclk_en_uart = 0;
        SYSCTL->UART1_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
        __NOP();
        __NOP();
        SYSCTL->UART1_CLKRST_CTRL_f.sw_fclk_sel_uart = (uint8_t)clkSrc;
        SYSCTL->UART1_CLKRST_CTRL_f.sw_pclk_en_uart = 1;
        SYSCTL->UART1_CLKRST_CTRL_f.sw_fclk_en_uart = 1;
        SYSCTL->UART1_CLKRST_CTRL_f.sw_rst_uart = 0;
    }else if(uart == UART2)
    {
        SYSCTL->UART2_CLKRST_CTRL_f.sw_rst_uart = 1;
        SYSCTL->UART2_CLKRST_CTRL_f.sw_pclk_en_uart = 0;
        SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_en_uart = 0;
        __NOP();
        __NOP();
        SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_sel_uart = (uint8_t)clkSrc;
        SYSCTL->UART2_CLKRST_CTRL_f.sw_pclk_en_uart = 1;
        SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_en_uart = 1;
        SYSCTL->UART2_CLKRST_CTRL_f.sw_rst_uart = 0;
    }else if(uart == LPUART)
    {
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_rst_lpuart = 1;
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_pclk_en_aon = 0;
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_lpuart_hs = 0;
        __NOP();
        __NOP();
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_sel_lpuart_hs = (uint8_t)clkSrc;
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_pclk_en_aon = 1;
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_lpuart_hs = 1;
        SYSCTL->AONTOP_CLKRST_CTRL_f.sw_rst_lpuart = 0;
    }
}

 /********************************************************
 @brief  获取UART时钟值
 @param  uart：uart寄存器基地址
 @retval UART时钟值
 *********************************************************/
 uint32_t GetUartFclkVal(T_UART *uart)
 {
    uint8_t uartSelFclk = 0;

     if(uart == UART0)
     {
        uartSelFclk = SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_sel_uart;
     } else if (uart == UART1)
     {
        uartSelFclk = SYSCTL->UART1_CLKRST_CTRL_f.sw_fclk_sel_uart;
     } else if (uart == UART2)
     {
        uartSelFclk = SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_sel_uart;
     } else if (uart == LPUART)
     {
        uartSelFclk = SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_sel_lpuart_hs;
     }
     if (uartSelFclk == 0x00)
     {
        if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_27_DATA)
            return 27000000ul;
        else if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_24_DATA)
            return 24000000ul;
        else if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_16_DATA)
            return 16000000ul;
        else
            return 16000000ul;
     }
     else if (uartSelFclk == 0x2)
        return 32768ul;
     else if (uartSelFclk == 0x1)
        return 32000000ul;
     else
        return 0ul;

 }

/********************************************************
 @brief  UART引脚配置函数
 @param  uart：uart寄存器基地址
         option:引脚复用选项
 @retval void
 *********************************************************/
void UartIOConfig(T_UART *uart,uint8_t option)
{
    if(uart == UART0)
    {
        if(option == 3)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin6, AF0);
            GPIO_SetAFMode(GPIOPortB, GPIOPin7, AF0);
        }
        else if(option == 2)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin2, AF1);
            GPIO_SetAFMode(GPIOPortB, GPIOPin3, AF1);
        }
        else
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin6, AF0);
            GPIO_SetAFMode(GPIOPortB, GPIOPin7, AF0);
        }
    }
    else if(uart == UART1)
    {
        if(option == 3)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin0, AF2);
            GPIO_SetAFMode(GPIOPortB, GPIOPin1, AF2);
        }
        else if(option == 2)
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin4, AF1);
            GPIO_SetAFMode(GPIOPortB, GPIOPin5, AF1);
        }
        else
        {
            GPIO_SetAFMode(GPIOPortB, GPIOPin0, AF2);
            GPIO_SetAFMode(GPIOPortB, GPIOPin1, AF2);
        }
    }
    else if(uart == LPUART) //AON
    {
        GPIO_SetAFMode(GPIOPortA, GPIOPin4, AF1);
        //GPIO_SetAFMode(GPIOPortA, GPIOPin5, AF1);
    }
    else if(uart == UART2)
    {
        GPIO_SetAFMode(GPIOPortC, GPIOPin1, AF1);
        GPIO_SetAFMode(GPIOPortC, GPIOPin2, AF1);
    }

}

/********************************************************
 @brief  切换0X00,0X04寄存器的功能
 @param  uart：uart寄存器基地址
         val:切换功能选项 0表示使能收发BUFF和IER 1表示使能DLL/DLH
 @retval void
 *********************************************************/
void UART_DivideLatch(T_UART *uart,unsigned int val)
{
    unsigned int rdData = 0;
    rdData = uart->LCR;
    rdData = (rdData&0xffffff7f)|(val <<7);
    uart->LCR = rdData;
}

/********************************************************
 @brief  设置波特率
 @param  uart：uart寄存器基地址
         dlh:波特率除数的高8位
         dll:波特率除数的低8位
         frac:波特率除数的小数部分
 @retval void
 *********************************************************/
void UART_Divided(T_UART *uart,unsigned int dlh,unsigned int dll, unsigned int frac)
{
    UART_DivideLatch(uart,1);
    uart->DLH = dlh;
    uart->DLL = dll;
    uart->EFCR = ((uart->EFCR & ~(0xF << 8))|(frac << 8));
    UART_DivideLatch(uart,0);
}

/********************************************************
 @brief  自动计算并设置波特率
 @param  uart：uart寄存器基地址
         baudrate:比特率
 @retval void
 *********************************************************/
void SetUartBaudrate(T_UART *uart,uint32_t baudrate)
{
    uint32_t div,uartClock;
    uint8_t frac = 0;
    int32_t i;
    uartClock = GetUartFclkVal(uart);
    /*
    * formula to calculate baudrate, baudrate = clock_in / (prescalar * divisor_value),
    * where prescalar = MFCR_PRESCALE_FACTOR(4,8,16), divisor_value = DLH:DLL.EFCR_FRAC
    */
    for(i = 0; i <= 2; i++)
    {
        div = (1 << i) * uartClock / baudrate;
        frac = div & 0xf;
        div >>= 4;
        // Integer part of divisor value shall not be zero, otherwise, the result is invalid
        if (div != 0)
            break;
    }
    UART_Divided(uart, ((div >> 8) & 0xff), ((div >> 0) & 0xff), frac);
    uart->MFCR = (uart->MFCR&(~(0x03<<8)))|(i<<8);
}

/********************************************************
 @brief  获取接收FIFO中断阈值字节数
 @param  uart：uart寄存器基地址
 @retval void
 *********************************************************/
uint8_t GetRxFifoTrigNum(uint8_t triggerLevel)
{
    uint8_t value = 0;
    if(triggerLevel == 0)
    {
        value = 1;
    }
    else if(triggerLevel == 1)
    {
        value = 8;
    }
    else if(triggerLevel == 2)
    {
        value = 16;
    }
    else if(triggerLevel == 3)
    {
        value = 30;
    }
    return value;
}

/********************************************************
 @brief  注销中断函数
 @param  uart：uart寄存器基地址
 @retval void
 *********************************************************/
void DisableUartInt(T_UART *uart)
{
    uart->IER  = 0x00; 
    if(uart == UART0)
    {
        XIC_DisableIRQ(FIC_Uart0Rx_IRQn);
        XIC_DisableIRQ(FIC_Uart0Tx_IRQn);
        XIC_DisableIRQ(FIC_Uart0Err_IRQn);
    }
    else if(uart == UART1)
    {
        XIC_DisableIRQ(FIC_Uart1Rx_IRQn);
        XIC_DisableIRQ(FIC_Uart1Tx_IRQn);
        XIC_DisableIRQ(FIC_Uart1Err_IRQn);
    }
    else if(uart == LPUART)//AON
    {
        XIC_DisableIRQ(AIC_Aon_UartRx_IRQn);
        XIC_DisableIRQ(AIC_Aon_UartTx_IRQn);
        XIC_DisableIRQ(AIC_Aon_UartErr_IRQn);
    }

    else if(uart == UART2)
    {

        XIC_DisableIRQ(LIC_Uart2Rx_IRQn);
        XIC_DisableIRQ(LIC_Uart2Tx_IRQn);
        XIC_DisableIRQ(LIC_Uart2Err_IRQn);
    }
}

/********************************************************
 @brief  uart初始化函数
 @param  uartConfig：uart配置结构体
 @retval void
 *********************************************************/
void UART_Init(uartconfig_t *uartConfig)
{
    uint32_t tempReg __attribute__((unused)) = 0;
    DisableUartInt(uartConfig->uart);  //interrupt disable

    //SetUartFclkSource(uartConfig->uart, FCLK_SRC_16M);

    uartConfig->uart->MFCR = 0x0;
    SetUartBaudrate(uartConfig->uart,uartConfig->baudRate);
    UartIOConfig(uartConfig->uart, uartConfig->pinmuxOption);

    uartConfig->uart->FCR  = 0x07|(uartConfig->rxFifoTriggerLevel<<6)|(uartConfig->txFifoTriggerLevel<<4);           //fifo_en = 1   rx_fifo_avail_trig_level = 1  tx_fifo_empty_trig_level = 1
    uartConfig->uart->RTIC = 0xff;                                                                                   //timeout bit num
    uartConfig->uart->LCR  = (uartConfig->wordLength)|(uartConfig->parity)|(uartConfig->stopBits);                   //char_len = 3 8-bit character 1 stop bit none 
    uartConfig->uart->EFCR |= (uartConfig->rxPolarity);                                                              //Receive takes negative 
    uartConfig->uart->EFCR |= (uartConfig->txPolarity);                                                              //Transmit takes negative 
    uartConfig->uart->MFCR = (uartConfig->uart->MFCR&(~(0x01|(0x01<<4)|(0x01<<5))))|(uartConfig->hwFlowCtl)|0x01;    //CTS RTS  uart_enable  
    if(uartConfig->dmaConfig.txDmaEn||uartConfig->dmaConfig.rxDmaEn) //dma_enable 
        uartConfig->uart->MFCR |= (1<<2);
    if(uartConfig->uartIrqEn)//interrupt enable 
        uartConfig->uart->IER = 0x1F; 
    tempReg = uartConfig->uart->LSR;    // Read to clear the line status

    // Ensure a clean start, no data in either TX or RX FIFO.
    while((UART_GetFlagStatus(uartConfig->uart,UART_LSR_TX_DATA_REQ) == 0)||(UART_GetFlagStatus(uartConfig->uart,UART_LSR_TX_EMPTY) == 0))
        continue;
    while (UART_GetFlagStatus(uartConfig->uart,UART_LSR_RX_DATA_READY))
    {
        tempReg=uartConfig->uart->RBR;
    }

}

/********************************************************
 @brief  1字节发送函数，block方式
 @param  uart:uart寄存器基地址
         data:1字节数据
 @retval void
 *********************************************************/
void UART_SendOneData(T_UART *uart,uint8_t data)
{
    while((uart->LSR & UART_LSR_TX_DATA_REQ) == 0);
    uart->TBR = (data & 0xFF);    
}

/********************************************************
 @brief  多字节发送函数
 @param  uart:uart寄存器基地址
         pu8Buffer:发送数据BUFF
         u32Len：发送数据长度
 @retval void
 *********************************************************/
void UART_SendData(T_UART *uart,uint8_t *pu8Buffer, uint32_t u32Len)
{
    while ( u32Len != 0 )
    {
        /* Send one character to UART */
        UART_SendOneData(uart,*pu8Buffer);
        pu8Buffer++;
        u32Len--;
    }
}

/********************************************************
 @brief  多字节发送函数DMA方式
 @param  uartConfig:uart配置结构体
         pu8Buffer:发送数据BUFF
         len：发送数据长度
 @retval void
 *********************************************************/
void UART_SendDataDma(uartconfig_t *uartConfig,uint8_t *pu8Buffer, uint16_t len)
{
    PeripheralsTxDmaConfig( ((uint32_t*)(&(uartConfig->uart->TBR))),\
                            (uint32_t*)pu8Buffer,\
                            len,\
                            uartConfig->dmaConfig.txChannel,\
                            uartConfig->dmaConfig.txRequest,\
                            uartConfig->dmaConfig.txDalgn,\
                            uartConfig->dmaConfig.txWidth,\
                            uartConfig->dmaConfig.txBurst,\
                            uartConfig->dmaConfig.txEndIrq);
}

/********************************************************
 @brief  1字节接收函数，no-block方式
 @param  uart:uart寄存器基地址
         pu8Buffer:1字节数据
 @retval 1-表示成功 0-表示失败
 *********************************************************/
uint8_t UART_ReceiveOneDataUnblocked(T_UART *uart,uint8_t *pu8Buffer)
{
    uint8_t u8Len = 0;

    if(uart->LSR & UART_LSR_RX_DATA_READY)
    {
        *pu8Buffer = uart->RBR;
        u8Len = 1;
    }
    return u8Len;
}

/********************************************************
 @brief  1字节接收函数，block方式
 @param  uart:uart寄存器基地址
 @retval 接收的数据
 *********************************************************/
uint8_t UART_ReceiveOneDataBlocked(T_UART *uart)
{
    while((uart->LSR & UART_LSR_RX_DATA_READY) == 0);
    return (uart->RBR & 0xFF);
}

/********************************************************
 @brief  多字节接收函数，DMA方式
 @param  uartConfig:uart配置结构体
         pu8Buffer:接收BUFF
         len:接收字节数
 @retval 接收的数据
 *********************************************************/
void UART_RcvDataDma(uartconfig_t *uartConfig,uint8_t *pu8Buffer, uint16_t len)
{
    PeripheralsRxDmaConfig((uint32_t*)pu8Buffer,\
                         (uint32_t*)&(uartConfig->uart->RBR),\
                         len,\
                         uartConfig->dmaConfig.rxChannel,\
                         uartConfig->dmaConfig.rxRequest,\
                         uartConfig->dmaConfig.rxDalgn,\
                         uartConfig->dmaConfig.rxWidth,
                         uartConfig->dmaConfig.rxBurst,\
                         uartConfig->dmaConfig.rxEndIrq);

}

/********************************************************
 @brief  获取UART标识位
 @param  uartConfig:uart配置结构体
         uart:uart寄存器基地址
         uartFlag：需要的标识位
 @retval 标识位的值
 *********************************************************/
int UART_GetFlagStatus(T_UART *uart,uint16_t uartFlag)
{
    int bitStatus = 0;

    if ( (uart->LSR & uartFlag) != 0)
    {
        bitStatus = 1;
    }
    else
    {
        bitStatus = 0;
    }

    return bitStatus;
}

