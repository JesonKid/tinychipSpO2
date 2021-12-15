#ifndef _SYSTEM_DRIVER_
#define _SYSTEM_DRIVER_

#include "DataType.h"



#define AIC_IWDG_MASK               0x00000040


#define SYS_BLE_SELECTED        0


// SARADC RX DMA CHANNEL
#define SARADC_RX_DMA_CHANNEL 0

/*
********************************************************************************
 * [Func.Name  ] void SYS_Hardware_Init(void)                                   *
 * [Func.Spec. ] Initialize sytem congiguration.                              *
 * [Func.Input ]                                                              *
 *               void                                                         *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
extern void SYS_Hardware_Init(void);

/*
********************************************************************************
 * [Func.Name  ] void SYS_ModuleDrive_Init(void)                                   *
 * [Func.Spec. ] Initialize module drive, such as OLED, Accelerate, Bluetooth,*
 *               Algorithm, and so on.                                        *
 * [Func.Input ]                                                              *
 *               void                                                         *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
extern void SYS_ModuleDrive_Init(void);

/*
********************************************************************************
 * [Func.Name  ] void EnableInterrupt(void)                                   *
 * [Func.Spec. ] Enable interrupt.                                            *
 * [Func.Input ]                                                              *
 *               void                                                         *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
extern void SYS_Interrupt_Enable(void);



extern void IwdgCallback(void);

#endif

