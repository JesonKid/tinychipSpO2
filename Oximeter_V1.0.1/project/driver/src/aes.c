/**
 *****************************************************************************
 * @brief   aes driver source file.
 *
 * @file   aes.c
 * @author 
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
#include "aes.h"


/*******************************************************************************
* Definitions��˽��(����Դ�ļ���ʹ��)�ꡢö�١��ṹ��Ķ���
******************************************************************************/

/*******************************************************************************
* Variables������ȫ�ֱ���(�ⲿ����̬��������ָ��)�Ķ���
******************************************************************************/
volatile uint8_t g_aesEndFlag = 0;
dmarequest_t aesTxDmaRequest = dmaCryptoTx;
dmarequest_t aesRxDmaRequest = dmaCryptoRx;

/*******************************************************************************
* Prototypes���ڲ�����(�� static ����)������
******************************************************************************/

/*******************************************************************************
* Code�����к���(�ⲿ���ڲ�)�Ķ���
******************************************************************************/


/********************************************************
@brief  AES�жϺ���
@param  void
@retval void
*********************************************************/
static void AesIRQHandler(void)
{
    g_aesEndFlag = 1;
}

/*******************************************************************************
@brief  �ȴ�AESDMA�������
@param  void
@retval void
******************************************************************************/
static void AesWaitActionDone()
{
    uint16_t i = 0xFFFF;
    while(i--)
    {            
        if((DMA->DCSR[AES_RX_DMA_CHANNEL]&0x80000000UL) == 0) /* �ȴ�dma������� */
            break;
    }
    if (i == 0)
        printf("AES act timeout...\r\n");
    CRYPTO->SPICFG_CLKRST_CTRL_f.start = 0;
}

/*******************************************************************************
@brief  BUFF��AESģ��FIFO dma���ú���
@param  
        aesReg��AESģ��Ĵ�����ַ
        pu8Buffer��DMA������Դ��ַ
        len����������ݳ���
@retval void
******************************************************************************/
static void AestxDmaConfig(T_CRYPTO* aesReg,uint8_t *pu8Buffer,uint16_t len)
{
    //  master dma config
    // dma request setting
    DMA->DRCMR[aesTxDmaRequest] = (AES_TX_DMA_CHANNEL | 0x80);
    // dma channel setting
    // clear all interrupt bits and set as no-descriptor-fetch mode
    DMA->DCSR[AES_TX_DMA_CHANNEL] = 0x4000021F;
    // byte aligned
   // DMA->DALGN[AES_TX_DMA_CHANNEL] = 0x01;
    // source address
    DMA->DESC[AES_TX_DMA_CHANNEL].DSADR = (uint32_t)(pu8Buffer);
    // destination address
    DMA->DESC[AES_TX_DMA_CHANNEL].DTADR = ((uint32_t)(&(aesReg->ififo)));
    // incsrcaddr, flowtrg, burst size = 8 bytes, width = 3 byte
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD = 0x90014000;
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.LEN = (len&0x1fff);
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.BURST = 2;     // 16 bytes
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.WIDTH = 3;     // word (4 bytes)
    DMA_StartChannel(AES_TX_DMA_CHANNEL);
}

/*******************************************************************************
@brief  AESģ��FIFO��BUFF dma���ú���
@param  
        aesReg��AESģ��Ĵ�����ַ
        pu8Buffer��DMA������Ŀ���ַ
        len����������ݳ���
@retval void
******************************************************************************/
static void AesrxDmaConfig(T_CRYPTO* aesReg,uint8_t *pu8Buffer,uint16_t len)
{
    //  master dma config
    // dma request setting
    DMA->DRCMR[aesRxDmaRequest] = (AES_RX_DMA_CHANNEL | 0x80);
    // dma channel setting
    // clear all interrupt bits and set as no-descriptor-fetch mode
    DMA->DCSR[AES_RX_DMA_CHANNEL] = 0x4000021F;
    // byte aligned
    //DMA->DALGN[i2cConfig->dma_config.rx_ch] = 0x00;
    // source address
    DMA->DESC[AES_RX_DMA_CHANNEL].DSADR = ((uint32_t)(&(aesReg->ofifo)));
    // destination address
    DMA->DESC[AES_RX_DMA_CHANNEL].DTADR = (uint32_t)(pu8Buffer);
    // inctrgaddr, flowtrg, burst size = 16 bytes, width = 3 byte
    DMA->DESC[AES_RX_DMA_CHANNEL].DCMD = 0x60014000;
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.LEN = (len&0x1fff);
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.BURST = 2;     // 16 bytes
    DMA->DESC[AES_TX_DMA_CHANNEL].DCMD_f.WIDTH = 3;     // word (4 bytes)

    DMA_StartChannel(AES_RX_DMA_CHANNEL);
}

/*******************************************************************************
@brief  AESģ��START
@param  void
@retval void
******************************************************************************/
void AesStart(void)
{
    IC_PowupInit();
    XIC_EnableIRQ(FIC_Aes_IRQn,AesIRQHandler);
    IC_EnableNVIC();
}

/*******************************************************************************
@brief  AESģ��END
@param  void
@retval void
******************************************************************************/
void AesEnd(void)
{
    IC_PowupDeinit();
    IC_DisableNVIC();
}

