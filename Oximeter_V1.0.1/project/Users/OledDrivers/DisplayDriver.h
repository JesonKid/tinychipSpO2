#ifndef _DISPLAY_DRIVER_
#define _DISPLAY_DRIVER_

#include "DataType.h"
#include "UG2864Driver.h"

//----------------------------------------------------------------------------------------------------

#define SOFTWARE_VERSION           (0x010001)
#define HARDWARE_VERSION           (0x010000)

//Update period
#define GUI_UPDATE_RATE       (SAT_SAMPLE_RATE)           //OLED refresh frequency, depend on calling function


#define GUI_CURSOR_LEN        (8)                         //Plethysmogram cursor length
#define GUI_PLETH_HORLEN      (100)                       //Plethysmogram horizontal display span
#define GUI_PLETH_VERLEN      (64)                        //Plethysmogram horizontal display span

#define GUI_BATTERY_LEN       (16)                        //Battery voltage length

//Key hold time(50Hz)
#define KEY_SHORT_MIN            (4)                      //Minium time for short press key(80ms)
#define KEY_SHORT_MAX            (48)                     //Maxium time for short press key(960ms)
#define KEY_POWER_OFF            (100)                    //Power off times(50Hz, 2s)
#define KEY_AUTO_OFF1            (200)                    //Automatic power off(50Hz, 4s)
#define KEY_AUTO_OFF2            (800)                    //Automatic power off(50Hz, 16s)
#define MAX_NO_OPR_TM            (2000)                   //Max No Operation Time in menu
#define KEY_PARAM_HOLD           (200)                    //Parameter hold on(4s)
#define MAX_KEEP_BRI_TM          (7500)                   //Max Time for Display Brightness Keeping(150s)
#define BIAS_SAT                 (1)                      //Step for spo2 threshold
#define BIAS_BPM                 (5)                      //Step for pulse threshold
#define PARAMS_TO_INFO_CNT       (8)                      //Key press cnt for params page change to info page


//GUI interface type
typedef enum
{
    GUI_SHOW_LOGO = 0,                                    //Display logo
    //GUI_SHOW_LOGO_AFT,                                    //After Display logo
    GUI_SHOW_PARAM,                                       //Display parameter
    GUI_SHOW_MENU,                                        //Display alarm setup menu
    GUI_SHOW_PROBEOFF,                                    //Display probe off tips
    GUI_SHOW_FAC_INFO,                                    //Factory Information

} GUI_SHOW_E;



//GUI logo type(level-0)
typedef enum
{
    LOGO_FIRMS = 0,                                       //Firms logo
    LOGO_VERSION,                                         //Firmware version
    LOGO_MARGIN,

} LOGO_TYPE_E;



//GUI display sequence(level-1)
typedef enum
{
    PARAM_SEQ_RESET = 0,                                  //Reset screen
    PARAM_SEQ_CAPTION,                                    //Refresh caption
    PARAM_SEQ_BATTERY,                                    //Refresh battery
    PARAM_SEQ_MARGIN,

} GUI_SEQ_E;



//GUI Alarm setup
typedef enum
{
    MENU_ALM_RESET = 0,                                   //Reset alarm
    MENU_ALM_LO_SAT,                                      //Spo2 low threshold
    MENU_ALM_LO_BPM,                                      //Pulse low threshold
    MENU_ALM_HI_BPM,                                      //Pulse high threshold
    MENU_ALM_BEEP,                                        //Beep switch
    MENU_ALM_RESTORE,                                     //Restore default configure
    MENU_ALM_EXIT,                                        //Exit menu setup
    MENU_ALM_MARGIN,

} GUI_MENU_E;



//Probe off tips
typedef enum
{
    APOD_RESET = 0,                                       //Reset screen
    APOD_TIPS,                                            //Probe off tips
    APOD_MARGIN,

} APOD_TIP_E;



//Display sequence
typedef struct
{
    UCHAR guiType;                                        //GUI first type(level-0)
    UCHAR subType;                                        //Sub-type(layout_1/2)
    UCHAR optItem;                                        //Selected item
    UCHAR mDirect;                                        //Current orientation
    UCHAR tmCount;                                        //Time counter(50Hz)
    UCHAR bReset;                                         //Pleth and Bar reset flag
    //UCHAR preVolt;                                        //Previous voltage for voltage update
    UCHAR preSAT;                                         //Previous spo2 for update
    UINT16 preBPM;                                        //Previous pulse for update
    UINT16 tmProbe;                                       //Probe off detect
    UINT16 tmParmLast;                                    //Probe off and Param Keeping timing
    UINT16 tmVolt;                                        //Battery deplete counter
    UINT16 tmNoOprMenu;                                   //No Operation Time During Menu UI
    INT16 tmNoOprParam;                                  //No Operation Time During Param UI

} GUI_CTRL_T;



//Key control
typedef struct
{
    UCHAR bErrFlg;                                        //Abnomral operation during Logo display
    UCHAR bUpdate;                                        //Update display content(according to key sequence)
    UCHAR bTrgKey;                                        //Key triggered
    UINT16 tmKeyOn;                                       //hold time of pressed key
    UCHAR shortKeyCnt;                                    //short press key count

} KEY_TRL_T;



//Alarm control
typedef struct
{
    UCHAR  blkSAT;                                        //Saturation blink
    UCHAR  blkBPM;                                        //Pulse rate blink
    UCHAR  swBeep;                                        //Pulse beep switch
    UCHAR  swAlarm;                                       //Alarm switch
    UCHAR  digOprDir;                                     //Digital operation type: plus or minus
    UCHAR  upSAT;                                         //SpO2 high threshold
    UCHAR  lowSAT;                                        //SpO2 low threshold
    UINT16 upBPM;                                         //Pulse up threshold
    UINT16 lowBPM;                                        //Pulse low threshold

} ALM_CTRL_T;

//----------------------------------------------------------------------------------------------------

extern GUI_CTRL_T
gGuiCtrl;                               //GUI display controller
extern KEY_TRL_T  gKeyCtrl;                                  //Key controller
extern ALM_CTRL_T gAlmCtrl;                               //Alarm controller

extern INT16 gCellVolt;                                    //Battery voltage
extern const UCHAR
gDirMapping[];                         //Display Direction Change Mapping
extern const UCHAR
gDirMappingBigFont[];                  //Display Direction Change Mapping when bigFont
extern UCHAR gMemuPageNo;                                 //Menu Page No.


//----------------------------------------------------------------------------------------------------


/*
********************************************************************************
 * [FUNC.name  ] void Display_Init(void)                                          *
 * [FUNC.spec. ] Initialize GUI display content.                              *
 * [FUNC.input ]                                                              *
 *               void                                                         *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void Display_Init(void);

/*
********************************************************************************
 * [FUNC.name  ] void Display_GUI(void)                                       *
 * [FUNC.spec. ] Interface display (parameters/menu/trend), refresh frequency *
 *               is 100Hz, must guaranteed by  calling function.              *
 * [FUNC.input ]                                                              *
 *               void                                                         *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void Display_GUI(void);




#endif

