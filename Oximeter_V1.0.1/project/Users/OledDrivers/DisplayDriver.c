#include "DisplayDriver.h"
#include "fontlibs.h"
#include "Spo2Interface.h"
#include "string.h"
#include "DefaultSetup.h"
#include "tcasxxx.h"
#include "gpio.h"


//----------------------------------------------------------------------------------------------------
INT16 gCellVolt  = 15;            //Battery voltage
UCHAR gMenuBuf[120] = {0};        //Buffer for menu setup([5*8]40*24)
UCHAR gMemuPageNo = 0;                      //Menu Page No.

GUI_CTRL_T gGuiCtrl = {0};        //GUI display controller
KEY_TRL_T  gKeyCtrl = {0};        //Key controller
ALM_CTRL_T gAlmCtrl = {0};        //Alarm controller

SAT_SAMP_T   gSatSamp  = {0};        //SpO2 samples
SAT_SETUP_T  gSatCfg   = {0};        //SpO2 algorithm setups
SAT_RESULT_T gSatParam = {0};        //SpO2 algorithm outputs


const UCHAR gDirMapping[GUI_DIR_MARGIN] =
{
    GUI_DIR_VERTICAL_POS,
    GUI_DIR_HORIZON_NEG,
    GUI_DIR_VERTICAL_NEG,
    GUI_DIR_HORIZON_POS
};

const UCHAR gDirMappingBigFont[GUI_DIR_MARGIN] =
{
    GUI_DIR_HORIZON_NEG,
    GUI_DIR_MARGIN,
    GUI_DIR_HORIZON_POS,
    GUI_DIR_MARGIN          //Invalid
};

//----------------------------------------------------------------------------------------------------




static UINT32 GetSoftwareVer(void)
{
    return SOFTWARE_VERSION;
}



