#include "BuzzDriver.h"
#include "DisplayDriver.h"
#include "tcasxxx.h"


//----------------------------------------------------------------------------------------------------

BUZZ_CTRL_T gBeepCtrl = 
{
	BUZZ_CLOSE,
	0,
	0,
	0,
	0
};   

//----------------------------------------------------------------------------------------------------

/*
********************************************************************************
 * [Func.Name  ] void Buzz_Speaker(void)                              *
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
void Buzz_Speaker(void)
{
    //Invalid beep
    if (BUZZ_CLOSE == gBeepCtrl.mType)
    {
        return;
    }

    //Double beep gaps
    if (0 < gBeepCtrl.tmGap)
    {
        gBeepCtrl.tmGap--;
        return;
    }

    //Start Beep
    if (0 == gBeepCtrl.tmCnt)
    {
        TIMER3->matchb0_val = gBeepCtrl.mDuty;
    }

    //End Beep
    gBeepCtrl.tmCnt++;
    if (gBeepCtrl.tmHold <= gBeepCtrl.tmCnt)
    {
        //Reset for next beep
        gBeepCtrl.tmCnt = 0;
        gBeepCtrl.tmGap = 10;         										//Double beep gap(200ms)
        gBeepCtrl.mDuty += (gBeepCtrl.mDuty >> 1);       	//Descend 1 times
        gBeepCtrl.tmHold <<= 1;       										//Ascend 1 times
        //Utilize the counter overflow feature to force low level
        TIMER3->matchb0_val = 0;

        //Reset controller
        gBeepCtrl.mType--;
        if (BUZZ_CLOSE == gBeepCtrl.mType)
        {
            gBeepCtrl.mType = BUZZ_CLOSE;
            gBeepCtrl.mDuty  = 184;
            gBeepCtrl.tmHold = 2;
            gBeepCtrl.tmCnt = 0;
            gBeepCtrl.tmGap = 0;
        }
    }
}
