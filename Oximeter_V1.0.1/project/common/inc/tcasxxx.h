/**
 *****************************************************************************
 * @brief   Device Peripheral Access Layer Header File.
 *          This file contains all the peripheral register's definitions, bits
 *          definitions and memory mapping for TCASxxx chip.
 *
 * @file   tcasxxx.h
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
#ifndef __TCASXXX_H__
#define __TCASXXX_H__

#include "base_types.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


/**
  * @brief TCAS SDK version number V1.0.0
  */
#define __TCAS_SDK_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __TCAS_SDK_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __TCAS_SDK_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __TCAS_SDK_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __TCAS_SDK_VERSION        ((__TCAS_SDK_VERSION_MAIN << 24)\
                                  |(__TCAS_SDK_VERSION_SUB1 << 16)\
                                  |(__TCAS_SDK_VERSION_SUB2 << 8 )\
                                  |(__TCAS_SDK_VERSION_RC))



//#define __CHECK_DEVICE_DEFINES
typedef enum IRQn
{
    /******  Cortex-M# Processor Exceptions Numbers ***************************************************/

    /* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M4 device                   */
    NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
    HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
    SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
    PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
    SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

    /******  Device Specific Interrupt Numbers ********************************************************/
    /* ToDo: add here your device specific external interrupt numbers
             according the interrupt handlers defined in startup_Device.s
             eg.: Interrupt for Timer#1       TIM1_IRQHandler   ->   TIM1_IRQn                        */

    Fic_IRQn                     = 0,
    Lic_IRQn                     = 1,
    Aic_IRQn                     = 2,

    /* FIC Interrupt Numbers  */
    FIC_Ovf_IRQn                  = 32, //0
    FIC_Flash_Error_IRQn          = 33, //1
    FIC_Flash_Program_IRQn        = 34, //2
    FIC_Flash_Erase_IRQn          = 35, //3
    FIC_Aes_IRQn                  = 36, //4
    FIC_Sha_IRQn                  = 37, //5
    FIC_Trng_IRQn                 = 38, //6
    FIC_Crc_IRQn                  = 39, //7
    FIC_Gpio_IRQn                 = 40, //8
    FIC_Timer0_IRQn               = 41, //9
    FIC_Timer1_IRQn               = 42, //10
    FIC_Timer2_IRQn               = 43, //11
    FIC_Timer3_IRQn               = 44, //12
    FIC_Timer4_IRQn               = 45, //13
    FIC_Timer5_IRQn               = 46, //14
    FIC_I2c0_IRQn                 = 47, //15
    FIC_I2c1_IRQn                 = 48, //16
    FIC_Spi0_IRQn                 = 49, //17
    FIC_Spi1_IRQn                 = 50, //18
    FIC_Uart0Err_IRQn             = 51, //19
    FIC_Uart0Tx_IRQn              = 52, //20
    FIC_Uart0Rx_IRQn              = 53, //21
    FIC_Uart1Err_IRQn             = 54, //22
    FIC_Uart1Tx_IRQn              = 55, //23
    FIC_Uart1Rx_IRQn              = 56, //24
    FIC_WdgTimeout_IRQn           = 57, //25
    FIC_WdgT2_IRQn                = 58, //26
    FIC_WdgAdv_IRQn               = 59, //27
    /* LIC Interrupt Numbers  */
    LIC_Ovf_IRQn                  = 60, //0  28
    LIC_Lpdma_IRQn                = 61, //1  29
    LIC_Lpgpt_IRQn                = 62, //2  30
    LIC_Uart2Err_IRQn             = 63, //3  31
    LIC_Uart2Tx_IRQn              = 64, //4  32
    LIC_Uart2Rx_IRQn              = 65, //5  33
    LIC_Reserved0_IRQn            = 66, //6  34
    LIC_Reserved1_IRQn            = 67, //7  35
    LIC_SARADCDC_IRQn             = 68, //8  36
    LIC_SARADCRR_IRQn             = 69, //9  37
    LIC_SARADCFO_IRQn             = 70, //10  38
    LIC_SARADCFU_IRQn             = 71, //11  39
    LIC_SARADCTD_IRQn             = 72, //12  40
    LIC_VADCTR_IRQn               = 73, //13  41
    LIC_VADCFO_IRQn               = 74, //14  42
    LIC_TEMPDIGITAL_IRQn          = 75, //15  43
    LIC_SDADCDC_IRQn              = 76, //16  44
    LIC_SDADCRR_IRQn              = 77, //17  45
    LIC_SDADCFO_IRQn              = 78, //18  46
    LIC_SDADCFU_IRQn              = 79, //19  47
    LIC_SDADCTD_IRQn              = 80, //20  48
    LIC_VDDLOW_POS                = 81, //21  49
    LIC_VDDLOW_NEG                = 82, //22  50
    LIC_VDDLOW_ORG                = 83, //23  51

    /* AIC Interrupt Numbers  */
    AIC_Ovf_IRQn                  = 84, //0  52
    AIC_Top_Gpio_IRQn             = 85, //1  53
    AIC_Aon_Gpio_IRQn             = 86, //2  54
    AIC_Aon_UartErr_IRQn          = 87, //3  55
    AIC_Aon_UartTx_IRQn           = 88, //4  56
    AIC_Aon_UartRx_IRQn           = 89, //5  57
    AIC_Aon_Wdg_IRQn              = 90, //6  58
    AIC_Calendar_IRQn             = 91, //7  59
    AIC_Wave_Calendar_IRQn        = 92, //8  60
    AIC_IRQ_CP0_IRQn              = 93, //9  61
    AIC_IRQ_CP1_IRQn              = 94, //10  62
    AIC_IRQ_LVD_TOP_IRQn          = 95, //11  63
    AIC_IRQ_LVD15_IRQn            = 96, //12  64
    AIC_VddlowPOS_IRQn            = 97, //13  65
    AIC_VddlowNeg_IRQn            = 98, //14  66
    AIC_VddlowORG_IRQn            = 99  //15  67
} IRQn_Type;

/************************************************************************/
/*  Processor and Core Peripheral Section                               */
/************************************************************************/
/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined (__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning 586
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif
/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM0_REV                 0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT             0U        /* no MPU present */
#define __VTOR_PRESENT            0U        /* no VTOR present */
#define __NVIC_PRIO_BITS          2U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */

#include <core_cm0.h>

#define DEFAULT_SYSTEM_CLOCK     (16000000UL)

#define CHIP_TCASXXX             (0x6001UL)


#define DMA_NUMBER_OF_HW_CHANNEL_SUPPORTED      (8)
#define DMA_NUMBER_OF_DEVICES_SUPPORTED         (32)

#define PGA_MAGNIFICATION_BYPASS         (0X00)
#define PGA_MAGNIFICATION_X1             (0X01)
#define PGA_MAGNIFICATION_X2             (0X02)
#define PGA_MAGNIFICATION_X4             (0X03)
#define PGA_MAGNIFICATION_X8             (0X04)
#define PGA_MAGNIFICATION_X16            (0X05)
#define PGA_MAGNIFICATION_X32            (0X06)


typedef enum adc_trigger_num
{
    ADC_TRIGGER_NUM_1,
    ADC_TRIGGER_NUM_8,
    ADC_TRIGGER_NUM_16,
    ADC_TRIGGER_NUM_32,
    ADC_TRIGGER_NUM_64,
    ADC_TRIGGER_NUM_128,
    ADC_TRIGGER_NUM_256,
    ADC_TRIGGER_NUM_480
} enumAdcTriggerNum_t;

typedef enum adc_fifo_threshold
{
    ADC_FIFO_THRHOLD_1WORD,
    ADC_FIFO_THRHOLD_2WORD,
    ADC_FIFO_THRHOLD_3WORD,
    ADC_FIFO_THRHOLD_4WORD,
} enumAdcFifoThresholdNum_t;

typedef enum adc_signal_type
{
    ADC_SINGLE_SIGNAL,
    ADC_DIFFERENTIAL_SIGNAL
} enumAdcSignalType_t;

typedef enum adc_channel_num
{
    ADC_CHANNEL_AIN0,
    ADC_CHANNEL_AIN1,
    ADC_CHANNEL_AIN2,
    ADC_CHANNEL_AIN3,
    ADC_CHANNEL_AIN4,
    ADC_CHANNEL_AIN5,
    ADC_CHANNEL_AIN6,
    ADC_CHANNEL_AIN7,
    ADC_CHANNEL_NONE
} enumAdcChannelNum_t;


typedef struct
{
    __IO uint32_t sw_16m_sel                : 2;//bit 0 1
    __IO uint32_t sw_refclk_en_sdadc        : 1;//bit 2
    __IO uint32_t sw_refclk_sel_sdadc       : 1;//bit 3
    __IO uint32_t sw_refclk_div_sdadc       : 10;//bit 4-13
    __IO uint32_t sw_rst_sdadc              : 1; //bit 14
    __IO uint32_t sw_pclk_en_sdadc          : 1; //bit 15
} tc_sysctl_sdm_field_t;

typedef struct
{
    __IO uint32_t sw_rst_aon                : 1;//0
    __IO uint32_t sw_pclk_en_aon            : 1;//1
    __IO uint32_t sw_rst_tgpio              : 1;//2
    __IO uint32_t sw_pclk_en_tgpio_hs       : 1;//3
    __IO uint32_t sw_rst_aic                : 1;//4
    __IO uint32_t sw_rst_agpio              : 1;//5
    __IO uint32_t sw_rst_awdg               : 1;//6
    __IO uint32_t sw_rst_acfg               : 1;//7
    __IO uint32_t sw_rst_cldr               : 1;//8
    __IO uint32_t sw_rst_lpuart             : 1;//9
    __IO uint32_t reserved                  : 1;//10
    __IO uint32_t sw_fclk_en_lpuart_hs      : 1;//11
    __IO uint32_t sw_fclk_sel_lpuart_hs     : 2;//12 13
    __IO uint32_t sw_fclk_div_lpuart_hs     : 4;//14 17
    __IO uint32_t sw_fclk_en_rtc            : 1;//18
    __IO uint32_t sw_fclk_sel_rtc           : 2;//19 20
    __IO uint32_t sw_fclk_div_rtc           : 8;//21 28
} tc_sysctl_aontop_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_pmu_ena                : 1;
    __IO uint32_t reserve1                  : 7;
    __IO uint32_t sw_pup_cycle              : 6;
    __IO uint32_t reserve2                  : 2;
    __IO uint32_t sw_eflash_pup_cycle       : 6;
    __IO uint32_t reserve3                  : 2;
    __IO uint32_t sw_eflash_init_cycle      : 6;
} tc_sysctl_pmu_ctrl_field_t;

typedef struct
{
    __IO uint32_t reserved                  : 1;//0
    __IO uint32_t sw_m0_lockup_en           : 1;//1
    __IO uint32_t sw_hclk_div_m0            : 4;//2 5
    __IO uint32_t reserve1                  : 10;//6 15
    __IO uint32_t sw_fclk_en_systick        : 1;//16
    __IO uint32_t sw_fclk_sel_systick       : 1;//17
    __IO uint32_t sw_fclk_div_systick       : 4;//18 21
} tc_sysctl_m0_clkrst_ctrl_field_t;


typedef struct
{
    __IO uint32_t sw_rst_syscfg              : 1;
    __IO uint32_t reserved                   : 1;
    __IO uint32_t sw_hclk_en_fab             : 1;
    __IO uint32_t sw_rst_flash               : 1;
    __IO uint32_t sw_hclk_en_flash           : 1;
    __IO uint32_t reserved1                  : 1;
    __IO uint32_t sw_hclk_en_sramc           : 1;
    __IO uint32_t sw_rst_tpinmux             : 1;
    __IO uint32_t sw_pclk_en_tpinmux         : 1;
    __IO uint32_t sw_rst_lic                 : 1;
    __IO uint32_t sw_pclk_en_lic             : 1;
    __IO uint32_t sw_rst_fic                 : 1;
    __IO uint32_t sw_pclk_en_fic             : 1;
    __IO uint32_t sw_rst_divider             : 1;
    __IO uint32_t sw_pclk_en_divider         : 1;
    __IO uint32_t sw_rst_trng                : 1;
    __IO uint32_t sw_pclk_en_trng            : 1;
    __IO uint32_t sw_rst_anareg              : 1;
    __IO uint32_t sw_pclk_en_anareg          : 1;
} tc_sysctl_hclk_pclk_ctrl_field_t;

typedef struct
{
    __IO uint32_t reserved                     : 1;
    __IO uint32_t sw_pclk_div_bridge           : 3;
    __IO uint32_t sw_hclk_autocg_bridge        : 1;
    __IO uint32_t sw_pclk_autocg_bridge        : 1;
    __IO uint32_t sw_en_bridge_lpmode          : 1;
} tc_sysctl_ahb2apb_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_psram               : 1;
    __IO uint32_t sw_hclk_en_psram           : 1;
    __IO uint32_t sw_pclk_en_psram           : 1;
    __IO uint32_t sw_fclk_en_psram           : 1;
    __IO uint32_t sw_fclk_sel_lpgpt          : 2;
    __IO uint32_t sw_fclk_div_lpgpt          : 4;
} tc_sysctl_psram_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_lpgpt               : 1;
    __IO uint32_t sw_pclk_en_lpgpt           : 1;
    __IO uint32_t sw_fclk_en_lpgpt           : 1;
    __IO uint32_t sw_fclk_sel_lpgpt          : 2;
    __IO uint32_t sw_fclk_div_lpgpt          : 4;
} tc_sysctl_lpgpt_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_lpuart               : 1;
    __IO uint32_t sw_pclk_en_lpuart           : 1;
    __IO uint32_t sw_fclk_en_lpuart           : 1;
    __IO uint32_t sw_fclk_sel_lpuart          : 2;
    __IO uint32_t sw_fclk_div_lpuart          : 4;
} tc_sysctl_lpuart_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_crypto               : 1;
    __IO uint32_t sw_pclk_en_crypto           : 1;
    __IO uint32_t sw_fclk_en_crypto           : 1;
    __IO uint32_t sw_fclk_sel_crypto          : 2;
    __IO uint32_t sw_fclk_div_crypto          : 4;
} tc_sysctl_crypto_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_gpt               : 1;
    __IO uint32_t sw_pclk_en_gpt           : 1;
    __IO uint32_t sw_fclk_en_gpt           : 1;
    __IO uint32_t sw_fclk_sel_gpt          : 2;
    __IO uint32_t sw_fclk_div_gpt          : 4;
} tc_sysctl_gpt_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_pwm               : 1;
    __IO uint32_t sw_pclk_en_pwm           : 1;
    __IO uint32_t sw_fclk_en_pwm           : 1;
    __IO uint32_t sw_fclk_sel_pwm          : 2;
    __IO uint32_t sw_fclk_div_pwm          : 4;
} tc_sysctl_pwm_clkrst_ctrl_field_t;


typedef struct
{
    __IO uint32_t sw_rst_i2c               : 1;
    __IO uint32_t sw_pclk_en_i2c           : 1;
    __IO uint32_t sw_fclk_en_i2c           : 1;
    __IO uint32_t sw_fclk_sel_i2c          : 2;
    __IO uint32_t sw_fclk_div_i2c          : 4;
} tc_sysctl_i2c_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_spi               : 1;
    __IO uint32_t sw_pclk_en_spi           : 1;
    __IO uint32_t sw_fclk_en_spi           : 1;
    __IO uint32_t sw_fclk_sel_spi          : 2;
    __IO uint32_t sw_fclk_div_spi          : 4;
} tc_sysctl_spi_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_uart               : 1;
    __IO uint32_t sw_pclk_en_uart           : 1;
    __IO uint32_t sw_fclk_en_uart           : 1;
    __IO uint32_t sw_fclk_sel_uart          : 2;
    __IO uint32_t sw_fclk_div_uart          : 4;
} tc_sysctl_uart_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_wdg               : 1;
    __IO uint32_t sw_pclk_en_wdg           : 1;
    __IO uint32_t sw_fclk_en_wdg           : 1;
    __IO uint32_t sw_fclk_sel_wdg          : 2;
    __IO uint32_t sw_fclk_div_wdg          : 12;
} tc_sysctl_wdg_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t ext_pwm0_en         : 1;      // 1 使能EXT_PWM0_EN
    __IO uint32_t ext_pwm1_en         : 1;      // 1 使能EXT_PWM1_EN
    __IO uint32_t ext_pwm2_en         : 1;      // 1 使能EXT_PWM2_EN
} tc_sysctl_ext_pwm_en_field_t;

