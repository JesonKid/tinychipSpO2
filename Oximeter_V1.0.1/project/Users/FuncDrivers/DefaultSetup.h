/**
*****************************************************************************
* @brief
*
* @file   DefaultSetup.h
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

#ifndef _DEFAULT_SETUP_
#define _DEFAULT_SETUP_

#include "DataType.h"
#include "UG2864Driver.h"

//----------------------------------------------------------------------------------------------------

#define FMC_CFG_PAGE        (0x1000FE00)                  //Configuration flash page
#define FMC_BYTE_NUM        (4)                           //byte size
#define PAGE_SIZE           (512)


#define FMC_CFG_SIZE        (10)                           //Configuration byte number
#define TM_CHK_SPAN         (16)                           //On-site detection time span

#define DEF_SW_BEEP             (0)
#define DEF_SW_ALM              (1)
#define DEF_DISP_BRIGHTNESS (4)
#define DEF_DISP_DIR            (GUI_DIR_HORIZON_POS)
#define DEF_DIG_OPRDIR          (0)
#define DEF_BIG_FONT            (0)
#define DEF_UP_SAT              (100)
#define DEF_LOW_SAT             (94)
#define DEF_UP_PR               (130)
#define DEF_LOW_PR              (50)



//Configuration threshold
typedef struct
{
    UINT16 swBeep;                                        //Pulse beep switch
    UINT16 swAlarm;                                       //Alarm switch
    UINT16 brightness;                                    //Display brightness
    UINT16 dispDir;                                       //Display direction
    UINT16 digOprDir;                                     //Alarm threshold Operation Direction
    UINT16 bigFont;                                       //Big Font
    UINT16 upSAT;                                         //SpO2 high threshold
    UINT16 lowSAT;                                        //SpO2 low threshold
    UINT16 upBPM;                                         //Pulse up threshold
    UINT16 lowBPM;                                        //Pulse low threshold

    // Do not store to Flash
    UINT16 swDemo;                                        //Demo mode switch
    UINT16 swLowBright;                                   //Brightness set to lowest after a long time no operation

} SETUP_CFGCtrl_T;

//On-site detection
typedef struct
{
    CHAR chkFlag;                                         //Check finished flag
    CHAR chkBLH;                                          //Bluetooth on-site check

} SETUP_CHKFunc_T;



extern SETUP_CFGCtrl_T
gCfgCtrl;                                  //Configuration controller
extern SETUP_CHKFunc_T
gChkFunc;                                  //Function check

//----------------------------------------------------------------------------------------------------

/**
 * @description: Read configuration infor from flash
 * @param void
 * @return void
 */
extern void ReadFlashCfg(void);

/**
 * @description: Write current configuration to flash.
 * @param void
 * @return void
 */
extern void WriteFlashCfg(void);

/**
 * @description: Restore oximeter setup.
 * @param void
 * @return void
 */
extern void Setup_RestoreDevice(void);

/**
 * @description: Update oximeter setup.
 * @param void
 * @return void
 */
extern void Setup_UpdateDevice(void);


/**
 * @description: Default Setting Check
 * @param void
 * @return {UCHAR}
 */
extern UCHAR Setup_IsRestoreValue(void);


#endif
