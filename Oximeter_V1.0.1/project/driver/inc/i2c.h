
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
 * Definitions ����(�ɱ�����Դ�ļ�ʹ��)�ꡢö�١��ṹ��Ķ���
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
    uint8_t slaveen;                   // slave����
    uint8_t slaveaddrmode;             // 0: 7-bit address mode  1: 10-bit address mode
    uint8_t slaveaddr;                 // ��Ϊ�ӻ�ʱ�ĵ�ַ
    uint8_t rate;                      // i2c���� 0-100kbit/s,1-400kbit/s,2-3.4kbit/s
    uint32_t icen;                     // �жϿ���
                                        /*
                                        bit17  DPE     dedicate point �ж�ʹ��
                                        bit16  SSTE    SCL���쳬ʱ�ж�ʹ��
                                        bit15  WFBITE  �ȴ����߿��г�ʱ�ж�ʹ��
                                        bit14  WRFE    �ȴ�����FIFO�ж�ʹ��
                                        bit13  WTFE    �ȴ�����FIFO�ж�ʹ��
                                        bit12  ALE     �ٲ�ʧ���ж�ʹ��
                                        bit11  BEE     ���ߴ����ж�ʹ��
                                        bit10  DSE     ���STOP�ж�ʹ��
                                        bit9   TDE     ��������ж�ʹ��
                                        bit8   SAME    �ӻ���ַƥ���ж�ʹ��
                                        bit7   RNE     ���յ�NACK�ж�ʹ��
                                        bit6   RODE    ���յ�һ�������ж�ʹ��
                                        bit5   TODE    ����һ�������ж�ʹ��
                                        bit4   RFOE    ����FIFO����ж�ʹ��
                                        bit3   RFFE    ����FIFO���ж�ʹ��
                                        bit2   TFOE    ����FIFO����ж�ʹ��
                                        bit1   TFUE    ����FIFO underrun �ж�ʹ��
                                        bit0   TFEE    ����FIFO ���ж�ʹ��
                                        */

    uint8_t rx_fifo_threshold;         // ����FIFO��ֵ
    uint8_t tx_fifo_threshold;         // ����FIFO��ֵ
    dmaconfig_t dmaConfig;
} i2cconfig_t;

/* �ⲿȫ�ֱ��������� */
/*******************************************************************************
 * API
 ******************************************************************************/

/* �ⲿ����(�ɼ� extern ����)������ */


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