typedef struct
{
    __IO uint32_t sw_rst_cp            : 1;
    __IO uint32_t sw_pclk_en_cp        : 1;
    __IO uint32_t sw_fclk_en_cp        : 1;
    __IO uint32_t sw_fclk_sel_cp       : 2;
    __IO uint32_t sw_fclk_div_cp       : 12;
} tc_sysctl_cp_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_lvd            : 1;
    __IO uint32_t sw_pclk_en_lvd        : 1;
    __IO uint32_t sw_fclk_en_lvd        : 1;
    __IO uint32_t sw_fclk_sel_lvd       : 2;
    __IO uint32_t sw_fclk_div_lvd       : 12;
} tc_sysctl_lvd_clkrst_ctrl_field_t;
typedef struct
{
    __IO uint32_t sw_rst_saradc            : 1;
    __IO uint32_t sw_pclk_en_saradc        : 1;
    __IO uint32_t sw_fclk_en_saradc        : 1;
    __IO uint32_t sw_fclk_sel_saradc       : 2;
    __IO uint32_t sw_fclk_div_saradc       : 12;
} tc_sysctl_saradc_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t sw_rst_vdac            : 1;
    __IO uint32_t sw_pclk_en_vdac        : 1;
    __IO uint32_t sw_fclk_en_vdac        : 1;
    __IO uint32_t sw_fclk_sel_vdac       : 2;
    __IO uint32_t sw_fclk_div_vdac       : 12;
} tc_sysctl_vdac_clkrst_ctrl_field_t;



typedef struct
{
    __IO uint32_t sw_ahb_hsize_spi         : 2;
    __IO uint32_t sw_addr_keep_spi         : 1;
    __IO uint32_t sw_no_prefetch_spi       : 1;
} tc_sysctl_spicfg_clkrst_ctrl_field_t;

typedef struct
{
    __IO uint32_t tp_en          : 29;
    __IO uint32_t tp_sel         : 3;
} tc_sysctl_tp_field_t;


typedef struct
{
    __IO uint32_t saradc_coeff_a : 12;
    __IO uint32_t reserved       : 4;
    __IO uint32_t saradc_coeff_b : 12;
} tc_trim_adsar_coeff_field_t;


/**< Read Cycle Timing Configuration,offset: 0x0 */
typedef struct
{
    __IO uint32_t rc                      : 4;      /** Read time (HCLK) ,  reset:1*/
    __IO uint32_t cfs                     : 2;      /** ADDRESS/DIN/CEb to CONFEN setup time (HCLK), reset:3*/
    __IO uint32_t cfh                     : 2;      /** ADDRESS/DIN/CEb to CONFEN hold time (HCLK), reset:3 */
    __IO uint32_t confen                  : 2;      /** CONFEN pulse width (HCLK) , reset:3*/
} tc_flash_rctc_field_t;

/**< Write (Program/Erase) Cycle Timing Configuration 1,offset: 0x4 */
typedef struct
{
    __IO uint32_t nvs                     : 6;      /** PROG/ERASE/CEb/NVR/Address to WEb setup time (1MHz), reset:28*/
    __IO uint32_t rcv_prog                : 6;      /** Recovery time for program (1MHz), reset:24*/
    __IO uint32_t rcv_esect               : 10;     /** Recovery time for erase sector (8x) (1MHz), reset:240*/
    __IO uint32_t rcv_echip               : 10;     /** Recovery time for erase chip (8x) (1MHz), reset:1023*/
} tc_flash_wctc1_field_t;

/**< Write (Program/Erase) Cycle Timing Configuration 2,offset: 0x8 */
typedef struct
{
    __IO uint32_t pgs                     : 5;      /** WEb low to PROG2 high setup time (1MHz) */
    __IO uint32_t pgh                     : 2;      /** PROG2 low to WEb high hold time (HCLK) */
    __IO uint32_t ads                     : 2;      /** BYTE setup time (HCLK) */
    __IO uint32_t adh                     : 2;      /** BYTE hold time (HCLK) */
    __IO uint32_t prog                    : 5;      /** Byte program time (1MHz) */
    __IO uint32_t erase_sect              : 5;      /** Sector erase time (1KHz) */
    __IO uint32_t erase_chip              : 8;      /** Chip Erase time (1KHz), reset: 128 */
    __IO uint32_t latency                 : 3;      /** Latency to next operation after PROG/ERASE low (HCLK) */
} tc_flash_wctc2_field_t;

/**< Erase Configuration,offset: 0xC */
typedef struct
{
    __IO uint32_t sector_index            : 11;     /** [10:0] Main memory sector address when config_nvr_select = 0;
                                                        [9:0] NVR sector address if config_nvr_select > 0;
                                                        */
    __IO uint32_t reserved1               : 13;     /** reserved */
    __IO uint32_t nvr_select              : 2;      /** Indicates NVR selected of flash bank0 or bank1
                                                        00: Main memory (NVM) selected
                                                        01: NVR bank0 selected
                                                        10: NVR bank1 selected
                                                        11: NVR both bank0 and bank1 selected */
} tc_flash_econf_field_t;

/**< Erase Command Register,offset: 0x10 */
typedef struct
{
    __IO uint32_t sector_erase            : 1;      /** Command of sector erase operation
                                                        - Write 1 to erase the sector specified in the register of econf.
                                                        */
    __IO uint32_t reserved1               : 15;     /** reserved */
    __IO uint32_t chip_erase              : 1;      /** Command of chip erase operation
                                                        - Write 1 followed by write 0 to erase whole chip.
                                                        - Use the register of config_nvr_select[1:0] to select which bank to erase.
                                                        config_nvr_select=1 : erase bank0;
                                                        config_nvr_select=2 : erase bank1;
                                                        config_nvr_select=3 : erase both bank0 and bank1;
                                                        config_nvr_select=0 : no flash bank erased */
} tc_flash_ecomm_field_t;

/**< Miscellaneous Control Register,offset: 0x14 */
typedef struct
{
    __IO uint32_t deep_standby            : 1;      /** Set eflash to deep standby (DPSTB) mode */
    __IO uint32_t reserved1               : 3;      /** reserved */
    __IO uint32_t clk1m_divisor           : 4;      /** The divisor of 1MHz clock generator from RC16M */
    __IO uint32_t clk1k_divisor           : 5;      /** The divisor of 1KHz clock generator from OSC32K */
    __IO uint32_t reserved2               : 3;      /** reserved */
    __IO uint32_t program_busy_0          : 1;      /** Indicates flash bank0 state's busy in programming */
    __IO uint32_t program_busy_1          : 1;      /** Indicates flash bank1 state's busy in programming */
    __IO uint32_t erase_busy_0            : 1;      /** Indicates flash bank0 state's busy in erasing */
    __IO uint32_t erase_busy_1            : 1;      /** Indicates flash bank1 state's busy in erasing */
} tc_flash_mcr_field_t;



typedef struct
{
    __IO uint32_t verf_trim            : 10;
} tc_tsdm_verf_trim_field_t;

typedef struct
{
    __IO uint32_t temp_rtc_0d0625      : 16;
} tc_temp_rtc_0d0625_field_t;

typedef union
{
    __IO uint32_t trim_prot_en         : 1;
    __IO uint32_t trim_prot            : 12;
} tc_tsdm_trim_prot_field_t;
typedef struct
{
    __IO uint32_t otp1_vbg_off_set     : 10;
    __IO uint32_t reserve              : 2;
    __IO uint32_t otp2_vbg_slope       : 8;
    __IO uint32_t otp3_vbg_curve       : 8;
} tc_tsdm_trim_cfg1_field_t;

typedef struct
{
    __IO uint32_t otp4_vbg_slope_zero   : 8;
    __IO uint32_t otp5_ts_slope         : 16;
    __IO uint32_t otp6_ts_offset        : 8;
} tc_tsdm_trim_cfg2_field_t;

typedef struct
{
    __IO uint32_t dem_clk_sel        :2;
    __IO uint32_t sdm_clk_sel        :2;
    __IO uint32_t ctrl_dem           :1;
    __IO uint32_t en_clamp           :1;
    __IO uint32_t en_dem             :1;
    __IO uint32_t en_chop            :1;
    __IO uint32_t en_test            :1;
    __IO uint32_t tsdm_en            :1;
    __IO uint32_t offchip_bs_sel     :1;
    __IO uint32_t ctrl_vinvcm        :1;
    __IO uint32_t reserve            :12;
    __IO uint32_t gain_pga_stg1_sel  :3;
    __IO uint32_t gain_pga_stg2_sel  :3;

} tc_tsdm_ctrl_field_t;

typedef struct
{
    __IO uint32_t offset_1uv         :24;
    __IO uint32_t tsdm_dig_en        :1;
    __IO uint32_t trim_data_sel      :1;
    __IO uint32_t down_samp_sel      :3;

} tc_tsdm_dig_cfg1_field_t;

typedef struct
{
    __IO uint32_t gainv                  :20;
    __IO uint32_t reserve                :1;
    __IO uint32_t reg_opt8_vbg_adj_step  :3;
    __IO uint32_t reg_otp7_vbg_curve_zero :6;
} tc_tsdm_dig_cfg2_field_t;

typedef struct
{
    __IO uint32_t reg_meter_cal_temp       :8;
    __IO uint32_t reg_meter_cal_temp_hyst  :4;
} tc_tsdm_cal_cfg_field_t;

typedef struct
{
    __IO uint32_t tadc       :24;
} tc_tsdm_dig_adc_field_t;

typedef struct
{
    __IO uint32_t tsdm_threshold     :16;
} tc_tsdm_threshold_field_t;

typedef struct
{
    __IO uint32_t tsdm_int     :1;
} tc_tsdm_int_field_t;


typedef struct
{
    __IO uint32_t adc_ext_input_p       :4;
    __IO uint32_t adc_ext_input_n       :4;
    __IO uint32_t adc_pga_sel           :1;
    __IO uint32_t adc_sar_sel           :1;
    __IO uint32_t adc_pga2sar_sel       :1;
    __IO uint32_t adc_idac_sel          :4;
    __IO uint32_t adc_vdac_sel          :1;
    __IO uint32_t adc_pga_from_pad_sel  :1;
    __IO uint32_t adc_pga_from_ptat_sel :1;
    __IO uint32_t adc_pga_from_ibist_sel:1;
    __IO uint32_t adc_sw_input_for_vcin :8;
    __IO uint32_t adc_sw_input_for_vrefp:1;
    __IO uint32_t adc_sw_input_for_vrefn:1;
} tc_reg_adsar_mux_in_field_t;

/*  ctrl_vdac<7:0>: ctrl_bit of 12bit vdac. default value: 0010_0000. */
typedef struct
{
    __IO uint32_t reserved              :1;         /*  <0>: reserved.              */
    __IO uint32_t vout_gain_ctrl        :3;         /*  <3:1>: vout gain ctrl bit, default:000.
                                                        000: x1.0; 001:x1.2; 010:x1.5; 011:x1.6; 100:x1.8; 101:x2.0; 110/111:nil.
                                                        */
    __IO uint32_t vadc_vcm_sel          :3;         /*  <6:4>: vcm sel bit, default 010.
                                                        000:1.2v; 001:1.4v; 010:1.5v; 011:1.6v; 100:1.8v; 101:2.0v; 110:2.5v. 111:nil.
                                                        */
    __IO uint32_t vdac_test_en          :1;         /*  <7>: test en, default 0.*/
} tc_reg_vdac_field_t;

typedef struct
{
    __IO uint32_t adc_scan_chnum    :5;
    __IO uint32_t adc_trig_num      :3;
    __IO uint32_t adc_fifo_format   :2;
    __IO uint32_t adc_dma_en        :1;
    __IO uint32_t adc_trigger_en    :1;
    __IO uint32_t adc_irq_mode      :3;
    __IO uint32_t adc_fifo_thrhold  :2;
    __IO uint32_t reserved0         :1;
    __IO uint32_t adc_wait_cycle    :6;
    __IO uint32_t adc_smp_cycle     :8;
} tc_reg_adc_conf1_field_t;

// 0xB0 reg_adc_fifo_state
typedef struct
{
    __IO uint32_t adcsar_fifo_entry_valid       :3;         // Read saradc fifo when valid high. default 0
    __IO uint32_t adcsd_fifo_entry_valid        :3;         // Read sdadc fifo when valid high. default 0
    __IO uint32_t vdac_fifo_free                :3;         // Read vdac fifo free. default 4
} tc_reg_adc_fifo_state_field_t;


typedef struct
{
    __IO uint32_t sw_32k_sel        :1;     //high to select xtal32k; low to select the rc32k if rc32k_ready is 1 or xtal32k if rc32k_ready is 0
    __IO uint32_t sw_fclk_sel_tgpio :1;     //high to select high speed clock for tgpio. The high speed clock freq is configured in syscfg.
    __IO uint32_t sw_fclk_en_tgpio  :1;     //high to enable fclk for tgpio to input detection
    __IO uint32_t sw_fclk_en_agpio  :1;     //high to enable fclk for agpio to input detection
    __IO uint32_t sw_pclk_en_auart  :1;     //high to enable pclk for auart.
    __IO uint32_t sw_fclk_en_auart  :1;     //high to enable fclk for auart
    __IO uint32_t sw_fclk_en_cldr   :1;     //high to enable fclk for cldr
    __IO uint32_t sw_fclk_en_awdg   :1;     //high to enable fclk for awdg
    __IO uint32_t sw_fclk_div_awdg  :8;     //fclk_awdg= 32k/(div+1)
    __IO uint32_t sw_fclk_en_cp0    :1;     //high to enable fclk for comparator0 filter
    __IO uint32_t sw_fclk_en_cp1    :1;     //high to enable fclk for comparator1 filter
    __IO uint32_t sw_fclk_en_lvd    :1;     //high to enable fclk for lvd filter
    __IO uint32_t trig_rtc_en       :1;     //high to enable rtc to trig sleep walking
} tc_clk_en_field_t;

typedef struct
{
    __IO uint32_t sw_ma30_pa00_en  :1;
    __IO uint32_t sw_ma30_pa01_en  :1;
    __IO uint32_t sw_ma30_pb00_en  :1;
    __IO uint32_t sw_ma30_pb01_en  :1;
    __IO uint32_t sw_ma30_pb02_en  :1;
    __IO uint32_t sw_ma30_pb03_en  :1;

} tc_pinmux_ma30en_field_t;

/*
    Note:  if sw_pulldown == 0 and sw_pullup == 0, the status is high impdence
           if open_drain == 0  and  open_source == 0,the status is    push pull
*/
typedef struct
{
    __IO uint32_t reserved0        :8;                               //N/A
    __IO uint32_t sw_src_sel       :3;                               //i: select the ith source out of 5 sources to this pad
    __IO uint32_t reserved1        :5;                               //N/A
    __IO uint32_t sw_pull_sel      :1;                               //1: Using [18:17] as pullup and pulldn 0: Using muxed hardware pullup and pulldn
    __IO uint32_t sw_pulldown      :1;                               //1: Pull down enable                    0: Pull down disable
    __IO uint32_t sw_pullup        :1;                               //1: Pull up enable                      0: Pull up disable
    __IO uint32_t sw_ie            :1;                               //1: enable input buffer                 0: Disable input buffer
    __IO uint32_t cs_sel           :1;                               //1: Schmitt-trigger input buffer        0: CMOS input buffer
    __IO uint32_t driver_strength  :1;                               //1: High driving strength               0: Low driving strength
    __IO uint32_t open_drain       :1;                               //1: Enable open drain output            0: Disable open drain output
    __IO uint32_t open_source      :1;                               //1: Enable open source output           0: Disable open source output
    __IO uint32_t slew_rate        :1;                               //1: Fast slew rate                      0: Slow slew rate
} tc_pinmux_config_field_t;

