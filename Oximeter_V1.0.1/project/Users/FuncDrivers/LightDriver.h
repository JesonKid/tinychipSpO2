/**
*****************************************************************************
* @brief
*
* @file   LightDriver.h
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

#ifndef _LIGHT_DRIVE_
#define _LIGHT_DRIVE_

#include "DataType.h"



//Maxium and minium
#define Lmax(dtA, dtB)          ((dtA) >= (dtB) ? (dtA) : (dtB))
#define Lmin(dtA, dtB)          ((dtA) >= (dtB) ? (dtB) : (dtA))
//Absolute difference value of two data
#define Labsdif(dtA, dtB)       (((dtA) >= (dtB)) ? ((dtA) - (dtB)) : ((dtB) - (dtA)))

//3.3V * 14.7k / (80k + 14.7k) / 5 ~= 102.5mA
#define DRV_EXTRA          (3584)                        //80.0mA = 102.5mA(current) * 3200 / 4096(span)
#define DRV_MAX            (3200)                        //80mA = 102.5mA(current) * 3200 / 4096(span)
#define DRV_MIN            (320)                         //3.375mA = 108mA(current) * 128 / 4096(span)

//Light adjust period
#define ADJUST_WORK        (16)                          //0.16ms for automatic probe detect during 1s period
#define ADJUST_IDLE        (32)                          //Idle status during 1s period
#define ADJUST_STAB        (8)                           //Stable span for 40ms

//Voltage 3.3V, 30K'Ohm, Current 108mA, ADC 16bit(65536)
#define VLT_THRD_HIGH      (61440)                       //Probe off high threshold(2^10 * 60)
#define VLT_THRD_LOW       (1024)                        //Probe off low threshold(2^10)
#define VLT_WNDSIZE        (8192)                        //Fluctuate span(2^10 * 8)
#define VLT_BASELINE       (40960)                       //Sample proper threshold(2^10 * 40)

//Adpative adjust threshold
#define ADP_DRV_NUM        (4)                           //AC component buffer
#define ADP_DRV_SEC        (SAT_SAMPLE_RATE)             //2 second span
#define ADP_AC_HIGH        (1024)                        //AC component maxium threshold(2^10)
#define ADP_AC_LOW         (128)                         //AC component minium threshold(2^7)
#define ADP_DC_HIGH        (VLT_THRD_HIGH - VLT_WNDSIZE) //AC component maxium threshold(2^10 * 50)
#define ADP_DC_LOW         (VLT_THRD_LOW + VLT_WNDSIZE)  //AC component minium threshold(2^10 * 10)

//Probe off detect
#define PRO_NOISE_DC       (1024)                        //DC noise threshold
#define PRO_NOISE_AC       (64)                          //AC noise threshold
#define PRO_NOISE_NUM      (2)                           //Noise counter(2s)
#define PRO_NOISE_AMB      (3072)                        //Ambient disturb threshold
#define PRO_ERR_LOW_THRD   (3072)                        //Probe error threshold
#define PRO_ERR_HIGH_THRD  (57344)                       //Probe error threshold
#define PRO_ERR_NUM        (4)                           //Probe error time counter(4S)



//Drive control array
typedef struct
{
    UCHAR  mStatus;                                      //FALSE_Probe off TRUE_probe connect
    UCHAR  mIdleTm;                                      //Idle time of light close
    UCHAR  mAdjCnt;                                      //Rough adjust time counter
    UCHAR  mStabTm;                                      //Stable counter
    UINT16 mDrvSet;                                      //Drive values
    UINT16 mPreDrv;                                      //Previous drive values
    UINT16 adpThrd;                                      //Adaptive threshold
    UINT16 adpWnd;                                       //Adpative window size
    UINT32 aveLine;                                      //Average baseline

} LIGHT_DRVCTRL_T;



//Adptive AC component adjust control
typedef struct
{
    CHAR  adpFlag;                                       //bit_0 ascend bit_1 descend
    INT16 mAcBuf[ADP_DRV_NUM];                           //AC component buffer
    INT32 sumBase;                                       //Baseline

} LIGHT_ADAPCTRL_T;



//Maxium and minium unit
typedef struct
{
    UINT16 vMax;
    UINT16 vMin;

} LIGHT_EXTERM_T;



//Statistic signal and ambient fluctuate traits
typedef struct
{
    UCHAR  errFlg;                                       //Samples abnormal flag (TRUE_abnormal)
    UCHAR  bReady;                                       //Statistics valid for APOD driver adjust
    INT16  mSecCnt;                                      //1 second counter
    LIGHT_EXTERM_T
    recAmbR;                              //Red ambient AC component record
    LIGHT_EXTERM_T
    recAmbI;                              //Infrared ambient AC component record
    LIGHT_EXTERM_T
    recSigR;                              //Red signal AC component record
    LIGHT_EXTERM_T
    recSigI;                              //Infrared signal AC component record
    INT16  acAmbR;                                       //Red ambient AC amplitude during 1 second
    INT16  acAmbI;                                       //Infrared ambient AC amplitude during 1 second
    INT16  acSigR;                                       //Red signal AC amplitude during 1 second
    INT16  acSigI;                                       //Infrared signal AC amplitude during 1 second
    INT32  dcAmbR;                                       //Red ambient DC amplitude during 1 second
    INT32  dcAmbI;                                       //Infrared ambient DC amplitude during 1 second
    INT32  dcSigR;                                       //Red signal DC amplitude during 1 second
    INT32  dcSigI;                                       //Infrared signal DC amplitude during 1 second

} LIGHT_FLUCCTRL_T;



//Probe error detect
typedef struct
{
    UCHAR cntRed;                                        //Red abnomral detect
    UCHAR cntIrd;                                        //Infrared abnormal detect
    INT16 mSecCnt;                                       //1 second counter
    INT32 dcAmbR;                                        //Red signal AC amplitude during 1 second
    INT32 dcAmbI;                                        //Infrared signal AC amplitude during 1 second
    INT32 dcSigR;                                        //Red signal DC amplitude during 1 second
    INT32 dcSigI;                                        //Infrared signal DC amplitude during 1 second

} LIGHT_ERRCTRL_T;




extern UINT16  gDriveRed;            // Red driver
extern UINT16  gDriveIrd;            // Ird driver



/**
 * @description: Adjust Infrared & Red light driver
 * @param void
 * @return {*}   Probe drive adjust status
 */
extern UCHAR Light_ProbeDriveAdjust(void);

#endif
