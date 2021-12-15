
/**
 *****************************************************************************
 * @brief   i2c driver header.
 *
 * @file    i2c.h
 * @author  hc
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

#ifndef __I2C_H
#define __I2C_H

#include "tcasxxx.h"
#include "dma.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/


/*******************************************************************************
 * Definitions 公共(可被其他源文件使用)宏、枚举、结构体的定义
 ******************************************************************************/

#define IIC1_TX_DMA_REQUEST 5
#define IIC1_TX_DMA_CHANNEL 0
#define IIC1_RX_DMA_REQUEST 6
#define IIC1_RX_DMA_CHANNEL 1

#define IIC0_TX_DMA_REQUEST 3
#define IIC0_TX_DMA_CHANNEL 0
#define IIC0_RX_DMA_REQUEST 4
#define IIC0_RX_DMA_CHANNEL 1

typedef struct _i2c_config
{
    T_I2C  *i2c;                       // registers base address
    uint8_t pinmuxOption;
    uint8_t slaveen;                   // slave开关
    uint8_t slaveaddrmode;             // 0: 7-bit address mode  1: 10-bit address mode
    uint8_t slaveaddr;                 // 作为从机时的地址
    uint8_t rate;                      // i2c速率 0-100kbit/s,1-400kbit/s,2-3.4kbit/s
    uint32_t icen;                     // 中断开关
                                        /*
                                        bit17  DPE     dedicate point 中断使能
                                        bit16  SSTE    SCL延伸超时中断使能
                                        bit15  WFBITE  等待总线空闲超时中断使能
                                        bit14  WRFE    等待接收FIFO中断使能
                                        bit13  WTFE    等待发送FIFO中断使能
                                        bit12  ALE     仲裁失败中断使能
                                        bit11  BEE     总线错误中断使能
                                        bit10  DSE     检测STOP中断使能
                                        bit9   TDE     传输结束中断使能
                                        bit8   SAME    从机地址匹配中断使能
                                        bit7   RNE     接收到NACK中断使能
                                        bit6   RODE    接收到一个数据中断使能
                                        bit5   TODE    发送一个数据中断使能
                                        bit4   RFOE    接收FIFO溢出中断使能
                                        bit3   RFFE    接收FIFO满中断使能
                                        bit2   TFOE    发送FIFO溢出中断使能
                                        bit1   TFUE    发送FIFO underrun 中断使能
                                        bit0   TFEE    发送FIFO 空中断使能
                                        */

    uint8_t rx_fifo_threshold;         // 接收FIFO阈值
    uint8_t tx_fifo_threshold;         // 发送FIFO阈值
    dmaconfig_t dmaConfig;
} i2cconfig_t;

/* 外部全局变量的声明 */
/*******************************************************************************
 * API
 ******************************************************************************/

/* 外部函数(可加 extern 修饰)的声明 */


void I2C_ConfigIO(i2cconfig_t *i2ccfg);
uint8_t I2C_SendData(i2cconfig_t *i2ccfg,uint8_t *data,uint16_t Tslaveaddr,uint16_t size,int32_t Timeout);
uint8_t I2C_ReceiveData(i2cconfig_t *i2ccfg,uint16_t Tslaveaddr,uint8_t* rvdata,uint16_t size,int32_t Timeout);
void I2C0_Master_Dma_Init(void);
void I2C1_Master_Dma_Init(void);
void I2C1_Master_Init(void);
void I2C_Init(i2cconfig_t *i2ccfg);
int I2C_GetFlagISR(T_I2C *i2c,uint32_t I2C_FLAG);
int I2C_GetFlagStatus(T_I2C *i2c,uint32_t I2C_FLAG);

void I2C_RxDmaConfig(i2cconfig_t *i2cConfig,uint32_t Slave_addr,uint8_t *pu8Buffer,uint16_t len);
void I2C_TxDmaConfig(i2cconfig_t *i2cConfig,uint32_t Slave_addr,uint8_t *pu8Buffer,uint16_t len);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* __I2C_H */