typedef struct
{
    __IO uint32_t ctrl_oscrc16m_0_enable          :1;
    __IO uint32_t sdm_ctrl_7_enable               :1;
    __IO uint32_t pga_ctrl_7_enable               :1;
    __IO uint32_t ctrl_fd_en_0_enable             :1;
    __IO uint32_t ctrl_fll_0_enable               :1;
    __IO uint32_t ctrl_adsar_chargepump_0_enable  :1;
    __IO uint32_t ctrl_crystal_16m_9_enable       :1;
    __IO uint32_t ctrl_sar_8_enable               :1;
    __IO uint32_t bgr_ctrl_6_enable               :1;
    __IO uint32_t ctrl_ldo3rf_4_enable            :1;
    __IO uint32_t ctrl_idac1_5_enable             :1;
    __IO uint32_t ctrl_idac2_5_enable             :1;
    __IO uint32_t ctrl_vdac_0_enable              :1;
    __IO uint32_t ctrl_power_switch_vrefn_enable  :1;
} tc_analog_block_enable_field_t;

typedef struct
{
    __IO uint32_t ctrl_oscrc16m_0_enable_vcclow         :1;
    __IO uint32_t sdm_ctrl_7_enable_vcclow              :1;
    __IO uint32_t pga_ctrl_7_enable_vcclow              :1;
    __IO uint32_t ctrl_fd_en_0_enable_vcclow            :1;
    __IO uint32_t ctrl_fll_0_enable_vcclow              :1;
    __IO uint32_t ctrl_adsar_chargepump_0_enable_vcclow :1;
    __IO uint32_t ctrl_crystal_16m_9_enable             :1;
    __IO uint32_t ctrl_sar_8_enable                     :1;
    __IO uint32_t bgr_ctrl_6_enable                     :1;
    __IO uint32_t ctrl_ldo3rf_4_enable                  :1;
    __IO uint32_t ctrl_idac1_5_enable                   :1;
    __IO uint32_t ctrl_idac2_5_enable                   :1;
    __IO uint32_t ctrl_vdac_0_enable                    :1;
    __IO uint32_t ctrl_power_switch_vrefn_enable_vcclow :1;
    __IO uint32_t reserve1                              :2;
    __IO uint32_t ctrl_ldo3_0_enable_vcclow             :1;
    __IO uint32_t reserve2                              :1;
    __IO uint32_t comp0_ctrl_0_enable_vcclow            :1;
    __IO uint32_t comp1_ctrl_0_enable_vcclow            :1;
    __IO uint32_t dac_ctrl_4_enable_vcclow              :1;
    __IO uint32_t ctrl_lvd5_8_enable_vcclow             :1;
} tc_analog_block_enable_vcclow_t;

typedef struct
{
    __IO uint32_t ctrl_powerswitch_3_0              :4;     /** change falling trigger point of vcc.
                                                                0000:1.5V, 0001:1.6V,.....1001:2.4V.
                                                                NA when>1010.                    */
    __IO uint32_t ctrl_powerswitch_4_en6            :1;     /** module enable signal(en6, only ctrl by software), 0:disable; 1:enable; analog ip power-switch ennable, high active default 1. */
    __IO uint32_t ctrl_powerswitch_5_selvcc         :1;     /** sel vcc for analog ip  default 1.  0: vbat ; 1:vcc5  */
    __IO uint32_t ctrl_powerswitch_10_6             :5;     /** <10>: test_vcc5low enable, high active, default 0.
                                                                <9>: test_vsw5aon enable, high active, default 0.
                                                                <8>: test_vsw5 enable, high active, default 0.
                                                                <7>: power down detect function contrl, high active, default 0.
                                                                <6>: test_vcc5 enable, high active, default 0.       */
} tc_power_switch_top_t;


//reg_oscrc32k_1p5v_top  0xd010
typedef struct
{
    __IO uint32_t ctrl_oscrc32k_0_enable            :1;     /** module enable signal. 0:disable; 1:enable;deault 1.  */
    __IO uint32_t ctrl_oscrc32k_2_1                 :2;     /** <2> test_en deault 0
                                                                <1> select_32k: default 1;
                                                                0: f=40khz;
                                                                1: f=32khz.                  */
} tc_oscrc32k_1p5v_top_t;


typedef struct
{
    __IO uint32_t ctrl_deglitch_oscrc32k                 :5;        /** contrl bit of deglitch_2500n, default value: 0_0010
                                                                        <4>: test en, default 0.
                                                                        <3:1>: RC-time ctrl , default 001.
                                                                        <0>: bypass the deglitch module. 0: deglitch the spur of the clk; 1:bypass the deglitch module; default 0.
                                                                        */
    __IO uint32_t reserved                               :3;
    __IO uint32_t ctrl_deglitch_xosc32k                  :5;        /** contrl bit of deglitch_2500n, default value: 0_0010
                                                                        <4>: test en, default 0.
                                                                        <3:1>: RC-time ctrl , default 001.
                                                                        <0>: bypass the deglitch module. 0: deglitch the spur of the clk; 1:bypass the deglitch module; default 0.
                                                                        */
} tc_reg_clk_deglitch_2500n_field_t;


typedef struct
{
    __IO uint32_t ctrl_xosc32k_0_enable                 :1;
    __IO uint32_t ctrl_xosc32k_14_1                     :14;
} tc_reg_xosc32k_top_field_t;

typedef struct
{
    __IO uint32_t ctrl_ldo3_0_enable                    :1;
    __IO uint32_t ctrl_ldo3_15_1                        :15;
} tc_reg_ldo3_top_field_t;

typedef struct
{
    __IO uint32_t ctrl_oscrc32k_clkready_32k_0_enable   :1;
    __IO uint32_t ctrl_oscrc32k_clkready_32k_1          :1;
    __IO uint32_t ctrl_xosc32k_clkready_32k_0_enable    :1;
    __IO uint32_t ctrl_xosc32k_clkready_32k_1           :1;
    __IO uint32_t a2d_oscrc32k_ready                    :1;
    __IO uint32_t a2d_xosc32k_ready                     :1;
} tc_reg_clkready_32k_withDiv2_field_t;

typedef struct
{
    __IO uint32_t ctrl_ldo15_0_enable                    :1;
    __IO uint32_t ctrl_ldo15_2_1                         :2;
    __IO uint32_t ctrl_ldo15_3_out_enable                :1;
    __IO uint32_t ctrl_ldo15_15_4                        :12;           /** ctrl_ldo15_top<15:0> control bit of ldo15,default value:0001_0010_0000_1111
                                                                            <15> reserved, default 0
                                                                            <14:12> ldo_opamp bias copy control with weitht of 1/900, default 001,1/900
                                                                            <11:9> ldo_opamp bias control with weight of 1uA and offset 1uA, default 001,2uA
                                                                            <8:7> select of ldo15_lvd reset voltage,default 00
                                                                            0,1.25V; 1,1.3V; 2,1.35V; 3,1.4V
                                                                            <6:4> select of ldo15 output voltage, default 000,1.5V
                                                                            0,1.5V;1,1.55V;2,1.6V;3,1.65V; 4~7,1.7V
                                                                            <3> enable of ldo15 output 0 when disable, default 1
                                                                            <2> enable of ldo15 high power mode, default 1
                                                                            <1> module enable signal. enable of ldo15_lvd, default 1
                                                                            <0> module enable signal. enable of ldo15, default              */
} tc_reg_ldo15_top_field_t;


//ctrl_vbias_ibias_top<23:0>: ctrl bits of vbias_ibias_top. default value: 0000_0000_0000_0000_0011_1101.
typedef struct
{
    __IO uint32_t ctrl_vbias_ibias_top_me                 :1;           /*  module enable signal. 0:disable; 1:enable; default 1.*/
    __IO uint32_t ctrl_vbias_ibias_top_svb                :1;           /*  sel vbg bit
                                                                            0: lowpower bgr; 1: high accuracy bgr */
    __IO uint32_t ctrl_vbias_ibias_top_vbe                :1;           /*  vcm buffer enable   */
    __IO uint32_t ctrl_vbias_ibias_top_vpbe               :1;           /*  vrefp10 buffer enable   */
    __IO uint32_t ctrl_vbias_ibias_top_fbe                :1;           /*  flash buffer enable */
    __IO uint32_t ctrl_vbias_ibias_top_vge                :1;           /*  vbegen enable */
    __IO uint32_t ctrl_vbias_ibias_top_vsb                :14;          /*   vcm sel bits.
                                                                            0000_0000_0000_01: 0.2V
                                                                            ......
                                                                            1000_0000_0000_00: 1.5V   */
    __IO uint32_t reserved                                :12;
} tc_reg_vbias_ibias_top_field_t;

// ctrl_opamp_iv<18:0> control bit of opamp_iv, default value: 000_0000_0000_0010_1111
typedef struct
{
    __IO uint32_t opa_iv_stg1                           :2;         /*  <1:0> current select of opamp_iv stg1,default 11
                                                                        00, 20uA; 01,25uA; 10,30uA; 11,35uA.         */
    __IO uint32_t opa_iv_stg2                           :2;         /** <3:2> current select of opamp_iv stg2,default 11
                                                                        00, 60uA; 01,105uA; 10,150uA; 11,200uA  */
    __IO uint32_t opa_iv_me                             :1;         // module enable signal. enable of opamp_iv, default 0
    __IO uint32_t opa_iv_high_en                        :1;         // enable of opamp_iv high power, default 1
    __IO uint32_t reserved                              :2;
    __IO uint32_t opa_output_vol                        :4;         // <11:8> buffer output voltage sel from 0 to 1.5V with step of 0.1V,
                                                                    //        default 0000
    __IO uint32_t opa_out_pgan_en                       :1;         //<12> enable opamp output send to pga_n, default 0
    __IO uint32_t opa_pos_in_pgan_en                    :1;         //<13> enable opamp positive input send to pga_n, default 0
    __IO uint32_t opa_out_pgap_en                       :1;         //<14> enable opamp output send to pga_p, default 0
    __IO uint32_t opa_pos_in_pgap_en                    :1;         //<15> enable opamp positive input send to pga_p, default 0
    __IO uint32_t opa_out_rail_en                       :1;         //<16> enable opamp output send to rail_to_rail opamp, default 0
    __IO uint32_t opa_vol_sel                           :2;         /*  <18:17> opamp commom voltage sel, default 00
                                                                        00, offchip vcm; 01, buffer output; 10,idac vcm; 11, vdac vcm
                                                                    **/
} tc_reg_opa_iv_field_t;

typedef struct
{
    __IO uint32_t comp0_ctrl_me                           :1;           /*  module enable signal. 0:disable; 1:enable; default 1.*/
    __IO uint32_t comp0_ctrl_cp0he                        :1;           /*  cp0_hs_en, default 0.*/
    __IO uint32_t comp0_ctrl_cp0hs                        :3;           /*  cp0_hs_sel<2:0>, default 000.*/
    __IO uint32_t comp0_ctrl_cp0oie                       :1;           /*  cp0_out_inv_en, default 0.*/
    __IO uint32_t comp0_ctrl_cp0le                        :1;           /*  cp0_lpf_en, default 0.*/
    __IO uint32_t comp0_ctrl_cp0sl                        :1;           /*  cp0_sel_lpf, default 0.*/
    __IO uint32_t comp0_ctrl_cp0svn                       :4;           /*  cp0_sel_vn<3:0>, input vn select ctrl,default 0000.
                                                                            0000-0111--?chanel 0-7；
                                                                            1000：测试输入；
                                                                            1001：opamp-iv输出；
                                                                            1010：vd1aon；
                                                                            1011：vbias_ibias中vbegen(低精度temp-sensor)r输出；
                                                                            1100：comp内部dac输出(与p端互斥)*/
    __IO uint32_t comp0_ctrl_cp0svp                       :4;           /*  cp0_sel_vp<3:0>, input vp select ctrl,default 0000.
                                                                            0000-0111--?chanel 0-7；
                                                                            1000：测试输入；
                                                                            1001：opamp-iv输出；
                                                                            1010：vd1aon；
                                                                            1011：vbias_ibias中vbegen(低精度temp-sensor)输出；
                                                                            1100：comp内部dac输出(与n端互斥)*/
    __IO uint32_t comp0_ctrl_cpvne                        :1;           /*  cp0_vn_en, default 1 */
    __IO uint32_t comp0_ctrl_cp0vpe                       :1;           /*  cp0_vp_en, default 1.*/
    __IO uint32_t comp0_ctrl_tcp0e                        :1;           /*  test cp0 enable, default 0.*/
    __IO uint32_t reserved                                :13;
} tc_reg_comparator_top1_field_t;

typedef struct
{
    __IO uint32_t comp1_ctrl_me                           :1;           /*  module enable signal. 0:disable; 1:enable; default 1.*/
    __IO uint32_t comp1_ctrl_cp1he                        :1;           /*  cp1_hs_en, default 0.*/
    __IO uint32_t comp1_ctrl_cp1hs                        :3;           /*  cp1_hs_sel<2:0>, default 000.*/
    __IO uint32_t comp1_ctrl_cp1oie                       :1;           /*  cp1_out_inv_en, default 0.*/
    __IO uint32_t comp1_ctrl_cp1le                        :1;           /*  cp1_lpf_en, default 0.*/
    __IO uint32_t comp1_ctrl_cp1sl                        :1;           /*  cp1_sel_lpf, default 0.*/
    __IO uint32_t comp1_ctrl_cp1svn                       :4;           /*  cp1_sel_vn<3:0>, input vn select ctrl,default 0000.
                                                                            0000-0111--?chanel 0-7；
                                                                            1000：测试输入；
                                                                            1001：opamp-iv输出；
                                                                            1010：vd1aon；
                                                                            1011：vbias_ibias中vbegen(低精度temp-sensor)r输出；
                                                                            1100：comp内部dac输出(与p端互斥)*/
    __IO uint32_t comp1_ctrl_cp1svp                       :4;           /*  cp1_sel_vp<3:0>, input vp select ctrl,default 0000.
                                                                            0000-0111--?chanel 0-7；
                                                                            1000：测试输入；
                                                                            1001：opamp-iv输出；
                                                                            1010：vd1aon；
                                                                            1011：vbias_ibias中vbegen(低精度temp-sensor)输出；
                                                                            1100：comp内部dac输出(与n端互斥)*/
    __IO uint32_t comp1_ctrl_cpvne                        :1;           /*  cp1_vn_en, default 1 */
    __IO uint32_t comp1_ctrl_cp1vpe                       :1;           /*  cp1_vp_en, default 1.*/
    __IO uint32_t comp1_ctrl_tcp1e                        :1;           /*  test cp1 enable, default 0.*/
    __IO uint32_t reserved                                :13;
} tc_reg_comparator_top2_field_t;


typedef struct
{
    __IO uint32_t comp2_ctrl_cp0db                        :1;           /*  cp0 dac buffers, default 0.*/
    __IO uint32_t comp2_ctrl_cp0ds                        :1;           /*  cp0_dac sw, default 0.*/
    __IO uint32_t comp2_ctrl_cp1db                        :1;           /*  cp1 dac buffers, default 0.*/
    __IO uint32_t comp2_ctrl_cp1ds                        :1;           /*  cp1_dac sw, default 0.*/
    __IO uint32_t comp2_ctrl_me                           :1;           /*  module enable signal.(for dac) 0:disable; 1:enable; default 1.*/
    __IO uint32_t comp2_ctrl_sdv                          :2;           /*  sel_dac_vref, default 000.
                                                                            00：dac参考为vd5；01：dac参考为1V；10：dac参考为片外vrep；11：dac参考为vd1aon。*/
    __IO uint32_t comp2_ctrl_td0e                         :1;           /*  test dac0 enable, default 0.*/
    __IO uint32_t reserved                                :24;
} tc_reg_comparator_top3_field_t;

// reg_acomp_dac        0xd038
typedef struct
{
    __IO uint32_t dac1_din2                               :7;
    __IO uint32_t reserve1                                :1;
    __IO uint32_t dac1_din1                               :7;
    __IO uint32_t reserve2                                :1;
    __IO uint32_t dac0_din2                               :7;
    __IO uint32_t reserve3                                :1;
    __IO uint32_t dac0_din1                               :7;
} tc_reg_acomp_dac_field_t;

// reg_acomp0_cfg       0xd03c
typedef struct
{
    __IO uint32_t cfg_cnt_max                              :16;     // 数字滤波：滤波时间/fclk_cp周期
    __IO uint32_t reserved                                 :10;
    __IO uint32_t cfg_irq_mode                             :3;      /** <26>1-使能下降沿触发中断；0-关闭该功能
                                                                        <27>1-使能上升沿触发中断；0-关闭该功能
                                                                        <28>1-使能高电平触发中断；0-关闭该功能      ***  */
    __IO uint32_t cfg_rst_en                               :1;      // reset default:0
    __IO uint32_t cfg_trig_en                              :1;      /** 1-VC中断配置为Tinywork事件发生源
                                                                        0-关闭该功能       */
    __IO uint32_t cfg_en                                   :1;      // 模块使能
} tc_reg_acomp0_cfg_field_t;

