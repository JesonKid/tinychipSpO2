/**
 *****************************************************************************
 * @brief   Flash driver source file.
 *
 * @file   flash.c
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
#include "stdio.h"
#include "tcasxxx.h"
#include "flash.h"



/**
  * @brief    FLASH Config RCTC register
  *
  * @param    rcTime : 0~0xf
  *
  * @return   None
  *
  * @details
  */
#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
 void Flash_ConfigRCTC(uint8_t rcTime)
 {
    uint32_t rctcData;
    
    rctcData = M0P_FLASH->RCTC;
    rctcData = M0P_FLASH->RCTC;
    //M0P_FLASH->RCTC_f.cfs = 0x3;    /** <5:4>ADDRESS/DIN/CEb to CONFEN setup time (HCLK), reset:3*/
    //M0P_FLASH->RCTC_f.cfh = 0x3;    /** <7:6>ADDRESS/DIN/CEb to CONFEN hold time (HCLK), reset:3 */
    //M0P_FLASH->RCTC_f.confen = 0x3; /** <9:8>CONFEN pulse width (HCLK) , reset:3*/
    rctcData &= 0xfff0;
    rctcData |= rcTime;
    M0P_FLASH->RCTC = rctcData;      /** Read time (HCLK) ,  reset:1*/
}

/**
  * @brief    FLASH init function
  *
  * @param    None
  *
  * @return   None
  *
  * @details
  */
 
void Flash_Init(void)
{
    Flash_ConfigRCTC(1);
    M0P_FLASH->WCTC2 = 0xB013DAB7;
}

/**
  * @brief    FLASH Deinit function
  *
  * @param    None
  *
  * @return   None
  *
  * @details  
  */
void Flash_DeInit(void)
{
	M0P_FLASH->WCTC2 = 0;
}

/**
 * @brief      Flash page erase
 *
 * @param[in]  u32Addr  Flash address 
 *
 * @details    To do flash page erase. The target address should be Main memory (NVM) .
 *             The page size is 512 bytes.
 *
 * @retval      0 Success
 * @retval     -1 Erase failed
 */
#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
void Flash_Erase(uint16_t wIndex)
{
	__disable_irq();
	
    /* erase sector */
    M0P_FLASH->ECONF_f.nvr_select = 0x0;        /* Main memory (NVM) selected */
    M0P_FLASH->ECONF_f.sector_index = wIndex;
    M0P_FLASH->ECOMM_f.sector_erase = 1;        /** Command of sector erase operation
                                                    - Write 1 to erase the sector/block specified in the register of econf. */
    while(M0P_FLASH->MCR_f.erase_busy_0 == 1);
	
	__enable_irq();
}


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
uint32_t Flash_Read(uint32_t u32Addr)
{
    uint32_t nReadData = 0; 
    
    if(u32Addr > FLASH_MAXSIZE)
    {
        return 0;
    }
    M0P_FLASH->ECONF_f.nvr_select = 0x0;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint32_t *)(FLASH_BASE_ADDR + u32Addr);
    
    return nReadData;
}

uint16_t Flash_Read16(uint32_t u32Addr)
{
    uint16_t nReadData = 0; 
    
    if(u32Addr > FLASH_MAXSIZE)
    {
        return 0;
    }
    M0P_FLASH->ECONF_f.nvr_select = 0x0;        /* Main memory (NVM) selected */
    nReadData = *(volatile uint16_t *)(FLASH_BASE_ADDR + u32Addr);
    
    return nReadData;
}

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
#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
void Flash_Write(uint32_t u32Addr, uint32_t u32Data)
{
	__disable_irq();
	
	M0P_FLASH->ECONF_f.nvr_select = 0x0;        /* Main memory (NVM) selected */
	
	*(volatile uint32_t *)(u32Addr) = u32Data;
	
    while(M0P_FLASH->MCR_f.program_busy_0 == 1);
	
	__enable_irq();
}


#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
/*
void Flash_Write16(uint32_t u32Addr, uint16_t u16Data)
{
    if(u32Addr > FLASH_MAXSIZE)
    {
        return;
    }
    M0P_FLASH->ECONF_f.nvr_select = 0x0;        // Main memory (NVM) selected

    *(volatile uint16_t *)(FLASH_BASE_ADDR + u32Addr) = u16Data;
    
    while(M0P_FLASH->MCR_f.program_busy_0 == 1);
}
*/


/////////////////////////////////////////////////////////////////////////
// NVR memory operation interface
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
#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
int NVR_Erase(uint8_t u8Index)
{
    char error = 0;
    int i = 0;

    if(u8Index > (NVR_FLASH_MAXSIZE / NVR_FLASH_SECTOR_SIZE))
    {
        return -1;
    }

    /* erase sector */
    M0P_FLASH->ECONF_f.nvr_select = 1;          /* Main memory (NVM) selected */
    //M0P_FLASH->ECONF_f.block_select = 0x0;      /* Sector erase */
    M0P_FLASH->ECONF_f.sector_index = u8Index;
    M0P_FLASH->ECOMM_f.sector_erase = 1;        /** Command of sector erase operation
                                                    - Write 1 to erase the sector/block specified in the register of econf. */
    
    while(M0P_FLASH->MCR_f.erase_busy_0 == 1);

    for (i = 0; i < NVR_FLASH_SECTOR_SIZE; i += 4)
    {
        if (*(volatile uint32_t *)(NVR_FLASH_BASE_ADDR + (u8Index*NVR_FLASH_SECTOR_SIZE) + i) != 0xffffffff)
        {
            error++;
        }
    }
    if(error != 0)
    {
        return -1;
    }
    return 0;
}

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
uint32_t NVR_Read(uint32_t u32Addr)
{
    uint32_t nReadData = 0; 
    
    if(u32Addr > NVR_FLASH_MAXSIZE)
    {
        return 0;
    }
    M0P_FLASH->ECONF_f.nvr_select = 1;        /* NVR selected */
    nReadData = *(volatile uint32_t *)(NVR_FLASH_BASE_ADDR + u32Addr);
    
    return nReadData;
}

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
#if defined   ( __ICCARM__   ) /* iar */
__ramfunc 
#elif defined ( __CC_ARM )     /* keil */
__attribute__((section("RAMCODE")))
#endif
void NVR_Write(uint32_t u32Addr, uint32_t u32Data)
{
    if(u32Addr > NVR_FLASH_MAXSIZE)
    {
        return;
    }
    M0P_FLASH->ECONF_f.nvr_select = 1;          /* NVR selected */

    *(volatile uint32_t *)(NVR_FLASH_BASE_ADDR + u32Addr) = u32Data;
    
    while((M0P_FLASH->MCR_f.program_busy_0 == 1) || (M0P_FLASH->MCR_f.program_busy_1 == 1));
}


