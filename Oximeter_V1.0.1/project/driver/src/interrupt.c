/**
 *****************************************************************************
 * @brief   interrupt source file.
 *
 * @file   interrupt.c
 * @author
 * @date   28/JUN/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */

#include "interrupt.h"
#include "gpio.h"
#include "DataType.h"
#include "Preprocess.h"
#include "LightDriver.h"
#include "DefaultSetup.h"
#include "BuzzDriver.h"
#include "DisplayDriver.h"
#include "SystemDriver.h"
#include "saradc.h"
#include "dma.h"
#include "uart.h"
#include "BluetoothDriver.h"
#include "SegDriver.h"


//----------------------------------------------------------------------------------------------------

#define Iabs(dtA)                       ((dtA) >= 0 ? (dtA) : (-dtA))

UINT32 gSumVal = 0;


#if defined (__CC_ARM)  /* Keil μVision 5.29.0.0 */
ISRFunc_T  jump_table_base[8] __attribute__((__section__(".jump_table_mem_area"))) =
{
    0, 0, 0, 0, 0, 0, 0, 0,
};
#elif defined (__ICCARM__)  /* IAR Ewarm  8.32.3 */
volatile ISRFunc_T  jump_table_base[8]@".jump_table_mem_area" =
{
    0, 0, 0, 0, 0, 0, 0, 0,
};
#endif


void NMI_Handler(void)
{
    while (1);
}

void HardFault_Handler(void)
{
    while (1);
}

void SVC_Handler(void)
{
    while (1);
}
#ifndef __FREERTOS__
void PendSV_Handler(void)
{
    while (1);
}
void SysTick_Handler(void)
{
    while (1);
}
#endif




/*
********************************************************************************
 * [Func.Name  ] UINT32 ADC_GetDmaValue(void)                                 *
 * [Func.Spec. ] DMA buffer for sample mean value.                            *
 * [Func.Input ]                                                              *
 *               void                                                         *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/

#if defined   ( __ICCARM__   ) /* iar */
__STATIC_INLINE UINT32 ADC_GetDmaValue(void)
#elif defined ( __CC_ARM )     /* keil */
__attribute__((always_inline)) __inline UINT32 ADC_GetDmaValue(void)
#endif
{
    UINT32 i;
    UINT32 mSumVal = 0;

    // Sum
    for (i = 0; i < DMA_ADC_NUM; ++i)
    {
        mSumVal += (gDmaAdcBuf[i] & 0x3FFF);
        mSumVal += (gDmaAdcBuf[i] >> 14);
    }

    return mSumVal;
}

/*
********************************************************************************
 * [Func.Name  ] void PwmToDacDrive(INT16 mValue)                             *
 * [Func.Spec. ] Transmit PWM duty to DAC current.                            *
 * [Func.Input ]                                                              *
 *               INT16 mValue     Current(0-4095)                             *
 * [Func.Output]                                                              *
 *               void                                                         *
 * [Func.Return]                                                              *
 *               void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
*/

#if defined   ( __ICCARM__   ) /* iar */
__STATIC_INLINE void PwmToDacDrive(INT16 mValue)
#elif defined ( __CC_ARM )     /* keil */
__attribute__((always_inline)) __inline void PwmToDacDrive(INT16 mValue)
#endif
{
    UCHAR mDuty = mValue >> 6;           //Accord to PWM_GAP_DRV

    //Output PWM
    if (0 != mDuty)
    {
        TIMER4->matcha0_val = PWM_GAP_FRQ - mDuty;
    }
    else
    {
        TIMER4->matcha0_val = 0;
    }
}

static uint8_t cnt = 0;