// reg_acomp1_cfg       0xd040
typedef struct
{
    __IO uint32_t cfg_cnt_max                              :16;     // 数字滤波：滤波时间/fclk_cp周期
    __IO uint32_t reserved                                 :10;
    __IO uint32_t cfg_irq_mode                             :3;      /** <26>1-使能下降沿触发中断；0-关闭该功能
                                                                        <27>1-使能上升沿触发中断；0-关闭该功能
                                                                        <28>1-使能高电平触发中断；0-关闭该功能      ***  */
    __IO uint32_t cfg_rst_en                               :1;      // reset default:0
    __IO uint32_t cfg_trig_en                              :1;      /** 1-VC中断配置为Tinywork事件发生源
                                                                        0-关闭该功能       */
    __IO uint32_t cfg_en                                   :1;      // 模块使能
} tc_reg_acomp1_cfg_field_t;

// reg_lvd_top       0xd044
// ctrl_lvd_top<12:0>: contrl bit of lvd5, default value: 0_0000_0001_0000
typedef struct
{
    __IO uint32_t ctrl_lvd_trip_volt                        :8;     // <7:0>: the trip voltage select bits,default 0001_0000
                                                                    // 0000_0001：1.7V；0000_0010：1.9V；……….1000_0000：3.1V。
    __IO uint32_t ctrl_lvd_enable                           :1;     // <8>: module enable signal.(high power model)  0:disable; 1:enable; default 0.
    __IO uint32_t ctrl_lvd_vref_sel                         :3;     /** <11:9>: vref_sel_ctrl,default 000.
                                                                        000~010:PB01-PB03；011：vd5；100：test_lvdin；101-111：gnd。*/
    __IO uint32_t ctrl_lvd_lp_enable                        :1;     // <12>: module enable signal.(low power model) 0:disable; 1:enable; default 0.
} tc_reg_lvd_top_field_t;

// reg_lvd_cfg      0xd048
typedef struct
{
    __IO uint32_t cfg_cnt_max                              :16;     /** 数字滤波：滤波时间/fclk_lvd周期 */
    __IO uint32_t reserved                                 :10;
    __IO uint32_t cfg_irq_mode                             :3;      /** <26>1-使能上升沿沿触发中断；0-关闭该功能
                                                                        <27>1-使能下降沿触发中断；0-关闭该功能
                                                                        <28>1-使能高电平触发中断；0-关闭该功能                       **/
    __IO uint32_t cfg_rst_en                               :1;      // reset, default:0
    __IO uint32_t cfg_trig_en                              :1;      // 1-中断配置为Tinywork事件发生源, 0-关闭该功能. default:0
    __IO uint32_t cfg_en                                   :1;      // 模块使能 default:0
} tc_reg_lvd_cfg_field_t;

// chip_defeature,      0xd04c
typedef struct
{
    __IO uint32_t fuse_enable_flash_shrink                 :1;
    __IO uint32_t fuse_enable_sram_shrink                  :1;
    __IO uint32_t reserved                                 :6;
    __IO uint32_t fuse_disable_swd                         :1;
    __IO uint32_t fuse_disable_sdadc                       :1;
    __IO uint32_t fuse_disable_saradc                      :1;
    __IO uint32_t fuse_disable_idac0                       :1;
    __IO uint32_t fuse_disable_idac1                       :1;
    __IO uint32_t fuse_disable_vdac                        :1;
    __IO uint32_t fuse_disable_op0                         :1;
    __IO uint32_t fuse_disable_op1                         :1;
    __IO uint32_t fuse_disable_op2                         :1;
    __IO uint32_t fuse_disable_qspi                        :1;
} tc_chip_defeature_field_t;


/**************************************************************************************/

typedef struct
{
    union
    {
        __IO uint32_t RBR; // Addr 0x0 includes 'Receive Buffer Register' 'Transmit Holding Register' 'Divisor Latch Low'
        __IO uint32_t TBR; // Addr 0x0 includes 'Receive Buffer Register' 'Transmit Holding Register' 'Divisor Latch Low'
        __IO uint32_t DLL;
    };
    union
    {
        __IO uint32_t DLH; // Addr 0x4 includes 'Divisor Latch High' 'Interrupt Enable Register'
        __IO uint32_t IER; // Addr 0x4 includes 'Divisor Latch High' 'Interrupt Enable Register'
    };
    union
    {
        __IO uint32_t IIR; // Addr 0x8 includes 'Interrupt Identification Register' 'FIFO Control Register'
        __IO uint32_t FCR; // Addr 0x8 includes 'Interrupt Identification Register' 'FIFO Control Register'
    };
    __IO uint32_t LCR;  // 0x0C -> Line Control Register
    __IO uint32_t MCR;  // 0x10 -> Modem Control Register
    __IO uint32_t LSR;  // 0x14 -> Line Status Register
    __IO uint32_t MSR;  // 0x18 -> Modem Status Register
    __IO uint32_t SCR;  // 0x1C -> Scratchpad Register
    __IO uint32_t MFCR; // 0x20 -> Main Function Control Register
    __IO uint32_t EFCR; // 0x24 -> Extened Function Control Register
    __IO uint32_t LPDR; // 0x28 -> SIR Divisor Register
    __IO uint32_t FCNR; // 0x2C -> Tx and Rx FIFO Character Number Register
    __IO uint32_t ADCR; // 0x30 -> Auto-baud Detect Control Register
    __IO uint32_t ADRR; // 0x34 -> Auto-baud Detect Result Register
    __IO uint32_t RTIC;
} T_UART;

#define UART_LSR_RX_DATA_READY    ((uint32_t)0x00000001)
#define UART_LSR_RX_OVERRUN_ERROR ((uint32_t)0x00000002)
#define UART_LSR_RX_PARITY_ERROR  ((uint32_t)0x00000004)
#define UART_LSR_RX_FRAME_ERROR   ((uint32_t)0x00000008)
#define UART_LSR_RX_BREAK         ((uint32_t)0x00000010)
#define UART_LSR_TX_DATA_REQ      ((uint32_t)0x00000020)
#define UART_LSR_TX_EMPTY         ((uint32_t)0x00000040)
#define UART_LSR_RX_FIFO_ERROR    ((uint32_t)0x00000080)

typedef struct
{
    __IO uint32_t CR0;           /*!< Offset: 0x000 Control Register 0 (R/W) */
    __IO uint32_t CR1;           /*!< Offset: 0x004 Control Register 1 (R/W) */
    __IO uint32_t DR;            /*!< Offset: 0x008 Data Register (R/W) */
    __I  uint32_t SR;            /*!< Offset: 0x00C Status Registe (R/ ) */
    __IO uint32_t CPSR;          /*!< Offset: 0x010 Clock Prescale Register (R/W) */
    __IO uint32_t IMSC;          /*!< Offset: 0x014 Interrupt Mask Set and Clear Register (R/W) */
    __I uint32_t RIS;            /*!< Offset: 0x018 Raw Interrupt Status Register (R/) */
    __I uint32_t MIS;            /*!< Offset: 0x01C Masked Interrupt Status Register (R/) */
    __O uint32_t ICR;            /*!< Offset: 0x020 SPIICR Interrupt Clear Register (/W) */
    __O uint32_t DMACR;          /* Offset:   0x024 DMA control register */
} T_SPI;

#define I2C_I2CFSR_TX_FIFO_FREE_NUM ((uint32_t)0x0000001F)
#define I2C_I2CFSR_RX_FIFO_DATA_NUM ((uint32_t)0x00001F00)

#define I2C_I2CISR_TX_FIFO_EMPTY             ((uint32_t)0x00000001)
#define I2C_I2CISR_TX_FIFO_UNDERRUN          ((uint32_t)0x00000002)
#define I2C_I2CISR_TX_FIFO_OVERFLOW          ((uint32_t)0x00000004)
#define I2C_I2CISR_RX_FIFO_FULL              ((uint32_t)0x00000008)
#define I2C_I2CISR_RX_FIFO_OVERFLOW          ((uint32_t)0x00000010)
#define I2C_I2CISR_TX_ONE_DATA               ((uint32_t)0x00000020)
#define I2C_I2CISR_RX_ONE_DATA               ((uint32_t)0x00000040)
#define I2C_I2CISR_RX_NACK                   ((uint32_t)0x00000080)
#define I2C_I2CISR_SLAVE_ADDR_MATCHED        ((uint32_t)0x00000100)
#define I2C_I2CISR_TRANSFER_DONE             ((uint32_t)0x00000200)
#define I2C_I2CISR_DETECT_STOP               ((uint32_t)0x00000400)
#define I2C_I2CISR_BUS_ERROR                 ((uint32_t)0x00000800)
#define I2C_I2CISR_ARBITRATATION_LOST        ((uint32_t)0x00001000)
#define I2C_I2CISR_WAIT_TX_FIFO              ((uint32_t)0x00002000)
#define I2C_I2CISR_WAIT_RX_FIFO              ((uint32_t)0x00004000)
#define I2C_I2CISR_WAIT_FOR_BUS_IDLE_TIMEOUT ((uint32_t)0x00008000)
#define I2C_I2CISR_STRETCH_SCL_TIMEOUT       ((uint32_t)0x00010000)
#define I2C_I2CISR_DEDICATE_POINT            ((uint32_t)0x00020000)


#define I2C_I2CSTR_BUSY                 ((uint32_t)0x00000001)
#define I2C_I2CSTR_ADDRESS_PHASE        ((uint32_t)0x00000002)
#define I2C_I2CSTR_DATA_PHASE           ((uint32_t)0x00000004)
#define I2C_I2CSTR_SLAVE_ADDRESSED_FLAG ((uint32_t)0x00000100)
#define I2C_I2CSTR_SLAVE_ADDRESSED_IND  ((uint32_t)0x00000200)
#define I2C_I2CSTR_SLAVE_RWN            ((uint32_t)0x00000400)
#define I2C_I2CSTR_TXRX_DATA_NUM        ((uint32_t)0x01FF0000)



typedef struct
{
    __IO uint32_t I2CMCR; // 0x0 -> I2C Main Control Register
    __IO uint32_t I2CSCR; // 0x4 -> I2C Secondary Control Register
    __IO uint32_t I2CSAR; // 0x8 -> I2C Slave Address Register
    __IO uint32_t I2CTPR; // 0xC -> I2C Timing Parameter Register
    union
    {
        __IO uint32_t I2CTDR; // Addr 0x10 includes 'I2C Transmit Data Register' 'I2C Receive Data Register'
        __IO uint32_t I2CRDR; // Addr 0x10 includes 'I2C Transmit Data Register' 'I2C Receive Data Register'
    };
    __IO uint32_t I2CTOR; // 0x14 -> I2C Timeout Register
    __IO uint32_t I2CISR; // 0x18 -> I2C Interrupt Status Register
    __IO uint32_t I2CIER; // 0x1C -> I2C Interrupt Enable Register
    __IO uint32_t I2CIMR; // 0x20 -> I2C Interrupt Mask Register
    __IO uint32_t I2CSTR; // 0x24 -> I2C Status Register
    __IO uint32_t I2CFSR; // 0x28 -> I2C FIFO Status Register
} T_I2C;


typedef struct
{
    /*start 0x40005000 */
    union
    {
        __IO uint32_t   PA_00;           //PA00;   0x40005000
        tc_pinmux_config_field_t PA_00_f;
    };
    union
    {
        __IO uint32_t   PA_01;           //PA01;   0x40005004
        tc_pinmux_config_field_t PA_01_f;
    };
    __IO uint32_t   PA_02_nouse;         //PA02_nouse;   0x40005008
    __IO uint32_t   PA_03_nouse;         //PA03_nouse;   0x4000500c
    __IO uint32_t   PA_04_nouse;         //PA04_nouse;   0x40005010
    __IO uint32_t   PA_05_nouse;         //PA05_nouse;   0x40005014
    __IO uint32_t   PA_06_nouse;         //PA06_nouse;   0x40005018
    __IO uint32_t   PA_07_nouse;         //PA07_nouse;   0x4000501c
    union
    {
        __IO uint32_t   PB_00;           //PB00;   0x40005020
        tc_pinmux_config_field_t PB_00_f;
    };
    union
    {
        __IO uint32_t   PB_01;           //PB01;  0x40005024
        tc_pinmux_config_field_t PB_01_f;
    };
    union
    {
        __IO uint32_t   PB_02;           //PB02;  0x40005028
        tc_pinmux_config_field_t PB_02_f;
    };
    union
    {
        __IO uint32_t   PB_03;           //PB03;  0x4000502c
        tc_pinmux_config_field_t PB_03_f;
    };
    union
    {
        __IO uint32_t   PB_04;           //SWDIO; 0x40005030
        tc_pinmux_config_field_t PB_04_f;
    };
    union
    {
        __IO uint32_t   PB_05;           //SWCLK; 0x40005034
        tc_pinmux_config_field_t PB_05_f;
    };
    union
    {
        __IO uint32_t   PB_06;           //PB06;  0x40005038
        tc_pinmux_config_field_t PB_06_f;
    };
    union
    {
        __IO uint32_t   PB_07;           //PB07;  0x4000503C
        tc_pinmux_config_field_t PB_07_f;
    };
    union
    {
        __IO uint32_t   PC_00;           //PC00;  0x40005040
        tc_pinmux_config_field_t PC_00_f;
    };
    union
    {
        __IO uint32_t   PC_01;           //PC01;  0x40005044
        tc_pinmux_config_field_t PC_01_f;
    };
    union
    {
        __IO uint32_t   PC_02;           //PC02;  0x40005048
        tc_pinmux_config_field_t PC_02_f;
    };
    union
    {
        __IO uint32_t   PC_03;           //PC03;  0x4000504C
        tc_pinmux_config_field_t PC_03_f;
    };
    union
    {
        __IO uint32_t   PC_04;           //PC04;  0x40005050
        tc_pinmux_config_field_t PC_04_f;
    };
    union
    {
        __IO uint32_t   PC_05;           //PC05;  0x40005054
        tc_pinmux_config_field_t PC_05_f;
    };
    union
    {
        __IO uint32_t   PC_06;           //PC06;  0x40005058
        tc_pinmux_config_field_t PC_06_f;
    };
    union
    {
        __IO uint32_t   PC_07;           //PC07;  0x4000505C
        tc_pinmux_config_field_t PC_07_f;
    };
    union
    {
        __IO uint32_t   PD_00;           //PD00;  0x40005060
        tc_pinmux_config_field_t PD_00_f;
    };
    union
    {
        __IO uint32_t   PD_01;           //PD01;  0x40005064
        tc_pinmux_config_field_t PD_01_f;
    };
    union
    {
        __IO uint32_t sw_ma30_en;//SW_MA30_EN;  0x40005068
        tc_pinmux_ma30en_field_t SW_MA30_EN_f;
    };
} T_PINMUXELSE;

typedef struct
{
    /*start 0x40009000 */
    union
    {
        __IO uint32_t   PA_02;           //PA02;  0x40009000
        tc_pinmux_config_field_t PA_02_f;
    };
    union
    {
        __IO uint32_t   PA_03;           //PA03;  0x40009004
        tc_pinmux_config_field_t PA_03_f;
    };
    union
    {
        __IO uint32_t   PA_04;           //PA04;  0x40009008
        tc_pinmux_config_field_t PA_04_f;
    };
    union
    {
        __IO uint32_t   PA_05;           //PA05;  0x4000900c
        tc_pinmux_config_field_t PA_05_f;
    };
    union
    {
        __IO uint32_t   PA_06;           //PA06;  0x40009010
        tc_pinmux_config_field_t PA_06_f;
    };
    union
    {
        __IO uint32_t   PA_07;           //PA07;  0x40009014
        tc_pinmux_config_field_t PA_07_f;
    };

} T_PINMUX2_7;