/*******************************************************************************
@brief  Sha256��֤����
@param  
        aesReg��AESģ��Ĵ�����ַ
        plainData�� ԭʼ����
        cipherData����֤����
        len:ԭʼ���ݳ���
@retval void
******************************************************************************/
void AesSha256(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint16_t len)/* max len = 0x1fff */
{
    aesReg->SPICFG_CLKRST_CTRL_f.mode = 0x05;
    aesReg->SPICFG_CLKRST_CTRL_f.enc_dec = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.burst = 1;
    aesReg->pbyte_l = (len*8);
    aesReg->pbyte_h = 0;
    AestxDmaConfig(aesReg,(uint8_t *)plainData,len);
    g_aesEndFlag = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.start = 1;
    AesWaitActionDone();
    memcpy(cipherData,(const void*)&(aesReg->sha_out_lll),32);  
}

/*******************************************************************************
@brief  Ecb���ܺ���
@param  
        aesReg��AESģ��Ĵ�����ַ
        plainData�� ��������
        cipherData����������
        key��key��Կ
        len:�������ݳ���
@retval void
******************************************************************************/
void AesEcbEnc(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint16_t len)  //ecb128 ���ݷ�����128��key = 128 /* max len = 0x1fff */
{
    aesReg->SPICFG_CLKRST_CTRL_f.mode = 0x00;
    aesReg->SPICFG_CLKRST_CTRL_f.enc_dec = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.burst = 2;
    aesReg->pbyte_l = len;
    aesReg->pbyte_h = 0;
    memcpy((uint8_t*)&(aesReg->key_ll),key,16);
    aesReg->SPICFG_CLKRST_CTRL_f.padding_mode = 4;/* nopadding if pbyte[6:0]==0 */
    AestxDmaConfig(aesReg,(uint8_t*)plainData,len);
    g_aesEndFlag = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.start = 1;
    AesrxDmaConfig(aesReg,(uint8_t*)cipherData,len);
    AesWaitActionDone();
}

/*******************************************************************************
@brief  Ecb���ܺ���
@param  
        aesReg��AESģ��Ĵ�����ַ
        plainData�� ��������
        cipherData����������
        key��key��Կ
        len:�������ݳ���
@retval void
******************************************************************************/
void AesEcbDecode(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint16_t len)  //ecb128 ���ݷ�����128��key = 128 /* max len = 0x1fff */
{
    aesReg->SPICFG_CLKRST_CTRL_f.mode = 0x00;
    aesReg->SPICFG_CLKRST_CTRL_f.enc_dec = 1;
    aesReg->SPICFG_CLKRST_CTRL_f.burst = 2;
    aesReg->pbyte_l = len;
    aesReg->pbyte_h = 0;
    memcpy((void *)&(aesReg->key_ll),(void *)key,16);
    aesReg->SPICFG_CLKRST_CTRL_f.padding_mode = 4;
    AestxDmaConfig(aesReg,(uint8_t*)cipherData,len);
    g_aesEndFlag = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.start = 1;
    AesrxDmaConfig(aesReg,(uint8_t*)plainData,len);
    AesWaitActionDone();
}

/*******************************************************************************
@brief  cbc���ܺ���
@param  
        aesReg��AESģ��Ĵ�����ַ
        plainData�� ��������
        cipherData����������
        key��key��Կ
        iv:  iv����
        len:�������ݳ���
@retval void
******************************************************************************/
void AesCbcEnc(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint32_t *iv,uint16_t len) 
{
    aesReg->SPICFG_CLKRST_CTRL_f.mode = 0x01;
    aesReg->SPICFG_CLKRST_CTRL_f.enc_dec = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.burst = 2;
    aesReg->pbyte_l = len;
    aesReg->pbyte_h = 0;
    memcpy((void *)&(aesReg->key_ll),(void *)key,16);
    memcpy((void *)&(aesReg->iv_ll),(void *)iv,16);
    aesReg->SPICFG_CLKRST_CTRL_f.padding_mode = 4;
    AestxDmaConfig(aesReg,(uint8_t*)plainData,len);
    g_aesEndFlag = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.start = 1;
    AesrxDmaConfig(aesReg,(uint8_t*)cipherData,len);
    AesWaitActionDone();
}

/*******************************************************************************
@brief  cbc���ܺ���
@param  
        aesReg��AESģ��Ĵ�����ַ
        plainData�� ��������
        cipherData����������
        key��key��Կ
        iv:  iv����
        len:�������ݳ���
@retval void
******************************************************************************/
void AesCbcDecode(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint32_t *iv,uint16_t len)  
{
    aesReg->SPICFG_CLKRST_CTRL_f.mode = 0x01;
    aesReg->SPICFG_CLKRST_CTRL_f.enc_dec = 1;
    aesReg->SPICFG_CLKRST_CTRL_f.burst = 2;
    aesReg->pbyte_l = len;
    aesReg->pbyte_h = 0;
    memcpy((uint8_t*)&(aesReg->key_ll),key,16);
    memcpy((uint8_t*)&(aesReg->iv_ll),iv,16);
    aesReg->SPICFG_CLKRST_CTRL_f.padding_mode = 4;
    AestxDmaConfig(aesReg,(uint8_t*)cipherData,len);
    g_aesEndFlag = 0;
    aesReg->SPICFG_CLKRST_CTRL_f.start = 1;
    AesrxDmaConfig(aesReg,(uint8_t*)plainData,len);
    AesWaitActionDone();
}
