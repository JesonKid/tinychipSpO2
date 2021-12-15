#include "BluetoothDriver.h"
#include "gpio.h"

/*
********************************************************************************
 * [Func.Name  ] BOOL BLE_IsConnect(void)                                     *
 * [Func.Spec. ] Get the connect state of bluetooth                           *
 * [Func.Input ]                                                              *
 * [Func.Output]                                                              *
 * [Func.Return]                                                              *
 *               BOOL -TRUE, bluetooth is connected                           *
 *                    -FALSE, bluetooth is not connected                      *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
BOOL BLE_IsConnect(void)
{
    return ((GPIO_PIN_0_15->data >> 11) & 0x01);        //PB04
}


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
void BLE_SetRxState(BOOL flag)
{
    if (TRUE == flag)
    {
        GPIO_SetBits(GPIOPortA, GPIOPin2);
    }
    else
    {
        GPIO_ClearBits(GPIOPortA, GPIOPin2);
    }
}
