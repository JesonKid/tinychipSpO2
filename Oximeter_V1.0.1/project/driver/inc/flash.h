/**
 *****************************************************************************
 * @brief   Flash driver header.
 *
 * @file   flash.h
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
#ifndef __FLASH_H__
#define __FLASH_H__

#include "tcasxxx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

// Main memory (NVM) define
#define  FLASH_BASE_ADDR                   0x10000000
#define  FLASH_MAXSIZE                     0x10000      // 64k

#define  FLASH_SECTOR_SIZE                 (512)

// NVR memory define
#define  NVR_FLASH_BASE_ADDR               0x10800000
#define  NVR_FLASH_MAXSIZE                 0xC00        // 3k, sector 0~5
#define  NVR_FLASH_SECTOR_SIZE             (512)


#define  BANK0_NVM_BASE_ADDR               ((unsigned int)0x10000000UL)
#define  BANK1_NVM_BASE_ADDR               ((unsigned int)0x10080000UL)
#define  BANK0_NVR_BASE_ADDR               ((unsigned int)0x10800000UL)
#define  BANK1_NVR_BASE_ADDR               ((unsigned int)0x10808000UL)
#define  BANK0_CFG_BASE_ADDR               ((unsigned int)0x10880000UL)
#define  BANK1_CFG_BASE_ADDR               ((unsigned int)0x10880040UL)


#define  NVR_SECTOR_SIZE                   (512)
#define  NVR_NUM_ONE_BANK_TC6001           (8)
#define  BANK_NUM_TC6001                   (1)
#define  NVR_NUM_ONE_BANK_TC5820           (3)
#define  BANK_NUM_TC5820                   (2)






/**
  * @brief    FLASH Config RCTC register
  *
  * @param    rcTime : 0~0xf
  *
  * @return   None
  *
  * @details
  */
void Flash_ConfigRCTC(uint8_t rcTime);


/**
  * @brief    FLASH init function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  
  */
void Flash_Init(void);

/**
  * @brief    FLASH Deinit function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  
  */
void Flash_DeInit(void);

/**
 * @brief      Flash page erase
 *
 * @param[in]  wIndex  Flash sector index 
 *
 * @details    To do flash page erase. The target address should be Main memory (NVM) .
 *             The page size is 512 bytes.
 *
 * @retval      0 Success
 * @retval     -1 Erase failed
 */
void Flash_Erase(uint16_t wIndex);

/**
 * @brief       Read 32-bit Data from specified address of flash
 *
 * @param[in]   u32Addr  Flash address include Main memory
 *
 * @return      The data of specified address
 *
 * @details     To read word data from Flash.
 *
 */
uint32_t Flash_Read(uint32_t u32Addr);
uint16_t Flash_Read16(uint32_t u32Addr);

/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  u32Addr  Flash address in Main memory
 * @param[in]  u32Data  32-bit Data to program
 *
 * @return     None
 *
 * @details    To program word data into Main memory
 *
 */
void Flash_Write(uint32_t u32Addr, uint32_t u32Data);
void Flash_Write16(uint32_t u32Addr, uint16_t u16Data);

/**
 * @brief      Flash NVR page erase
 *
 * @param[in]  u8Index - Flash sector index 
 *
 * @details    To do flash page erase. The target address should be NVR memory .
 *             The page size is 512 bytes.
 *
 * @retval      0 Success
 * @retval     -1 Erase failed
 */
int NVR_Erase(uint8_t u8Index);

/**
 * @brief       Read 32-bit Data from specified address of flash
 *
 * @param[in]   u32Addr  Flash address in NVR memory
 *
 * @return      The data of specified address
 *
 * @details     To read word data from NVR memory.
 *
 */
uint32_t NVR_Read(uint32_t u32Addr);

/**
 * @brief      Program 32-bit data into specified address of flash
 *
 * @param[in]  u32Addr  Flash address in NVR memory
 * @param[in]  u32Data  32-bit Data to program
 *
 * @return     None
 *
 * @details    To program word data into NVR memory
 *
 */
void NVR_Write(uint32_t u32Addr, uint32_t u32Data);


#ifdef __cplusplus
}
#endif
#endif /* __FLASH_H__ */


