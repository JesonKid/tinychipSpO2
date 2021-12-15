/**
 *****************************************************************************
 * @brief   rtc head file
 *
 * @file   rtc.h
 * @author
 * @date   17/Sep/2020
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd.
 *
 *****************************************************************************
 */
#ifndef __RTC_H__
#define __RTC_H__

#include "tcasxxx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define             RTC_PASSWORD                    0x1ACCE551
#define             RTC_CLK_EXTERNAL_EN             0x0
#define             RTC_CLK_INTERNAL_EN             0x1

#define             RTC_IT_BEACON                   (1<<6)
#define             RTC_IT_ALARM                    (1<<5)
#define             RTC_IT_MON                      (1<<4)
#define             RTC_IT_DAY                      (1<<3)
#define             RTC_IT_HOUR                     (1<<2)
#define             RTC_IT_MIN                      (1<<1)
#define             RTC_IT_SEC                      (1<<0)


#define             RTC_IT_BEACON_FLAG              (1<<6)
#define             RTC_IT_ALARM_FLAG               (1<<5)
#define             RTC_IT_MON_FLAG                 (1<<4)
#define             RTC_IT_DAY_FLAG                 (1<<3)
#define             RTC_IT_HOUR_FLAG                (1<<2)
#define             RTC_IT_MIN_FLAG                 (1<<1)
#define             RTC_IT_SEC_FLAG                 (1<<0)




/**
 ******************************************************************************
 ** \brief rtc time structure
 *****************************************************************************/

typedef struct STC_RTC_TIME
{
    uint32_t  second;
    uint32_t  minute;
    uint32_t  hour;
    uint32_t  day;
    uint32_t  week;
    uint32_t  month;
    uint32_t  year;
} stc_rtc_time_t;
/**
 ******************************************************************************
 ** \brief isr enum
 *****************************************************************************/
typedef enum EN_RTC_ISR
{
    RtcBeaconIsr = (1<<6),
    RtcAlarmIsr = (1<<5),
    RtcMonIsr  = (1<<4),
    RtcDayIsr = (1<<3),
    RtcHourIsr = (1<<2),
    RtcMinIsr = (1<<1),
    RtcSecIsr = (1<<0),
}en_rtc_isr_t;
/**
 ******************************************************************************
 ** \brief alarm time structure
 *****************************************************************************/

typedef struct STC_RTC_ALARM
{
    uint32_t  second;
    uint32_t  minute;
    uint32_t  hour;
} stc_rtc_alarm_t;


typedef struct
{
    uint8_t  second;        //00-59 BCD
    uint8_t  minute;        //00-59 BCD
    uint8_t  hour;          //00-23 BCD
} RTC_Alarm;

typedef struct
{
    uint8_t  second;        //00-59 BCD
    uint8_t  minute;        //00-59 BCD
    uint8_t  hour;          //00-23 BCD
    uint8_t  day;           //01-31 BCD
    uint8_t  week;          //00-06 BCD
    uint8_t  month;         //01-12 BCD
    uint8_t  year;          //00-99 BCD
} RTC_Time;



void RTC_TimeWrite(stc_rtc_time_t rtc_time);
stc_rtc_time_t RTC_TimeRead(void);
void RTC_IsrSet(en_rtc_isr_t rtc_isr);
void RTC_AlarmWrite(stc_rtc_alarm_t alarm_time);
void RTC_CmpThrelInit(void);


void RTC_DeInit(void);
void RTC_Enable(void);
void RTC_Lock(void);
void RTC_UnLock(void);
void RTC_SetAlarm(RTC_Alarm alarm);
void RTC_SetTime(RTC_Time      time);
void RTC_GetTime(RTC_Time      *ptime);
void RTC_ITConfig(uint8_t rtc_it,uint8_t state);
uint8_t RTC_GetITFlag(uint32_t flag);
void RTC_ClearITFlag(uint32_t flag);
uint8_t RTC_Ready(void);



#ifdef __cplusplus
}
#endif
#endif /* __RTC_H__ */