void fic_IRQHandler(void)
{
    FIC->clr_irq1 = 0xFFFFFFFF;
    FIC->clr_irq1 = 0;

    const CHAR fabStep[4] = {PWM_GAP_DRV, 0, -PWM_GAP_DRV, 0};    //Triangle period
    static UCHAR  gSrtCnt = 0;                                    //Triangle step
    static UINT16 sCellSpan = SEQ_BATTERY - 40;                   //Battery volume sample period counter
    static UINT16 sSpo2Span = 0;                                  //Saturation sample period counter
    static UINT32 gSampRedAmb  = 0;
    static UINT32 gSampRedSamp = 0;
    static UINT32 gSampIrdAmb  = 0;
    static UINT32 gSampIrdSamp = 0;
    static UINT32 gSampVolt    = 0;
    static UINT16 gDrvCur = 0;
    UINT32 mSampDat;
    static uint8_t ms_count = 0;
    if (ms_count++ > 30)
    {
        ms_count = 0;
        PT_SEM_SIGNAL(&PT_SpO2, &SEM_POLL);
        seg_show_thread(&PT_SpO2);
    }

    if (84 >= sSpo2Span)
    {
        /* SARADC DMA采样配置 */
        // restart dma
        ANAREG_1->reg_saradc_ctrl |= 0x01;

        //DMA->DCSR[SARADC_RX_DMA_CHANNEL] &= 0x7FFFFFFFUL;
        DMA->DESC[SARADC_RX_DMA_CHANNEL].DTADR = (uint32_t)(gDmaAdcBuf);
        DMA->DESC[SARADC_RX_DMA_CHANNEL].DCMD_f.LEN = DMA_LENTH;
        DMA->DCSR[SARADC_RX_DMA_CHANNEL] |= 0x80000000UL;

        // get samples
        mSampDat = ADC_GetDmaValue();

        // restart saradc
        ANAREG_1->reg_saradc_ctrl &= ~0x01;
    }
    else
    {
        goto INT_END;
    }

    switch (sSpo2Span)
    {
    case 0:
        gDrvCur = PWM_GAP_DRV;
        //gDrvCur = 0;
        gSrtCnt = 0;
        (GPIO_PIN_16_25->data_out) |= 0x00000004;        //Red  PC02 HIGH 开启红光通道
        break;
    case 24:
        gDrvCur = gDriveRed;
//                  gDrvCur = 512;
        if (!gDrvCur)
        {
            (GPIO_PIN_16_25->data_out) &= 0xFFFFFFFB;    //Red  PC02 LOW    关闭红光通道
        }
        break;
    case 36:
        gDrvCur = PWM_GAP_DRV;
        (GPIO_PIN_16_25->data_out) &= 0xFFFFFFFB;        //Red          PC02 LOW    关闭红光通道
        (GPIO_PIN_0_15->data_out) |= 1 << (8 + 4);        //Infrard  PB04 HIGH 开启红外光通道
        break;
    case 60:
        gDrvCur = gDriveIrd;
//              gDrvCur = 128;
        if (!gDrvCur)
        {
            (GPIO_PIN_0_15->data_out) &= ~(1 << (8 + 4));        //Infrard  PB04 LOW    关闭红外光通道
        }
        break;
    case 72:
        gDrvCur = 0;
        (GPIO_PIN_0_15->data_out) &= ~(1 << (8 + 4));                       //Infrard   PB04 LOW    关闭红外光通道
        break;
    }
//gDrvCur=0;
//      gSrtCnt = 1;
    if (gCfgCtrl.swDemo || 0 == gDrvCur)
    {
        PwmToDacDrive(0);           //关闭恒流控制
    }
    else
    {
        //Add triangle signal to light pulse
        PwmToDacDrive(gDrvCur + fabStep[gSrtCnt]);  //红外光恒流控制
//          PwmToDacDrive((64*4));  //64*1~64*63调光范围实测0~480mV
    }

    //Counter
    gSrtCnt++;
    if (4 <= gSrtCnt)
    {
        gSrtCnt = 0;
    }

    /* 红外光四段采样结果 */
    // Summary of Red ambient (8 times)
    if (16 < sSpo2Span && 24 >= sSpo2Span)
    {
        gSampRedAmb += mSampDat;
    }
    // Summary of Red light (8 times)
    else if (28 < sSpo2Span && 36 >= sSpo2Span)
    {
        gSampRedSamp += mSampDat;
    }
    // Summary of Infrared ambient (8 times)
    else if (52 < sSpo2Span && 60 >= sSpo2Span)
    {
        gSampIrdAmb += mSampDat;
    }
    // Summary of Infrared light (8 times)
    else if (64 < sSpo2Span && 72 >= sSpo2Span)
    {
        gSampIrdSamp += mSampDat;
    }
    // Trigger Tim2
    else if (73 == sSpo2Span)
    {
        // Get Sample Value(Ambient: 8 times Ired/Red: 8 times, DMA: 18bit)
        gSampArray[PHASE_RED_AMB]  = gSampRedAmb;
        gSampArray[PHASE_RED_SAMP] = gSampRedSamp;
        gSampArray[PHASE_IRD_AMB]  = gSampIrdAmb;
        gSampArray[PHASE_IRD_SAMP] = gSampIrdSamp;

        gSampArray[PHASE_RED_DRV] = gSampRedSamp >> 4;
        gSampArray[PHASE_IRD_DRV] = gSampIrdSamp >> 4;

        // Samples reset
        gSampRedAmb  = 0;
        gSampRedSamp = 0;
        gSampIrdAmb  = 0;
        gSampIrdSamp = 0;

        //Start Sample process
        NVIC_SetPendingIRQ(Lic_IRQn);       //触发LIC中断，采样数据处理
    }
    //Battery volume check，每隔约30s，检测一次电池电量
    else if (0 == sCellSpan)
    {
        //Switch to Battery sample channel(DMA need reset)
        if (74 == sSpo2Span)
        {
            //Select and start battery channel sample
            ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_p = SARADC_AIN7_PA07; // VBAT Sample pin，SARADC切换至PA07，电池电量检测
        }
        //4 Times average for battery volume sample
        else if (76 < sSpo2Span && 84 >= sSpo2Span)
        {
            gSampVolt += mSampDat;
            //Switch to PD sample channel
            if (84 == sSpo2Span)
            {
                //DMA_8 times Battery_8 times
                gSampVolt >>= 11;
                gSumVal = gSampVolt;
                //[0 - 15] = ((2 * Samp_Volt * 3300 / 63356) - 1900) / 100
                gCellVolt = (INT16)(gSampVolt - 161);
                if (16 < gCellVolt)
                {
                    gCellVolt = 16;
                }
                if (0 > gCellVolt)
                {
                    GPIO_ClearBits(GPIOPortA, GPIOPin4);   // power off，电池电量低于阈值，系统断电
                }
                //Reset and switch to normal sample
                gSampVolt = 0;

                //Restore normal spo2 sample
                ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_p = SARADC_AIN3_PA03;    //SARADC切换至PA03，spo2检测
            }
        }
    }

INT_END:
    // 111.1~Hz == 9.0ms == 60us * 150period
    sSpo2Span++;
    if (SEQ_PERIOD <= sSpo2Span)
    {
        sSpo2Span = 0;
        //battery volume sample period(30s ~ 3600 * 60us * 150span)
        sCellSpan++;
        if (SEQ_BATTERY <= sCellSpan)
        {
            sCellSpan = 0;
        }
    }
}