typedef struct
{
    union
    {
        __IO uint32_t sdm;
        tc_sysctl_sdm_field_t SDM_f;                                      /**< SDM,0x00 */
    };
    union
    {
        __IO uint32_t aontop_clkrst_ctrl;
        tc_sysctl_aontop_clkrst_ctrl_field_t AONTOP_CLKRST_CTRL_f;        /**< AONTOP_CLKRST_CTRL,0x04 */
    };
    union
    {
        __IO uint32_t pmu_ctrl;
        tc_sysctl_pmu_ctrl_field_t PMU_CTRL_f;                           /**< PMU_CTRL,0x08 */
    };
    union
    {
        __IO uint32_t m0_clkrst_ctrl;
        tc_sysctl_m0_clkrst_ctrl_field_t MO_CLKRST_CTRL_f;               /**< MO_CLKRST_CTRL, 0x0c */
    };
    union
    {
        __IO uint32_t hclk_pclk_ctrl;
        tc_sysctl_hclk_pclk_ctrl_field_t HCLK_PCLK_CTRL_f;               /**< HCLK_PCLK_CTRL, 0x10 */
    };
    union
    {
        __IO uint32_t ahb2apb_clkrst_ctrl;
        tc_sysctl_ahb2apb_clkrst_ctrl_field_t AHB2APB_CLKRST_CTRL_f;  /**< AHB2APB_CLKRST_CTRL, 0x14 */
    };
    union
    {
        __IO uint32_t psram_clkrst_ctrl;
        tc_sysctl_psram_clkrst_ctrl_field_t PSRAM_CLKRST_CTRL_f;      /**< PSRAM_CLKRST_CTRL, 0x18 */
    };
    union
    {
        __IO uint32_t lpgpt_clkrst_ctrl;
        tc_sysctl_lpgpt_clkrst_ctrl_field_t LPGPT_CLKRST_CTRL_f;      /**< LPGPT_CLKRST_CTRL, 0x1C */
    };
    union
    {
        __IO uint32_t uart2_clkrst_ctrl;
        tc_sysctl_uart_clkrst_ctrl_field_t UART2_CLKRST_CTRL_f;      /**< UART2_CLKRST_CTRL, 0x20 */
    };
    union
    {
        __IO uint32_t crypto_clkrst_ctrl;
        tc_sysctl_crypto_clkrst_ctrl_field_t CRYPTO_CLKRST_CTRL_f;      /**< CRYPTO_CLKRST_CTRL, 0x24 */
    };
    union
    {
        __IO uint32_t gpt0_clkrst_ctrl;
        tc_sysctl_gpt_clkrst_ctrl_field_t GPT0_CLKRST_CTRL_f;      /**< GPT0_CLKRST_CTRL, 0x28 */
    };
    union
    {
        __IO uint32_t gpt1_clkrst_ctrl;
        tc_sysctl_gpt_clkrst_ctrl_field_t GPT1_CLKRST_CTRL_f;      /**< GPT1_CLKRST_CTRL, 0x2C */
    };
    union
    {
        __IO uint32_t gpt2_clkrst_ctrl;
        tc_sysctl_gpt_clkrst_ctrl_field_t GPT2_CLKRST_CTRL_f;      /**< GPT2_CLKRST_CTRL, 0x30 */
    };
    union
    {
        __IO uint32_t pwm0_clkrst_ctrl;
        tc_sysctl_pwm_clkrst_ctrl_field_t PWM0_CLKRST_CTRL_f;         /**< PWM0_CLKRST_CTRL, 0x34 */
    };
    union
    {
        __IO uint32_t pwm1_clkrst_ctrl;
        tc_sysctl_pwm_clkrst_ctrl_field_t PWM1_CLKRST_CTRL_f;         /**< PWM1_CLKRST_CTRL, 0x38 */
    };
    union
    {
        __IO uint32_t pwm2_clkrst_ctrl;
        tc_sysctl_pwm_clkrst_ctrl_field_t PWM2_CLKRST_CTRL_f;        /**< PWM2_CLKRST_CTRL, 0x3C */
    };
    union
    {
        __IO uint32_t i2c0_clkrst_ctrl;
        tc_sysctl_i2c_clkrst_ctrl_field_t I2C0_CLKRST_CTRL_f;         /**< I2C0_CLKRST_CTRL, 0x40 */
    };
    union
    {
        __IO uint32_t i2c1_clkrst_ctrl;
        tc_sysctl_i2c_clkrst_ctrl_field_t I2C1_CLKRST_CTRL_f;         /**< I2C1_CLKRST_CTRL, 0x44 */
    };

    union
    {
        __IO uint32_t spi0_clkrst_ctrl;
        tc_sysctl_spi_clkrst_ctrl_field_t SPI0_CLKRST_CTRL_f;          /**< SPI0_CLKRST_CTRL, 0x48 */
    };
    union
    {
        __IO uint32_t spi1_clkrst_ctrl;
        tc_sysctl_spi_clkrst_ctrl_field_t SPI1_CLKRST_CTRL_f;           /**< SPI1_CLKRST_CTRL, 0x4C */
    };

    union
    {
        __IO uint32_t uart0_clkrst_ctrl;
        tc_sysctl_uart_clkrst_ctrl_field_t UART0_CLKRST_CTRL_f;         /**< UART0_CLKRST_CTRL, 0x50 */
    };
    union
    {
        __IO uint32_t uart1_clkrst_ctrl;
        tc_sysctl_uart_clkrst_ctrl_field_t UART1_CLKRST_CTRL_f;         /**< UART1_CLKRST_CTRL, 0x54 */
    };

    union
    {
        __IO uint32_t wdg_clkrst_ctrl;
        tc_sysctl_wdg_clkrst_ctrl_field_t WDG_CLKRST_CTRL_f;            /**< WDG_CLKRST_CTRL, 0x58 */
    };
    union
    {
        __IO uint32_t ext_pwm_en;
        tc_sysctl_ext_pwm_en_field_t EXT_PWM_EN_f;                      /**< EXT_PWM_EN, 0x5C */
    };
    union
    {
        __IO uint32_t cp0_clkrst_ctrl;
        tc_sysctl_cp_clkrst_ctrl_field_t CP0_CLKRST_CTRL_f;            /**< CP0_CLKRST_CTRL, 0x60 */
    };
    union
    {
        __IO uint32_t cp1_clkrst_ctrl;
        tc_sysctl_cp_clkrst_ctrl_field_t CP1_CLKRST_CTRL_f;            /**< CP1_CLKRST_CTRL, 0x64 */
    };
    union
    {
        __IO uint32_t lvd_clkrst_ctrl;
        tc_sysctl_lvd_clkrst_ctrl_field_t LVD_CLKRST_CTRL_f;            /**< LVD_CLKRST_CTRL, 0x68 */
    };

    union
    {
        __IO uint32_t saradc_clkrst_ctrl;
        tc_sysctl_saradc_clkrst_ctrl_field_t SARADC_CLKRST_CTRL_f;      /**< SARADC_CLKRST_CTRL, 0x6C */
    };
    __IO uint32_t reserve1;                                             /**< reserve          , 0x70 */
    union
    {
        __IO uint32_t vdac_clkrst_ctrl;
        tc_sysctl_vdac_clkrst_ctrl_field_t VDAC_CLKRST_CTRL_f;         /**< VDAC_CLKRST_CTRL, 0x74 */
    };
    union
    {
        __IO uint32_t spicfg_clkrst_ctrl;
        tc_sysctl_spicfg_clkrst_ctrl_field_t SPICFG_CLKRST_CTRL_f;     /**< SPICFG_CLKRST_CTRL, 0x78 */
    };
    union
    {
        __IO uint32_t tp;
        tc_sysctl_tp_field_t TP_f;                                      /**< TP,               0x7C */
    };
} T_SYSCTL;

typedef struct
{
    union
    {
        __IO uint32_t RCTC;
        tc_flash_rctc_field_t RCTC_f;        /**< Read Cycle Timing Configuration,offset: 0x0 */
    };
    union
    {
        __IO uint32_t WCTC1;
        tc_flash_wctc1_field_t wctc1_f;      /**< Write (Program/Erase) Cycle Timing Configuration 1,offset: 0x4 */
    };
    union
    {
        __IO uint32_t WCTC2;
        tc_flash_wctc2_field_t wctc2_f;     /**< Write (Program/Erase) Cycle Timing Configuration 2,offset: 0x8 */
    };
    union
    {
        __IO uint32_t ECONF;
        tc_flash_econf_field_t ECONF_f;     /**< Erase Configuration,offset: 0xC */
    };
    union
    {
        __IO uint32_t ECOMM;
        tc_flash_ecomm_field_t ECOMM_f;    /**< Erase Command Register,offset: 0x10 */
    };
    union
    {
        __IO uint32_t MCR;
        tc_flash_mcr_field_t MCR_f;        /**< Miscellaneous Control Register,offset: 0x14 */
    };

} T_FLASH;

typedef struct
{
    __IO uint32_t latch_irq;
    __IO uint32_t irq_status1;
    __IO uint32_t mask1;
    __IO uint32_t pend_ena1;
    __IO uint32_t sw_gen_irq1;
    __IO uint32_t clr_irq1;
    __IO uint32_t clr_ovf1;
    __IO uint32_t ovf_status1;
} T_FIC;


typedef struct
{
    __IO uint32_t latch_irq;
    __IO uint32_t irq_status1;
    __IO uint32_t mask1;
    __IO uint32_t pend_ena1;
    __IO uint32_t sw_gen_irq1;
    __IO uint32_t clr_irq1;
    __IO uint32_t clr_ovf1;
    __IO uint32_t ovf_status1;
} T_ALIC;

typedef struct
{
    __IO uint32_t cfg;          // gpt work mode configuartion, bit[8:1] can't be changed when bit[0] is high
    __IO uint32_t sw_force_con;
    __IO uint32_t prescale;
    __IO uint32_t period;
    __IO uint32_t repeat_value;
    __IO uint32_t latched_cnt;
    __IO uint32_t matcha_op;
    __IO uint32_t matcha0_val;
    __IO uint32_t matcha1_val;
    __IO uint32_t matcha2_val;
    __IO uint32_t matcha3_val;
    __IO uint32_t matchb_op;
    __IO uint32_t matchb0_val;
    __IO uint32_t matchb1_val;
    __IO uint32_t matchb2_val;
    __IO uint32_t matchb3_val;
    __IO int32_t delta;
} T_TIMER;
typedef struct
{
    __IO uint32_t wdg_enable                          : 1;    /* write 1 to enable wdg */
    __IO uint32_t wdg_rst_en_for_ovf                  : 1;    /* write 1 to enable rst upon ovf_int */
    __IO uint32_t wdg_rst_en_for_t2                   : 1;    /* write 1 to enable rst upon t2_int,   note: only availabllee for wwdg */
    __IO uint32_t wdg_rst_en_for_adv                  : 1;    /* write 1 to enable rst upon adv_int,  note: only availabllee for wwdg */
} tc_wdg_ctrl_field_t;
typedef struct
{
    __IO uint32_t wdg_cnt                             : 7;    /* wdg counter */
    __IO uint32_t ovf_int                             : 1;    /* interrupt status for ovf */
    __IO uint32_t t2_int                              : 1;    /* interrupt status for t2 , note : only available for wwdg */
    __IO uint32_t adv_int                             : 1;    /* interrupt status for adv, note : only availabe for wwdg */
} tc_wdg_status_field_t;

typedef struct
{
    __IO uint32_t clr_ovf_int                         : 1;    /* write 1 to clear ovf_int, self-cleared*/
    __IO uint32_t clr_t2_int                          : 1;    /* write 1 to clear t2_int, self-cleared, only availabllee for wwdg */
    __IO uint32_t clr_adv_int                         : 1;    /* write 1 to clear adv_int, self-cleared, only availabllee for wwdg */
} tc_wdg_icr_field_t;


typedef struct
{
    union
    {
        __IO uint32_t  ctrl;
        __IO tc_wdg_ctrl_field_t ctrl_f;                      /* Watchdog Control Register */
    };
    __IO uint32_t cnt_max;                                    /* Watchdog Count Register */
    __IO uint32_t wt;                                         /* Watchdog WT time Register */
    __IO uint32_t t2;                                         /* Watchdog T2 time Register */
    __IO uint32_t ccr;                                        /* Watchdog Reload Register */
    union
    {
        __IO uint32_t  icr;
        __IO tc_wdg_icr_field_t icr_f;                        /* Watchdog Interupt Control Register */
    };
    __IO uint32_t lock;                                       /* write 0x12345678 to [31:0] to lock all the above configuration registers
                                                                 write 0xaaaa5555 to [31:0] to unlock all the above configuration registers*/
    union
    {
        __IO uint32_t  status;
        __IO tc_wdg_status_field_t status_f;                  /* Watchdog Status Register */
    };
} T_WDG;

typedef struct
{
    union
    {
        __IO uint32_t  clk_en;
        __IO tc_clk_en_field_t clk_en_f;
    };
    __IO uint32_t hclk_sel;
} T_ACLKRST;

typedef struct
{
    __IO uint32_t wait_cycle;
    __IO uint32_t sleep_mode;
    __IO uint32_t areg[4];
} T_PMU;




typedef struct
{
    __IO uint32_t comp0_trim                               :1;
    __IO uint32_t comp1_trim                               :1;
} tc_trim_comparator_value2_field_t;


typedef struct
{
    union
    {
        __IO uint32_t reg_analog_block_enable;
        tc_analog_block_enable_field_t an_block_f;
    };
    union
    {
        __IO uint32_t reg_analog_block_enable_vcclow;
        tc_analog_block_enable_vcclow_t an_block_vcclow_f;
    };
    union
    {
        __IO uint32_t reg_power_switch_top;
        tc_power_switch_top_t power_switch_f;
    };
    union
    {
        __IO uint32_t reg_oscrc32k_1p5v_top;
        tc_oscrc32k_1p5v_top_t oscrc32k_1p5v_f;
    };

    union
    {
        __IO uint32_t reg_clk_deglitch_2500n;       //0x10
        tc_reg_clk_deglitch_2500n_field_t clk_deglitch_2500n_f;
    };
    union
    {
        __IO uint32_t reg_xosc32k_top;              //0x14
        tc_reg_xosc32k_top_field_t reg_xosc32k_top_f;
    };
    union
    {
        __IO uint32_t reg_ldo3_top;                 //0x18
        tc_reg_ldo3_top_field_t reg_ldo3_top_f;
    };
    union
    {
        __IO uint32_t reg_clkready_32k_withDiv2;    //0x1c
        tc_reg_clkready_32k_withDiv2_field_t reg_clkready_32k_withDiv2_f;
    };
    union
    {
        __IO uint32_t reg_ldo15_top;                //0x20

        tc_reg_ldo15_top_field_t reg_ldo15_top_f;
    };
    union
    {
        __IO uint32_t reg_vbias_ibias_top;          //0x24
        tc_reg_vbias_ibias_top_field_t reg_vbias_ibias_top_f;
    };
    union
    {
        __IO uint32_t reg_opa_iv;                       //0x28  default: 19'h2F
        tc_reg_opa_iv_field_t reg_opa_iv_f;
    };
    union
    {
        __IO uint32_t reg_comparator_top1;          //0x2c
        tc_reg_comparator_top1_field_t reg_comparator_top1_f;
    };
    union
    {
        __IO uint32_t reg_comparator_top2;          //0x30
        tc_reg_comparator_top2_field_t reg_comparator_top2_f;
    };
    union
    {
        __IO uint32_t reg_comparator_top3;          //0x34
        tc_reg_comparator_top3_field_t reg_comparator_top3_f;
    };
    union
    {
        __IO uint32_t reg_acomp_dac;                //0x38
        tc_reg_acomp_dac_field_t reg_acomp_dac_f;
    };
    union
    {
        __IO uint32_t reg_acomp0_cfg;               //0x3c
        tc_reg_acomp0_cfg_field_t reg_acomp0_cfg_f;
    };
    union
    {
        __IO uint32_t reg_acomp1_cfg;               //0x40
        tc_reg_acomp1_cfg_field_t reg_acomp1_cfg_f;
    };
    union
    {
        __IO uint32_t reg_lvd_top;                  //0x44
        tc_reg_lvd_top_field_t reg_lvd_top_f;
    };
    union
    {
        __IO uint32_t reg_lvd_cfg;                  //0x48
        tc_reg_lvd_cfg_field_t reg_lvd_cfg_f;
    };
    union
    {
        __IO uint32_t chip_defeature;               //0x4c
        tc_chip_defeature_field_t chip_defeature_f;
    };
}  T_AON_ANACFG;
typedef struct
{
    __IO uint32_t trim_cp0_1p5v                            :3;               //trim bit for cp0, default value: 000;
    __IO uint32_t trim_cp1_1p5v                            :3;               //trim bit for cp1, default value: 000;
} tc_trim_comparator_value_field_t;

