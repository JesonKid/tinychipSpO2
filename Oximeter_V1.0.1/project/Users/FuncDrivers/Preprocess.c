/**
*****************************************************************************
* @brief
*
* @file   Preprocess.c
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

#include "Preprocess.h"
#include "Spo2Interface.h"
#include "string.h"
#include "tcasxxx.h"

//----------------------------------------------------------------------------------------------------

BOOL is_new_sample_ready = FALSE;

//Fir filter(Equiripple FS_111.1~Hz Fpass_5.0, Fstop_15.0, Apass_0.005, Astop_80, Zoom 2^10 times)
const FLOAT gSampleFIR[FIR_ORDER >> 1] =
{
    0.0145387523839853, -0.214348952102283, -0.569446023392970, -1.05311647854226,
    -1.43012454571205,   -1.33434317676224,  -0.389286496521704,  1.56848995410509,
    4.24946477776027,    6.78335398859369,   7.83044576162558,   5.99046554705443,
    0.442040810382293,  -8.38997905514764, -18.4265516790600,  -26.0485183354183,
    -26.8490860907234,   -16.9063757698134,    5.77886192318207,  40.1724220603669,
    81.8781833067420,   123.830164453315,   157.902379750556,   177.001323546688,
};


//----------------------------------------------------------------------------------------------------

UINT32 gDmaAdcBuf[DMA_ADC_NUM]  = {0};                 //DMA0_CH0 buffer for samples
INT32  gSampArray[PHASE_MARGIN] = {0};                 //4 steps sample data

CHAR     gInitCnt =
    TRUE;                              //Reset smooth and FIR filter
LPFIR_BUF_T gFirBuf  = {0};                            //Low pass FIR filter buffer
SMO_CTRL_T  gSmoCtrl = {0};                            //Fixed smooth filter control

//----------------------------------------------------------------------------------------------------


/**
 * @description: Statistic FIR buffer to determine whether filter reset.
 * @param {LPFIR_BUF_T} *pArray  FIR filter buffer
 * @return void
 */
static void Process_ResetFilterJudge(LPFIR_BUF_T *pArray)
{
    UCHAR i = 0;
    //INT16 mIndex = 0;
    INT32 maxRed = 0;
    INT32 minRed = 0;
    INT32 maxIrd = 0;
    INT32 minIrd = 0;
    INT32 aveRed = 0;
    INT32 aveIrd = 0;

    //Statistic relevant variables
    maxRed = pArray->mRed[0];
    minRed = pArray->mRed[0];
    aveRed = pArray->mRed[0];
    maxIrd = pArray->mIrd[0];
    minIrd = pArray->mIrd[0];
    aveIrd = pArray->mIrd[0];
    for (i = 1; i < FIR_ORDER; i++)
    {
        maxRed  = Pmax(maxRed, pArray->mRed[i]);
        minRed  = Pmin(minRed, pArray->mRed[i]);
        aveRed += pArray->mRed[i];
        maxIrd  = Pmax(maxIrd, pArray->mIrd[i]);
        minIrd  = Pmin(minIrd, pArray->mIrd[i]);
        aveIrd += pArray->mIrd[i];
    }
    aveRed /= FIR_ORDER;
    aveIrd /= FIR_ORDER;

    //Set initialize flag
    if (Pabsdif(maxRed, minRed) > (aveRed >> 1)
        || Pabsdif(maxIrd, minIrd) > (aveIrd >> 1))
    {
        gInitCnt = FIR_ORDER;
    }
}

/**
 * @description: FIR filter for high frequence disturb
 * @param {SAMP_CTRL_T} *pData   Samples
 * @param {LPFIR_BUF_T} *pArray  FIR filter buffer
 * @return void
 */