void lic_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(Lic_IRQn);

    //111.1~Hz resample to 50Hz，采样数据处理
    if (FALSE == Process_SampleData())
    {
        return;
    }
    cnt++;
    if (cnt > 6 && cnt <= 14)state = LEVEL2;
    if (cnt > 14 && cnt <= 19)state = LEVEL3;
    if (cnt > 19 && cnt < 24)state = LEVEL4;
    if (cnt == 24)isShowHeartSign = !isShowHeartSign;
    if (cnt > 24 && cnt <= 30)state = LEVEL5;
    if (cnt > 30 && cnt <= 37)state = LEVEL4;
    if (cnt > 37 && cnt <= 42)state = LEVEL3;
    if (cnt > 42 && cnt <= 45)state = LEVEL2;
    if (cnt > 45)
    {
        cnt = 0;
        state = LEVEL1;
        isShowHeartSign = TRUE;
    }
    //Sensor status judge (Sensor off)
    gSatParam.sysAlarm.ProbeOff = !Light_ProbeDriveAdjust();

    //Aic_IRQn for GUI display and COM communication
//    NVIC_SetPendingIRQ(Aic_IRQn);       //触发AIC中断，显示各指标信息，警告，蜂鸣器等
}



void aic_IRQHandler(void)
{
    uint32_t  intStatus;
    static UCHAR sBuzzSet = TRUE;
    static UCHAR sSecCnt  = 0;

    NVIC_ClearPendingIRQ(Aic_IRQn);

    AIC->latch_irq = 0;
    intStatus = AIC->irq_status1;

    if (intStatus)
    {
        if (intStatus & AIC_IWDG_MASK)
        {
            AIC->clr_irq1 |= AIC_IWDG_MASK;
            AIC->clr_irq1 &= ~AIC_IWDG_MASK;

            IwdgCallback();
        }
    }
    //Single buzz when power on
    if (TRUE == sBuzzSet)
    {
        //Initialize buzz
        gBeepCtrl.mDuty  = 184;
        gBeepCtrl.tmHold = 2;
        gBeepCtrl.tmCnt = 0;
        gBeepCtrl.tmGap = 0;
        //Set single beep for power on
        gBeepCtrl.mType = BUZZ_SIGNLE_BEEP;
        sBuzzSet = FALSE;
    }
    else
    {
        gAlmCtrl.swAlarm = TRUE;
        gSatParam.sysParam.mSpO2 = 1;
        if (gAlmCtrl.swAlarm) // Alarm switch on
        {
            // Single buzz
            if (/*gAlmCtrl.upSAT >= gSatParam.sysParam.mSpO2
             && */gAlmCtrl.lowSAT <= gSatParam.sysParam.mSpO2
                && gAlmCtrl.upBPM >= gSatParam.sysParam.mPulse
                && gAlmCtrl.lowBPM <= gSatParam.sysParam.mPulse)
            {
                if ((TRUE == gSatParam.sysParam.mBeep) && gAlmCtrl.swBeep)
                {
                    gBeepCtrl.mType = BUZZ_SIGNLE_BEEP;
                }
            }

            // Double buzz
            else if (0 != gSatParam.sysParam.mSpO2)
            {
                sSecCnt++;
                if (SAT_SAMPLE_RATE <= sSecCnt)
                {
                    sSecCnt = 0;
                    gBeepCtrl.mType = BUZZ_DOUBLE_BEEP;
                }
            }
        }
        else if (gAlmCtrl.swBeep) // Alarm switch off and Beep switch on
        {
            if (TRUE == gSatParam.sysParam.mBeep)
            {
                gBeepCtrl.mType = BUZZ_SIGNLE_BEEP;
            }
        }
    }

//    Display_GUI();

//    Buzz_Speaker();
}