typedef struct
{
    __IO uint32_t trim_lpldo_value;//0x0
    __IO uint32_t trim_oscrc32k_value;//0x04
    __IO uint32_t trim_vbias_ibias_value;//0x08
    __IO uint32_t trim_opamp_iv_value;//0x0c
    union
    {
        __IO uint32_t trim_comparator_value;//0x10
        tc_trim_comparator_value_field_t trim_comparator_value_f;
    };
} T_AON_ANACFG_TRIM;

/*0x40150380*/
typedef struct
{
    union
    {
        __IO uint32_t tsdm_verf_trim;
        tc_tsdm_verf_trim_field_t tsdm_verf_trim_f;    //0x00
    };
    union
    {
        __IO uint32_t temp_rtc_0d0625;
        tc_temp_rtc_0d0625_field_t temp_rtc_0d0625_f;  //0x04
    };
    union
    {
        __IO uint32_t tsdm_trim_prot;
        tc_tsdm_trim_prot_field_t tsdm_trim_prot_f;  //0x08
    };
    union
    {
        __IO uint32_t tsdm_trim_cfg1;
        tc_tsdm_trim_cfg1_field_t tsdm_trim_cfg1_f;  //0x0c
    };
    union
    {
        __IO uint32_t tsdm_trim_cfg2;
        tc_tsdm_trim_cfg2_field_t tsdm_trim_cfg2_f;  //0x10
    };
    union
    {
        __IO uint32_t tsdm_ctrl;
        tc_tsdm_ctrl_field_t tsdm_ctrl_f;  //0x14
    };
    union
    {
        __IO uint32_t tsdm_dig_cfg1;
        tc_tsdm_dig_cfg1_field_t tsdm_dig_cfg1_f;  //0x18
    };
    union
    {
        __IO uint32_t tsdm_dig_cfg2;
        tc_tsdm_dig_cfg2_field_t tsdm_dig_cfg2_f;  //0x1c
    };
    union
    {
        __IO uint32_t tsdm_cal_cfg;
        tc_tsdm_cal_cfg_field_t tsdm_cal_cfg_f;  //0x20
    };
    union
    {
        __IO uint32_t tsdm_dig_adc;
        tc_tsdm_dig_adc_field_t tsdm_dig_adc_f;  //0x24
    };
    union
    {
        __IO uint32_t tsdm_threshold;
        tc_tsdm_threshold_field_t tsdm_threshold_f;  //0x28
    };
    union
    {
        __IO uint32_t tsdm_int;
        tc_tsdm_int_field_t tsdm_int_f;  //0x2c
    };

} T_TEMP_SENSOR;
typedef struct
{
    __IO uint32_t reg_rail_op_mux_in4_vipsb     :2;             /*   OP2 vip sel bit
                                                                    01: input_p from vip_pad_shield<0>.?PD00
                                                                    10: input_p from vip_pad_shield<1>.?PC03*/
//    __IO uint32_t reserved1                     :15;
    __IO uint32_t reg_rail_op_mux_in4_vinsb     :2;             /*OP2 vin sel bit
                                                                    01: input_n from vin_pad_shield<0>.?PC07
                                                                    10: input_n from vin_pad_shield<1>.?PC04*/
    __IO uint32_t reserved2                     :13;
} tc_reg_rail_op_mux_in4_field_t;

typedef struct
{
    __IO uint32_t reg_rail_op_mux_in2_op2o      :2;             /*rail op2 output sel bit
                                                                    01: output to a_pad<0>? PD01
                                                                    10: output to a_pad<1>? PC05*/
    __IO uint32_t reserved                      :30;
} tc_reg_rail_op2_mux_in2_field_t;


typedef struct
{
    __IO uint32_t reg_rail_opa_be         : 1;                //buffer en. default 0
    __IO uint32_t reg_rail_opa_gains      : 3;                /*gain sel for in phase or inverse amp. deault 000.
                                                                    in phase: 000-111：x1.21,x1.45,x1.94,x2.9,x4.85,x8.72,x16.5,x32
                                                                    inverse: 000-111：x0.25,x0.5,x1,x2,x4,x8,x16,x32*/
    __IO uint32_t reg_rail_opa_vope       : 1;                //vout of op to pad enable, default 0。
    __IO uint32_t reg_rail_opa_resinsel   : 2;                /*Res in sel, default 10
                                                                    11: to vip_pad,10: to vin_pad, 01: to vip_chip, 00:to gnd.*/
    __IO uint32_t reg_rail_opa_vns        : 2;                /*vinn sel, default 00
                                                                    11:vin_pad; 10: vin_chip, 01: pga(in phase or inverse), 00:buffer.*/
    __IO uint32_t reg_rail_opa_vps        : 2;                /*vinp sel, default 00
                                                                    11:vip_pad; 10: vip_chip, 01:vip_ref(ref=vdac/idac/vcm); 00:disable.*/
    __IO uint32_t reg_rail_opa_fre        : 1;                //feedback res en, default 0.
    __IO uint32_t reg_rail_opa_vcvpe      : 1;                //vout_chip conect to vout_pad en, default 0.
    __IO uint32_t reg_rail_opa_chop       : 1;                //chop, used for trimming offset default 0
    __IO uint32_t reg_rail_opa_me         : 1;                //module enable signal. en, default 0.
    __IO uint32_t reg_rail_opa_ote        : 1;                //output test en, default 0.
    __IO uint32_t reg_rail_opa_svmvc      : 1;                //sel vdac to vip on chip
    __IO uint32_t reg_rail_opa_sivc       : 1;                //sel idac to vip on chip
    __IO uint32_t reg_rail_opa_svcvc      : 1;                //sel vcm to vip on chip
    __IO uint32_t reserved                  : 13;               //reserved
} tc_reg_rail_opa_field_t;

/*
typedef struct
{
    __IO uint32_t reg_rail_opa_2_be         : 1;                //buffer en. default 0
    __IO uint32_t reg_rail_opa_2_gains      : 3;                //gain sel for in phase or inverse amp. deault 000.
                               2                                    in phase: 000-111：x1.21,x1.45,x1.94,x2.9,x4.85,x8.72,x16.5,x32
                               2                                    inverse: 000-111：x0.25,x0.5,x1,x2,x4,x8,x16,x32
    __IO uint32_t reg_rail_opa_2_vope       : 1;                //vout of op to pad enable, default 0。
    __IO uint32_t reg_rail_opa_2_resinsel   : 2;                //Res in sel, default 10
                               2                                    11: to vip_pad,10: to vin_pad, 01: to vip_chip, 00:to gnd.
    __IO uint32_t reg_rail_opa_2_vns        : 2;                //vinn sel, default 00
                               2                                    11:vin_pad; 10: vin_chip, 01: pga(in phase or inverse), 00:buffer.
    __IO uint32_t reg_rail_opa_2_vps        : 2;                //vinp sel, default 00
                               2                                    11:vip_pad; 10: vip_chip, 01:vip_ref(ref=vdac/idac/vcm); 00:disable.
    __IO uint32_t reg_rail_opa_2_fre        : 1;                //feedback res en, default 0.
    __IO uint32_t reg_rail_opa_2_vcvpe      : 1;                //vout_chip conect to vout_pad en, default 0.
    __IO uint32_t reg_rail_opa_2_chop       : 1;                //chop, used for trimming offset default 0
    __IO uint32_t reg_rail_opa_2_me         : 1;                //module enable signal. en, default 0.
    __IO uint32_t reg_rail_opa_2_ote        : 1;                //output test en, default 0.
    __IO uint32_t reg_rail_opa_2_svmvc      : 1;                //sel vdac to vip on chip
    __IO uint32_t reg_rail_opa_2_sivc       : 1;                //sel idac to vip on chip
    __IO uint32_t reg_rail_opa_2_svcvc      : 1;                //sel vcm to vip on chip
    __IO uint32_t reserved                  : 13;               //reserved
} tc_reg_rail_opa_2_field_t;
*/

typedef struct
{
    __IO uint32_t adc_ch_pair0_inp          :4;
    __IO uint32_t reserved1                 :1;
    __IO uint32_t adc_ch_pair0_inn          :4;
    __IO uint32_t reserved2                 :1;
    __IO uint32_t adc_ch_pair0_mode         :1;
    __IO uint32_t reserved3                 :5;
    __IO uint32_t adc_ch_pair1_inp          :4;
    __IO uint32_t reserved4                 :1;
    __IO uint32_t adc_ch_pair1_inn          :4;
    __IO uint32_t reserved5                 :1;
    __IO uint32_t adc_ch_pair1_mode         :1;
} reg_adc_conf3_field_t;


typedef struct
{
    __IO uint32_t adc_ch_pair2_inp          :4;
    __IO uint32_t reserved1                 :1;
    __IO uint32_t adc_ch_pair2_inn          :4;
    __IO uint32_t reserved2                 :1;
    __IO uint32_t adc_ch_pair2_mode         :1;
    __IO uint32_t reserved3                 :5;
    __IO uint32_t adc_ch_pair3_inp          :4;
    __IO uint32_t reserved4                 :1;
    __IO uint32_t adc_ch_pair3_inn          :4;
    __IO uint32_t reserved5                 :1;
    __IO uint32_t adc_ch_pair3_mode         :1;
} reg_adc_conf4_field_t;

typedef struct
{
    __IO uint32_t adc_ch_pair4_inp          :4;
    __IO uint32_t reserved1                 :1;
    __IO uint32_t adc_ch_pair4_inn          :4;
    __IO uint32_t reserved2                 :1;
    __IO uint32_t adc_ch_pair4_mode         :1;
    __IO uint32_t reserved3                 :5;
    __IO uint32_t adc_ch_pair5_inp          :4;
    __IO uint32_t reserved4                 :1;
    __IO uint32_t adc_ch_pair5_inn          :4;
    __IO uint32_t reserved5                 :1;
    __IO uint32_t adc_ch_pair5_mode         :1;
} reg_adc_conf5_field_t;

typedef struct
{
    __IO uint32_t adc_ch_pair6_inp          :4;
    __IO uint32_t reserved1                 :1;
    __IO uint32_t adc_ch_pair6_inn          :4;
    __IO uint32_t reserved2                 :1;
    __IO uint32_t adc_ch_pair6_mode         :1;
    __IO uint32_t reserved3                 :5;
    __IO uint32_t adc_ch_pair7_inp          :4;
    __IO uint32_t reserved4                 :1;
    __IO uint32_t adc_ch_pair7_inn          :4;
    __IO uint32_t reserved5                 :1;
    __IO uint32_t adc_ch_pair7_mode         :1;
} reg_adc_conf6_field_t;

typedef struct
{
    __IO uint32_t adc_ch_pair8_inp          :4;
    __IO uint32_t reserved1                 :1;
    __IO uint32_t adc_ch_pair8_inn          :4;
    __IO uint32_t reserved2                 :1;
    __IO uint32_t adc_ch_pair8_mode         :1;
    __IO uint32_t reserved3                 :1;
    __IO uint32_t adc_raw_out17             :17;
} reg_adc_conf7_field_t;

typedef struct
{
    __IO uint32_t reg_a2d_chip_id_1p5v;//0x0
    __IO uint32_t reg_porb_aon_blias;//0x04
    __IO uint32_t reg_test_mux_top1;//0x08
    __IO uint32_t reg_test_mux_top2;//0x0c

    __IO uint32_t reg_ldo3rf_top;//0x10
    __IO uint32_t reg_bgr;
    __IO uint32_t reg_clk_deglitch_5n;
    __IO uint32_t reg_adsar_top;

    __IO uint32_t reg_adc_mux_ref;//0x20
    __IO uint32_t reg_crystal_16m_top;
    union
    {
        __IO uint32_t reg_adsar_mux_in;
        tc_reg_adsar_mux_in_field_t reg_adsar_mux_in_f;
    };
    __IO uint32_t reg_adsar_chargpump;

    __IO uint32_t reg_fll;      //0x30
    __IO uint32_t reg_fll2;
    __IO uint32_t reg_freq_doubler_top;
    __IO uint32_t reg_pga;

    __IO uint32_t reg_tsdm;      //0x40
    __IO uint32_t reg_clkready_16m_withDiv2;
    __IO uint32_t reg_oscrc_16m_top;
    __IO uint32_t reg_IREEP_6B_top1;            /** idac1_ctrl_1p5v<7:0>: ctrl bit of idac1. default value: 0010_0001.
                                                    <7>:nil.
                                                    <6>:idac1 test en, default 0.
                                                    <5>: module enable signal for idac1, 0:disable; 1:enable; default 1.
                                                    <4:0>: ibias sel bit for idac1 basic current; default 0_0001.
                                                    <4>:8uA; <3>:4uA; <2>:2uA; <1>:1uA; <0>:1uA.
                                                    0_0000:0uA; 0_0001:1uA; 0_0010:1uA; 0_0011:2uA ........ 1_1111:16uA.
                                                    **/

    __IO uint32_t reg_IREEP_6B_top2;            //0x50
                                                /** idac2_ctrl_1p5v<7:0>: ctrl bit of idac1. default value: 0010_0001.
                                                    <7>:nil.
                                                    <6>:idac1 test en, default 0.
                                                    <5>: module enable signal for idac1, 0:disable; 1:enable; default 1.
                                                    <4:0>: ibias sel bit for idac1 basic current; default 0_0001. 
                                                    <4>:8uA; <3>:4uA; <2>:2uA; <1>:1uA; <0>:1uA.
                                                    0_0000:0uA; 0_0001:1uA; 0_0010:1uA; 0_0011:2uA ........ 1_1111:16uA.
                                                    **/
    
    __IO uint32_t reg_idac_mux_in4;             //0x54
                                                /** <3:0>: idac_mux_in4 sel bit, default value: 0000.
                                                <3:2>:  idac2 output sel bit
                                                01: output to idac2->PA01
                                                10: output to idac2->PA03
                                                <1:0>: idac1 output sel bit
                                                01: output to idac1->PA00
                                                10: output to idac1->PA02           */
    union
    {
        __IO uint32_t reg_vdac;                 //0x58
                                                /*  ctrl_vdac<7:0>: ctrl_bit of 12bit vdac. default value: 0010_0000.
                                                    <7>: test en, default 0.
                                                    <6:4>: vcm sel bit, default 010.
                                                    000:1.2v; 001:1.4v; 010:1.5v; 011:1.6v; 100:1.8v; 101:2.0v; 110:2.5v. 111:nil.
                                                    <3:1>: vout gain ctrl bit, default:000.
                                                    000: x1.0; 001:x1.2; 010:x1.5; 011:x1.6; 100:x1.8; 101:x2.0; 110/111:nil.
                                                    <0>: reserved.              */
        tc_reg_vdac_field_t reg_vdac_f;
    };

                                                
    __IO uint32_t reg_vdac_mux_in2;         // 0x5C <1:0>:ctrl_vdac_mux_in2
                                            /*  <1:0>: vdac output sel bit,   default value: 00.
                                                01: output to a_pad<0>?PC01
                                                10: output to a_pad<1>?PC02   */
    union
    {
        __IO uint32_t reg_rail_op_mux_in4;      //0x60
        tc_reg_rail_op_mux_in4_field_t reg_rail_op_mux_in4_f;
    };
    union
    {
        __IO uint32_t reg_rail_op2_mux_in2;
        tc_reg_rail_op2_mux_in2_field_t reg_rail_op2_mux_in2_f;
    };
    union
    {
        __IO uint32_t reg_rail_opa_1;
        tc_reg_rail_opa_field_t reg_rail_opa_1_f;
    };
    union
    {
        __IO uint32_t reg_rail_opa_2;
        tc_reg_rail_opa_field_t reg_rail_opa_2_f;
    };
    __IO uint32_t reg_saradc_ctrl;          //0x70
    __IO uint32_t reg_sdadc_ctrl;
    union
    {
        __IO uint32_t reg_adc_conf1;
        tc_reg_adc_conf1_field_t reg_adc_conf1_f;
    };
    __IO uint32_t reg_adc_comp_bott;

    __IO uint32_t reg_adc_comp_ceil;      //0x80
    union
    {
        __IO uint32_t reg_adc_conf3;                                /*Scan chnnel control, only impact when adc_scan_chnum != 0
                                                                        [26]:  
                                                                        1: pair mode
                                                                        0: none pair mode
                                                                        [19:16] : INP mux
                                                                        [24:21]: INN mux
                                                                        [10]:  
                                                                        1: pair mode
                                                                        0: none pair mode
                                                                        [3:0] : INP mux
                                                                        [8:5]: INN mux
                                                                        */
        reg_adc_conf3_field_t reg_adc_conf3_f;
    };
    union
    {
        __IO uint32_t reg_adc_conf4;
        reg_adc_conf4_field_t reg_adc_conf4_f;
    };
    union
    {
        __IO uint32_t reg_adc_conf5;
        reg_adc_conf5_field_t reg_adc_conf5_f;
    };
    union
    {
        __IO uint32_t reg_adc_conf6;            //0x90
        reg_adc_conf6_field_t reg_adc_conf6_f;
    };
    union
    {
        __IO uint32_t reg_adc_conf7;
        reg_adc_conf7_field_t reg_adc_conf7_f;
    };
    __IO uint32_t reg_adc_conf8;
    __IO uint32_t reg_adc_conf9;

    __IO uint32_t reg_vdac_conf0;           //0xA0  vdac_sw_start
    __IO uint32_t reg_vdac_conf1;
    __IO uint32_t reg_vdac_conf2;           //0xA8  <12:0> Vdac fifo wr
    __IO uint32_t reserve;

    union
    {
        __IO uint32_t reg_adc_fifo_state;   // OxB0
        tc_reg_adc_fifo_state_field_t reg_adc_fifo_state_f;
    };

    __IO uint32_t reserve1[3];

    __IO uint32_t reg_idac_in;          //0xC0    IDAC电流输入值        Iout=I_lbs * d2a_idac1_in
    __IO uint32_t reserve2[3];

    __IO uint32_t reg_tw_conf;          //0xD0
    __IO uint32_t reserve3[3];
} T_ANAREG;