static void Process_LowPassFiltFIR(SAMP_CTRL_T *pData, LPFIR_BUF_T *pArray)
{
    UCHAR i = 0;
    INT16 mIndex = 0;
    FLOAT sumIrd = 0;
    FLOAT sumRed = 0;

    //Reset FIR filter
    if (TRUE == gInitCnt)
    {
        for (i = 0; i < FIR_ORDER; i++)
        {
            pArray->mRed[i] = pData->orgRed;
            pArray->mIrd[i] = pData->orgIrd;
        }
    }
    //Fulfill FIR buffer
    else
    {
        mIndex = (FIR_ORDER - 1) * sizeof(INT32);
        memmove(&pArray->mRed[1], &pArray->mRed[0], mIndex);
        memmove(&pArray->mIrd[1], &pArray->mIrd[0], mIndex);
        pArray->mRed[0] = pData->orgRed;
        pArray->mIrd[0] = pData->orgIrd;
    }

    //Filter current samples
    for (i = 0; i < (FIR_ORDER >> 1); i++)
    {
        mIndex = FIR_ORDER - 1 - i;
        sumRed += (pArray->mRed[i] + pArray->mRed[mIndex]) * gSampleFIR[i];
        sumIrd += (pArray->mIrd[i] + pArray->mIrd[mIndex]) * gSampleFIR[i];
    }

    //Output filtered samples
    pData->orgRed = ((INT32)sumRed) >> 10;
    pData->orgIrd = ((INT32)sumIrd) >> 10;
}

/**
 * @description: Low frequency noise filter of fixed size
 * @param {INT32} mIrd  Orignal Infrared Samples
 * @param {INT32} mRed  Orignal Red Samples
 * @return {DAT_UNIT_T}    DATUNIT
 */
static DAT_UNIT_T Process_FixedSmoothFilter(INT32 mIrd, INT32 mRed)
{
    INT16 i = 0;
    DAT_UNIT_T datUnit = {0};

    //Reset smooth filter
    if (TRUE == gInitCnt)
    {
        //Reset buffer
        memset(&gSmoCtrl, 0, sizeof(gSmoCtrl));
        gSmoCtrl.mFocusPos = SMOOTH_HALFWND;
        gSmoCtrl.mWritePos = SMOOTH_HALFWND << 1;
        //Initialize value
        for (i = 0; i < SMOOTH_BUFSIZE; i++)
        {
            gSmoCtrl.mRed[i] = mRed;
            gSmoCtrl.mIrd[i] = mIrd;
        }
        gSmoCtrl.mAvgRed = mRed * SMOOTH_BUFSIZE;
        gSmoCtrl.mAvgIrd = mIrd * SMOOTH_BUFSIZE;
    }

    //Calculate cumulative sum(discard position equal to add position)
    gSmoCtrl.mAvgIrd += mIrd - gSmoCtrl.mIrd[gSmoCtrl.mWritePos];
    gSmoCtrl.mAvgRed += mRed - gSmoCtrl.mRed[gSmoCtrl.mWritePos];

    DIVIDER->dividend = gSmoCtrl.mAvgIrd;
    DIVIDER->divisor = SMOOTH_BUFSIZE;
    datUnit.dcIrd = DIVIDER->quotient;

    DIVIDER->dividend = gSmoCtrl.mAvgRed;
    //DIVIDER->divisor = SMOOTH_BUFSIZE;
    datUnit.dcRed = DIVIDER->quotient;

    //Calculate AC components
    datUnit.acIrd = (INT32)(gSmoCtrl.mIrd[gSmoCtrl.mFocusPos] - datUnit.dcIrd);
    datUnit.acRed = (INT32)(gSmoCtrl.mRed[gSmoCtrl.mFocusPos] - datUnit.dcRed);
    //Remove DC components
    datUnit.acRed = (INT32)(datUnit.acRed * (gSmoCtrl.mAvgIrd /
                            (DOUBLE)gSmoCtrl.mAvgRed));

    //Store New Samples
    gSmoCtrl.mIrd[gSmoCtrl.mWritePos] = mIrd;
    gSmoCtrl.mRed[gSmoCtrl.mWritePos] = mRed;

    //Data Buffer Index Loop
    gSmoCtrl.mWritePos++;
    if (SMOOTH_BUFSIZE <= gSmoCtrl.mWritePos)
    {
        gSmoCtrl.mWritePos = 0;
    }

    //Focus Index Loop
    gSmoCtrl.mFocusPos++;
    if (SMOOTH_BUFSIZE <= gSmoCtrl.mFocusPos)
    {
        gSmoCtrl.mFocusPos = 0;
    }

    return datUnit;
}



/**
 * @description: Resample AC & DC component into 50Hz
 * @param {SAMPCTRL} *pData  Samples
 * @return {CHAR}  filtered datum
 */
