/**
*****************************************************************************
* @brief
*
* @file   DefaultSetup.c
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

#include "DefaultSetup.h"
#include "Spo2Interface.h"
#include "DisplayDriver.h"
#include "flash.h"


//----------------------------------------------------------------------------------------------------


SETUP_CFGCtrl_T gCfgCtrl = {0};                                   //Configuration controller
SETUP_CHKFunc_T gChkFunc = {0};                                   //Function check

static UINT32 gBaseAddr =
    FMC_CFG_PAGE;             //Configuration information baseAddr

//----------------------------------------------------------------------------------------------------

/**
 * @description: erase selected page
 * @param void
 * @return void
 */
void ErasePage(void)
{
    Flash_Erase((uint16_t)(FMC_CFG_PAGE - FLASH_BASE_ADDR) >> 9);
}

/**
 * @description: Read configuration infor from flash
 * @param void
 * @return void
 */
void ReadFlashCfg(void)
{
    UCHAR i = 0;
    UINT16 *pData   = (UINT16*)&gCfgCtrl;
    UINT32 *pAdress = (UINT32*)FMC_CFG_PAGE;

    while ( 0 == (*pAdress) )
    {
        pAdress += (FMC_CFG_SIZE + 1);
    }

    if ((UINT32*)FMC_CFG_PAGE < pAdress)
    {
        pAdress -= (FMC_CFG_SIZE + 1);
    }

    // set base address
    gBaseAddr = (UINT32)pAdress;

    //Write configuration byte
    if (0xFFFFFFFF == pAdress[0])
    {
        //Default setup
        pData[0] = DEF_SW_BEEP;
        pData[1] = DEF_SW_ALM;
        pData[2] = DEF_DISP_BRIGHTNESS;
        pData[3] = DEF_DISP_DIR;
        pData[4] = DEF_DIG_OPRDIR;
        pData[5] = DEF_BIG_FONT;
        pData[6] = DEF_UP_SAT;
        pData[7] = DEF_LOW_SAT;
        pData[8] = DEF_UP_PR;
        pData[9] = DEF_LOW_PR;
        //Save default configuration
        WriteFlashCfg();
    }
    //Read configuration byte
    else
    {
        for (i = 0; i < FMC_CFG_SIZE; i++)
        {
            pData[i] = pAdress[i + 1];
        }

        gBaseAddr += (FMC_CFG_SIZE + 1) * 4;
    }
}

/**
 * @description: Write current configuration to flash.
 * @param void
 * @return void
 */
void WriteFlashCfg(void)
{
    UCHAR i = 0;
    UINT16 *pData   = (UINT16*)&gCfgCtrl;
    UINT32  mAdress = gBaseAddr;

    Flash_Init(); //Enable flash write/erase

    if (gBaseAddr + (FMC_CFG_SIZE + 1) * 4 >= (FMC_CFG_PAGE + PAGE_SIZE))
    {
        ErasePage();
        gBaseAddr = FMC_CFG_PAGE;
        mAdress = gBaseAddr;
    }

    // Write data
    for (i = 0; i < FMC_CFG_SIZE; i++)
    {
        //4 byte
        Flash_Write(mAdress + 4, pData[i]);
        mAdress += FMC_BYTE_NUM;
    }

    // flag
    Flash_Write(gBaseAddr, 0);

    // Set Base Addr
    gBaseAddr = mAdress + 4;

    Flash_DeInit();
}


/**
 * @description: Restore oximeter setup.
 * @param void
 * @return void
 */
void Setup_RestoreDevice(void)
{
    //Get default oximter setup
    ReadFlashCfg();

    //Restore
    gAlmCtrl.swBeep =
        gCfgCtrl.swBeep;                            //Pulse beep switch
    gAlmCtrl.swAlarm = gCfgCtrl.swAlarm;                          //Alarm switch
    gAlmCtrl.digOprDir =
        gCfgCtrl.digOprDir;                      //Digital Opeartion type
    gAlmCtrl.upSAT =
        gCfgCtrl.upSAT;                              //SpO2 high threshold
    gAlmCtrl.lowSAT =
        gCfgCtrl.lowSAT;                            //SpO2 low threshold
    gAlmCtrl.upBPM  =
        gCfgCtrl.upBPM;                             //Pulse up threshold
    gAlmCtrl.lowBPM =
        gCfgCtrl.lowBPM;                            //Pulse low threshold
}

/**
 * @description: Update oximeter setup.
 * @param void
 * @return void
 */
void Setup_UpdateDevice(void)
{
    //Get newly setup
    gCfgCtrl.swBeep =
        gAlmCtrl.swBeep;                            //Pulse beep switch
    gCfgCtrl.swAlarm = gAlmCtrl.swAlarm;                          //Alarm switch
    gCfgCtrl.digOprDir =
        gAlmCtrl.digOprDir;                      //Digital Operation type
    gCfgCtrl.upSAT =
        gAlmCtrl.upSAT;                              //SpO2 high threshold
    gCfgCtrl.lowSAT =
        gAlmCtrl.lowSAT;                            //SpO2 low threshold
    gCfgCtrl.upBPM  =
        gAlmCtrl.upBPM;                             //Pulse up threshold
    gCfgCtrl.lowBPM =
        gAlmCtrl.lowBPM;                            //Pulse low threshold

    //Update setip
    WriteFlashCfg();
}


/**
 * @description: Default Setting Check
 * @param void
 * @return {UCHAR}
 */
UCHAR Setup_IsRestoreValue(void)
{
    if (gAlmCtrl.upSAT != DEF_UP_SAT)
    {
        return 0;
    }
    if (gAlmCtrl.lowSAT != DEF_LOW_SAT)
    {
        return 0;
    }
    if (gAlmCtrl.upBPM != DEF_UP_PR)
    {
        return 0;
    }
    if (gAlmCtrl.lowBPM != DEF_LOW_PR)
    {
        return 0;
    }
    if (gAlmCtrl.swAlarm != DEF_SW_ALM)
    {
        return 0;
    }
    if (gAlmCtrl.swBeep != DEF_SW_BEEP)
    {
        return 0;
    }
    if (gAlmCtrl.digOprDir != DEF_DIG_OPRDIR)
    {
        return 0;
    }
    if (gCfgCtrl.brightness != DEF_DISP_BRIGHTNESS)
    {
        return 0;
    }
    if (gCfgCtrl.bigFont != DEF_BIG_FONT)
    {
        return 0;
    }
    if (gCfgCtrl.dispDir != DEF_DISP_DIR)
    {
        return 0;
    }
    if (gCfgCtrl.swDemo != 0)
    {
        return 0;
    }

    return 1;
}