static int32_t XIC_GetVectIdx(IRQn_Type IRQn)
{
    int32_t idx = (int32_t)IRQn - FIC_Ovf_IRQn;
    /* validity check */
    if (idx < 0)
    {
        /*!!! add assert code later */
        while (1);
    }
    return idx;
}

static void *XIC_GetModule(int32_t vectIdx)
{
    return (vectIdx >= NUM_FIC_INT) ? ((vectIdx >= (NUM_LIC_INT + NUM_FIC_INT)) ? (void *)AIC : (void *)LIC) : (void *)FIC;
}

void XIC_SetVector(IRQn_Type IRQn, ISRFunc_T vector)
{
    // first 5 is for nmi etc. handlers
    int32_t idx = (int32_t)IRQn;

    if (idx < 0)
    {
        if (idx == -14)
            jump_table_base[0] = vector;
        else if (idx == -13)
            jump_table_base[1] = vector;
        else if (idx == -5)
            jump_table_base[2] = vector;
        else if (idx == -2)
            jump_table_base[3] = vector;
        else if (idx == -1)
            jump_table_base[4] = vector;
    }
    else if (idx <= Aic_IRQn)
    {
        jump_table_base[5 + idx] = vector;
    }
    else
    {
        jump_table_base[8 + idx - FIC_Ovf_IRQn] = vector;
    }
}


void XIC_EnableIRQ(IRQn_Type IRQn, ISRFunc_T vector)
{
    int32_t idx = XIC_GetVectIdx(IRQn);
    void *xic = XIC_GetModule(idx);
    jump_table_base[8 + idx] = vector;
    /* read current interrupt mask register and unmask the corresponding bit */
    if (xic == FIC)
    {
        FIC->mask1 &= ~( 1 << (idx & 0x1FUL) );
    }
    else if (xic == LIC)
    {
        LIC->mask1 &= ~( 1 << ((idx - NUM_FIC_INT) & 0x1FUL) );
    }
    else if (xic == AIC)
    {
        //printf("enable AIC 0x%x\r\n", idx);
        AIC->mask1 &= ~( 1 << ((idx - NUM_FIC_INT - NUM_LIC_INT) & 0x1FUL) );
    }
}


void XIC_DisableIRQ(IRQn_Type IRQn)
{
    int32_t idx = XIC_GetVectIdx(IRQn);
    void *xic = XIC_GetModule(idx);
    /* read current interrupt mask register and mask the corresponding bit */
    if (xic == FIC)
    {
        FIC->mask1 |= (1 << (idx & 0x1FUL));
    }
    else if (xic == LIC)
    {
        LIC->mask1 |= ( 1 << ((idx - NUM_FIC_INT) & 0x1FUL) );
    }
    else if (xic == AIC)
    {
        AIC->mask1 |= ( 1 << ((idx - NUM_FIC_INT - NUM_LIC_INT) & 0x1FUL) );
    }
}

int calculate_CLZ(unsigned int *num)
{
    unsigned int value = 0x80000000;
    int i = 0;

    if (*num == 0)
        return 32;

    while (value != 0)
    {
        if (*num & (value >> i))
        {
            *num &= ~(value >> i);
            return i;
        }
        i++;
    }
    return 0;
}

void IC_PowupInit(void)
{
    // init FIC/LIC/AIC interrupt vector table
    memset((void *)jump_table_base, 0, sizeof((void *)jump_table_base));

    jump_table_base[0] = NMI_Handler;
    jump_table_base[1] = HardFault_Handler;
    jump_table_base[2] = SVC_Handler;
    jump_table_base[3] = PendSV_Handler;
    jump_table_base[4] = SysTick_Handler;
    jump_table_base[5] = fic_IRQHandler;
    jump_table_base[6] = lic_IRQHandler;
    jump_table_base[7] = aic_IRQHandler;
}

void IC_PowupDeinit(void)
{
    /* init FIC/AIC/LIC interrupt vector table*/
    memset((void *)jump_table_base, 0, sizeof((void *)jump_table_base));
}

void IC_EnableNVIC(void)
{
    /* enable NVIC #0~2 */
    NVIC->ISER[0] = 0x07;
}

void IC_DisableNVIC(void)
{
    /* disable NVIC #0~2 */
    NVIC->ISER[0] = 0x0;
}


