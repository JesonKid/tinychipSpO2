/**
 *****************************************************************************
 * @brief   aes driver header file.
 *
 * @file   aes.h
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
#ifndef __AES_H__
#define __AES_H__

#include "tcasxxx.h"
#include "dma.h"
#include "interrupt.h"

/*******************************************************************************
* Definitions：
******************************************************************************/
/* 公共(可被其他源文件使用)宏、枚举、结构体的定义 */
#define AES_TX_DMA_CHANNEL    0
#define AES_RX_DMA_CHANNEL    1

/* 外部全局变量的声明 */


/*******************************************************************************
* API：外部函数(可加 extern 修饰)的声明
******************************************************************************/
#if defined(__cplusplus)
    extern "C" {
#endif /*_cplusplus*/

void AesStart(void);
void AesEnd(void);
void AesSha256(T_CRYPTO* aesReg,uint32_t *data,uint32_t *shaOut,uint16_t len); 
void AesEcbDecode(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint16_t len);
void AesEcbEnc(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint16_t len);
void AesCbcEnc(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint32_t *iv,uint16_t len);
void AesCbcDecode(T_CRYPTO* aesReg,uint32_t *plainData,uint32_t *cipherData,uint32_t *key,uint32_t *iv,uint16_t len);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif/* __AES_H__ */

