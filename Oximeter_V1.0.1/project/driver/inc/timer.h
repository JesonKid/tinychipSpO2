/**
 *****************************************************************************
 * @brief   Tiemr driver header.
 *
 * @file   timer.h
 * @author
 * @date   28/JUN/2020
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
#ifndef __TIMER_H__
#define __TIMER_H__

#include "tcasxxx.h"

  /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
 extern "C"
 {
#endif

 /******************************************************************************* 
  * Definitions 
  ******************************************************************************/
 
 /* ����(�ɱ�����Դ�ļ�ʹ��)�ꡢö�١��ṹ��Ķ��� */



 typedef struct _timer_config
 {
    T_TIMER* timer;     /* �Ĵ�������ַ */
    uint8_t  mode;
                        /*
                        000���򵥼�ʱ��
                        001�������������
                        010���������
                        011������Ƚ�
                        100��������ģʽ
                        101������PWMģʽ
                        110������PWMģʽ
                        111���������Ļ���PWMģʽ
                        ֻ�и߼���ʱ��֧�� 101/110/111 ģʽ
                        */
    uint8_t  wave;
                        /*
                        0����Ե����ģʽ
                        1�����Ķ���ģʽ
                        */
    uint8_t  matchnum;
                        /*
                        00��match point 0 ��ʹ��
                        01��match point 0-1��ʹ��
                        10��match point 0-2��ʹ��
                        11��match point 0-3��ʹ��
                        */
    uint8_t  ece;
                        /*
                        0��ʹ��bit0��ʹ��gpt
                        1��ʹ���ⲿ�Ĵ�����ʹ��gpt��ֻ���ڸ߼���ʱ������Ч
                        */
    uint8_t  channel;    /* bit0 = 1��ʾʹ��ͨ��A         bit1 = 1��ʾʹ��ͨ��B ����λ��Ч */
                        
    uint32_t prescale;  /* ��ʱ����Ƶϵ�� */
    uint32_t period;    /* �������� */
    uint32_t repeatVal;
                        /*
                        �ǵ�����ģʽ�£�����Ϊ0��������ģʽ�£������ڴﵽrepeat val����ʱ��ֹ��
                        ������Ӱ��timer�����ڣ�����Ӱ���жϷ���ʱ�䣬�ж��ڵ�repeat val��match�����
                        */
    uint16_t matchAOp;
                        /*
                        11��9��ʾMATCHx3_OP       8��6��ʾMATCHx2_OP      5��3��ʾMATCHx1_OP      2��0��ʾMATCHx1_OP
                        MATCHxx_OP 000:��cnt = matcha3_valʱ������ߵ�ƽ
                        001:��cnt = matcha3_valʱ������͵�ƽ
                        010:��cnt = matcha3_valʱ����ת��ƽ
                        011:�����
                        100����cnt=0������͵�ƽ����cnt = matcha3_valʱ����ת��ƽ��
                        101����cnt=0������ߵ�ƽ����cnt = matcha3_valʱ����ת��ƽ
                        110������ߵ�ƽ
                        111������͵�ƽ
                        ���߼���ʱ����Ч
                        */
    uint32_t matchA0;   /* ƥ��ֵ0 */
    uint32_t matchA1;   /* ƥ��ֵ1 */
    uint32_t matchA2;   /* ƥ��ֵ2 */
    uint32_t matchA3;   /* ƥ��ֵ3 */
    
    uint16_t matchBOp;
    uint32_t matchB0;
    uint32_t matchB1;
    uint32_t matchB2;
    uint32_t matchB3;
    int32_t  delta;
 }timerconfig_t;



 /******************************************************************************* 
  * API 
  ******************************************************************************/
 
 /* �ⲿ����(�ɼ� extern ����)������ */


void Hal_Timer0Handler(void);
void Hal_Timer1Handler(void);
void Hal_Timer2Handler(void);
void Hal_Timer3Handler(void);
void Hal_Timer4Handler(void);
void Hal_Timer5Handler(void);
void Hal_LptimerHandler(void);

void ADT_StartTimer(timerconfig_t *timerConfig);
void ADT_StopTimer(timerconfig_t *timerConfig);

void SetTimerIsr(T_TIMER* Timer);
void CalExtPulse(T_TIMER* Timer);
void SoftwareForceBK(T_TIMER* advTimer);
void SyncStartStop(uint8_t mode);
void SetTimer(timerconfig_t *timerConfig);

#ifdef __cplusplus
}
#endif
#endif /* __TIMER_H__ */

