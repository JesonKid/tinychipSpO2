#ifndef _BLUE_TOOTH_DRIVE_
#define _BLUE_TOOTH_DRIVE_

#include "DataType.h"
 

/*
********************************************************************************
 * [Func.Name  ] BOOL BLE_IsConnect(BLE_MODE_E mode)                 *
 * [Func.Spec. ] Get the connect state of bluetooth                           *
 * [Func.Input ]                                                              *
 * [Func.Output]                                                              *
 * [Func.Return]                                                              *
 *               BOOL -TRUE, bluetooth is connected                           *
 *                    -FALSE, bluetooth is not connected                      *
 *----------------------------------------------------------------------------*

********************************************************************************
*/
extern BOOL BLE_IsConnect(void);

 

/*
********************************************************************************
 * [Func.Name  ] void BLE_SetRxState(BOOL flag)                          *
 * [Func.Spec. ] Enable or disable the bluetooth Rx pin, after setting from   *
 *               disable state to enable state, send the data with at least a *
 *               5ms delay                                                    *
 * [Func.Input ]                                                              *
 *               BOOL flag -TRUE, Enable the bluetooth Rx; -FALSE, Disable the*
 *               bluetooth Rx                                                 *
 * [Func.Output]                                                              *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*

********************************************************************************
*/
extern void BLE_SetRxState(BOOL flag);

#endif

