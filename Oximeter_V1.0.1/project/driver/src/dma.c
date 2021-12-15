/**
 *****************************************************************************
 * @brief  DMA source file.
 *
 * @file   dma.c
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
#include "dma.h"
#include "interrupt.h"

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
@brief  MEM->MEM dma���亯��
@param  void
@retval void
*********************************************************/
void DMA_M2MTransferSetup(uint32_t channel, void *srcBaseAddr, void *dstBaseAddr, uint32_t totoalLength)
{
    uint32_t command;

    // set as no-descriptor-fetch mode
    DMA->DCSR[channel] = 0x40000000UL;

    DMA->DESC[channel].DSADR = (uint32_t)srcBaseAddr;
    DMA->DESC[channel].DTADR = (uint32_t)dstBaseAddr;

    // increment source and target address
    // no flow control
    // enable END IRQ
    // burst size = 64 bytes
    // width = 0 for M2M transfer
    command = 0xC0240000UL;
    command |= (totoalLength & 0x1FFFUL);

    DMA->DESC[channel].DCMD = command;

}


/********************************************************
@brief  ����DMA channel �Ĵ���
@param  channel:DMA����ͨ��0~7
@retval void
*********************************************************/
void DMA_StartChannel(uint32_t channel)
{
    //set run bit
    DMA->DCSR[channel] |= 0x80000000UL;
}

void DMA_StopChannel(uint32_t channel)
{
    // clear out all status bits and run bit, w1c bit will be cleared when write back
    DMA->DCSR[channel] &= ~0x80000000UL;
}


/********************************************************
@brief  ��ʼ������
@param  void
@retval void
*********************************************************/
void DMA_Init(void)
{
    int i;

    //clear mapping
    for(i = 0; i < DMA_NUMBER_OF_DEVICES_SUPPORTED; i++)
    {
        DMA->DRCMR[i] = 0;
    }

    //clear registers
    for(i = 0; i < DMA_NUMBER_OF_HW_CHANNEL_SUPPORTED; i++)
    {
        // first write back to clear w1c bits
        DMA->DCSR[i] = DMA->DCSR[i];
        // then clear enable bits and compare status
        DMA->DCSR[i] = 0x01000000UL;
        DMA->DESC[i].DDADR = 0; 
        DMA->DESC[i].DSADR = 0;
        DMA->DESC[i].DTADR = 0;
        DMA->DESC[i].DCMD  = 0;
        // set aligenment to be not aligned
        DMA->DALGN[i] = 1U;
    }
}

/********************************************************
@brief  �������DMA����
@param  targetAddress��Ŀ���ַ
        sourceFIFO:����FIFO
        len:dma����ĳ���
        rxDmaChannel:DMA����ͨ��
        rxDmareq:dam����request
        align:���뷽ʽ
                0 = Source and target addresses of channel x are default
                aligned (internal peripherals default to 4 byte alignment,
                external bus addresses default to 8 byte alignment)
                1 = Source and target addresses of channel x are as
                defined by user (byte aligned)
        width:��Ӧ����FIFO�Ŀ��
        burst:һ��burst���䳤�ȣ�
                Width of the on-chip peripheral
                This field is reserved for operations that do not involve
                on-chip peripherals, such as memory-to-memory moves
                and companion-chip-related operations.
                WIDTH must be 0x0 for memory-to-memory moves or
                companion-chip-related operations.
                0x0 = Reserved for on-chip peripheral-related transactions
                0x1 = 1 byte
                0x2 = half-word (2 bytes)
                0x3 = word (4 bytes)
                0x4 = 64 bytes
                0x5~0x7=Reserved
                The size must be less than or equal to the serviced
                peripheral FIFO trigger threshold to properly handle the
                respective FIFO trailing bytes.
        endIrq:0��ʾ�رս����ж�        
               1��ʾʹ�ܽ����ж�
@retval void
*********************************************************/
void PeripheralsRxDmaConfig(uint32_t *targetAddress,uint32_t *sourceFIFO,uint16_t len,uint8_t rxDmaChannel,dmarequest_t rxDmareq,uint8_t align,uint8_t width,uint8_t burst,uint8_t endIrq)
{
    // master dma config
    // dma request setting
    DMA->DRCMR[rxDmareq] = (rxDmaChannel | 0x80);
    // dma channel setting
    // clear all interrupt bits and set as no-descriptor-fetch mode
    DMA->DCSR[rxDmaChannel] = 0x4000021F;
    // align
    DMA->DALGN[rxDmaChannel] = align;
    // source address
    DMA->DESC[rxDmaChannel].DSADR = ((uint32_t)(sourceFIFO));
    // destination address
    DMA->DESC[rxDmaChannel].DTADR = ((uint32_t)(targetAddress));
    // inctrgaddr, flowtrg, burst size = 8 bytes, width = 4 byte
    DMA->DESC[rxDmaChannel].DCMD = 0x60000000;
    DMA->DESC[rxDmaChannel].DCMD_f.LEN = (len&0x1fff);
    DMA->DESC[rxDmaChannel].DCMD_f.BURST = burst;
    DMA->DESC[rxDmaChannel].DCMD_f.WIDTH = width;
    DMA->DESC[rxDmaChannel].DCMD_f.ENDIRQEN = endIrq;
    
    DMA_StartChannel(rxDmaChannel);
}

