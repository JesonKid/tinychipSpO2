/**
 *****************************************************************************
 * @brief   Watchdog header file
 *
 * @file   wdg.h
 * @author
 * @date   17/JULy/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */
#ifndef __WDG_H__
#define __WDG_H__

#include "tcasxxx.h"


/******************************************************************************* 
 * Definitions 
 ******************************************************************************/
/* 共(可被其他源文件使用)宏、枚举、结构体的定义 */
/* 外部全局变量的声明 */

/******************************************************************************* 
 * API 
 ******************************************************************************/

/* 外部函数(可加 extern 修饰)的声明 */
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define     WDG_ADV_FLAG        0x01
#define     WDG_IT2_FLAG        0x02
#define     WDG_IOVF_FLAG       0x03

#define     WDG_PRESCALER_4     (4-1)
#define     WDG_PRESCALER_8     (8-1)
#define     WDG_PRESCALER_16    (16-1)
#define     WDG_PRESCALER_32    (31-1)
#define     WDG_PRESCALER_64    (64-1)
#define     WDG_PRESCALER_128   (128-1)
#define     WDG_PRESCALER_256   (256-1)

#define     WWDG_CLK_16M        0x00
#define     WWDG_CLK_FMULT      0x01
#define     WWDG_CLK_32K        0x02
//#define     WWDG_CLK_DFILL      0x03


typedef struct
{
    uint8_t Wwdg_Adv_Mode;
    uint8_t Wwdg_T2_Mode;
    uint8_t Wwdg_Ovf_Mode;
}WWDG_TiggerMode;

typedef struct
{
    uint8_t wt;
    uint8_t t2;
    uint8_t count;
    uint8_t clkSrc;
    uint8_t preScaler;
   WWDG_TiggerMode Wwdg_Mode;
}WWDG_InitType;


extern void EnableIWDG(T_WDG* iwdg);
extern void EnableWWDG(T_WDG* wwdg);

//extern void __attribute__((weak))Hal_IWDGHandler(void);
//extern void __attribute__((weak))Hal_WWDGAdvHandler(void);
//extern void __attribute__((weak))Hal_WWDGT2Handler(void);
//extern void __attribute__((weak))Hal_WWDGTimeOutHandler(void);

void IWDG_Lock(void);
void IWDG_UnLock(void);


void IWDG_Enable(void);
void IWDG_Disable(void);
void IWDG_ResetCnt(void);

void IWDG_Init(uint8_t clkDiv,uint16_t loadVal,uint8_t mode);
void IWDG_ClearITPending(uint8_t it_flag);
uint8_t IWDG_GetITFlag(uint8_t it_flag);

void WWDG_Lock(void);
void WWDG_UnLock(void);

void WWDG_Enable(void);
void WWDG_Disable(void);
void WWDG_ResetCnt(void);

void WWDG_Init(WWDG_InitType * pInitSt);
void WWDG_ClearITPending(uint8_t it_flag);
uint8_t WWDG_GetITFlag(uint8_t it_flag);



#ifdef __cplusplus
}
#endif
#endif /* __WDG_H__ */

