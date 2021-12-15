/**
 *****************************************************************************
 * @brief   uart driver header.
 *
 * @file   uart.h
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
#ifndef __UART_H__
#define __UART_H__

#include "tcasxxx.h"
#include "system_tcasxxx.h"
#include  "dma.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/


/******************************************************************************* 
 * Definitions 
 ******************************************************************************/

/* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */

#define UARTBUFSIZE 100

#define TX_INTERRUPT    0   /* 0 if TX uses polling, 1 interrupt driven. */

#define PRINT_UART           UART0

#define DOWNLOAD_UART        UART1


#define IIR_PEND    0x01
#define IIR_RLS     0x01
#define IIR_CTI     0x02
#define IIR_MS      0x03
#define IIR_BUSY    0x04


#define LSR_RDR     0x01
#define LSR_ROE     0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80
#define LSR_TOE     0x100
#define LSR_TBI     0x200
#define LSR_RBI     0x400


//wordLength UART Word Length
#define UART_WORDLENGTH_5B                  0x00000000U
#define UART_WORDLENGTH_6B                  0x00000001U
#define UART_WORDLENGTH_7B                  0x00000002U
#define UART_WORDLENGTH_8B                  0x00000003U

//stopBits UART Number of Stop Bits
#define UART_STOPBITS_1                     0x00000000U
#define UART_STOPBITS_1_5                   (0x00000001U << 1)  //1.5 stop bits when Word Length Select is 0, else 2 stop bit


//parity UART Parity
#define UART_PARITY_NONE                    0x00000000U
#define UART_PARITY_EVEN                    (0x00000003U<<3)
#define UART_PARITY_ODD                     (0x00000001U<<3)

//hwFlowCtl UART Hardware Flow Control
#define UART_HWCONTROL_NONE                  0x00000000U
#define UART_HWCONTROL_RTS                   (0x00000001U<<4)
#define UART_HWCONTROL_CTS                   (0x00000002U<<4)
#define UART_HWCONTROL_RTS_CTS               (0x00000003U<<4)

//txPolarity 
#define UART_TX_POL_POSITIVE                 0x00000000U
#define UART_TX_POL_NEGATIVE                 (0x00000001U<<3)


//rxPolarity
#define UART_RX_POL_POSITIVE                 0x00000000U
#define UART_RX_POL_NEGATIVE                 (0x00000001U<<4)

typedef struct _uart_config
{
    T_UART      *uart;                  /* UART registers base address */
    uint32_t    baudRate;
    uint8_t     wordLength;
    uint8_t     stopBits;
    uint8_t     parity;
    uint8_t     hwFlowCtl;              /* Specifies whether the hardware flow control mode is enabled or disabled. */
    uint8_t     txPolarity;  
    uint8_t     rxPolarity;
    uint8_t     rxFifoTriggerLevel;     /* Rx FIFO Availible Trigger Level
                                        When the character number in RX FIFO is greater than or equal to the value indicated by the level, DMA RX request & Receiver Data Available Interrupt will be released according to the configurations.
                                        00: 1 character
                                        01: 1/4 FIFO depth
                                        10: 1/2 FIFO depth
                                        11: FIFO depth - 2 */
    uint8_t     txFifoTriggerLevel;     /*TX FIFO Empty Trigger Level
                                        When the number of the empty spaces in TX FIFO is greater than or equal to the value indicated by the level, DMA TX request & Transmit Data Request Interrupt will be released according to the configurations.
                                        00: FIFO empty
                                        01: 2 characters in FIFO
                                        10: 1/4 FIFO depth
                                        11: 1/2 FIFO depth */
    uint8_t     uartIrqEn;
    dmaconfig_t dmaConfig;
    uint8_t     pinmuxOption;            /* 引脚复用选项 */
}uartconfig_t;



/******************************************************************************* 
 * API 
 ******************************************************************************/

/* 外部函数(可加 extern 修饰)的声明 */
 
void     UART_Init(uartconfig_t *uartConfig);
void     UART_Config(T_UART *uart,int type);
void     UART_SendOneData(T_UART *uart,uint8_t data);
uint8_t  UART_ReceiveOneDataBlocked(T_UART *uart);
int      UART_GetFlagStatus(T_UART *uart,uint16_t uartFlog);
void     DisableUartInt(T_UART *uart);
void     UART_SendData(T_UART *uart,uint8_t *pu8Buffer, uint32_t u32Len);
uint8_t  UART_ReceiveOneDataUnblocked(T_UART *uart,uint8_t *pu8Buffer);
void     UART_SendDataDma(uartconfig_t *uartConfig,uint8_t *pu8Buffer, uint16_t len);
void     UART_RcvDataDma(uartconfig_t *uartConfig,uint8_t *pu8Buffer, uint16_t len);
uint8_t  GetRxFifoTrigNum(uint8_t triggerLevel);
void     UART_RcvDataStart(uartconfig_t *uartConfig,uint16_t len);
void     SetUartFclkSource(T_UART *uart, enumModuleFclkSrc_t clkSrc);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* __UART_H__ */

