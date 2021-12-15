/**
*****************************************************************************
* @brief
*
* @file   LightDriver.c
* @author
* @date   2021-10-20
*****************************************************************************
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* Copyright (c) 2021 Tinychip Microelectronics Co.,Ltd.
*
*****************************************************************************
*/

#include "LightDriver.h"
#include "Preprocess.h"
#include "Spo2Interface.h"
#include "string.h"
#include "tcasxxx.h"


//----------------------------------------------------------------------------------------------------


LIGHT_DRVCTRL_T  gCtrlRed = {0};                         //Red light control
LIGHT_DRVCTRL_T  gCtrlIrd = {0};                         //Ird light control

LIGHT_ADAPCTRL_T gAdptRed = {0};                                 //Red Adaptive adjust control
LIGHT_ADAPCTRL_T gAdptIrd = {0};                                 //Ird Adaptive adjust control

LIGHT_FLUCCTRL_T gFluCtrl = {0};                                 //Sample AC component statistic control
LIGHT_ERRCTRL_T  gErrCtrl = {0};                                 //Probe error detect

UINT16  gDriveRed = DRV_MAX;                             //Red drive value
UINT16  gDriveIrd = DRV_EXTRA;                           //Ird drive value

//----------------------------------------------------------------------------------------------------


/**
 * @description: Probe error detect.
 * @param {LIGHT_ERRCTRL_T} *pCtrl  Probe error controller
 * @return {UCHAR} Probe error flag
 */
static UCHAR Light_ProbeErrDetect(LIGHT_ERRCTRL_T *pCtrl)
{
    UCHAR bErrProbe = FALSE;

    //DC component summary
    pCtrl->dcAmbR += gSatSamp.ambRed;
    pCtrl->dcAmbI += gSatSamp.ambIrd;
    pCtrl->dcSigR += gSampArray[PHASE_RED_DRV];
    pCtrl->dcSigI += gSampArray[PHASE_IRD_DRV];

    //Second coutner
    pCtrl->mSecCnt--;
    if (0 >= pCtrl->mSecCnt)
    {
        //Average for DC component
        pCtrl->dcAmbR /= ADP_DRV_SEC;
        pCtrl->dcAmbI /= ADP_DRV_SEC;
        pCtrl->dcSigR /= ADP_DRV_SEC;
        pCtrl->dcSigI /= ADP_DRV_SEC;
        //Error state judge(Red)
        if ((PRO_ERR_LOW_THRD >= pCtrl->dcAmbR && PRO_ERR_LOW_THRD >= pCtrl->dcSigR
             && PRO_NOISE_AC >= Labsdif(pCtrl->dcAmbR, pCtrl->dcSigR))
            || (PRO_ERR_HIGH_THRD <= pCtrl->dcAmbR && PRO_ERR_HIGH_THRD <= pCtrl->dcSigR))
        {
            if (PRO_ERR_NUM > pCtrl->cntRed)
            {
                pCtrl->cntRed++;
            }
        }
        else
        {
            if (0 < pCtrl->cntRed)
            {
                pCtrl->cntRed--;
            }
        }
        //Error state judge(Infrared)
        if ((PRO_ERR_LOW_THRD >= pCtrl->dcAmbI && PRO_ERR_LOW_THRD >= pCtrl->dcSigI
             && PRO_NOISE_AC >= Labsdif(pCtrl->dcAmbI, pCtrl->dcSigI))
            || (PRO_ERR_HIGH_THRD <= pCtrl->dcAmbI && PRO_ERR_HIGH_THRD <= pCtrl->dcSigI))
        {
            if (PRO_ERR_NUM > pCtrl->cntIrd)
            {
                pCtrl->cntIrd++;
            }
        }
        else
        {
            if (0 < pCtrl->cntIrd)
            {
                pCtrl->cntIrd--;
            }
        }
        //Reset
        pCtrl->dcAmbR = 0;
        pCtrl->dcAmbI = 0;
        pCtrl->dcSigR = 0;
        pCtrl->dcSigI = 0;
        pCtrl->mSecCnt = ADP_DRV_SEC;
    }

    if ((TRUE == gCtrlRed.mStatus && PRO_ERR_NUM <= pCtrl->cntRed)
        || (TRUE == gCtrlIrd.mStatus && PRO_ERR_NUM <= pCtrl->cntIrd))
    {
        bErrProbe = TRUE;
    }

    return bErrProbe;
}

