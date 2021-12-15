/**
 *****************************************************************************
 * @brief   rtc source file.
 *
 * @file   rtc.c
 * @author
 * @date   17/Sep/2020
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
#include "rtc.h"

#define             DEFAULT_PPMA0_VALUE             0x3B438             //#-19400
#define             DEFAULT_PPMA1_VALUE             0x8CB4              //#36020
#define             DEFAULT_PPMA2_VALUE             0xD6674             //-170380
#define             DEFAULT_PPMA3_VALUE             0x2D5A4             //#-76380



/**
 ******************************************************************************
 ** \brief RTC weak isr handler
 **
 ** \param [in]
 **
 ** \retval void
 **
 ******************************************************************************/

void __attribute__((weak))Hal_CalendarIRQHandler()
{
    printf("Calendar interrupt,no handler function defined/r/n");
}
void __attribute__((weak))Hal_WaveCalendarIRQHandler()
{
    printf("Wave interrupt,no handler function defined/r/n");
}

/**
 ******************************************************************************
 ** \brief  rtc time register init (setting)
 **
 ** \param [in]
 **
 ** \retval void
 **
 ******************************************************************************/
void RTC_TimeWrite(stc_rtc_time_t rtc_time)
{
    /*enable clk*/
    RTC->rtccon1 |= (1<<10);
    /*enable lock reg to write*/
    RTC->calllock = 0x1ACCE551;
    RTC->sec = rtc_time.second;
    RTC->minute = rtc_time.minute;
    RTC->hour = rtc_time.hour;
    RTC->day = rtc_time.day;
    RTC->week = rtc_time.week;
    RTC->month = rtc_time.month;
    RTC->year = rtc_time.year;
}
/**
 ******************************************************************************
 ** \brief  rtc time read
 **
 ** \param [in]
 **
 ** \retval void
 **
 ******************************************************************************/

stc_rtc_time_t RTC_TimeRead(void)
{
    stc_rtc_time_t time;
    if(0 == (RTC->rtcsta1&0x02))
    {
        time.second = RTC->sec;
        time.minute = RTC->minute;
        time.hour = RTC->hour;
        time.day = RTC->day;
        time.week = RTC->week;
        time.month = RTC->month;
        time.year = RTC->year;
    }
    return time;
}

/**
 ******************************************************************************
 ** \brief  rtc isr set
 **
 ** \param [in] en_rtc_isr_t rtc_isr
 **
 ** \retval void
 **
 ******************************************************************************/
void RTC_IsrSet(en_rtc_isr_t rtc_isr)
{
    RTC->rtccon1 |= (1<<10);
    RTC->calllock = 0x1ACCE551;
    /*clr interrupt flag*/
    RTC->rtcif = 0xFF;
    /*disable all rtc isr*/
    RTC->rtcie = 0;
    /*enable rtc isr*/
    RTC->rtcie = rtc_isr;
}
/**
 ******************************************************************************
 ** \brief  rtc alarm isr set
 **
 ** \param [in] stc_rtc_alarm_t alarm_time
 **
 ** \retval void
 **
 ******************************************************************************/
void RTC_AlarmWrite(stc_rtc_alarm_t alarm_time)
{
    RTC->almsec =  alarm_time.second;
    RTC->almmin =  alarm_time.minute;
    RTC->almhour = alarm_time.hour;
    RTC_IsrSet(RtcAlarmIsr);
}

void RTC_CmpThrelInit(void)
{
    RTC->rtccon1_f.cmp_en = 1;
    RTC->rtccon1_f.ext_temp_en = 0;
    RTC->ppma0 = DEFAULT_PPMA0_VALUE;
    RTC->ppma1 = DEFAULT_PPMA1_VALUE;
    RTC->ppma2 = DEFAULT_PPMA2_VALUE;
    RTC->ppma3 = DEFAULT_PPMA3_VALUE;
    RTC->rtccon1_f.bn_intp_en = 1;
    RTC->beacon_intp_sel_out = 0;
}
/**
 ******************************************************************************
 ** \brief  RTC_DeInit
 **
 ** \param [in] 
 ** \retval void
 **
 ******************************************************************************/
void RTC_DeInit(void)
{
    RTC->rtcie = 0x0;           //disable all rtc interupt
    RTC->rtcif = 0xFF;          //clear all rtc interupt flag
    RTC->rtccon1 = 0x0;         //clear all rtc configure
}

/**
 ******************************************************************************
 ** \brief  RTC_Enable
 **
 ** \param [in]
 ** \retval void
 **
 ******************************************************************************/
void RTC_Enable(void)
{
    RTC->rtccon1_f.rtc_clk_sel = RTC_CLK_INTERNAL_EN;
}
/**
 ******************************************************************************
 ** \brief  RTC_Lock
 **
 ** \param [in]
 ** \retval void
 ******************************************************************************/