static UINT32 GetHardwareVer(void)
{
    return HARDWARE_VERSION;
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Version()                                        *
 * [FUNC.spec. ] Display Version.                                             *
 * [FUNC.input ]                                                              *
 *               void                                                         *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
static void Display_Version()
{
    UINT32 ver;
    UCHAR start = 32;
    UCHAR tmp;
    UCHAR i;

    //Display HW Ver
    UG2864_Display((UCHAR*)gHorHw, 0x01, 0x01, start, 18, GUI_DIR_HORIZON_POS);
    start += 18;

    ver = GetHardwareVer();
    for (i = 0; i < 2; ++i)
    {
        tmp = ver >> ((2 - i) * 8);
        UG2864_Display((UCHAR*)(gAlarmDigit + (tmp / 10) * 6), 0x01, 0x01, start, 6,
                       GUI_DIR_HORIZON_POS);
        start += 6;
        UG2864_Display((UCHAR*)(gAlarmDigit + (tmp % 10) * 6), 0x01, 0x01, start, 6,
                       GUI_DIR_HORIZON_POS);
        start += 6;
        UG2864_Display((UCHAR*)(gHorParamPI + 198), 0x01, 0x01, start, 8,
                       GUI_DIR_HORIZON_POS);
        start += 8;
    }

    tmp = ver;
    UG2864_Display((UCHAR*)(gAlarmDigit + (tmp / 10) * 6), 0x01, 0x01, start, 6,
                   GUI_DIR_HORIZON_POS);
    start += 6;
    UG2864_Display((UCHAR*)(gAlarmDigit + (tmp % 10) * 6), 0x01, 0x01, start, 6,
                   GUI_DIR_HORIZON_POS);


    //Display SW Ver
    start = 32;
    UG2864_Display((UCHAR*)gHorSw, 0x00, 0x00, start, 18, GUI_DIR_HORIZON_POS);
    start += 18;

    ver = GetSoftwareVer();
    for (i = 0; i < 2; ++i)
    {
        tmp = ver >> ((2 - i) * 8);
        UG2864_Display((UCHAR*)(gAlarmDigit + (tmp / 10) * 6), 0x00, 0x00, start, 6,
                       GUI_DIR_HORIZON_POS);
        start += 6;
        UG2864_Display((UCHAR*)(gAlarmDigit + (tmp % 10) * 6), 0x00, 0x00, start, 6,
                       GUI_DIR_HORIZON_POS);
        start += 6;
        UG2864_Display((UCHAR*)(gHorParamPI + 198), 0x00, 0x00, start, 8,
                       GUI_DIR_HORIZON_POS);
        start += 8;
    }

    tmp = ver;
    UG2864_Display((UCHAR*)(gAlarmDigit + (tmp / 10) * 6), 0x00, 0x00, start, 6,
                   GUI_DIR_HORIZON_POS);
    start += 6;
    UG2864_Display((UCHAR*)(gAlarmDigit + (tmp % 10) * 6), 0x00, 0x00, start, 6,
                   GUI_DIR_HORIZON_POS);
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Logo(LOGO_TYPE_E type, GUI_DIR_E dir)           *
 * [FUNC.spec. ] Display LOGO.                                                *
 * [FUNC.input ]                                                              *
 *               LOGO_TYPE_E   type     Logo types                            *
 *               GUI_DIR_E       dir         Direction type                       *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
static void Display_Logo(LOGO_TYPE_E type, GUI_DIR_E dir)
{
    UCHAR *pData = NULL;

    //Display LOGO
    switch (type)
    {
        case LOGO_FIRMS:
        {
            //Display caption
            pData = (UCHAR*)gLogoNarigmed;

            if (GUI_DIR_HORIZON_POS == dir || GUI_DIR_VERTICAL_POS == dir)
            {
                //Horizontal direction
                UG2864_Display(pData, 0x02, 0x05, 9, 109, GUI_DIR_HORIZON_POS);
            }
            else
            {
                //other direction
                UG2864_Display(pData, 0x02, 0x05, 9, 109, GUI_DIR_HORIZON_NEG);
            }
        }
        break;

        default:
            break;
    }
}



/*
********************************************************************************
 * [FUNC.name  ] void Display_Battery(UCHAR mVolt, GUI_DIR_E dir)             *
 * [FUNC.spec. ] Display battery.                                             *
 * [FUNC.input ]                                                              *
 *               UCHAR  mVolt     Voltage value                               *
 *               GUI_DIR_E dir   Direction type                              *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*

********************************************************************************
 */
static void Display_Battery(UCHAR mVolt, GUI_DIR_E dir)
{
    UCHAR i = 0;
    UCHAR *pData = (UCHAR*)gAllBattery;
    UCHAR pageL = 0;
    UCHAR pageR = 0;
    static UCHAR sBlink = FALSE;

    //blink
    if (3 > mVolt)
    {
        sBlink = !sBlink;
    }
    else
    {
        sBlink = FALSE;
    }

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 6;
                pageR = 7;
            }
            else
            {
                pageL = 0;
                pageR = 1;
            }

            //Blink
            if (TRUE == sBlink)
            {
                UG2864_ClearRegion(pageL, pageR, 106, 22, dir);
                return;
            }

            //Battery bottom
            UG2864_Display(pData, pageL, pageR, 106, 2, dir);
            //Battery top
            UG2864_Display(pData + 4, pageL, pageR, 124, 2, dir);
            //Battery head
            UG2864_Display(pData + 8, pageL, pageR, 127, 1, dir);
            //Solid battery voltage
            for (i = 0; i < mVolt; i++)
            {
                UG2864_Display(pData + 10, pageL, pageR, 108 + i, 1, dir);
            }
            //Hollow battery voltage
            for (i = mVolt; i < GUI_BATTERY_LEN; i++)
            {
                UG2864_Display(pData + 12, pageL, pageR, 108 + i, 1, dir);
            }
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 6;
                pageR = 7;
            }
            else
            {
                pageL = 0;
                pageR = 1;
            }
            //Blink
            if (TRUE == sBlink)
            {
                UG2864_ClearRegion(pageL, pageR, 0, 22, dir);
                return;
            }
            //Battery bottom
            UG2864_Display(pData, pageL, pageR, 2, 2, dir);
            //Battery top
            UG2864_Display(pData + 4, pageL, pageR, 20, 2, dir);
            //Battery head
            UG2864_Display(pData + 8, pageL, pageR, 0, 1, dir);
            //Solid battery voltage
            for (i = 0; i < mVolt; i++)
            {
                UG2864_Display(pData + 10, pageL, pageR, 19 - i, 1, dir);
            }
            //Hollow battery voltage
            for (i = mVolt; i < GUI_BATTERY_LEN; i++)
            {
                UG2864_Display(pData + 12, pageL, pageR, 19 - i, 1, dir);
            }
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Ble(UCHAR mVolt, GUI_DIR_E dir)                 *
 * [FUNC.spec. ] Display Ble logo.                                            *
 * [FUNC.input ]                                                              *
 *               GUI_DIR_E dir   Direction type                              *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*

********************************************************************************
 */
static void Display_Ble(GUI_DIR_E dir)
{
    UCHAR *pData = (UCHAR*)gHorBluetoothLogo;
    UCHAR pageL = 0;
    UCHAR pageR = 0;
    UCHAR startx = 106;

    switch (dir)
    {
        case GUI_DIR_HORIZON_NEG:
        {
            pageL = 0;
            pageR = 1;
        }
        break;

        case GUI_DIR_HORIZON_POS:
        {
            pageL = 6;
            pageR = 7;
        }
        break;

        case GUI_DIR_VERTICAL_NEG:
        {
            pageL = 0;
            pageR = 1;
            startx = 0;
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        {
            pageL = 6;
            pageR = 7;
            startx = 0;
        }
        break;

        default:
            break;
    }

    UG2864_Display(pData, pageL, pageR, startx, 22, dir);
}
/*
********************************************************************************
 * [FUNC.name  ] void Display_Caption(GUI_DIR_E dir)                          *
 * [FUNC.spec. ] Display caption.                                             *
 * [FUNC.input ]                                                              *
 *               GUI_DIR_E dir   Direction type                              *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
static void Display_Caption(GUI_DIR_E dir)
{
    UCHAR *pData = NULL;
    UCHAR pageL = 0;
    UCHAR pageR = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 4;
                pageR = 5;
            }
            else
            {
                pageL = 6;
                pageR = 7;
            }
            //SpO2%
            pData = (UCHAR*)gHorCaptionSat;
            UG2864_Display(pData, pageL, pageR, 10, 35, dir);
            //PR/min
            pData = (UCHAR*)gHorCaptionPR;
            UG2864_Display(pData, pageL, pageR, 89, 39, dir);
            //PI%
            pData = (UCHAR*)gHorCaptionPI;
            UG2864_Display(pData, pageL, pageL, 60, 21, dir);
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 0;
                pageR = 4;
            }
            else
            {
                pageL = 2;
                pageR = 6;
            }
            //SpO2%
            pData = (UCHAR*)gVerCaptionSat;
            UG2864_Display(pData, pageL, pageR, 0, 12, dir);
            //PR/min
            pData = (UCHAR*)gVerCaptionPR;
            UG2864_Display(pData, pageL, pageR, 54, 9, dir);
            //PI%
            pData = (UCHAR*)gVerCaptionPI;
            UG2864_Display(pData, pageL, pageR - 2, 100, 7, dir);
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Perfusion(INT16 mPI, GUI_DIR_E dir)             *
 * [FUNC.spec. ] Display perfusion index.                                     *
 * [FUNC.input ]                                                              *
 *               INT16  mPI       Perfusion index                             *
 *               GUI_DIR_E dir   Direction type                              *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
static void Display_Perfusion(INT16 mPI, GUI_DIR_E dir)
{
    UCHAR i = 0;
    UCHAR tmpCal = 0;
    UCHAR *pData = NULL;
    UCHAR *pTmpAdr = NULL;
    UCHAR bitShift1[13] = {0};
    UCHAR bitShift2[13] = {0};
    UCHAR pageL = 0;
    UCHAR pageR = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 1;
                pageR = 2;
            }
            else
            {
                pageL = 3;
                pageR = 4;
            }
            //Address
            pData = (UCHAR*)gHorParamPI;
            if (0 == mPI)
            {
                //"-"(Index_10*18)
                UG2864_Display(pData + 180, pageL, pageR, 61, 9, dir);
                //"-"(Index_10*18)
                UG2864_Display(pData + 180, pageL, pageR, 70, 9, dir);
                //"-"(Index_10*18)
                UG2864_Display(pData + 180, pageL, pageR, 79, 9, dir);
                //Clear
                UG2864_ClearRegion(pageL, pageR, 57, 4, dir);
            }
            else
            {
                tmpCal = mPI / 10000;
                if (0 != tmpCal)
                {
                    //Byte_1[0-9](value * 18)
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 57, 9, dir);
                    //Byte_2[0-9](value * 18)
                    tmpCal = (mPI % 10000) / 1000;
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 66, 9, dir);
                    //Dot(12 * 18)
                    UG2864_Display(pData + 198, pageL, pageR, 75, 4, dir);
                    //Byte_3[0-9](value * 18)
                    tmpCal = (mPI % 1000) / 100;
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 79, 9, dir);
                }
                else
                {
                    //Byte_1[0-9](value * 18)
                    tmpCal = (mPI % 10000) / 1000;
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 57, 9, dir);
                    //Dot(12 * 18)
                    UG2864_Display(pData + 198, pageL, pageR, 66, 4, dir);
                    //Byte_2[0-9](value * 18)
                    tmpCal = (mPI % 1000) / 100;
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 70, 9, dir);
                    //Byte_3[0-9](value * 18)
                    tmpCal = (mPI % 100) / 10;
                    UG2864_Display(pData + tmpCal * 18, pageL, pageR, 79, 9, dir);
                }
            }
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 2;
                pageR = 5;
            }
            else
            {
                pageL = 4;
                pageR = 7;
            }
            //Address
            pData = (UCHAR*)gVerParamPI;
            if (0 == mPI)
            {
                //"-"(Index_10*13)
                UG2864_Display(pData + 130, pageL, pageL, 115, 13, dir);
                //"-"(Index_10*13)
                UG2864_Display(pData + 130, pageL + 1, pageL + 1, 115, 13, dir);
                //"-"(Index_10*13)
                UG2864_Display(pData + 130, pageR - 1, pageR - 1, 115, 13, dir);
                //Clear
                UG2864_ClearRegion(pageR, pageR, 115, 13, dir);
            }
            else
            {
                tmpCal = mPI / 10000;
                if (0 != tmpCal)
                {
                    //Byte_1[0-9](value * 13)
                    UG2864_Display(pData + tmpCal * 13, pageL, pageL, 115, 13, dir);

                    //Shift bit for margin bewteen two bytes
                    tmpCal = (mPI % 10000) / 1000;
                    pTmpAdr = pData + tmpCal * 13;
                    for (i = 0; i < 13; i++)
                    {
                        bitShift1[i] = (*pTmpAdr) << 2;
                        bitShift2[i] = ((*pTmpAdr) & 0xC0) >> 6;
                        pTmpAdr++;
                    }
                    //Byte_2[0-9](value * 13)
                    UG2864_Display(bitShift1, pageL + 1, pageL + 1, 115, 13, dir);

                    //Shift bit for margin bewteen two bytes
                    pTmpAdr = pData + 143;
                    for (i = 0; i < 13; i++)
                    {
                        bitShift1[i] = ((*pTmpAdr) << 3) | bitShift2[i];
                        pTmpAdr++;
                    }
                    //Dot(12 * 13)
                    UG2864_Display(bitShift1, pageR - 1, pageR - 1, 115, 13, dir);

                    //Byte_3[0-9](value * 13)
                    tmpCal = (mPI % 1000) / 100;
                    UG2864_Display(pData + tmpCal * 13, pageR, pageR, 115, 13, dir);
                }
                else
                {
                    //Byte_1[0-9](value * 13)
                    tmpCal = (mPI % 10000) / 1000;
                    UG2864_Display(pData + tmpCal * 13, pageL, pageL, 115, 13, dir);

                    //Shift bit for margin bewteen two bytes
                    pTmpAdr = pData + 143;
                    for (i = 0; i < 13; i++)
                    {
                        bitShift1[i] = (*pTmpAdr++) << 1;
                    }
                    pTmpAdr = pData + ((mPI % 1000) / 100) * 13;
                    for (i = 0; i < 13; i++)
                    {
                        bitShift1[i] |= ((*pTmpAdr) & 0x07) << 5;
                        bitShift2[i]  = (*pTmpAdr) >> 3;
                        pTmpAdr++;
                    }
                    //Dot(12 * 13) + part Byte_2
                    UG2864_Display(bitShift1, pageL + 1, pageL + 1, 115, 13, dir);

                    //Shift bit for margin bewteen two bytes
                    pTmpAdr = pData + ((mPI % 100) / 10) * 13;
                    for (i = 0; i < 13; i++)
                    {
                        bitShift1[i] = bitShift2[i] | (((*pTmpAdr) & 0x01) << 7);
                        bitShift2[i] = (*pTmpAdr) >> 1;
                        pTmpAdr++;
                    }
                    //Byte_2[0-9](value * 13) + part Byte_3
                    UG2864_Display(bitShift1, pageR - 1, pageR - 1, 115, 13, dir);

                    //Byte_3[0-9](value * 13)
                    UG2864_Display(bitShift2, pageR, pageR, 115, 13, dir);
                }
            }
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Pulse(INT16 valPulse, GUI_DIR_E dir)            *
 * [FUNC.spec. ] Display pulse rate.                                          *
 * [FUNC.input ]                                                              *
 *               INT16   valPulse  Pulse rate                                 *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_Pulse(INT16 valPulse, GUI_DIR_E dir)
{
    UCHAR *pData = NULL;
    UCHAR pageL = 0;
    UCHAR pageR = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 0;
                pageR = 2;
            }
            else
            {
                pageL = 2;
                pageR = 4;
            }
            //Blink
            if (2 == gAlmCtrl.blkBPM)
            {
                UG2864_ClearRegion(pageL, pageR, 89, 39, dir);
                break;
            }
            //Obtain address
            pData = (UCHAR*)gHorPluse;
            if (0 == valPulse)
            {
                //"-"(Index_10*39)
                UG2864_Display(pData + 390, pageL, pageR, 102, 13, dir);
                //"-"(Index_10*39)
                UG2864_Display(pData + 390, pageL, pageR, 115, 13, dir);
                //Null
                UG2864_ClearRegion(pageL, pageR, 89, 13, dir);
            }
            else
            {
                //Byte_2[0-9](value * 39)
                UG2864_Display(pData + (((valPulse % 100) / 10) * 39), pageL, pageR, 102, 13,
                               dir);
                //Byte_3[0-9](value * 39)
                UG2864_Display(pData + (((valPulse % 100) % 10) * 39), pageL, pageR, 115, 13,
                               dir);
                //Byte_1[0-3](value * 39)
                if (100 <= valPulse)
                {
                    UG2864_Display(pData + ((valPulse / 100) * 39), pageL, pageR, 89, 13, dir);
                }
                else
                {
                    UG2864_ClearRegion(pageL, pageR, 89, 13, dir);
                }
            }
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 0;
                pageR = 5;
            }
            else
            {
                pageL = 2;
                pageR = 7;
            }
            //Blink
            if (2 == gAlmCtrl.blkBPM)
            {
                UG2864_ClearRegion(pageL, pageR, 74, 18, dir);
                break;
            }
            //Obtain address
            pData = (UCHAR*)gVerPluse;
            if (0 == valPulse)
            {
                //"-"(Index_10*36)
                UG2864_Display(pData + 360, pageL + 2, pageR - 2, 74, 18, dir);
                //"-"(Index_10*36)
                UG2864_Display(pData + 360, pageR - 1, pageR, 74, 18, dir);
                //Null(Index_11*36)
                UG2864_ClearRegion(pageL, pageL + 1, 74, 18, dir);
            }
            else
            {
                //Byte_2[0-9](value * 36)
                UG2864_Display(pData + (((valPulse % 100) / 10) * 36), pageL + 2, pageR - 2, 74,
                               18, dir);
                //Byte_3[0-9](value * 36)
                UG2864_Display(pData + (((valPulse % 100) % 10) * 36), pageR - 1, pageR, 74, 18,
                               dir);
                //Byte_1[0-3](value * 36)
                if (100 <= valPulse)
                {
                    UG2864_Display(pData + ((valPulse / 100) * 36), pageL, pageL + 1, 74, 18, dir);
                }
                else
                {
                    UG2864_ClearRegion(pageL, pageL + 1, 74, 18, dir);
                }
            }
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_SpO2(CHAR valSpo2, GUI_DIR_E dir)               *
 * [FUNC.spec. ] Display saturation.                                          *
 * [FUNC.input ]                                                              *
 *               CHAR    valSpo2   saturation                                 *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_SpO2(CHAR valSpo2, GUI_DIR_E dir)
{
    UCHAR *pData = NULL;
    UCHAR pageL = 0;
    UCHAR pageR = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 0;
                pageR = 3;
            }
            else
            {
                pageL = 2;
                pageR = 5;
            }
            //Blink
            if (2 == gAlmCtrl.blkSAT)
            {
                UG2864_ClearRegion(pageL, pageR, 12, 44, dir);
                break;
            }
            //Obtain address
            pData = (UCHAR*)gHorSpO2;
            if (0 == valSpo2)
            {
                //"-"(Index_10*64)
                UG2864_Display(pData + 640, pageL, pageR, 12, 16, dir);
                //"-"(Index_10*64)
                UG2864_Display(pData + 640, pageL, pageR, 28, 16, dir);
                //Null
                UG2864_ClearRegion(pageL, pageR, 44, 12, dir);
            }
            else
            {
                if (100 > valSpo2)
                {
                    //Byte_2[0-9](value * 64)
                    UG2864_Display(pData + (((valSpo2 % 100) / 10) << 6), pageL, pageR, 12, 16,
                                   dir);
                    //Byte_3[0-9](value * 64)
                    UG2864_Display(pData + (((valSpo2 % 100) % 10) << 6), pageL, pageR, 28, 16,
                                   dir);
                    UG2864_ClearRegion(pageL, pageR, 44, 12, dir);
                }
                else
                {
                    UG2864_Display(pData + 704, pageL, pageR, 12, 12, dir);
                    //Byte_2[0-9](value * 64)
                    UG2864_Display(pData + (((valSpo2 % 100) / 10) << 6), pageL, pageR, 24, 16,
                                   dir);
                    //Byte_3[0-9](value * 64)
                    UG2864_Display(pData + (((valSpo2 % 100) % 10) << 6), pageL, pageR, 40, 16,
                                   dir);
                }
            }
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 0;
                pageR = 5;
            }
            else
            {
                pageL = 2;
                pageR = 7;
            }
            //Blink
            if (2 == gAlmCtrl.blkSAT)
            {
                UG2864_ClearRegion(pageL, pageR, 22, 22, dir);
                break;
            }
            //Obtain address
            pData = (UCHAR*)gVerSpO2;
            if (0 == valSpo2)
            {
                //"-"(Index_10*44)
                UG2864_Display(pData + 440, pageL + 2, pageR - 2, 22, 22, dir);
                //"-"(Index_10*44)
                UG2864_Display(pData + 440, pageR - 1, pageR, 22, 22, dir);
                //Null
                UG2864_ClearRegion(pageL, pageL + 1, 22, 22, dir);
            }
            else
            {
                //Byte_2[0-9](value * 44)
                UG2864_Display(pData + (((valSpo2 % 100) / 10) * 44), pageL + 2, pageR - 2, 22,
                               22, dir);
                //Byte_3[0-9](value * 44)
                UG2864_Display(pData + (((valSpo2 % 100) % 10) * 44), pageR - 1, pageR, 22, 22,
                               dir);
                //Byte_1[0-1](value * 44)
                if (0 == valSpo2 / 100)
                {
                    UG2864_ClearRegion(pageL, pageL + 1, 22, 22, dir);
                }
                else
                {
                    UG2864_Display(pData  + 44, pageL, pageL + 1, 22, 22, dir);
                }
            }
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_Bargraph(... ...)                                   *
 * [FUNC.spec. ] Display plethysmograph.                                      *
 * [FUNC.input ]                                                              *
 *               CHAR      valBar    Bargraph values                          *
 *               GUI_DIR_E    dir   Direction type                           *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_Bargraph(CHAR valBar, GUI_DIR_E dir)
{
    UCHAR i = 0;
    UCHAR j = 0;
    UCHAR mapBar = 0;
    UCHAR *pData = NULL;
    UCHAR pageL = 0;
    UCHAR pageR = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_HORIZON_NEG:
        {
            if (GUI_DIR_HORIZON_POS == dir)
            {
                pageL = 0;
                pageR = 3;
            }
            else
            {
                pageL = 2;
                pageR = 5;
            }
            //Transfer bargraph
            mapBar = (valBar + 1) >> 1;
            //Select library
            switch (mapBar)
            {
                case 0:
                case 1:      //(Page_0 Column_6)
                {
                    pData = (UCHAR*)gHorBarGrid_15;
                }
                break;

                case 2:      //(Page_0 Column_6)
                {
                    pData = (UCHAR*)gHorBarGrid_16;
                }
                break;

                case 3:      //(Page_1 Column_8)
                {
                    pData = (UCHAR*)gHorBarGrid_27;
                }
                break;

                case 4:      //(Page_1 Column_8)
                {
                    pData = (UCHAR*)gHorBarGrid_28;
                }
                break;

                case 5:      //(Page_2 Column_10)
                {
                    pData = (UCHAR*)gHorBarGrid_39;
                }
                break;

                case 6:      //(Page_2 Column_10)
                {
                    pData = (UCHAR*)gHorBarGrid_3A;
                }
                break;

                case 7:      //(Page_3 Column_12)
                {
                    pData = (UCHAR*)gHorBarGrid_4B;
                }
                break;

                case 8:      //(Page_3 Column_12)
                {
                    pData = (UCHAR*)gHorBarGrid_4C;
                }
                break;

                default:
                    break;
            }
            //Display bargraph(Automatic page and column)
            if (0 == mapBar)
            {
                i = 0;
            }
            else
            {
                i = (mapBar - 1) >> 1;
            }
            j = (i << 1) + 6;
            UG2864_Display(pData, pageL, pageL + i, 0, j, dir);
            //Clear display range
            UG2864_ClearRegion(pageL + i + 1, pageR, 0, 12, dir);
        }
        break;

        case GUI_DIR_VERTICAL_POS:
        case GUI_DIR_VERTICAL_NEG:
        {
            if (GUI_DIR_VERTICAL_POS == dir)
            {
                pageL = 6;
                pageR = 7;
            }
            else
            {
                pageL = 0;
                pageR = 1;
            }
            //Address
            pData = (UCHAR*)gVerBarGrid;

            //Transfer bargraph
            mapBar = (valBar + 1) >> 1;
            j = mapBar << 2;
            i = 32 - j;

            //Display bargraph
            UG2864_Display(pData + i, pageL, pageL, 96 + i, j, dir);
            UG2864_Display(pData + i + 32, pageR, pageR, 96 + i, j, dir);

            //Clear display range
            UG2864_ClearRegion(pageL, pageR, 96, i, dir);
        }
        break;

        default:
            break;
    }
}



/*
********************************************************************************
 * [FUNC.name  ] void Display_Pleth(... ...)                                   *
 * [FUNC.spec. ] Display plethysmograph.                                      *
 * [FUNC.input ]                                                              *
 *               CHAR      valPleth  Plethysmograph values                    *
 *               GUI_DIR_E    dir   Direction type                           *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_Pleth(CHAR valPleth, GUI_DIR_E dir)
{
    UCHAR mStart = 0;
    UCHAR mStop  = 0;
    UCHAR mIndex = 0;
    UINT16 mGuiPleth = 0;
    UCHAR pageL = 0;
    UCHAR pageR = 0;
    UINT16 AucBuf[16] = {0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF,
                         0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
                        };
    static UCHAR sPreIndex  = 0;
    static UCHAR sUpdatePos = 0;

    //Direction changed, pleth position reset
    if (TRUE == gGuiCtrl.bReset)
    {
        sPreIndex  = 0;
        sUpdatePos = 0;
        gGuiCtrl.bReset = FALSE;
    }

    //Display plethymogram under horizontal direction
    if (GUI_DIR_HORIZON_POS != dir && GUI_DIR_HORIZON_NEG != dir)
    {
        return;
    }

    if (GUI_DIR_HORIZON_POS == dir)
    {
        pageL = 6;
        pageR = 7;
    }
    else
    {
        pageL = 0;
        pageR = 1;
    }

    //Refresh cursor
    if (GUI_PLETH_HORLEN - GUI_CURSOR_LEN <= sUpdatePos)
    {
        mIndex = GUI_PLETH_HORLEN - sUpdatePos;
        UG2864_ClearRegion(pageL, pageR, sUpdatePos, mIndex, dir);
        mIndex = GUI_CURSOR_LEN - mIndex;
        UG2864_ClearRegion(pageL, pageR, 0, mIndex, dir);
    }
    else
    {
        UG2864_ClearRegion(pageL, pageR, sUpdatePos, GUI_CURSOR_LEN, dir);
    }

    //Transfer to GUI pleth(GUI = 16 * (Pleth + 50) / 100)
    mIndex = ((valPleth + 50) << 2) / 25;

    //Single dot depict
    if (GUI_DIR_HORIZON_NEG == dir)
    {
        mGuiPleth = 1 << mIndex;

        //2Pixel for display
        if (0 < mIndex)
        {
            mGuiPleth |= 1 << (mIndex - 1);
        }

        //Calculate gap span
        if (sPreIndex < mIndex)
        {
            mStart = sPreIndex;
            mStop  = mIndex;
        }
        else if (mIndex < sPreIndex)
        {
            mStart = mIndex;
            mStop  = sPreIndex;
        }

        //Expand display span
        while (0 != sUpdatePos && mStart != mStop)
        {
            mGuiPleth |= 1 << (mStart++);
        }

        //Record Index position
        sPreIndex = mIndex;
    }
    //Area udner curve depict
    else
    {
        mGuiPleth = AucBuf[mIndex];
    }

    //Display plethysmograph
    UG2864_Display((UCHAR*)(&mGuiPleth), pageL, pageR, sUpdatePos, 1, dir);

    //Display span update
    sUpdatePos++;
    if (GUI_PLETH_HORLEN <= sUpdatePos)
    {
        sUpdatePos = 0;
    }
}


/*
********************************************************************************
 * [FUNC.name  ] void Display_AlarmThrd(UCHAR IconType, GUI_DIR_E dir)            *
 * [FUNC.spec. ] Setup specified alarm threshold.                             *
 * [FUNC.input ]                                                              *
 *               UCHAR   IconType  Icon type                                  *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_AlarmThrd(UCHAR IconType, GUI_DIR_E dir)
{
    CHAR i = 0;
    UCHAR *pByte0 = NULL;
    UCHAR *pByte1 = NULL;
    UCHAR *pByte2 = NULL;
    UCHAR *pData = (UCHAR*)gAlarmDigit;
    static UCHAR sDelay = 0;

    //Only support horizontal direction
    dir = GUI_DIR_HORIZON_POS;

    //Adjust threshold
    switch (IconType)
    {
        //Spo2 low threshold
        case MENU_ALM_LO_SAT:
        {
            gAlmCtrl.lowSAT -= BIAS_SAT;
            if (65 > gAlmCtrl.lowSAT)
            {
                gAlmCtrl.lowSAT = 95;
            }
        }
        break;

        //Pulse low threshold
        case MENU_ALM_LO_BPM:
        {
            gAlmCtrl.lowBPM -= BIAS_BPM;
            if (30 > gAlmCtrl.lowBPM)
            {
                if (150 > gAlmCtrl.upBPM)
                {
                    gAlmCtrl.lowBPM = gAlmCtrl.upBPM - BIAS_BPM;
                }
                else
                {
                    gAlmCtrl.lowBPM = 150;
                }
            }
        }
        break;

        //Pulse high threshold
        case MENU_ALM_HI_BPM:
        {
            gAlmCtrl.upBPM += BIAS_BPM;
            if (250 < gAlmCtrl.upBPM)
            {
                if (100 < gAlmCtrl.lowBPM)
                {
                    gAlmCtrl.upBPM = gAlmCtrl.lowBPM + BIAS_BPM;
                }
                else
                {
                    gAlmCtrl.upBPM = 100;
                }
            }
        }
        break;

        //Beep
        case MENU_ALM_BEEP:
        {
            sDelay++;
            if (2 == sDelay)
            {
                sDelay = 0;
                gAlmCtrl.swBeep = !gAlmCtrl.swBeep;
            }
        }
        break;

        //Restore default configure
        case MENU_ALM_RESTORE:
        {
            gAlmCtrl.lowSAT = 90;
            gAlmCtrl.upBPM  = 120;
            gAlmCtrl.lowBPM = 45;
            gAlmCtrl.swBeep = FALSE;
        }
        break;

        //Exit menu setup
        case MENU_ALM_EXIT:
        {
            gGuiCtrl.guiType = GUI_SHOW_PARAM;
            gGuiCtrl.subType = PARAM_SEQ_RESET;
            gGuiCtrl.bReset  = TRUE;
            //Clear key end state
            gKeyCtrl.bErrFlg = TRUE;
        }
        return;

        default:
            break;
    }

    //SPO2 ALM LO
    if (MENU_ALM_LO_SAT == IconType || MENU_ALM_RESTORE == IconType ||
        MENU_ALM_RESET == IconType)
    {
        pByte0 = (UCHAR*)(pData + (gAlmCtrl.lowSAT / 10) * 6);
        pByte1 = (UCHAR*)(pData + (gAlmCtrl.lowSAT % 10) * 6);
        //Line_4([5*8]40*24)
        for (i = 0; i < 6; i++)
        {
            gMenuBuf[24 * 4 + 10 + i] = pByte0[i];
            gMenuBuf[24 * 4 + 18 + i] = pByte1[i];
        }
    }

    //PR ALM LO
    if (MENU_ALM_LO_BPM == IconType || MENU_ALM_RESTORE == IconType ||
        MENU_ALM_RESET == IconType)
    {
        if (100 <= gAlmCtrl.lowBPM)
        {
            pByte0 = (UCHAR*)(pData + 60 + (gAlmCtrl.lowBPM / 100) * 12);
        }
        else
        {
            pByte0 = NULL;
        }
        pByte1 = (UCHAR*)(pData + 60 + ((gAlmCtrl.lowBPM % 100) / 10) * 12);
        pByte2 = (UCHAR*)(pData + 60 + (gAlmCtrl.lowBPM % 10) * 12);
        //Clear display region
        for (i = 0; i < 24; i++)
        {
            gMenuBuf[24 * 3 + i] &= 0x00;
            gMenuBuf[24 * 2 + i] &= 0x0F;
        }
        //Line_2/3([5*8]40*24)
        for (i = 0; i < 6; i++)
        {
            if (NULL != pByte0)
            {
                gMenuBuf[24 * 3 + 2 + i] |= pByte0[i + 6];
                gMenuBuf[24 * 2 + 2 + i] |= pByte0[i];
            }
            gMenuBuf[24 * 3 + 10 + i] |= pByte1[i + 6];
            gMenuBuf[24 * 2 + 10 + i] |= pByte1[i];
            gMenuBuf[24 * 3 + 18 + i] |= pByte2[i + 6];
            gMenuBuf[24 * 2 + 18 + i] |= pByte2[i];
        }
    }

    //PR ALM HI
    if (MENU_ALM_HI_BPM == IconType || MENU_ALM_RESTORE == IconType ||
        MENU_ALM_RESET == IconType)
    {
        if (100 <= gAlmCtrl.upBPM)
        {
            pByte0 = (UCHAR*)(pData + 180 + (gAlmCtrl.upBPM / 100) * 12);
        }
        else
        {
            pByte0 = NULL;
        }
        pByte1 = (UCHAR*)(pData + 180 + ((gAlmCtrl.upBPM % 100) / 10) * 12);
        pByte2 = (UCHAR*)(pData + 180 + (gAlmCtrl.upBPM % 10) * 12);
        //Clear
        for (i = 0; i < 24; i++)
        {
            gMenuBuf[24 * 2 + i] &= 0xF0;
            gMenuBuf[24 * 1 + i] &= 0x0F;
        }
        //Line_1/2([5*8]40*24)
        for (i = 0; i < 6; i++)
        {
            if (NULL != pByte0)
            {
                gMenuBuf[24 * 2 + 2 + i] |= pByte0[i + 6];
                gMenuBuf[24 * 1 + 2 + i] |= pByte0[i];
            }
            gMenuBuf[24 * 2 + 10 + i] |= pByte1[i + 6];
            gMenuBuf[24 * 1 + 10 + i] |= pByte1[i];
            gMenuBuf[24 * 2 + 18 + i] |= pByte2[i + 6];
            gMenuBuf[24 * 1 + 18 + i] |= pByte2[i];
        }
    }

    //Beep
    if (MENU_ALM_BEEP == IconType || MENU_ALM_RESTORE == IconType ||
        MENU_ALM_RESET == IconType)
    {
        if (TRUE == gAlmCtrl.swBeep)
        {
            pByte0 = (UCHAR*)(pData + 300);
        }
        else
        {
            pByte0 = (UCHAR*)(pData + 334);
        }
        //Clear
        for (i = 0; i < 24; i++)
        {
            gMenuBuf[24 + i] &= 0xF0;
            gMenuBuf[i]      &= 0x00;
        }
        //Line_0/1([5*8]40*24)
        for (i = 0; i < 17; i++)
        {
            gMenuBuf[24 + 7 + i] |= pByte0[i + 17];
            gMenuBuf[7 + i] |= pByte0[i];
        }
    }

    //Display setup value
    UG2864_Display(gMenuBuf, 1, 5, 104, 24, dir);
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_BlinkSelection(UCHAR IconType, GUI_DIR_E dir)          *
 * [FUNC.spec. ] Select alarm item.                                           *
 * [FUNC.input ]                                                              *
 *               UCHAR   IconType  Icon type                                  *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_BlinkSelection(UCHAR IconType, GUI_DIR_E dir)
{
    UCHAR i = 0;
    UCHAR *pData = (UCHAR*)gAlarmFont;
    UCHAR tmpArray[120] = {0};
    static UCHAR sBlink = FALSE;

    //Only support horizontal direction
    dir = GUI_DIR_HORIZON_POS;

    //Copy data
    memcpy(tmpArray, gMenuBuf, 120);

    //Blink flag
    sBlink = !sBlink;

    switch (IconType)
    {
        //Spo2 low threshold
        case MENU_ALM_LO_SAT:
        {
            //Restore
            UG2864_Display(pData + 535, 0, 0, 101, 27, dir);
            //Clear
            if (TRUE == sBlink)
            {
                for (i = 0; i < 24; i++)
                {
                    tmpArray[24 * 4 + i] &= 0x00;
                }
            }
        }
        break;

        //Pulse low threshold
        case MENU_ALM_LO_BPM:
        {
            //Clear
            if (TRUE == sBlink)
            {
                for (i = 0; i < 24; i++)
                {
                    tmpArray[24 * 3 + i] &= 0x00;
                    tmpArray[24 * 2 + i] &= 0x0F;
                }
            }
        }
        break;

        //Pulse high threshold
        case MENU_ALM_HI_BPM:
        {
            //Clear
            if (TRUE == sBlink)
            {
                for (i = 0; i < 24; i++)
                {
                    tmpArray[24 * 2 + i] &= 0xF0;
                    tmpArray[24 * 1 + i] &= 0x0F;
                }
            }
        }
        break;

        //Beep
        case MENU_ALM_BEEP:
        {
            //Clear
            if (TRUE == sBlink)
            {
                for (i = 0; i < 24; i++)
                {
                    tmpArray[24 + i] &= 0xF0;
                    tmpArray[i]      &= 0x00;
                }
            }
        }
        break;

        //Restore default configure
        case MENU_ALM_RESTORE:
        {
            //Clear
            if (TRUE == sBlink)
            {
                UG2864_ClearRegion(0, 0, 0, 53, dir);
            }
            else
            {
                UG2864_Display(pData + 482, 0, 0, 0, 53, dir);
            }
        }
        break;

        //Exit menu setup
        case MENU_ALM_EXIT:
        {
            //Restore
            UG2864_Display(pData + 482, 0, 0, 0, 53, dir);
            //Clear
            if (TRUE == sBlink)
            {
                UG2864_ClearRegion(0, 0, 101, 27, dir);
            }
            else
            {
                UG2864_Display(pData + 535, 0, 0, 101, 27, dir);
            }
        }
        break;

        default:
            break;
    }

    //Display setup value
    if (MENU_ALM_RESTORE >= IconType)
    {
        UG2864_Display(tmpArray, 1, 5, 104, 24, dir);
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_AlarmMenu(GUI_DIR_E dir)                        *
 * [FUNC.spec. ] Display Alarm menu.                                          *
 * [FUNC.input ]                                                              *
 *               UCHAR   IconType  Icon type                                  *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_AlarmMenu(UCHAR IconType, GUI_DIR_E dir)
{
    UCHAR *pData = (UCHAR*)gAlarmFont;

    //Only support horizontal direction
    dir = GUI_DIR_HORIZON_POS;

    switch (IconType)
    {
        //Setting
        case 0:
        {
            UG2864_Display(pData, 6, 7, 34, 59, dir);
        }
        break;

        //Spo2 Alm LO / PR Alm LO / PR Alm Hi / BEEP Part1_2
        case 1:
        {
            UG2864_Display(pData + 118, 2, 5, 0, 32, dir);
            UG2864_Display(pData + 246, 2, 5, 32, 32, dir);
        }
        break;

        //Spo2 Alm LO / PR Alm LO / PR Alm Hi / BEEP Part3_4
        case 2:
        {
            UG2864_Display(pData + 374, 2, 5, 64, 19, dir);
            UG2864_Display(pData + 450, 1, 1, 0, 32, dir);
        }
        break;

        //Restore / EXIT
        case 3:
        {
            UG2864_Display(pData + 482, 0, 0, 0, 53, dir);
            UG2864_Display(pData + 535, 0, 0, 101, 27, dir);
        }
        break;

        //Update digital number
        case 4:
        {
            //Update digit
            Display_AlarmThrd(MENU_ALM_RESET, dir);
            //Enforce to return
            gGuiCtrl.subType = MENU_ALM_EXIT;
        }
        break;

        default:
            break;
    }
}

/*
********************************************************************************
 * [FUNC.name  ] void Display_ProbeOff(GUI_DIR_E dir)                         *
 * [FUNC.spec. ] Display probe off tips.                                      *
 * [FUNC.input ]                                                              *
 *               UCHAR   bBlink    Blink flag                                 *
 *               GUI_DIR_E  dir   Direction type                             *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void Display_ProbeOff(UCHAR bBlink, GUI_DIR_E dir)
{
    UCHAR  *pData = NULL;

    //Only support horizontal direction
    if (1 == dir % 2)
    {
        dir--;
    }

    if (TRUE == bBlink)
    {
        //Finger Out
        if (0 != gCellVolt)
        {
            pData = (UCHAR*)gProbeOff;
            UG2864_Display(pData, 3, 4, 22, 84, dir);
        }
        //Deplete battery
        else
        {
            pData = (UCHAR*)gDepletePWR;
            UG2864_Display(pData, 3, 4, 48, 31, dir);
        }
    }
    else
    {
        UG2864_ClearRegion(3, 4, 22, 84, dir);
    }
}

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
void Display_GUI(void)
{
    UCHAR mDivider = 0;
    static UCHAR sOffCnt = 0;
    static CHAR oldProbeOff = FALSE;
    static UCHAR showBleLogo = FALSE;
    static UCHAR oldKeyVal = FALSE;

    // lowest brightness mode
    if (gCfgCtrl.swLowBright)
    {
        // Probe Off->On
        if ((oldProbeOff > gSatParam.sysAlarm.ProbeOff) || (0 == gGuiCtrl.tmNoOprParam))
        {
            UG2864_Set_Contrast_Control(
                gBrightnessTab[gCfgCtrl.brightness]); // Reset display brightness
            gGuiCtrl.tmNoOprParam = 1;
            gCfgCtrl.swLowBright = FALSE;
        }

        oldProbeOff = gSatParam.sysAlarm.ProbeOff;
    }
    else if (GUI_SHOW_PARAM == gGuiCtrl.guiType)
    {
        if (MAX_KEEP_BRI_TM <= gGuiCtrl.tmNoOprParam)
        {
            UG2864_Set_Contrast_Control(gBrightnessTab[0]); //Set brightness to lowest
            gCfgCtrl.swLowBright = TRUE;
        }
        else
        {
            gGuiCtrl.tmNoOprParam++;
        }
    }

    if (GUI_SHOW_LOGO != gGuiCtrl.guiType &&
        oldKeyVal != ((GPIO_PIN_16_25->data >> 7) & 0x01)) //PC7
    {
        oldKeyVal = !oldKeyVal;

        //Key error precaution (Automatic power on or key press for power on)
        gKeyCtrl.bTrgKey = !gKeyCtrl.bTrgKey;

        if (FALSE == gKeyCtrl.bTrgKey && (GPIO_PIN_16_25->data >> 7) & 0x01)  //PC7
        {
            gKeyCtrl.bTrgKey = TRUE;
            gKeyCtrl.tmKeyOn = 0;
            gGuiCtrl.tmProbe = 0;
        }

        //Return under LOGO display model or error flag
        if (TRUE == gKeyCtrl.bErrFlg)
        {
            memset(&gKeyCtrl, 0, sizeof(gKeyCtrl));
            return;
        }

        //Switch to other function
        switch (gGuiCtrl.guiType)
        {
            //Parameter interface
            case GUI_SHOW_PARAM:
            {
                if (gCfgCtrl.swLowBright) // Reset Brightness
                {
                    gKeyCtrl.bErrFlg = TRUE;
                    break;
                }

                //Direction change
                if (KEY_SHORT_MIN <= gKeyCtrl.tmKeyOn && KEY_SHORT_MAX >= gKeyCtrl.tmKeyOn)
                {
                    gGuiCtrl.guiType = GUI_SHOW_PARAM;
                    gGuiCtrl.subType = PARAM_SEQ_RESET;
                    gGuiCtrl.bReset  = TRUE;

                    if (gCfgCtrl.swDemo)
                    {
                        gKeyCtrl.shortKeyCnt++;
                    }

                    if (gCfgCtrl.bigFont)
                    {
                        gGuiCtrl.mDirect = gDirMappingBigFont[gGuiCtrl.mDirect];
                    }
                    else
                    {
                        gGuiCtrl.mDirect = gDirMapping[gGuiCtrl.mDirect];
                    }

                    if (GUI_DIR_HORIZON_NEG == gGuiCtrl.mDirect)
                    {
                        gCfgCtrl.bigFont = !gCfgCtrl.bigFont;
                    }

                    //store the display direction
                    gCfgCtrl.dispDir = gGuiCtrl.mDirect;

                    Setup_UpdateDevice();
                }
                //Enter into alarm menu
                else if (KEY_SHORT_MAX < gKeyCtrl.tmKeyOn)
                {
                    if (0 == gCfgCtrl.swDemo || PARAMS_TO_INFO_CNT != gKeyCtrl.shortKeyCnt)
                    {
                        gGuiCtrl.guiType = GUI_SHOW_MENU;
                        gGuiCtrl.subType = MENU_ALM_RESET;
                        gGuiCtrl.bReset  = TRUE;
                        gGuiCtrl.optItem = MENU_ALM_LO_SAT;
                    }
                    else
                    {
                        gGuiCtrl.guiType = GUI_SHOW_FAC_INFO;
                    }

                    gKeyCtrl.shortKeyCnt = 0;
                }
            }
            break;

            //Alarm setup interface
            case GUI_SHOW_MENU:
            {
                //Short key press for option select
                if (KEY_SHORT_MIN <= gKeyCtrl.tmKeyOn && KEY_SHORT_MAX >= gKeyCtrl.tmKeyOn)
                {
                    gGuiCtrl.optItem++;
                    if (MENU_ALM_MARGIN <= gGuiCtrl.optItem)
                    {
                        gGuiCtrl.optItem = MENU_ALM_LO_SAT;
                    }
                    //Notice for ICON refresh
                    //Force to update sel icon position
                    gGuiCtrl.subType = 3;
                    gGuiCtrl.bReset = TRUE;
                }
                //Store device setup
                else if (KEY_SHORT_MAX < gKeyCtrl.tmKeyOn)
                {
                    Setup_UpdateDevice();
                }
            }
            break;

            //Factory Info interface
            case GUI_SHOW_FAC_INFO:
            {
                //Short key press return Params interface
                if (KEY_SHORT_MIN <= gKeyCtrl.tmKeyOn && KEY_SHORT_MAX >= gKeyCtrl.tmKeyOn)
                {
                    gGuiCtrl.guiType = GUI_SHOW_PARAM;
                    gGuiCtrl.subType = PARAM_SEQ_RESET;
                    gGuiCtrl.bReset  = TRUE;
                }
            }
            break;

            default:
                break;
        }

        //Reset counter
        gKeyCtrl.tmKeyOn = 0;
        gGuiCtrl.tmProbe = 0;
        gGuiCtrl.tmNoOprMenu = 0;
        gGuiCtrl.tmNoOprParam = 0;
    }

    //Key hold time statistic(50Hz)
    if (TRUE == gKeyCtrl.bTrgKey)
    {
        gKeyCtrl.tmKeyOn++;
    }

    //Automatic power off(parameter interface)
    if (GUI_SHOW_LOGO != gGuiCtrl.guiType)
    {
        if (TRUE == gSatParam.sysAlarm.ProbeOff || 0 == gCellVolt)
        {
            gGuiCtrl.tmProbe++;
            //Tips
            if (GUI_SHOW_PROBEOFF != gGuiCtrl.guiType
                && ((KEY_AUTO_OFF1 <= gGuiCtrl.tmProbe && GUI_SHOW_PARAM == gGuiCtrl.guiType)
                    || (KEY_AUTO_OFF2 <= gGuiCtrl.tmProbe && GUI_SHOW_MENU == gGuiCtrl.guiType)))
            {
                gGuiCtrl.guiType = GUI_SHOW_PROBEOFF;
                gGuiCtrl.subType = PARAM_SEQ_RESET;
                gGuiCtrl.tmProbe = 0;
            }
        }
    }

    if (GUI_SHOW_MENU == gGuiCtrl.guiType || GUI_SHOW_FAC_INFO == gGuiCtrl.guiType)
    {
        if (GUI_SHOW_FAC_INFO == gGuiCtrl.guiType && 0 == gGuiCtrl.tmNoOprMenu)
        {
            UG2864_Fill_RAM(0);
            Display_Version();
        }

        gGuiCtrl.tmNoOprMenu++;
        // back to params
        if (MAX_NO_OPR_TM < gGuiCtrl.tmNoOprMenu)
        {
            Setup_UpdateDevice();

            gGuiCtrl.guiType = GUI_SHOW_PARAM;
            gGuiCtrl.subType = PARAM_SEQ_RESET;
            gGuiCtrl.mDirect = gCfgCtrl.dispDir;
            gGuiCtrl.bReset  = TRUE;
            gGuiCtrl.tmNoOprMenu = 0;
        }
    }

    switch (gGuiCtrl.guiType)
    {
        case GUI_SHOW_LOGO:
        {
            //1.0S for firms logo
            if (GUI_UPDATE_RATE == gGuiCtrl.tmCount)
            {
                Display_Logo(LOGO_FIRMS, (GUI_DIR_E)gGuiCtrl.mDirect);
                Display_Version();
            }
            //Switch to default show type
            else if (1 == gGuiCtrl.tmCount)
            {
                gGuiCtrl.guiType = GUI_SHOW_PARAM;
            }
        }
        break;

        case GUI_SHOW_PARAM:
        {
            //Gradually refresh content if orientation changed
            if (PARAM_SEQ_MARGIN > gGuiCtrl.subType)
            {
                //Clear screen
                if (PARAM_SEQ_RESET == gGuiCtrl.subType)
                {
                    UG2864_Fill_RAM(0);
                }
                //Refresh caption
                else if (PARAM_SEQ_CAPTION == gGuiCtrl.subType)
                {
                    Display_Caption((GUI_DIR_E)gGuiCtrl.mDirect);
                }
                //Refresh battery
                else if (PARAM_SEQ_BATTERY == gGuiCtrl.subType)
                {
                    Display_Battery(gCellVolt, (GUI_DIR_E)gGuiCtrl.mDirect);
                }

                //Sequence
                gGuiCtrl.subType++;
                //Reset counter
                gGuiCtrl.tmCount = GUI_UPDATE_RATE;
                //Reset invalid data to enforce refresh
                //gGuiCtrl.preVolt = 16;
                gGuiCtrl.preSAT  = 128;
                gGuiCtrl.preBPM  = 512;
                return;
            }

            //Enter Alarm menu
            if (KEY_SHORT_MAX < gKeyCtrl.tmKeyOn && FALSE == gKeyCtrl.bErrFlg)
            {
                gGuiCtrl.guiType = GUI_SHOW_MENU;
                gGuiCtrl.subType = MENU_ALM_RESET;
                gGuiCtrl.bReset  = TRUE;
                gGuiCtrl.optItem = MENU_ALM_LO_SAT;
                //Clear key end state
                gKeyCtrl.bErrFlg = TRUE;
            }

            //(25.0Hz)Display Plethysmograph
            if (0 != gGuiCtrl.tmCount % 2)
            {
                Display_Pleth(gSatParam.sysParam.mPleth, (GUI_DIR_E)gGuiCtrl.mDirect);
            }
            //(16.6~Hz)Display Bargraph
            else
            {
                Display_Bargraph(gSatParam.sysParam.mBar, (GUI_DIR_E)gGuiCtrl.mDirect);
            }

            //Divide frequency
            mDivider = gGuiCtrl.tmCount % 10;

            //(5Hz)Display SpO2
            if (0 == mDivider)
            {
                //SpO2 blink counter
                if (0 < gSatParam.sysParam.mSpO2 && gAlmCtrl.lowSAT > gSatParam.sysParam.mSpO2)
                {
                    gAlmCtrl.blkSAT++;
                    if (5 <= gAlmCtrl.blkSAT)
                    {
                        gAlmCtrl.blkSAT = 0;
                    }
                }
                else
                {
                    gAlmCtrl.blkSAT = 0;
                }
                //Update
                if (gSatParam.sysParam.mSpO2 != gGuiCtrl.preSAT
                    || 2 == gAlmCtrl.blkSAT || 4 == gAlmCtrl.blkSAT)
                {
                    Display_SpO2(gSatParam.sysParam.mSpO2, (GUI_DIR_E)gGuiCtrl.mDirect);
                    gGuiCtrl.preSAT = gSatParam.sysParam.mSpO2;
                }
            }

            //(5Hz)Display Pulse
            else if (9 == mDivider)
            {
                //Pulse blink flag
                if (0 != gSatParam.sysParam.mPulse
                    && (gAlmCtrl.lowBPM > gSatParam.sysParam.mPulse ||
                        gAlmCtrl.upBPM < gSatParam.sysParam.mPulse))
                {
                    gAlmCtrl.blkBPM++;
                    if (5 <= gAlmCtrl.blkBPM)
                    {
                        gAlmCtrl.blkBPM = 0;
                    }
                }
                else
                {
                    gAlmCtrl.blkBPM = 0;
                }

                //Update
                if (gSatParam.sysParam.mPulse != gGuiCtrl.preBPM
                    || 2 == gAlmCtrl.blkBPM || 4 == gAlmCtrl.blkBPM)
                {
                    Display_Pulse(gSatParam.sysParam.mPulse, (GUI_DIR_E)gGuiCtrl.mDirect);
                    gGuiCtrl.preBPM = gSatParam.sysParam.mPulse;
                }
            }

            //(1Hz)Display perfusion index
            if (45 == gGuiCtrl.tmCount)
            {
                Display_Perfusion(gSatParam.sysParam.mPerfuse, (GUI_DIR_E)gGuiCtrl.mDirect);
            }

            //(required)Display battery(1Hz blink)
            if (FALSE == showBleLogo)
            {
                if (0 == gGuiCtrl.tmCount % 25)
                {
                    //gGuiCtrl.preVolt = gCellVolt;
                    Display_Battery(gCellVolt, (GUI_DIR_E)gGuiCtrl.mDirect);
                }
            }
            else //show BleLogo
            {
                if (GUI_UPDATE_RATE == gGuiCtrl.tmCount)
                {
                    Display_Ble((GUI_DIR_E)gGuiCtrl.mDirect);
                }
            }

        }
        break;

        //Alarm setup menu
        case GUI_SHOW_MENU:
        {
            //Gradually refresh content
            if (MENU_ALM_MARGIN > gGuiCtrl.subType)
            {
                //Clear screen
                if (MENU_ALM_RESET == gGuiCtrl.subType)
                {
                    UG2864_Fill_RAM(0);
                }

                //Refresh content
                Display_AlarmMenu(gGuiCtrl.subType, (GUI_DIR_E)gGuiCtrl.mDirect);

                //Sequence
                gGuiCtrl.subType++;
                //Reset counter
                gGuiCtrl.tmCount = GUI_UPDATE_RATE;
                return;
            }

            //(5Hz)Alarm set
            if (0 == gGuiCtrl.tmCount % 16)
            {
                if (KEY_SHORT_MAX < gKeyCtrl.tmKeyOn && FALSE == gKeyCtrl.bErrFlg)
                {
                    Display_AlarmThrd(gGuiCtrl.optItem, (GUI_DIR_E)gGuiCtrl.mDirect);
                }
                else
                {
                    Display_BlinkSelection(gGuiCtrl.optItem, (GUI_DIR_E)gGuiCtrl.mDirect);
                }
            }
        }
        break;

        //Finger out tips
        case GUI_SHOW_PROBEOFF:
        {
            //Gradually refresh content
            if (APOD_MARGIN > gGuiCtrl.subType)
            {
                if (APOD_RESET == gGuiCtrl.subType)
                {
                    UG2864_Fill_RAM(0);
                }
                else
                {
                    Display_ProbeOff(FALSE, (GUI_DIR_E)gGuiCtrl.mDirect);
                }

                //Sequence
                gGuiCtrl.subType++;
                //Reset counter
                gGuiCtrl.tmCount = GUI_UPDATE_RATE;
                return;
            }

            //Probe off precaution
            if (TRUE == gSatParam.sysAlarm.ProbeOff)
            {
                if (8 > sOffCnt)
                {
                    sOffCnt++;
                }
            }
            else
            {
                if (0 < sOffCnt)
                {
                    sOffCnt--;
                }
            }

            //Blink tips
            if (48 == gGuiCtrl.tmCount)
            {
                Display_ProbeOff(FALSE, (GUI_DIR_E)gGuiCtrl.mDirect);
            }
            else if (24 == gGuiCtrl.tmCount)
            {
                Display_ProbeOff(TRUE, (GUI_DIR_E)gGuiCtrl.mDirect);
            }

            //Power off
            if (KEY_AUTO_OFF1 < gGuiCtrl.tmProbe)
            {
                UG2864_Set_Display_On_Off(0x00);
                GPIO_ClearBits(GPIOPortA, GPIOPin4); // power off
            }
            //Return normal detect if probe on during power off tips span
            else if (FALSE == gSatParam.sysAlarm.ProbeOff && 0 != gCellVolt && 0 == sOffCnt)
            {
                gGuiCtrl.tmProbe = 0;
                //Back to parameter interface
                gGuiCtrl.guiType = GUI_SHOW_PARAM;
                gGuiCtrl.subType = PARAM_SEQ_RESET;
                gGuiCtrl.bReset  = TRUE;
            }
        }
        break;

        default:
            break;
    }

    //Time counter
    gGuiCtrl.tmCount--;
    if (0 == gGuiCtrl.tmCount)
    {
        gGuiCtrl.tmCount = GUI_UPDATE_RATE;
        if (gChkFunc.chkBLH) //exchange Ble logo and battery vol display state
        {
            showBleLogo = !showBleLogo;
            if (7 >= gCellVolt) //low battery volt, don't show ble logo
            {
                showBleLogo = FALSE;
            }
        }
    }
}

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
void Display_Init(void)
{
//    //Reset relevant controller
//    memset(&gGuiCtrl, 0, sizeof(gGuiCtrl));
//    memset(&gKeyCtrl, 0, sizeof(gKeyCtrl));
//    //Reset counter
//    gGuiCtrl.tmCount = GUI_UPDATE_RATE;
//    //OLED configuration
//    UG2864_OLED_Init();
//    //Set Display Orientation
//    gGuiCtrl.mDirect = gCfgCtrl.dispDir;
	
}


