/**
 *****************************************************************************
 * @brief   dma driver header.
 *
 * @file   dma.h
 * @author
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
#ifndef __DMA_H__
#define __DMA_H__

#include "tcasxxx.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*******************************************************************************
* Definitions
******************************************************************************/
/* ����(�ɱ�����Դ�ļ�ʹ��)�ꡢö�١��ṹ��Ķ��� */

/* DMA request list */
typedef enum
{
    /* dreq0--2 */
    dmaRequest1 = 0,
    dmaRequest2,
    dmaRequest3,
    
    /* preq 3--31 */    
    dmaI2c0Tx = 3,
    dmaI2c0Rx,
    dmaI2c1Tx,
    dmaI2c1Rx,
    dmaSpi0Tx,
    dmaSpi0Rx,
    dmaSpi1Tx,
    dmaSpi1Rx,
    dmaUart0Tx,
    dmaUart0Rx,
    dmaUart1Tx,
    dmaUart1Rx,
    dmaCryptoTx,
    dmaCryptoRx,
    dmaLpuartTx,
    dmaLpuartRx,
    dmaAuartTx,
    dmaAuartRx,
    dmaSaradc_rx,
    dmaVdac_tx,
    dmaSddac_rx,        // 23

    dmaMaxRequest
} dmarequest_t;

typedef struct _dma_config {
  int8_t                txChannel;                                 /* Transmit channel number */
  dmarequest_t          txRequest;                                 /* Transmit DMA request number */
  uint8_t               txDmaEn;                                   /* ��1ʹ������DMA���� */
  uint8_t               txBurst;                                   /* 0x0 = 8 bytes,0x1 = 8 bytes,0x2 = 16 bytes,0x3 = 32 bytes,0x4 = 64 bytes,0x5~0x7=Reserved */
  uint8_t               txWidth;                                   /* 0x0 = Reserved for for memory-to-memory 0x1 = 1 byte  0x2 = half-word (2 bytes) 0x3 = word (4 bytes) */
  uint8_t               txDalgn;                                   /* ���뷽ʽ, 0��Ŀ���Դ��ַ���뷽ʽΪĬ��(�ڲ�����Ĭ��4 byte ����,�ⲿ���ߵ�ַĬ��8 byte ����) 1��Ŀ���Դ��ַ���뷽ʽ���û��Զ���(byte aligned) */
  uint8_t               txEndIrq;                                  /* 0:�ر�DMA��������ж� 1:��DMA��������ж� */

  uint8_t               rxChannel;                                 /* Receive channel number */
  dmarequest_t          rxRequest;                                 /* Receive DMA request number */
  uint8_t               rxDmaEn;                                   /* ��1ʹ������DMA���� */
  uint8_t               rxBurst;                                   /* 0x0 = 8 bytes,0x1 = 8 bytes,0x2 = 16 bytes,0x3 = 32 bytes,0x4 = 64 bytes,0x5~0x7=Reserved */
  uint8_t               rxWidth;                                   /* 0x0 = Reserved for for memory-to-memory 0x1 = 1 byte  0x2 = half-word (2 bytes) 0x3 = word (4 bytes) */
  uint8_t               rxDalgn;                                   /* ���뷽ʽ, 0��Ŀ���Դ��ַ���뷽ʽΪĬ��(�ڲ�����Ĭ��4 byte ����,�ⲿ���ߵ�ַĬ��8 byte ����) 1��Ŀ���Դ��ַ���뷽ʽ���û��Զ���(byte aligned) */
  uint8_t               rxEndIrq;                                  /* 0:�ر�DMA��������ж� 1:��DMA��������ж� */
} dmaconfig_t;


/*******************************************************************************
* API  �ⲿ����(�ɼ� extern ����)������
******************************************************************************/

void DMA_StartChannel(uint32_t channel);
void DMA_StopChannel(uint32_t channel);
void DMA_Init(void);
void PeripheralsRxDmaConfig(uint32_t *targetAddress,uint32_t *sourceFIFO,uint16_t len,uint8_t rxDmaChannel,dmarequest_t rxDmareq,uint8_t align,uint8_t width,uint8_t burst,uint8_t endIrq);
void PeripheralsTxDmaConfig(uint32_t *targetFIFO,uint32_t *sourceAddress,uint16_t len,uint8_t txDmaChannel,dmarequest_t txDmareq,uint8_t align,uint8_t width,uint8_t burst,uint8_t endIrq);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* __DMA_H__ */