/**
 * @description: Identify whether ambient flucuate too large.
 * @param {LIGHT_FLUCCTRL_T} *pCtrl  Disturb controller
 * @param {SATSAMP} *pData  Sample value
 * @return void
 */
static void Light_StatSampleACVariety(LIGHT_FLUCCTRL_T *pCtrl,
                                      SAT_SAMP_T *pData)
{
    UCHAR bAmbFlag = FALSE;

    //Adaptive probe off detect controller reset(Wait 4s at initial time)
    if (FALSE == gCtrlRed.mStatus || FALSE == gCtrlIrd.mStatus)
    {
        memset(pCtrl, 0, sizeof(*pCtrl));
        pCtrl->mSecCnt = ADP_DRV_SEC << 3;
        return;
    }

    //Wait for 7 seconds
    if (ADP_DRV_SEC < pCtrl->mSecCnt)
    {
        pCtrl->mSecCnt--;
        return;
    }

    //Ambient red
    if (0 == pCtrl->recAmbR.vMax || 0 == pCtrl->recAmbR.vMin)
    {
        pCtrl->recAmbR.vMax = pData->ambRed;
        pCtrl->recAmbR.vMin = pData->ambRed;
    }
    else
    {
        pCtrl->recAmbR.vMax = Lmax(pCtrl->recAmbR.vMax, pData->ambRed);
        pCtrl->recAmbR.vMin = Lmin(pCtrl->recAmbR.vMin, pData->ambRed);
    }

    //Ambient infrared
    if (0 == pCtrl->recAmbI.vMax || 0 == pCtrl->recAmbI.vMin)
    {
        pCtrl->recAmbI.vMax = pData->ambIrd;
        pCtrl->recAmbI.vMin = pData->ambIrd;
    }
    else
    {
        pCtrl->recAmbI.vMax = Lmax(pCtrl->recAmbI.vMax, pData->ambRed);
        pCtrl->recAmbI.vMin = Lmin(pCtrl->recAmbI.vMin, pData->ambRed);
    }

    //Signal red
    if (0 == pCtrl->recSigR.vMax || 0 == pCtrl->recSigR.vMin)
    {
        pCtrl->recSigR.vMax = pData->orgRed;
        pCtrl->recSigR.vMin = pData->orgRed;
    }
    else
    {
        pCtrl->recSigR.vMax = Lmax(pCtrl->recSigR.vMax, pData->orgRed);
        pCtrl->recSigR.vMin = Lmin(pCtrl->recSigR.vMin, pData->orgRed);
    }

    //Signal infrared
    if (0 == pCtrl->recSigI.vMax || 0 == pCtrl->recSigI.vMin)
    {
        pCtrl->recSigI.vMax = pData->orgIrd;
        pCtrl->recSigI.vMin = pData->orgIrd;
    }
    else
    {
        pCtrl->recSigI.vMax = Lmax(pCtrl->recSigI.vMax, pData->orgIrd);
        pCtrl->recSigI.vMin = Lmin(pCtrl->recSigI.vMin, pData->orgIrd);
    }

    //DC amplitude sum
    pCtrl->dcSigR += pData->orgRed;
    pCtrl->dcSigI += pData->orgIrd;
    pCtrl->dcAmbR += pData->ambRed;
    pCtrl->dcAmbI += pData->ambIrd;

    //2 second counter
    pCtrl->mSecCnt--;
    pCtrl->bReady = FALSE;
    if (0 >= pCtrl->mSecCnt)
    {
        //Notice flag for adaptive driver adjust controller
        pCtrl->bReady = TRUE;

        //Calculate AC amplitude
        pCtrl->acAmbR = pCtrl->recAmbR.vMax - pCtrl->recAmbR.vMin;
        pCtrl->acAmbI = pCtrl->recAmbI.vMax - pCtrl->recAmbI.vMin;
        pCtrl->acSigR = pCtrl->recSigR.vMax - pCtrl->recSigR.vMin;
        pCtrl->acSigI = pCtrl->recSigI.vMax - pCtrl->recSigI.vMin;

        DIVIDER->dividend = pCtrl->dcSigR;
        DIVIDER->divisor = ADP_DRV_SEC;
        pCtrl->dcSigR = DIVIDER->quotient;

        DIVIDER->dividend = pCtrl->dcSigI;
        //DIVIDER->divisor = ADP_DRV_SEC;
        pCtrl->dcSigI = DIVIDER->quotient;

        DIVIDER->dividend = pCtrl->dcAmbR;
        //DIVIDER->divisor = ADP_DRV_SEC;
        pCtrl->dcAmbR = DIVIDER->quotient;

        DIVIDER->dividend = pCtrl->dcAmbI;
        //DIVIDER->divisor = ADP_DRV_SEC;
        pCtrl->dcAmbI = DIVIDER->quotient;

        //FakeSignal_1: Signal DC component too small
        //FakeSignal_2: Signal AC component too large
        //FakeSignal_3: Ambient AC component too large
        //FakeSignal_4: Ambient DC component too large
        if ((PRO_NOISE_DC > pCtrl->dcSigR || PRO_NOISE_DC > pCtrl->dcSigI)
            || (pCtrl->acSigR > (pData->RedDC >> 1) && pCtrl->acSigI > (pData->IrdDC >> 1))
            || (pCtrl->acAmbR > (pCtrl->dcAmbR << 1) &&
                pCtrl->acAmbI > (pCtrl->dcAmbI << 1))
            || (40960 < pCtrl->dcAmbR || 40960 < pCtrl->dcAmbI))
        {
            bAmbFlag = TRUE;
        }
        //FakeSignal_5: Skeptical signal with disturbed ambient
        else if ((2048 <= pCtrl->dcAmbR && 2048 <= pCtrl->dcAmbI)
                 && (512 >= pCtrl->acSigR || 512 >= pCtrl->acSigI)
                 && (pCtrl->acAmbR > pCtrl->acSigR && pCtrl->acAmbI > pCtrl->acSigI))
        {
            //Low perfusion index
            if ((pCtrl->dcSigR >> 9) > pCtrl->acSigR ||
                (pCtrl->dcSigI >> 9) > pCtrl->acSigI)
            {
                bAmbFlag = TRUE;
            }
            //Low driver + large sample + low perfusion index
            if ((VLT_BASELINE < pCtrl->dcSigR && 1600 > gDriveRed &&
                 (pCtrl->dcSigR >> 8) > pCtrl->acSigR)
                || (VLT_BASELINE < pCtrl->dcSigI && 1600 > gDriveIrd &&
                    (pCtrl->dcSigI >> 8) > pCtrl->acSigI))
            {
                bAmbFlag = TRUE;
            }
            //Ambient AC component larger than signal AC component
            if (pCtrl->acAmbR > (pCtrl->acSigR << 1) ||
                pCtrl->acAmbI > (pCtrl->acSigI << 1))
            {
                bAmbFlag = TRUE;
            }
        }

        //Disturb judgement
        if (TRUE == bAmbFlag)
        {
            if (PRO_NOISE_NUM > pCtrl->errFlg)
            {
                pCtrl->errFlg++;
            }
        }
        else
        {
            if (0 < pCtrl->errFlg)
            {
                pCtrl->errFlg--;
            }
        }

        //Reset
        pCtrl->recAmbR.vMax = 0;
        pCtrl->recAmbR.vMin = 0;
        pCtrl->recAmbI.vMax = 0;
        pCtrl->recAmbI.vMin = 0;
        pCtrl->recSigR.vMax = 0;
        pCtrl->recSigR.vMin = 0;
        pCtrl->recSigI.vMax = 0;
        pCtrl->recSigI.vMin = 0;
        pCtrl->dcSigR = 0;
        pCtrl->dcSigI = 0;
        pCtrl->dcAmbR = 0;
        pCtrl->dcAmbI = 0;
        pCtrl->mSecCnt = ADP_DRV_SEC;
    }
}