void RTC_Lock(void)
{
    RTC->calllock = 0;
}
/**
 ******************************************************************************
 ** \brief  RTC_UnLock
 **
 ** \param [in]
 ** \retval void
 ******************************************************************************/
void RTC_UnLock(void)
{
    RTC->calllock = RTC_PASSWORD;       //enable register write
}

/**
 ******************************************************************************
 ** \brief  RTC_SetAlarm
 **
 ** \param [in] RTC_Alarm
 **
 ** \retval void
 ******************************************************************************/
void RTC_SetAlarm(RTC_Alarm alarm)
{
    RTC->almsec =  alarm.second;
    RTC->almmin =  alarm.minute;
    RTC->almhour = alarm.hour;
}

/**
 ******************************************************************************
 ** \brief  RTC_SetTime
 **
 ** \param [in] RTC_InitType
 **
 ** \retval void
 ******************************************************************************/
void RTC_SetTime(RTC_Time  time)
{
    RTC->sec    =   time.second;
    RTC->minute =   time.minute;
    RTC->hour   =   time.hour;
    RTC->day    =   time.day;
    RTC->week   =   time.week;
    RTC->month  =   time.month;
    RTC->year   =   time.year;
}

/**
 ******************************************************************************
 ** \brief  RTC_SetTime
 **
 ** \param [in] RTC_Time
 **
 ** \retval void
 ******************************************************************************/
void RTC_GetTime(RTC_Time  *ptime)
{
    ptime->second   =   RTC->sec;
    ptime->minute   =   RTC->minute;
    ptime->hour     =   RTC->hour;
    ptime->day      =   RTC->day;
    ptime->week     =   RTC->week;
    ptime->month    =   RTC->month;
    ptime->year     =   RTC->year;

}

/**
 ******************************************************************************
 ** \brief  RTC_ITConfig
 **
 ** \param [in] rtc_it
                    RTC_IT_BEACON_EN
                    RTC_IT_ALARM_EN
                    RTC_IT_MON_EN
                    RTC_IT_DAY_EN
                    RTC_IT_HOUR_EN
                    RTC_IT_MIN_EN
                    RTC_IT_SEC_EN
 **
 ** \retval void
 ******************************************************************************/

void RTC_ITConfig(uint8_t rtc_it,uint8_t state)
{
    if(rtc_it & RTC_IT_BEACON)
    {
        RTC->rtcie_f.bie = state;
    }
    if(rtc_it & RTC_IT_ALARM)
    {
        RTC->rtcie_f.aie = state;
    }
    if(rtc_it & RTC_IT_MON)
    {
        RTC->rtcie_f.monie = state;
    }
    if(rtc_it & RTC_IT_DAY)
    {
        RTC->rtcie_f.dayie = state;
    }
    if(rtc_it & RTC_IT_HOUR)
    {
        RTC->rtcie_f.hrie = state;
    }
    if(rtc_it & RTC_IT_MIN)
    {
        RTC->rtcie_f.minie = state;
    }
    if(rtc_it & RTC_IT_SEC)
    {
        RTC->rtcie_f.secie = state;
    }

}

/**
 ******************************************************************************
 ** \brief  RTC_GetITFlag
 **
 ** \param [in] flag
                    RTC_IT_BEACON_FLAG
                    RTC_IT_ALARM_FLAG
                    RTC_IT_MON_FLAG
                    RTC_IT_DAY_FLAG
                    RTC_IT_HOUR_FLAG
                    RTC_IT_MIN_FLAG
                    RTC_IT_SEC_FLAG
 **
 ** \retval void
 ******************************************************************************/

uint8_t RTC_GetITFlag(uint32_t flag)
{
    return ((RTC->rtcif) & flag)?1:0;
}
/**
 ******************************************************************************
 ** \brief  RTC_GetITFlag
 **
 ** \param [in] flag
                    RTC_IT_BEACON_FLAG
                    RTC_IT_ALARM_FLAG
                    RTC_IT_MON_FLAG
                    RTC_IT_DAY_FLAG
                    RTC_IT_HOUR_FLAG
                    RTC_IT_MIN_FLAG
                    RTC_IT_SEC_FLAG
 **
 ** \retval void
 ******************************************************************************/
void RTC_ClearITFlag(uint32_t flag)
{
    RTC->rtcif |= flag;
}


/**
 ******************************************************************************
 ** \brief  RTC_Ready
 **
 ** \param [in]
 **
 ** \retval 0:the rtc count is not busy ready for read 
             1:the rtc count is   busy cannot read
 ******************************************************************************/
uint8_t RTC_Ready(void)
{
    return RTC->rtcsta1_f.cnt_busy?FALSE:TRUE;
}



