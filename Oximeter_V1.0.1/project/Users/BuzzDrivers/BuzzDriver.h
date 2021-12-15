#ifndef _BUZZ_DRIVE_
#define _BUZZ_DRIVE_

#include "DataType.h"


//----------------------------------------------------------------------------------------------------

//Pulse beep
typedef enum
{
    BUZZ_CLOSE = 0,                                      //Null beep
    BUZZ_SIGNLE_BEEP = 1,                                //Single beep
    BUZZ_DOUBLE_BEEP = 2,                                //Double beep
    BUZZ_TRIPLE_BEEP = 3,                                //Triple beep
    BUZZ_MARGIN,
	
} BUZZ_TYPE_E;



//Beep controller
typedef struct
{
    BUZZ_TYPE_E mType;                                   //Beep type (double/single)
    CHAR tmCnt;                                          //Time counter
    CHAR tmGap;                                          //Double beep gap times
    CHAR tmHold;                                         //PWM continue times
    INT16 mDuty;                                         //PWM duty
	
} BUZZ_CTRL_T;

extern BUZZ_CTRL_T gBeepCtrl;                               //Beep flag

//----------------------------------------------------------------------------------------------------

/*
********************************************************************************
 * [Func.Name  ] void Buzz_Speaker(void)                                *
 * [Func.Spec. ] Using hardware PWM mode for buzz driver(4KHz).               *
 * [Func.Input ]                                                              *
 *               void                                                         *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
extern void Buzz_Speaker(void);

#endif

