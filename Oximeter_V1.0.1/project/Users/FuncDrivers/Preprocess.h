/**
*****************************************************************************
* @brief
*
* @file   Preprocess.h
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

#ifndef _PREPROCESS_
#define _PREPROCESS_

#include "DataType.h"
#include "Spo2Interface.h"


//----------------------------------------------------------------------------------------------------



//Maxium and minium
#define Pmax(dtA, dtB)          ((dtA) >= (dtB) ? (dtA) : (dtB))
#define Pmin(dtA, dtB)          ((dtA) >= (dtB) ? (dtB) : (dtA))

//Absolute difference value of two data
#define Pabsdif(dtA, dtB)           (((dtA) >= (dtB)) ? ((dtA) - (dtB)) : ((dtB) - (dtA)))

//FIR filter
#define FIR_ORDER           (48)                                //FIR filter

//Smooth filter
#define SMOOTH_HALFWND      (36)                                //Half Smooth Window Size(36)
#define SMOOTH_BUFSIZE      (SMOOTH_HALFWND * 2 + 1)            //Buffer Length (50Hz * 60 / 73 ¡Ö41.1BPM)



#define DMA_ADC_NUM           (8)
#define DMA_LENTH             (DMA_ADC_NUM << 2)

#define PWM_GAP_FRQ           (64)
#define PWM_GAP_DRV           (64)                       //(4096 / PWM_GAP_FRQ)

//Sequence period
#define SEQ_BATTERY           (3600)                     //Battery period(30s ~ 3600 * 160 * 60us)
#define SEQ_PERIOD            (150)                      //Sequence period(111.1~Hz = 150 * 60us)


//Sample phase
typedef enum
{
    PHASE_RED_AMB = 0,
    PHASE_RED_SAMP,
    PHASE_IRD_AMB,
    PHASE_IRD_SAMP,
    PHASE_RED_DRV,                                       //Red Samples for Probe Detect
    PHASE_IRD_DRV,                                       //Infrared Samples for Probe Detect
    PHASE_MARGIN,

} SAMP_PHASE_E;



//Resample data structure
typedef struct
{
    INT32 orgIrd;                                      //Orignal infrared data
    INT32 orgRed;                                      //Orignal red data
    INT32 ambIrd;                                      //Infrared ambient
    INT32 ambRed;                                      //Red ambient

} SAMP_CTRL_T;



//Sample components
typedef struct
{
    INT32 acIrd;                                       //Infrared signal AC component
    INT32 acRed;                                       //Red signal AC component
    INT32 dcIrd;                                       //Infrared signal DC component
    INT32 dcRed;                                       //Red signal DC component

} DAT_UNIT_T;



//FIR low pass filter buffer
typedef struct
{
    INT32 mRed[FIR_ORDER];                             //RedRed buffer
    INT32 mIrd[FIR_ORDER];                             //Infrared buffer

} LPFIR_BUF_T;



//Sample Data Buffer
typedef struct
{
    INT32 mAvgIrd;                                     //Infrared Window Summary
    INT32 mAvgRed;                                     //Red Window Summary
    INT16 mWritePos;                                   //Write Position
    INT16 mFocusPos;                                   //Focus Position
    INT32 mIrd[SMOOTH_BUFSIZE];                        //Infrared Buffer
    INT32 mRed[SMOOTH_BUFSIZE];                        //Red Buffer

} SMO_CTRL_T;


//----------------------------------------------------------------------------------------------------

extern UINT32
gDmaAdcBuf[DMA_ADC_NUM];               //DMA0_CH0 buffer for samples
extern INT32      gSampArray[PHASE_MARGIN];              //4 steps sample data
extern CHAR
gInitCnt;                              //Reset smooth and FIR filter

//----------------------------------------------------------------------------------------------------

/**
 * @description: Resample signal and high/low pass filter
 * @param void
 * @return {CHAR}   Samples valid status
 */
extern CHAR Process_SampleData(void);

extern BOOL is_new_sample_ready;

// -----------------------------------------------------------------------



#endif