/**
 * @description: Adaptive sample baseline for optimal samples
 * @param {LIGHT_ADAPCTRL_T} *pAdpCtrl   Automatic threshold adjust controller
 * @param {LIGHT_DRVCTRL_T}  *pDrvCtrl   Driver adjust controller
 * @param {UINT16}   valSamp     Current samples
 * @param {INT16}    sampAC      Current samples
 * @param {UCHAR}    flag        0-red, 1-ired
 * @return void
 */
void Light_AdaptBaselineSet(LIGHT_ADAPCTRL_T *pAdpCtrl,
                            LIGHT_DRVCTRL_T *pDrvCtrl, UINT16 valSamp, INT16 sampAC, UCHAR flag)
{
    UCHAR i = 0;
    INT32 valMean  = 0;
    INT32 mPosThrd = 0;
    INT32 mNegThrd = 0;

    //Calculate the baseline
    pAdpCtrl->adpFlag = 0x00;
    pAdpCtrl->sumBase += valSamp;
    if (FALSE == gFluCtrl.bReady)
    {
        return;
    }

    //Record AC component
    pAdpCtrl->mAcBuf[3] = pAdpCtrl->mAcBuf[2];
    pAdpCtrl->mAcBuf[2] = pAdpCtrl->mAcBuf[1];
    pAdpCtrl->mAcBuf[1] = pAdpCtrl->mAcBuf[0];
    pAdpCtrl->mAcBuf[0] = sampAC;

    //Calculate means and threshold
    for (i = 0; i < ADP_DRV_NUM; i++)
    {
        valMean += pAdpCtrl->mAcBuf[i];
    }
    //valMean /= ADP_DRV_NUM;
    DIVIDER->dividend = valMean;
    DIVIDER->divisor = ADP_DRV_NUM;
    valMean = DIVIDER->quotient;

    mPosThrd = valMean + (valMean >> 1);
    mNegThrd = valMean - (valMean >> 1);

    //Signal stable judge
    for (i = 0; i < ADP_DRV_NUM; i++)
    {
        if (mPosThrd < pAdpCtrl->mAcBuf[i] || mNegThrd > pAdpCtrl->mAcBuf[i])
        {
            break;
        }
    }

    //Adjust baseline
    if (ADP_DRV_NUM == i)
    {
        //Baseline
        //pAdpCtrl->sumBase /= ADP_DRV_SEC;
        DIVIDER->dividend = pAdpCtrl->sumBase;
        DIVIDER->divisor = ADP_DRV_SEC;
        pAdpCtrl->sumBase = DIVIDER->quotient;

        //Ascend(Fast response)
        if (ADP_AC_LOW >= valMean
            && ( ((0 == flag) && (DRV_MAX > pDrvCtrl->mDrvSet)) || ((1 == flag) &&
                    (DRV_EXTRA > pDrvCtrl->mDrvSet)) )
            && ADP_DC_HIGH > pDrvCtrl->adpThrd)
        {
            //First step to baseline
            if (VLT_BASELINE - VLT_WNDSIZE > pAdpCtrl->sumBase)
            {
                pDrvCtrl->adpThrd = VLT_BASELINE;
            }
            //Second step to high threshold
            else
            {
                pDrvCtrl->adpThrd = ADP_DC_HIGH;
            }
            //pDrvCtrl->mDrvSet = (pDrvCtrl->mDrvSet * (UINT32)pDrvCtrl->adpThrd) / pAdpCtrl->sumBase;
            DIVIDER->dividend = (pDrvCtrl->mDrvSet * (UINT32)pDrvCtrl->adpThrd);
            DIVIDER->divisor = pAdpCtrl->sumBase;
            pDrvCtrl->mDrvSet = DIVIDER->quotient;
            //Adpative adjust flag
            pAdpCtrl->adpFlag = 0x01;
        }

        //Restrict drive
        if ((0 == flag) && (DRV_MAX < pDrvCtrl->mDrvSet))
        {
            pDrvCtrl->mDrvSet = DRV_MAX;
        }
        if ((1 == flag) && (DRV_EXTRA < pDrvCtrl->mDrvSet))
        {
            pDrvCtrl->mDrvSet = DRV_EXTRA;
        }
        if (DRV_MIN > pDrvCtrl->mDrvSet)
        {
            pDrvCtrl->mDrvSet = DRV_MIN;
        }
    }

    //reset controller
    pAdpCtrl->sumBase = 0;
}