// __IO uint32_t reserve[12];      //0x70 0x80 0x90
typedef struct
{
    __IO uint32_t trim_sigma_delta0_value;      //0x100
    __IO uint32_t trim_sigma_delta1_value;
    __IO uint32_t trim_sigma_delta2_value;
    __IO uint32_t trim_sigma_delta3_value;

    __IO uint32_t trim_sigma_delta4_value;      //0x110
    __IO uint32_t trim_sigma_delta5_value;
    __IO uint32_t trim_sigma_delta6_value;
    __IO uint32_t trim_sigma_delta7_value;

    __IO uint32_t trim_sigma_delta8_value;      //0x120
    __IO uint32_t trim_sigma_delta9_value;
    __IO uint32_t trim_sigma_delta10_value;
    __IO uint32_t trim_sigma_delta11_value;

    __IO uint32_t trim_sigma_delta12_value;      //0x130
    __IO uint32_t trim_sigma_delta13_value;
    __IO uint32_t trim_sigma_delta14_value;
    __IO uint32_t trim_sigma_delta15_value;

    __IO uint32_t trim_sigma_delta16_value;      //0x140
    __IO uint32_t trim_sigma_delta17_value;
    __IO uint32_t trim_sigma_delta18_value;
    __IO uint32_t trim_sigma_delta19_value;
} T_ANAREG_TRIM_SIGMA_DELTA;

// __IO uint32_t reserve1[12];      //0x150 0x160 0x170
typedef struct
{
    __IO uint32_t trim_adsar_value0;      //0x180
    __IO uint32_t trim_adsar_value1;
    __IO uint32_t trim_adsar_value2;
    __IO uint32_t trim_adsar_value3;

    __IO uint32_t trim_adsar_value4;      //0x190
    __IO uint32_t trim_adsar_value5;
    __IO uint32_t trim_adsar_value6;
    __IO uint32_t trim_adsar_value7;

    __IO uint32_t trim_adsar_value8;      //0x1a0
    union
    {
        __IO uint32_t trim_adsar_coeff0;
        tc_trim_adsar_coeff_field_t TRIM_ADSAR_COEFF0_f;     //0x1A4
    };
    union
    {
        __IO uint32_t trim_adsar_coeff1;
        tc_trim_adsar_coeff_field_t TRIM_ADSAR_COEFF1_f;     //0x1A8
    };
    __IO uint32_t trim_vdac_value;

    __IO uint32_t trim_idac1_value;      //0x1b0
    __IO uint32_t trim_idac2_value;
    __IO uint32_t trim_oscrc16m_value;
    // __IO uint32_t reserve2;
} T_ANAREG_TRIM_MISC;
// __IO uint32_t reserve3[16];      //0x1c0 0x1d0 0x1e0 0x1f0
// __IO uint32_t reserve4[64];      //0x200

typedef struct
{
    __IO uint32_t analog_trim_status;      //0x300
    __IO uint32_t reserved;                //0x304
    // __IO uint32_t chip_defeature;          //0x308
} T_ANAREG_2;

typedef struct
{
    __IO uint32_t data;                                              //Data value [15:0]
    __IO uint32_t data_out;                                          //Data output Register value [15:0]
    __IO uint32_t res[2];                                            //Reserved
    __IO uint32_t out_en_set;                                        //Output enable set [15:0]
    __IO uint32_t out_en_clr;                                        //Output enable clear [15:0]
    __IO uint32_t alt_fun_set;                                       //Alternative function set [15:0]
    __IO uint32_t alt_fun_clr;                                       //Alternative function clear [15:0]
    __IO uint32_t int_en_set;                                        //Interrupt enable set [15:0]
    __IO uint32_t int_en_clr;                                        //Interrupt enable clear [15:0]
    __IO uint32_t int_type_set;                                      //Interrupt type set [15:0]
    __IO uint32_t int_type_clr;                                      //Interrupt type clear [15:0]
    __IO uint32_t int_pol_set;                                       //Polarity-level, edge IRQ configuration [15:0]
    __IO uint32_t int_pol_clr;                                       //Polarity-level, edge IRQ configuration [15:0]
    __IO uint32_t int_status;                                        //Write one to clear interrupt request
} T_GPIO;

typedef struct
{
    __IO uint32_t rd_format;
    __IO uint32_t wr_format;
    __IO uint32_t user_cmd;
    __IO uint32_t user_addr;
    __IO uint32_t user_data;
    __IO uint32_t status;
    __IO uint32_t clk_ce_ctrl;
    __IO uint32_t rd_clk_sel;
    __IO uint32_t addr_len;
    __IO uint32_t ce_cfg;
    __IO uint32_t init_done;
    __IO uint32_t version;

} T_PSARM;


typedef struct
{
    __IO uint32_t dividend;
    __IO uint32_t divisor;
    __IO uint32_t quotient;
    __IO uint32_t reminder;
} T_DIV;


// DMA Command Register
typedef struct
{
    uint32_t LEN                        : 13;   // <12:0>     Length of the transfer in bytes
    /* <13>  When this bit is set to 1- When a read occurs that is not an exact multiple of 8 bytes, 
        the DMA issues a read transaction of the highest and closest multiple of 8 bytes.
        (For example, if the read is 28 bytes, a read transaction of 32 bytes is issued).       */
    uint32_t OVERREAD                   : 1;    // <13> Overread bit
    uint32_t WIDTH                      : 2;    // <15:14> Width of the on-chip peripheral
                                                /*  0x0 = Reserved for on-chip peripheral-related transactions
                                                    0x1 = 1 byte
                                                    0x2 = half-word (2 bytes)
                                                    0x3 = word (4 bytes)    */
    uint32_t BURST                      : 3;    // <18:16> burst size of each data transfer
                                                /*  0x0 = 8 bytes
                                                    0x1 = 8 bytes
                                                    0x2 = 16 bytes
                                                    0x3 = 32 bytes
                                                    0x4 = 64 bytes
                                                    0x5~0x7=Reserved                       */
    uint32_t Reserved1                  : 2;    // <20:19> reserved
    uint32_t ENDIRQEN                   : 1;    // <21>, End interrupt enable
                                                /*  0 = Interrupt is not triggered when LENGTH decrements to  zero.
                                                    1 = Set the DINT interrupt bit for the channel when 
                                                        LENGTH decrements to zero        */
    uint32_t STARTIRQEN                 : 1;    // <22> Start interrupt enable
                                                /*  This bit indicates that the interrupt is enabled as soon as the descriptor is loaded
                                                    0 = Interrupt not triggered after descriptor is loaded.
                                                    1 = Set interrupt bit for that channel in the DINT[CHLINTR] 
                                                        when the descriptor (4 words) for the channel is loaded */
    uint32_t ADDRMODE                   : 1;    // <23> Addressing mode, default 0
                                                /*  This bit controls the addressing mode for descriptor comparison and is valid 
                                                    only in the descriptor compare mode (DCMDx[CMPEN] = 1)
                                                    0 = Source address field contains address, and target address field contains address
                                                    1 = Source address field contains address, and target address field contains data
                                                       */
    uint32_t Reserved2                  : 1;    // <24>
    uint32_t CMPEN                      : 1;    // <25> Descriptor compare enable, default 0
                                                /*  This bit must be cleared for normal DMA operations.
                                                    Setting the bit enables the descriptor-compare mode, in which the DMAC treats 
                                                    the current descriptor as a special case and compares data that corresponds to the source and target fields.
                                                    0 = DMA does not perform any address-compare operations
                                                    1 = DMA recognizes the current descriptor as a special case and compares data 
                                                        based on the source address and target address fields
                                                    */
    uint32_t Reserved3                  : 2;    // <27:26>
    uint32_t FLOWTRG                    : 1;    // <28> Target flow control, default 0 
                                                /*  The flow control of the target bit must be set if the target is
                                                    an on-chip peripheral or external companion chip
                                                    0 = Do not wait for request signals associated with this channel
                                                    1 = Wait for a request signal before initiating the data transfer
                                                    */
    uint32_t FLOWSRC                    : 1;    // <29> Source flow control, default 0
                                                /*  The flow control of the source bit must be set if the target is
                                                    an on-chip peripheral or external companion chip
                                                    0 = Do not wait for request signals associated with this channel
                                                    1 = Wait for a request signal before initiating the data transfer
                                                    */
    uint32_t INCTRGADDR                 : 1;    // <30> Target address increment, default 0
                                                /*  0 = Do not increment target address
                                                    1 = Increment target address
                                                    */
    uint32_t INCSRCADDR                 : 1;    // <31> Source address increment, default 0
                                                /*  0 = Do not increment source address
                                                    1 = Increment source address
                                                    */
}tc_DCMD_T;

typedef struct
{
    uint32_t      DDADR;  // DMA Descriptor Address
    uint32_t      DSADR;  // DMA Source Address
    uint32_t      DTADR;  // DMA Target Address
    union
    {
        uint32_t        DCMD;       // DMA Command Register
        tc_DCMD_T       DCMD_f;     
    };
} T_DMA_DESC;

typedef struct
{
    uint32_t    DCSR[DMA_NUMBER_OF_HW_CHANNEL_SUPPORTED];   //Control/Status Reg.   0x0000 0000
    uint32_t    Reserved0[8];
    uint32_t    DALGN[DMA_NUMBER_OF_HW_CHANNEL_SUPPORTED];  //Align Reg.            0x0000 0040
    uint32_t    Reserved1[8];
    uint32_t    DRQSR[3];                                   //DREQ0/1/2 Status Reg. 0x0000 0080
    uint32_t    DINT;                                       //Interrupt Pend. Reg.  0x0000 008c
    uint32_t    Reserved2[28];                              //Reserved1
    uint32_t    DRCMR[DMA_NUMBER_OF_DEVICES_SUPPORTED];     //Req to Channl Map     0x0000 0100
    uint32_t    Reserved3[32];
    T_DMA_DESC  DESC[DMA_NUMBER_OF_HW_CHANNEL_SUPPORTED];   //Descriptor            0x0000 0200
} T_DMA;


typedef struct
{
    __IO uint32_t beacon_sel                : 4;                /*时标输出频率:
                                                                    1111：32768Hz，50%占空比方波
                                                                    1110：16384Hz，50%占空比方波
                                                                    1101：8192Hz，50%占空比方波
                                                                    1100：4096Hz，50%占空比方波
                                                                    1011：2048Hz，50%占空比方波
                                                                    1010：1024Hz，50%占空比方波
                                                                    1001：512Hz，50%占空比方波
                                                                    1000：256Hz，50%占空比方波
                                                                    0111：128Hz，50%占空比方波
                                                                    0110：64Hz，50%占空比方波
                                                                    0101：32Hz，50%占空比方波
                                                                    0100：16Hz，50%占空比方波
                                                                    0011：8Hz，50%占空比方波
                                                                    0010：4Hz，50%占空比方波
                                                                    0001：2Hz，50%占空比方波
                                                                    0000：1Hz，50%占空比方波
                                                                    注意在32768分频时不产生中断，无法唤醒。*/
    __IO uint32_t bn_intp_en                : 1;                /*    时标输出不使能插补控制位:
                                                                        0b：   时标输出不使能插补
                                                                        1b：   时标输出使能插补                */
    __IO uint32_t ext_temp_en               : 1;                /*外部温度使能：
                                                                    0 ：用片内温度传感器值算补偿值
                                                                    1 ：用片外温度传感器值算补偿值                    */
    __IO uint32_t cload_cal_i               : 2;                /*CLOAD补偿计算间隔 :
                                                                    00：   每二秒启动CLOAD补偿计算
                                                                    01：   每四秒启动CLOAD补偿计算
                                                                    10：   每八秒启动CLOAD补偿计算
                                                                    11：   每十六秒启动CLOAD补偿计算*/
    __IO uint32_t cmp_en                    : 1;                /*温度补偿开关:
                                                                    0：    温度补偿关，温度采集，硬件补偿逻辑运算不再启动
                                                                    1：    温度补偿开*/
    __IO uint32_t bs_clk_sel                : 1;                /*时标插补时钟选择:
                                                                    0：选择系统时钟进行插补计数
                                                                    1：选择FLL倍频时钟进行插补计数*/
    __IO uint32_t rtc_clk_sel               : 1;                /*RTC时钟选择位:
                                                                    0： RTC模块工作时钟为Crystal 32K
                                                                    1： RTC模块工作时钟为内部32K OSC*/
} tc_rtc_rtccon1_field_t;


typedef struct
{
    __IO uint32_t trst                  : 1;                /*  0：日历时间寄存器被软件设置过，不为上电默认值
                                                                1：日历时间寄存器未被软件设置过，为上电默认值 */
    __IO uint32_t cnt_busy              : 1;                /*  0：  RTC时间计数器不在更新中，允许软件读取时钟寄存器
                                                                1：  RTC时间计数器在更新中（RTC寄存器更新的前后一个32k周期），不允许软件读取时钟寄存器*/
    __IO uint32_t cload_cmp             : 1;                /*  0：温度处在跳周期补偿范围内
                                                                1：温度处在电容补偿范围内*/
} tc_rtc_rtcsta1_field_t;

typedef struct
{
    __IO uint32_t lock              : 1;                /*锁定寄存器状态（读)
                                                            0：允许写日历相关寄存器
                                                            1：不允许写日历相关寄存器, 默认值*/
    __IO uint32_t lkbusy            : 1;                /*解锁同步状态标志位（读）
                                                            0：解锁同步已完成
                                                            1：解锁同步过程中*/
    __IO uint32_t ulbusy            : 1;                /*锁定同步状态标志位（读）
                                                            0：锁定同步已完成
                                                            1：锁定同步过程中*/
    __IO uint32_t password          : 29;               /*使能写寄存器（写）：
                                                           写0x1ACCE551将使能写日历相关秒、分钟、小时、天、周、月、年）寄存器，
                                                           并暂停RTC日历寄存器更新；写其它值或在解锁超时后将锁定上述寄存器，
                                                           并恢复RTC日历寄存器更新*/
} tc_rtc_calllock_field_t;


typedef struct
{
    __IO uint32_t secie             : 1;                    /*秒中断使能位
                                                                0:  秒中断不使能
                                                                1:  秒中断使能*/
    __IO uint32_t minie             : 1;                    /*分中断使能位
                                                               0： 分中断不使能
                                                               1： 分中断使能*/
    __IO uint32_t hrie              : 1;                    /*小时中断使能位
                                                               0： 小时中断不使能
                                                               1： 小时中断使能*/
    __IO uint32_t dayie             : 1;                    /*天中断使能位
                                                               0：    天中断不使能
                                                               1：    天中断使能*/
    __IO uint32_t monie             : 1;                    /*月中断使能位
                                                               0：    月中断不使能
                                                               1：    月中断使能*/
    __IO uint32_t aie               : 1;                    /*报警中断使能位
                                                               0：    报警中断不使能
                                                               1：    报警中断使能*/
    __IO uint32_t bie               : 1;                    /*时标中断使能位
                                                               0： 时标中断不使能
                                                               1： 时标中断使能*/
} tc_rtc_rtcie_field_t;


