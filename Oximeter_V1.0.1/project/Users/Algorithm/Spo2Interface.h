/**
*****************************************************************************
* @brief
*
* @file   Spo2Interface.h
* @author   lt
* @date   2021-11-11
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

#ifndef _SPO2_INTERFACE_
#define _SPO2_INTERFACE_



#include "DataType.h"


#ifdef FOR_PC_DISPLAY
#include "Spo2PcInterface.h"
#endif

//----------------------------------------------------------------------------------------------------

//Algorithm Version
#define SPO2_ALG_VERSION           (0x010000)

#define SAT_SAMPLE_RATE            (50)                 //SpO2 Sample Rate

//----------------------------------------------------------------------------------------------------



//Patient type
typedef enum
{
    SAT_ADULT = 0,
    SAT_PEDIATRIC,
    SAT_NEONATE,

} SAT_PATIENTTYPE_E;



//Sensitivity type
typedef enum
{
    SAT_LOW = 0,
    SAT_MID,
    SAT_HIGH,

} SAT_SENSITIVITY_E;



//Response type
typedef enum
{
    SAT_AVE_2  = 1,
    SAT_AVE_4  = 2,
    SAT_AVE_6  = 3,
    SAT_AVE_8  = 4,
    SAT_AVE_10 = 5,
    SAT_AVE_12 = 6,
    SAT_AVE_14 = 7,
    SAT_AVE_16 = 8,

} SAT_RESPONSE_E;

//----------------------------------------------------------------------------------------------------



//Input Sample Datum
typedef struct
{
    INT16 RedAC;              //Red Samples                  [-32767 - 32768] [Default 0]
    INT16 IrdAC;              //Infrared Samples             [-32767 - 32768] [Default 0]
    UINT16 RedDC;             //Red Sample DC component      [-32767 - 32768] [Default 0]
    UINT16 IrdDC;             //Infrared Sample DC component [-32767 - 32768] [Default 0]
    UINT16 orgRed;            //Red Samples                  [0 - 65536] [Default 0]
    UINT16 orgIrd;            //Infrared Samples             [0 - 65536] [Default 0]
    UINT16 ambRed;            //Red Ambient Samples          [0 - 65536] [Default 0]
    UINT16 ambIrd;            //Infrared Ambient Samples     [0 - 65536] [Default 0]

} SAT_SAMP_T;



//Algorithm Configuration
typedef struct
{
    CHAR bReset;               //Algorithm reset                      [0, 1]                    [Default 1]
    CHAR PatientType;          //Patient type                         [Adult Pediatric Neonate] [Default Adult]
    CHAR Sensitivity;          //Sensitivity for lower perfusion      [Low Median High]         [Default Median]
    CHAR RespondTime;          //Response for parameter average times [2, 4, 6, 8, 10, 12]      [Default 6]
    CHAR SwitchResp;           //Switch for Respiration parameter     [0, 1]                    [Default 0]
    CHAR SwitchPVI;            //Switch for PVI parameter             [0, 1]                    [Default 0]
//  CHAR SwitchORI;            //Switch for ORI parameter             [0, 1]                    [Default 0]

} SAT_SETUP_T;



//Parameters
typedef struct
{
    CHAR  mSQI;                //Signal Quality         [0 - 100]   [Invalid 0]
    CHAR  mSpO2;               //SpO2 Parameter         [0 - 100]   [Invalid 0]
    INT16 mPulse;              //Pulse Parameter        [20 - 300]  [Invalid 0]
    INT16 mPerfuse;            //Perfuison Index        [0 - 20000] [Invalid 0] (zoom rate 1000 times)
    CHAR  mResp;               //Respiration parameter  [5 - 75]    [Invalid 0]
    CHAR  mPleth;              //Plethysmogram          [-50 - 50]  [Invalid 0]
    CHAR  mBar;                //Bar Graph              [0 - 15]    [Invalid 0]
    CHAR  mBeep;               //Pulse beep             [0, 1]      [Default 0]

} SAT_PARAM_T;



//Alarm Information
typedef struct
{
    CHAR ProbeOff;             //Sensor off                    [0, 1] [Default 0]
    CHAR NoProbe;              //Probe un-connected            [0, 1] [Default 0]
    CHAR ErrProbe;             //Probe Error                   [0, 1] [Default 0]
    CHAR LowPI;                //Low perfusion index           [0, 1] [Default 0]
    CHAR SeekPulse;            //Seek pulse                    [0, 1] [Default 0]
    CHAR NoPulse;              //No pulse                      [0, 1] [Default 0]
    CHAR Motion;               //Motion                        [0, 1] [Default 0]

} SAT_ALARM_T;



//SpO2 synethic Parameters
typedef struct
{
    SAT_PARAM_T sysParam;                //Parameter Output
    SAT_ALARM_T sysAlarm;                //Alarm Output

} SAT_RESULT_T;

//----------------------------------------------------------------------------------------------------

//Algorithm I/O Interface
extern SAT_SAMP_T       gSatSamp;            //(Input) SpO2 samples
extern SAT_SETUP_T      gSatCfg;             //(Input) SpO2 algorithm setups
extern SAT_RESULT_T     gSatParam;           //(Output)SpO2 algorithm outputs

//----------------------------------------------------------------------------------------------------

/*
********************************************************************************
 * [FUNC.name  ] void SAT_AlgorithmReset(void)                                 *
 * [FUNC.spec. ] Algorithm Reset.                                             *
 * [FUNC.input ]                                                              *
 *               CHAR  mType   bit_0 Sample data reset                 *
 *               CHAR  mType   bit_1 Algorithm setups reset            *
 *               CHAR  mType   bit_2 Algorithm output reset            *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void SAT_AlgorithmReset(CHAR mType);

/*
********************************************************************************
 * [FUNC.name  ] void SAT_AlgorithmHandle(SATSAMP *pData, SATSETUP *pCfg)      *
 * [FUNC.spec. ] Main Function: Point by point execute SpO2 algorithm.        *
 * [FUNC.input ]                                                              *
 *               SAT_PARAM_T    *pData   Sample datas                             *
 *               SAT_SETUP_T   *pCfg    Algorithm configuration                  *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void SAT_AlgorithmHandle(SAT_PARAM_T *pData, SAT_SETUP_T *pCfg);

/*
********************************************************************************
 * [FUNC.name  ] void SAT_SpectrumThread(SATSETUP *pCfg)                       *
 * [FUNC.spec. ] Spectrum running thread, for outer calling (2s period).      *
 * [FUNC.input ]                                                              *
 *               SAT_SETUP_T   *pCfg    Algorithm configuration                  *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Physiological parameters from frequency domain algorithm.    *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void SAT_SpectrumThread(SAT_SETUP_T *pCfg);

/*
********************************************************************************
 * [FUNC.name  ] SAT_RESULT_T SAT_GetSystermParam(void)                              *
 * [FUNC.spec. ] Obtain algorithm outputs.                                    *
 * [FUNC.input ]                                                              *
 *               void                                                         *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Physiological parameters.                                    *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern SAT_RESULT_T SAT_GetSystermParam(void);

/*
********************************************************************************
 * [FUNC.name  ] void SAT_CheckMcuID(UCHAR *pData)                               *
 * [FUNC.spec. ] Deadly loop if MCU ID check failed.                          *
 * [FUNC.input ]                                                              *
 *               UCHAR *pData   MCU ID code                           *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void SAT_CheckMcuID(UCHAR *pData);

/********************************************************
*@brief 计算心率
*@param void
*@retval void
*********************************************************/
extern void heartbeat_algorithm(void);


/********************************************************
*@brief 计算血氧
*@param void
*@retval void
*********************************************************/
extern void SpO2_algorithm(void);

#endif