/**
 * @description: Adjust drives into suitable range.
 * @param {LIGHT_DRVCTRL_T} *pCtrl  Driver adjust controller
 * @param {UINT16} valSamp  Sample value
 * @param {UINT16} mThrd    maximum dirver limit
 * @return void
 */
void Light_AdjustDrive(LIGHT_DRVCTRL_T *pCtrl, UINT16 valSamp, UINT16 mThrd)
{
    INT16 curDrv = 0;

    //Set idle type
    if (ADJUST_WORK <= pCtrl->mAdjCnt)
    {
        pCtrl->mAdjCnt = 0;
        pCtrl->mIdleTm = ADJUST_IDLE;
    }

    //Idle type
    if (0 < pCtrl->mIdleTm)
    {
        pCtrl->mIdleTm--;
        pCtrl->mDrvSet = 0;
        //Start from high drive
        if (0 == pCtrl->mIdleTm)
        {
            pCtrl->mDrvSet = mThrd;
        }
        return;
    }

    //Probe off(Adpative baseline)
    if (0 == pCtrl->adpThrd || VLT_THRD_HIGH < valSamp || VLT_THRD_LOW > valSamp)
    {
        pCtrl->mStatus = FALSE;
        pCtrl->adpThrd = VLT_BASELINE;
        pCtrl->adpWnd  = VLT_WNDSIZE;
    }

    //Adaptive driver rough adjust
    if (pCtrl->adpThrd - pCtrl->adpWnd > valSamp ||
        pCtrl->adpThrd + pCtrl->adpWnd < valSamp)
    {
        pCtrl->mAdjCnt++;
        //curDrv = ((INT32)pCtrl->mDrvSet * pCtrl->adpThrd) / valSamp;
        DIVIDER->dividend = ((INT32)pCtrl->mDrvSet * pCtrl->adpThrd);
        DIVIDER->divisor = valSamp;
        curDrv = DIVIDER->quotient;
        //Restrict extremely large jump(positive)
        if (pCtrl->mDrvSet < curDrv)
        {
            curDrv = pCtrl->mDrvSet + (PWM_GAP_DRV <<
                                       2);   //PWM64 / WORK16 * (4096 / PWM64)
        }
        //Restrict extremely large jump(negative)
        else if (pCtrl->mDrvSet > curDrv)
        {
            curDrv = pCtrl->mDrvSet - (PWM_GAP_DRV <<
                                       2);   //PWM64 / WORK16 * (4096 / PWM64)
        }
        //Restrict range
        pCtrl->mDrvSet = Lmax(Lmin(curDrv, mThrd), DRV_MIN);
    }

    //Record current driver
    if (pCtrl->mDrvSet != pCtrl->mPreDrv)
    {
        pCtrl->mStabTm = 0;
        pCtrl->aveLine = 0;
        pCtrl->mPreDrv = pCtrl->mDrvSet;
    }
    //Confirm probe connect status
    else if (VLT_THRD_HIGH > valSamp && VLT_THRD_LOW < valSamp)
    {
        pCtrl->mStabTm++;
        pCtrl->aveLine += valSamp;
        //Hold for drive stable
        if (0 < pCtrl->mAdjCnt)
        {
            pCtrl->mAdjCnt--;
        }
        //Probe connected
        if (ADJUST_STAB <= pCtrl->mStabTm)
        {
            //Slightly adjust baseline before probe connect
            pCtrl->aveLine >>= 3;
            if ((FALSE == pCtrl->mStatus)
                && (pCtrl->adpThrd - 2048 > pCtrl->aveLine ||
                    pCtrl->adpThrd + 2048 < pCtrl->aveLine))
            {
                //curDrv = ((INT32)pCtrl->mDrvSet * pCtrl->adpThrd) / pCtrl->aveLine;
                DIVIDER->dividend = ((INT32)pCtrl->mDrvSet * pCtrl->adpThrd);
                DIVIDER->divisor = pCtrl->aveLine;
                curDrv = DIVIDER->quotient;
                //Restrict extremely large jump(positive)
                if (pCtrl->mDrvSet < curDrv)
                {
                    curDrv = pCtrl->mDrvSet + PWM_GAP_DRV;
                }
                //Restrict extremely large jump(negative)
                else if (pCtrl->mDrvSet > curDrv)
                {
                    curDrv = pCtrl->mDrvSet - PWM_GAP_DRV;
                }
                //Update driver
                pCtrl->mDrvSet = Lmax(Lmin(curDrv, mThrd), DRV_MIN);
            }

            //Reset
            pCtrl->mAdjCnt = 0;
            pCtrl->mStabTm = 0;
            pCtrl->aveLine = 0;
            pCtrl->mStatus = TRUE;
            pCtrl->adpWnd  = VLT_WNDSIZE << 1;
        }
    }
}

