/**
 *****************************************************************************
 * @brief   spi driver header.
 *
 * @file    spi.h
 * @author  hc
 * @date    28/JUN/2020
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


#ifndef __SPI_H
#define __SPI_H

#include <stdio.h>
#include "tcasxxx.h"
#include <stdbool.h> 
#include "DMA.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/


/*******************************************************************************
* Definitions：
******************************************************************************/
/* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */
typedef struct {
    T_SPI *spi;                           /* spi寄存器基地址 */
    bool slave;                           /* 0:使用主机功能 1：使用从机功能*/
    bool loopback;                        /* 0:禁用loopback功能 1:使能loopback功能*/
    bool CPOL;                            /* 时钟极性 */
    bool CPHA;                            /* 时钟相位 */
    uint8_t frf;                          /*帧格式
                                          00: Motorola SPI frame format.
                                          01: TI synchronous serial frame format.
                                          10: National Microwire frame format.
                                          11: Reserved, undefined operation.
                                          */
    uint8_t frame_size;
    uint8_t imsc;                         /* 中断使能配置 BIT0/BIT1/BIT2/BIT3 置1 分别使能 RORIM/RTIM/RXIM/TXIM 中断*/
    dmaconfig_t dmaConfig;
    uint8_t     pinmuxOption;             /* 引脚复用选项 */
} SPI_Config ;

/* Definitions */
#define SPI_POLLING_MODE    1
#define SPI_INT_MODE        2
#define SPI_DMA_MODE        3

#define SPI_IRQ_ENABLE      4
#define SPI_IRQ_DISABLE     5

/* SPI read and write buffer size */
#define SPI_BUFSIZE     16
#define FIFOSIZE        8

#define DELAY_COUNT     10
#define MAX_TIMEOUT     0xFF

/* PortXX is the SPI select pin */
#define SPI0_SEL        (0x1<<2)
    
/* SPI Status register */
#define SPISR_TFE       (0x1<<0)
#define SPISR_TNF       (0x1<<1) 
#define SPISR_RNE       (0x1<<2)
#define SPISR_RFF       (0x1<<3) 
#define SPISR_BSY       (0x1<<4)

/* SPI CR0 register */
#define SPICR0_DSS      (0x1<<0)
#define SPICR0_FRF      (0x1<<4)
#define SPICR0_SPO      (0x1<<6)
#define SPICR0_SPH      (0x1<<7)
#define SPICR0_SCR      (0x1<<8)

/* SPI CR1 register */
#define SPICR1_LBM      (0x1<<0)
#define SPICR1_SSE      (0x1<<1)
#define SPICR1_MS       (0x1<<2)
#define SPICR1_SOD      (0x1<<3)

/* SPI Interrupt Mask Set/Clear register */
#define SPIIMSC_RORIM   (0x1<<0)
#define SPIIMSC_RTIM    (0x1<<1)
#define SPIIMSC_RXIM    (0x1<<2)
#define SPIIMSC_TXIM    (0x1<<3)

/* SPI0 Interrupt Status register */
#define SPIRIS_RORRIS   (0x1<<0)
#define SPIRIS_RTRIS    (0x1<<1)
#define SPIRIS_RXRIS    (0x1<<2)
#define SPIRIS_TXRIS    (0x1<<3)

/* SPI0 Masked Interrupt register */
#define SPIMIS_RORMIS   (0x1<<0)
#define SPIMIS_RTMIS    (0x1<<1)
#define SPIMIS_RXMIS    (0x1<<2)
#define SPIMIS_TXMIS    (0x1<<3)

/* SPI0 Interrupt clear register */
#define SPIICR_RORIC    (0x1<<0)
#define SPIICR_RTIC     (0x1<<1)

/* RDSR status bit definition */
#define RDSR_RDY        0x01
#define RDSR_WEN        0x02

/* 外部全局变量的声明 */


/*******************************************************************************
* API：外部函数(可加 extern 修饰)的声明
******************************************************************************/

void SPI_Init(SPI_Config config);
int  SPI_GetFlagStatus(T_SPI *spi,uint16_t SPI_FLAG);
void SPI_SendData_Host(T_SPI *spi, uint32_t *buffer, uint32_t size);
void SPI_SendData_Slave(T_SPI *spi, uint32_t *buffer, uint32_t size);
void SPI_ReceiveData_Slave(T_SPI *spi, uint32_t *buffer, uint32_t size);
void SPI_ReceiveData_Host(T_SPI *spi, uint32_t *buffer, uint32_t size);
void SpiIOConfig(T_SPI *ssp);
void SPI_SetRate(T_SPI *ssp,uint8_t SCR,uint8_t CPSDVR);

void SPI_Receive_DMAConfig(SPI_Config *config,uint32_t *buffer, uint16_t len);
void SPI_Send_DMAConfig(SPI_Config *config,uint32_t *buffer, uint16_t len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SPI_H */

