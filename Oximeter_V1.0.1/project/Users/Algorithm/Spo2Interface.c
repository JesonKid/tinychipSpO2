/**
*****************************************************************************
* @brief
*
* @file   Spo2Interface.c
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

#include "Spo2Interface.h"
#include "Preprocess.h"
#include "wdg.h"
#include "trace.h"

#define TICK_MAX   100

// �����ռ�
static uint8_t heart_beat_sample[10] = {75, 75, 75, 75, 75, 75, 75, 75, 75, 75};
static uint8_t rollPos = 0;

/********************************************************
*@brief ��������
*@param void
*@retval void
*********************************************************/
void heartbeat_algorithm()
{
    static uint8_t tick = 0;
    static INT16 value = 0, value_last = 0;

    //�����ռ����Ĭ��ֵ
//    memset(heart_beat_sample,75,sizeof(heart_beat_sample));

    while (0)
    {
        IWDG_ResetCnt();
        //ѡȡһ�����
        if (is_new_sample_ready == TRUE)
        {
            is_new_sample_ready = FALSE;

            value = gSatSamp.IrdAC;
            if (value > 0 && value_last < 0)
            {
                //�׸������֣���ʼͳ��
                value = 0;
                value_last = 0;
                tick = 0;
                break;
            }
            value_last = value;
        }
    }
    //ͳ������ֵ
    while (0)
    {
        IWDG_ResetCnt();
        if (is_new_sample_ready == 1)
        {
            is_new_sample_ready = 0;

            tick++;
            //ȡ�������ж�
            value = gSatSamp.IrdAC;

            //ֱ����һ������ת��
            if (value > 0 && value_last < 0)break;
            value_last = value;
        }
    }
    //��������
    while (0)
    {
        IWDG_ResetCnt();
        static uint16_t heartRate = 60;

        //������������ֵ
        heartRate = 60 * 1000 / (20 * tick);

        if (heartRate > 100 || heartRate < 50)
        {
            //��ȡ�������¿�ʼ
//            printf("filter the abnormal heartRate %d\n", heartRate);
            tick = 0;
            break;
        }
        //д�������ռ�
        heart_beat_sample[rollPos++ % sizeof(heart_beat_sample)] = heartRate;
//              for(uint8_t i = 0; i < 10; i++)printf("heart_beat_sample[%d]= %d\n",i,heart_beat_sample[i]);
        //��ƽ��
        heartRate = 0;
        for (uint8_t i = 0; i < sizeof(heart_beat_sample); i++)heartRate += heart_beat_sample[i];
        heartRate = heartRate / sizeof(heart_beat_sample);

        //��־������
//        printf("heartRate = %d\n", heartRate);

        //������ʾ��GUI
        gSatParam.sysParam.mPulse = heartRate;

        //��λͳ������
        tick = 0;

        break;
    }
}

/********************************************************
*@brief ����Ѫ��
*@param void
*@retval void
*********************************************************/
void SpO2_algorithm()
{
    static uint8_t tick = 0;
    static INT16 acRed_max = 0, acIr_max = 0;
    static uint8_t spo2 = 0;
    while (1)
    {
        IWDG_ResetCnt();
        if (is_new_sample_ready == TRUE)
        {
            //50Hz����������Ƶ��
            is_new_sample_ready = FALSE;

            //��ȡʱ�䷶Χ���������ֵ
            if (acRed_max < gSatSamp.RedAC) acRed_max = gSatSamp.RedAC;
            if (acIr_max < gSatSamp.IrdAC) acIr_max = gSatSamp.IrdAC;

            tick++;
        }
        if (tick > TICK_MAX)
        {
            //����ٶ�2���ڿ϶���������ֵ
            tick = 0;
            static float Ratio = 0.0;

            //ͨ��ԭ�����Ѫ���ٷֱ�
            Ratio = ((float)acRed_max / (float)gSatSamp.RedDC) / ((float)acIr_max / (float)gSatSamp.IrdDC);
            spo2 = 110 - 25 * Ratio;

            //�˳�����
            if (spo2 > 100 || spo2 < 80)
            {
//                printf("filter the abnormal SpO2%% %d\n", spo2);
                break;
            }

            //��־���Ѫ���ٷֱ�
//            printf("SpO2%% = %d\n", spo2);

            //������ʾ��GUI
            gSatParam.sysParam.mSpO2 = spo2;

            break;
        }
    }

}