/**
 * @description: Adjust Infrared & Red light driver
 * @param void
 * @return       Probe drive adjust status
 */
UCHAR Light_ProbeDriveAdjust(void)
{
    static UCHAR sProbeFlag = FALSE;
    static UCHAR sOffDelay  = 0;

    //Adjust Red/Ird drive
    Light_AdjustDrive(&gCtrlRed, gSampArray[PHASE_RED_DRV], DRV_MAX);
    Light_AdjustDrive(&gCtrlIrd, gSampArray[PHASE_IRD_DRV], DRV_EXTRA);

    //Statisitic sample fluctuate traits
    Light_StatSampleACVariety(&gFluCtrl, &gSatSamp);

    //Probe off state counter(50Hz)
    if (FALSE == gCtrlRed.mStatus || FALSE == gCtrlIrd.mStatus)
    {
        if (16 > sOffDelay)
        {
            sOffDelay++;
        }
    }
    else
    {
        if (0 < sOffDelay)
        {
            sOffDelay--;
        }
    }

    //Probe off state
    if (8 <= sOffDelay
        || gSampArray[PHASE_RED_SAMP] < gSampArray[PHASE_RED_AMB] + PRO_NOISE_DC
        || gSampArray[PHASE_IRD_SAMP] < gSampArray[PHASE_IRD_AMB] + PRO_NOISE_DC)
    {
        sProbeFlag = FALSE;
    }
    //Adaptive AC component adjust
    else
    {
        //Reset adaptive adjust controller
        if (FALSE == sProbeFlag)
        {
            sProbeFlag = TRUE;
            //Adaptive driver adjust controller reset(Wait 16s at initial time)
            memset(&gAdptRed, 0, sizeof(gAdptRed));
            memset(&gAdptIrd, 0, sizeof(gAdptIrd));

            //Sample process reset
            gInitCnt = TRUE;
        }

        //Adaptive baseline adjust
        Light_AdaptBaselineSet(&gAdptRed, &gCtrlRed, gSatSamp.orgRed, gFluCtrl.acSigR,
                               0);
        Light_AdaptBaselineSet(&gAdptIrd, &gCtrlIrd, gSatSamp.orgIrd, gFluCtrl.acSigI,
                               1);

        //Reset fluctuate statistic controller if adaptive baseline adjusted
        if (0x01 == (gAdptRed.adpFlag & 0x01) || 0x01 == (gAdptIrd.adpFlag & 0x01))
        {
            memset(&gFluCtrl, 0, sizeof(gFluCtrl));
            gFluCtrl.mSecCnt = ADP_DRV_SEC << 1;
        }
    }

    //Get drive value
    gDriveRed = gCtrlRed.mDrvSet;
    gDriveIrd = gCtrlIrd.mDrvSet;

    return sProbeFlag;
}