/********************************************************
@brief  ���跢��DMA����
@param  targetFIFO�������FIFO
        sourceAddress:������FIFO���ݵ�BUFF
        len:dma����ĳ���
        txDmaChannel:DMA����ͨ��
        txDmareq:dam����request
        align:���뷽ʽ
                0 = Source and target addresses of channel x are default
                aligned (internal peripherals default to 4 byte alignment,
                external bus addresses default to 8 byte alignment)
                1 = Source and target addresses of channel x are as
                defined by user (byte aligned)
        width:��Ӧ����FIFO�Ŀ��
        burst:һ��burst���䳤�ȣ�
                Width of the on-chip peripheral
                This field is reserved for operations that do not involve
                on-chip peripherals, such as memory-to-memory moves
                and companion-chip-related operations.
                WIDTH must be 0x0 for memory-to-memory moves or
                companion-chip-related operations.
                0x0 = Reserved for on-chip peripheral-related transactions
                0x1 = 1 byte
                0x2 = half-word (2 bytes)
                0x3 = word (4 bytes)
                0x4 = 64 bytes
                0x5~0x7=Reserved
                The size must be less than or equal to the serviced
                peripheral FIFO trigger threshold to properly handle the
                respective FIFO trailing bytes.
        endIrq:0��ʾ�رս����ж�        
               1��ʾʹ�ܽ����ж�
@retval void
*********************************************************/
void PeripheralsTxDmaConfig(uint32_t *targetFIFO,uint32_t *sourceAddress,uint16_t len,uint8_t txDmaChannel,dmarequest_t txDmareq,uint8_t align,uint8_t width,uint8_t burst,uint8_t endIrq)
{
    // dma request setting
    DMA->DRCMR[txDmareq] = (txDmaChannel | 0x80);
    // dma channel setting
    // clear all interrupt bits and set as no-descriptor-fetch mode
    DMA->DCSR[txDmaChannel] = 0x4000021F;
    // byte aligned
    DMA->DALGN[txDmaChannel] = align;
    // source address
    DMA->DESC[txDmaChannel].DSADR = (uint32_t)(sourceAddress);
    // destination address
    DMA->DESC[txDmaChannel].DTADR = ((uint32_t)(targetFIFO));
    // incsrcaddr, flowtrg, burst size = 8 bytes, width = 1 byte
    DMA->DESC[txDmaChannel].DCMD = 0x90000000;

    DMA->DESC[txDmaChannel].DCMD_f.LEN = (len&0x1fff);
    DMA->DESC[txDmaChannel].DCMD_f.BURST = burst;
    DMA->DESC[txDmaChannel].DCMD_f.WIDTH = width;
    DMA->DESC[txDmaChannel].DCMD_f.ENDIRQEN = endIrq;

    DMA_StartChannel(txDmaChannel);
}