typedef struct
{
    __IO uint32_t secif             : 1;                    /*秒中断标志位
                                                                0：    秒中断未发生
                                                                1：    秒中断发生，软件对该位写1将其清除*/
    __IO uint32_t minif             : 1;                    /*分中断标志位
                                                                0：    分中断未发生
                                                                1：    分中断发生，软件对该位写1将其清除*/
    __IO uint32_t hrif              : 1;                    /*小时中断标志位
                                                                0：    小时中断未发生
                                                                1：    小时中断发生，软件对该位写1将其清除*/
    __IO uint32_t dayif             : 1;                    /*天中断标志位
                                                                0b：   天中断未发生
                                                                1b：   天中断发生，软件对该位写1将其清除*/
    __IO uint32_t monif             : 1;                    /*月中断标志位
                                                                0：    月中断未发生
                                                                1：    月中断发生，软件对该位写1将其清除*/
    __IO uint32_t aif               : 1;                    /*报警中断标志位
                                                                0：    报警中断未发生
                                                                1：    报警中断发生，软件对该位写1将其清除*/
    __IO uint32_t bif               : 1;                    /*时标中断标志位
                                                                0： 时标中断未发生。注意在32768分频时不产生中断。
                                                                1： 时标中断发生，软件对该位写1将其清除*/
} tc_rtc_rtcif_field_t;

typedef struct
{
    __IO uint32_t osc_32k_en                : 1;                /*osc 32k 使能控制
                                                                    0：    osc 32k关闭
                                                                    1：    osc 32k使能*/
    __IO uint32_t reserved                  : 7;
    __IO uint32_t cfin2                     : 4;                /*Cload精调2控制*/
    __IO uint32_t cfin1                     : 4;                /*Cload精调1控制*/
    __IO uint32_t craw                      : 4;                /*Cload粗调控制*/
    __IO uint32_t deglitch_pw_sel           : 2;                /*低频crystal deglitch延迟选择
                                                                    00 : 8ns
                                                                    01 : 12ns
                                                                    10 : 16ns
                                                                    11 : 20ns*/
    __IO uint32_t deglitch_bp_en            : 1;                /*deglitch bypass控制
                                                                    0 ： bypass关闭
                                                                    1 ： bypass开启*/
} tc_osc_32k_con1_field_t;

typedef struct
{
    __IO uint32_t cal_done              : 1;                /*RTC计算完成标志位*/
    __IO uint32_t cal_doing_pclk        : 1;                /*RTC计算中标志位*/
} tc_rtc_cal_status_field_t;

typedef struct
{
    __IO uint32_t ppm_rslt              : 18;               /*补偿高频时钟周期数，无符号数，有效范围0~255，
                                                              每个校准周期都不同，计数不超过最大值时，
                                                              每个周期的差值为固定数 */
    __IO uint32_t cal_cycle_32k         : 6;                /*补偿32K时钟周期数，有符号数，有效范围-32~+31，
                                                              每个校准周期最大会有+/-1的不同*/
    __IO uint32_t hf_cal_cycle          : 8;                /*定标精度Q4*/
} tc_rtc_ppm_rslt_field_t;



typedef struct
{
    union
    {
        __IO uint32_t rtccon1;                                  /*RTC控制寄存器*/
        tc_rtc_rtccon1_field_t rtccon1_f;
    };
    union
    {
        __IO uint32_t rtcsta1;                                  /*RTC状态寄存器*/
        tc_rtc_rtcsta1_field_t rtcsta1_f;
    };
    __IO uint32_t calllock;                                     /*日历锁定寄存器*/
    __IO uint32_t sec;                                          /*秒寄存器*/
    __IO uint32_t minute;                                       /*分寄存器*/
    __IO uint32_t hour;                                         /*小时寄存器*/
    __IO uint32_t day;                                          /*日期寄存器*/
    __IO uint32_t week;                                         /*星期寄存器*/
    __IO uint32_t month;                                        /*月寄存器*/
    __IO uint32_t year;                                         /*年寄存器*/
    union
    {
        __IO uint32_t rtcie;                                    /*RTC中断使能寄存器*/
        tc_rtc_rtcie_field_t rtcie_f;
    };
    union
    {
        __IO uint32_t rtcif;                                    /*RTC中断标志寄存器*/
        tc_rtc_rtcif_field_t rtcif_f;
    };
    __IO uint32_t almsec;                                       /*闹钟秒寄存器*/
    __IO uint32_t almmin;                                       /*闹钟分寄存器*/
    __IO uint32_t almhour;                                      /*闹钟小时寄存器*/
    __IO uint32_t ext_temp_data;                                /*外部温度设定值
                                                                     Q4【低4位为小数】二进制补码形式，温度范围为-511 ~ +511℃．【读数除16】．*/
    union
    {
        __IO uint32_t osc_32k_con1;                             /*闹钟小时寄存器*/
        tc_osc_32k_con1_field_t osc_32k_con1_f;
    };
    __IO uint32_t rtc_cmpthre_l;                                /*补偿方式低温阈值
                                                                    Q6二进制补码形式，温度范围为-256 ~ +256℃．*/
    __IO uint32_t rtc_cmpthre_h;                                /*补偿方式高温阈值
                                                                     Q6二进制补码形式，温度范围为-256 ~ +256℃．*/
    __IO uint32_t ppma3;                                        /*定标精度Q26二进制补码形式*/
    __IO uint32_t ppma2;                                        /*定标精度Q18二进制补码形式*/
    __IO uint32_t ppma1;                                        /*定标精度Q10 */
    __IO uint32_t ppma0;                                        /*定标精度Q4二进制补码形式*/
    union
    {
        __IO uint32_t cal_status;                               /*RTC内部计算状态寄存器*/
        tc_rtc_cal_status_field_t cal_status_f;
    };
    union
    {
        __IO uint32_t ppm_rslt;                                 /*PPM结果寄存器*/
        tc_rtc_ppm_rslt_field_t ppm_rslt_f;
    };
    __IO uint32_t skip_cyc_time;                                /*  跳周期补偿时间配置（通过配置寄存器的值选择多长时间进行补偿1~ 4095s）*/
    __IO uint32_t beacon_intp_sel_out;                          /*  3’b000 选择半秒插补时钟输出
                                                                    3’b001 选择1/4秒插补时钟输出
                                                                    3’b010 选择1/8秒秒插补时钟输出
                                                                    3’b011 选择1/16秒插补时钟输出
                                                                    3’b100 选择1/32秒插补时钟输出
                                                                    3’b101 选择1/64秒插补时钟输出
                                                                    3’b110 选择1/128秒秒插补时钟输出
                                                                    3’b111 选择1/256秒秒插补时钟输出*/
} T_RTC;


typedef struct
{
    __IO uint32_t start                     : 1;
    __IO uint32_t mode                      : 3;
    __IO uint32_t enc_dec                   : 1;
    __IO uint32_t padding_mode              : 3;
    __IO uint32_t nbyte                     : 4;
    __IO uint32_t tbyte                     : 5;
    __IO uint32_t autocg_en                 : 1;
    // __IO uint32_t big_endian              : 1;
    __IO uint32_t burst                     : 4;
} mode_field_t;

typedef struct
{
    __IO uint32_t ififo;
    __IO uint32_t ofifo;
    union
    {
        __IO uint32_t mode;
        mode_field_t SPICFG_CLKRST_CTRL_f;     /**< SPICFG_CLKRST_CTRL, 0x78 */
    };
    __IO uint32_t abyte_l;
    __IO uint32_t abyte_h;
    __IO uint32_t pbyte_l;
    __IO uint32_t pbyte_h;
    __IO uint32_t key_ll;
    __IO uint32_t key_lh;
    __IO uint32_t key_hl;
    __IO uint32_t key_hh;
    __IO uint32_t iv_ll;
    __IO uint32_t iv_lh;
    __IO uint32_t iv_hl;
    __IO uint32_t iv_hh;
    __IO uint32_t crc_init_value;
    __IO uint32_t crc_poly;
    __IO uint32_t mac_fail;
    __IO uint32_t sha_out_lll;
    __IO uint32_t sha_out_llh;
    __IO uint32_t sha_out_lhl;
    __IO uint32_t sha_out_lhh;
    __IO uint32_t sha_out_hll;
    __IO uint32_t sha_out_hlh;
    __IO uint32_t sha_out_hhl;
    __IO uint32_t sha_out_hhh;
    __IO uint32_t crc_out;
} T_CRYPTO;

/************************************************************************/
/*  APB address config                                                  */
/************************************************************************/


#define PERIPH_BASE                  ((unsigned int)0x40000000)
#define PSRAM_AHB_BASE               ((unsigned int)0x60000000)
#define APB_PERIPH_BASE              PERIPH_BASE

#define GPIOPIN_0_15_BASE_ADDR       (APB_PERIPH_BASE + 0)
#define GPIOPIN_16_25_BASE_ADDR      (APB_PERIPH_BASE + 0x1000)
#define GPIOPIN_2_7_BASE_ADDR        (APB_PERIPH_BASE + 0x4000)
#define PINMUXELSE_BASE_ADDR         (APB_PERIPH_BASE + 0x5000)
#define AON_UART_BASE_ADDR           (APB_PERIPH_BASE + 0x6000)
#define AIC_BASE_ADDR                (APB_PERIPH_BASE + 0x7000)
#define IWDG_BASE_ADDR               (APB_PERIPH_BASE + 0x8000)
#define PINMUX2_7_BASE_ADDR          (APB_PERIPH_BASE + 0x9000)
#define AON_CLKRST_BASE_ADDR         (APB_PERIPH_BASE + 0xA000)
#define AON_PMU_BASE_ADDR            (APB_PERIPH_BASE + 0xC000)
#define AON_ANACFG_BASE_ADDR         (APB_PERIPH_BASE + 0xD000)
#define AON_RTC_BASE_ADDR            (APB_PERIPH_BASE + 0xE000)

#define FIC_BASE_ADDR                (APB_PERIPH_BASE + 0x20000)
#define CRYPTO_BASE_ADDR             (APB_PERIPH_BASE + 0x30000)
#define DIVIDER_BASE_ADDR            (APB_PERIPH_BASE + 0x40000)

#define TRNG_BASE_ADDR               (APB_PERIPH_BASE + 0x60000)
#define TIMER0_BASE_ADDR             (APB_PERIPH_BASE + 0x70000)
#define TIMER1_BASE_ADDR             (APB_PERIPH_BASE + 0x80000)
#define TIMER2_BASE_ADDR             (APB_PERIPH_BASE + 0x90000)
#define TIMER3_BASE_ADDR             (APB_PERIPH_BASE + 0xA0000)
#define TIMER4_BASE_ADDR             (APB_PERIPH_BASE + 0xB0000)
#define TIMER5_BASE_ADDR             (APB_PERIPH_BASE + 0xC0000)
#define I2C0_BASE_ADDR               (APB_PERIPH_BASE + 0xD0000)
#define I2C1_BASE_ADDR               (APB_PERIPH_BASE + 0xE0000)
#define SPI0_BASE_ADDR               (APB_PERIPH_BASE + 0xF0000)
#define SPI1_BASE_ADDR               (APB_PERIPH_BASE + 0x100000)
#define UART0_BASE_ADDR              (APB_PERIPH_BASE + 0x110000)
#define UART1_BASE_ADDR              (APB_PERIPH_BASE + 0x120000)
#define WWDG_BASE_ADDR               (APB_PERIPH_BASE + 0x130000)
#define SYSCTL_BASE_ADDR             (APB_PERIPH_BASE + 0x140000)
#define ANACFG_BASE_ADDR             (APB_PERIPH_BASE + 0x150000)
#define LIC_BASE_ADDR                (APB_PERIPH_BASE + 0x160000)
#define LP_GPT_BASE_ADR              (APB_PERIPH_BASE + 0x170000)
#define UART2_BASE_ADR               (APB_PERIPH_BASE + 0x180000)
#define DMA_BASE_ADDRESS             (APB_PERIPH_BASE + 0x200000)


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/


#define M0P_FLASH                        ((T_FLASH *)  0x10ff0000UL)
#define SPI0                             ((T_SPI    *)  SPI0_BASE_ADDR)
#define SPI1                             ((T_SPI    *)  SPI1_BASE_ADDR)
#define UART0                            ((T_UART    *) UART0_BASE_ADDR)
#define UART1                            ((T_UART    *) UART1_BASE_ADDR)
#define UART2                            ((T_UART    *) UART2_BASE_ADR)
#define LPUART                           ((T_UART    *) AON_UART_BASE_ADDR)
#define I2C0                             ((T_I2C    *)  I2C0_BASE_ADDR)
#define I2C1                             ((T_I2C    *)  I2C1_BASE_ADDR)
#define PINMUX2_7                        ((T_PINMUX2_7 *)  PINMUX2_7_BASE_ADDR)
#define PINMUXELSE                       ((T_PINMUXELSE *)  PINMUXELSE_BASE_ADDR)
#define DMA                              ((T_DMA *)  DMA_BASE_ADDRESS)
#define AIC                              ((T_ALIC  *)  AIC_BASE_ADDR)
#define FIC                              ((T_FIC   *)  FIC_BASE_ADDR)
#define LIC                              ((T_ALIC  *)  LIC_BASE_ADDR)
#define TIMER0                           ((T_TIMER *)  TIMER0_BASE_ADDR)
#define TIMER1                           ((T_TIMER *)  TIMER1_BASE_ADDR)
#define TIMER2                           ((T_TIMER *)  TIMER2_BASE_ADDR)
#define TIMER3                           ((T_TIMER *)  TIMER3_BASE_ADDR)
#define TIMER4                           ((T_TIMER *)  TIMER4_BASE_ADDR)
#define TIMER5                           ((T_TIMER *)  TIMER5_BASE_ADDR)
#define LPTIMER                          ((T_TIMER *)  LP_GPT_BASE_ADR)
#define IWDG                             ((T_WDG  *)  IWDG_BASE_ADDR)
#define WWDG                             ((T_WDG  *)  WWDG_BASE_ADDR)
#define ACLKRST                          ((T_ACLKRST *)  AON_CLKRST_BASE_ADDR)
#define PMU                              ((T_PMU  *)  AON_PMU_BASE_ADDR)
#define GPIO_PIN_0_15                    ((T_GPIO *)  GPIOPIN_0_15_BASE_ADDR)
#define GPIO_PIN_16_25                   ((T_GPIO *)  GPIOPIN_16_25_BASE_ADDR)
#define GPIO_PIN_2_7                     ((T_GPIO *)  GPIOPIN_2_7_BASE_ADDR)
#define DIVIDER                          ((T_DIV *)  DIVIDER_BASE_ADDR)
#define SYSCTL                           ((T_SYSCTL *)  SYSCTL_BASE_ADDR)
#define AON_ANACFG                       ((T_AON_ANACFG *)  AON_ANACFG_BASE_ADDR)
#define AON_ANACFG_TRIM                  ((T_AON_ANACFG_TRIM *)(AON_ANACFG_BASE_ADDR + 0x100))
#define ANAREG_1                         ((T_ANAREG *)  ANACFG_BASE_ADDR)
#define ANAREG_TRIM_SIGMA_DELTA          ((T_ANAREG_TRIM_SIGMA_DELTA *)(ANACFG_BASE_ADDR + 0x100))
#define ANAREG_TRIM_MISC                 ((T_ANAREG_TRIM_MISC *)(ANACFG_BASE_ADDR + 0x180))
#define ANAREG_2                         ((T_ANAREG_2 *)(ANACFG_BASE_ADDR + 0x300))
#define TEMP_SENSOR                      ((T_TEMP_SENSOR *)(ANACFG_BASE_ADDR + 0x380))
#define RTC                              ((T_RTC *)  AON_RTC_BASE_ADDR)
#define CRYPTO                           ((T_CRYPTO*)  CRYPTO_BASE_ADDR)



/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
#pragma pop
#elif defined (__ICCARM__)
/* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#pragma clang diagnostic pop
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning restore
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

#ifdef __cplusplus
}
#endif
#endif /* __TCASXXX_H__ */