static CHAR Process_ResampleSignal(SAMP_CTRL_T *pData)
{
    CHAR bVaild = TRUE;
    INT16 oriRate = 18;                      //18ms (111.1~Hz)
    INT16 srcRate = 40;                      //40ms(50.00Hz)
    INT16 tmpPos  = 0;
    INT16 accSamp = oriRate >> 1;
    static INT16 sOriPos = 0;               //Orignal sample position
    static SAMP_CTRL_T sDatBuf[2] = {0};

    //Copy sample data
    sDatBuf[1] = sDatBuf[0];
    sDatBuf[0] = *pData;

    // Calculate resample data
    tmpPos = sOriPos % srcRate;
    if (0 == tmpPos)
    {
        sOriPos = 0;
    }
    else if (oriRate > tmpPos)
    {
        //Accuracy consideration
        tmpPos = oriRate - tmpPos;

        DIVIDER->dividend = (sDatBuf[0].orgIrd * oriRate + (sDatBuf[1].orgIrd -
                             sDatBuf[0].orgIrd) * tmpPos + accSamp);
        DIVIDER->divisor = oriRate;
        pData->orgIrd = DIVIDER->quotient;

        DIVIDER->dividend = (sDatBuf[0].orgRed * oriRate + (sDatBuf[1].orgRed -
                             sDatBuf[0].orgRed) * tmpPos + accSamp);
        //DIVIDER->divisor = oriRate;
        pData->orgRed = DIVIDER->quotient;

        DIVIDER->dividend = (sDatBuf[0].ambIrd * oriRate + (sDatBuf[1].ambIrd -
                             sDatBuf[0].ambIrd) * tmpPos + accSamp);
        //DIVIDER->divisor = oriRate;
        pData->ambIrd = DIVIDER->quotient;

        DIVIDER->dividend = (sDatBuf[0].ambRed * oriRate + (sDatBuf[1].ambRed -
                             sDatBuf[0].ambRed) * tmpPos + accSamp);
        //DIVIDER->divisor = oriRate;
        pData->ambRed = DIVIDER->quotient;
    }
    else
    {
        bVaild = FALSE;
    }

    //Originate sample accumulate
    sOriPos += oriRate;

    return bVaild;
}

/**
 * @description: Resample signal and high/low pass filter.
 * @param void
 * @return {CHAR}  Samples valid status
 */
CHAR Process_SampleData(void)
{
    DAT_UNIT_T  mSmoFilt = {0};
    SAMP_CTRL_T mCurData = {0};

    //Obtain samples
    mCurData.orgRed = gSampArray[PHASE_RED_SAMP];
    mCurData.orgIrd = gSampArray[PHASE_IRD_SAMP];
    mCurData.ambRed = gSampArray[PHASE_RED_AMB];
    mCurData.ambIrd = gSampArray[PHASE_IRD_AMB];

    //Remove red ambient
    if (mCurData.orgRed > mCurData.ambRed)
    {
        mCurData.orgRed -= mCurData.ambRed;
    }
    else
    {
        mCurData.orgRed = 0;
    }

    //Remove infrared ambient
    if (mCurData.orgIrd > mCurData.ambIrd)
    {
        mCurData.orgIrd -= mCurData.ambIrd;
    }
    else
    {
        mCurData.orgIrd = 0;
    }

    //High disturb remove
    Process_LowPassFiltFIR(&mCurData, &gFirBuf);

    //Resample and smooth filter
    if (TRUE == Process_ResampleSignal(&mCurData))
    {
        //Smooth filter(Low frequence disturb)
        mSmoFilt = Process_FixedSmoothFilter(mCurData.orgIrd, mCurData.orgRed);

        //Samples for algorithm
        gSatSamp.RedAC = mSmoFilt.acRed >> 3;     //Reserve 2^1 times
        gSatSamp.IrdAC = mSmoFilt.acIrd >> 3;     //Reserve 2^1 times
        gSatSamp.RedDC = mSmoFilt.dcRed >> 4;
        gSatSamp.IrdDC = mSmoFilt.dcIrd >> 4;

        //Orignal sample
        gSatSamp.orgRed = gSampArray[PHASE_RED_DRV];
        gSatSamp.orgIrd = gSampArray[PHASE_IRD_DRV];
        gSatSamp.ambRed = gSampArray[PHASE_RED_AMB] >> 4;
        gSatSamp.ambIrd = gSampArray[PHASE_IRD_AMB] >> 4;

        is_new_sample_ready = TRUE;

        //Reset filter flag
        if (gInitCnt)
        {
            gInitCnt = FALSE;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

