#include "SystemDriver.h"
#include "Spo2Interface.h"
#include "Preprocess.h"
#include "DisplayDriver.h"
#include "DefaultSetup.h"
#include "gpio.h"
#include "saradc.h"
#include "spi.h"
#include "cmp.h"
#include "op.h"
#include "tia.h"
#include "timer.h"
#include "interrupt.h"
#include "flash.h"
#include "trace.h"
#include "uart.h"
#include "wdg.h"



/**
 * @brief      SYS_GPIO_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_GPIO_Init(void)
{
    GPIO_Init_t gpio_init;

    /**************** AON配置 *****************************/
    AON_ANACFG->reg_xosc32k_top &= ~(0x01);
    AON_ANACFG->reg_comparator_top1 &= ~(0x01);
    AON_ANACFG->reg_comparator_top2 &= ~(0x01);
    AON_ANACFG->reg_analog_block_enable &= ~((0x03 << 1) | (0x03 << 10) |
                                           (0x01 << 6));

    /**************** 系统时钟及外设时钟源相关配置 *************/
    SYSCTL->SDM_f.sw_refclk_en_sdadc = DISABLE;
    SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_rtc = DISABLE;
    SYSCTL->PSRAM_CLKRST_CTRL_f.sw_fclk_en_psram = DISABLE;
    SYSCTL->CRYPTO_CLKRST_CTRL_f.sw_fclk_en_crypto = DISABLE;
    SYSCTL->GPT0_CLKRST_CTRL_f.sw_fclk_en_gpt = DISABLE;
    SYSCTL->GPT1_CLKRST_CTRL_f.sw_fclk_en_gpt = DISABLE;
    SYSCTL->LPGPT_CLKRST_CTRL_f.sw_fclk_en_lpgpt = DISABLE;
    SYSCTL->I2C0_CLKRST_CTRL_f.sw_fclk_en_i2c = DISABLE;
    SYSCTL->I2C1_CLKRST_CTRL_f.sw_fclk_en_i2c = DISABLE;
	  SYSCTL->SPI0_CLKRST_CTRL_f.sw_fclk_en_spi = DISABLE;
    SYSCTL->SPI1_CLKRST_CTRL_f.sw_fclk_en_spi = DISABLE;
    SYSCTL->UART0_CLKRST_CTRL_f.sw_fclk_en_uart = DISABLE;
    SYSCTL->UART2_CLKRST_CTRL_f.sw_fclk_en_uart = DISABLE;
    SYSCTL->AONTOP_CLKRST_CTRL_f.sw_fclk_en_lpuart_hs = DISABLE;
    SYSCTL->WDG_CLKRST_CTRL_f.sw_fclk_en_wdg = DISABLE;

    /*************** BOOST EN GPIO配置 **************************/
    // PWR_C (PA.4)
    GPIO_ClrAFMode(GPIOPortA, GPIOPin4);
    GPIO_SetBits(GPIOPortA, GPIOPin4);
    gpio_init.GPIO_Port = GPIOPortA;
    gpio_init.GPIO_Pin  = GPIOPin4;
    gpio_init.GPIO_Mode = GPIO_OUTPUT_MODE;
    gpio_init.GPIO_Ext_Mode = GPIO_PIN_PUSH_PULL;
    gpio_init.GPIO_Pull_Mode = GPIO_PULL_NONE;
    GPIO_Init(&gpio_init);

    /**************** BOOST 输出电压检测相关GPIO配置 ***********/
    // KEY_IN (PC.7)
    GPIO_ClrAFMode(GPIOPortC, GPIOPin7);
    gpio_init.GPIO_Port = GPIOPortC;
    gpio_init.GPIO_Pin  = GPIOPin7;
    gpio_init.GPIO_Mode = GPIO_INPUT_MODE;
    gpio_init.GPIO_Ext_Mode = GPIO_PIN_OPEN_DRAIN;
    gpio_init.GPIO_Pull_Mode = GPIO_PULL_NONE;
    GPIO_Init(&gpio_init);
    // Cmp0 output (PA.5)
    GPIO_SetAFMode(GPIOPortA, GPIOPin5, AF2);
    // Cmp0 positive input (PA.6)
    GPIO_SetAFMode(GPIOPortA, GPIOPin6, AF_ANALOG);

    /**************** 红外光控制和电流控制GPIO配置 ***************/
    // Red (PC.2)
    GPIO_ClrAFMode(GPIOPortC, GPIOPin2);
    GPIO_ClearBits(GPIOPortC, GPIOPin2);
    gpio_init.GPIO_Port = GPIOPortC;
    gpio_init.GPIO_Pin  = GPIOPin2;
    gpio_init.GPIO_Mode = GPIO_OUTPUT_MODE;
    gpio_init.GPIO_Ext_Mode = GPIO_PIN_PUSH_PULL;
    gpio_init.GPIO_Pull_Mode = GPIO_PULL_NONE;
    GPIO_Init(&gpio_init);
    // Infrared (PB.4)
    GPIO_SetAFMode(GPIOPortB, GPIOPin4, AF0); //复用为IO
    GPIO_ClearBits(GPIOPortB, GPIOPin4);
    gpio_init.GPIO_Port = GPIOPortB;
    gpio_init.GPIO_Pin  = GPIOPin4;
    gpio_init.GPIO_Mode = GPIO_OUTPUT_MODE;
    gpio_init.GPIO_Ext_Mode = GPIO_PIN_PUSH_PULL;
    gpio_init.GPIO_Pull_Mode = GPIO_PULL_NONE;
    GPIO_Init(&gpio_init);
    // Complementary PWM (PB.0 & PB.1)
    GPIO_SetAFMode(GPIOPortB, GPIOPin0, AF3);
    GPIO_SetAFMode(GPIOPortB, GPIOPin1, AF3);
    // OP1 INP(PC.3) & OP1 INN(PC.4) & OP1 OUT(PC.5)
    GPIO_SetAFMode(GPIOPortC, GPIOPin3, AF_ANALOG);
    GPIO_SetAFMode(GPIOPortC, GPIOPin4, AF_ANALOG);
    GPIO_SetAFMode(GPIOPortC, GPIOPin5, AF_ANALOG);



    /****************红外光接收GPIO配置 *******************************/
    // OP0 INP & OP0 INN & OP0 OUT(PC.6)
    GPIO_SetAFMode(GPIOPortC, GPIOPin6, AF_ANALOG);
    // OP2 INP(PD.0) & OP2 OUT(PD.1)
    GPIO_SetAFMode(GPIOPortD, GPIOPin0, AF_ANALOG);
    GPIO_SetAFMode(GPIOPortD, GPIOPin1, AF_ANALOG);
    // ADC sample for SPO2 (PA.3)
    GPIO_SetAFMode(GPIOPortA, GPIOPin3, AF_ANALOG);

    /**************** 电池电量检测GPIO配置 *******************************/
    // ADC sample for battery (PA.7)
    GPIO_SetAFMode(GPIOPortA, GPIOPin7, AF_ANALOG);
}

/**
 * @brief      SYS_OPA_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_OPA_Init(void)
{
    // TIA initialize
    Tia_Init_t tia_init_t = {0};
    Opa_Init_t op_init_t = {0};

    tia_init_t.tiaHighPowerEn = ENABLE;
    tia_init_t.tiaStg1Level = TIA_STG1_LEVEL3;
    tia_init_t.tiaStg2Level = TIA_STG2_LEVEL3;
    tia_init_t.tiaOutConfig = TIA_OUT_CONECT_NONE;
    tia_init_t.tiaPosConfig = TIA_VIP_CONECT_NONE;
    tia_init_t.tiaVipSelect = TIA_VIP_SELECT_EXTERNAL;
    tia_init_t.tiaOutSelect = TIA_OUT_SELECT_PAD;
    //Tia_PortInit();
    Tia_Init(&tia_init_t);
    Tia_Enable();

    // OP1 initialize
    op_init_t.opaBufferEn   = DISABLE;
    op_init_t.opaOutPadEn   = ENABLE;
    op_init_t.opaFeedbackEn = DISABLE;
    op_init_t.opaPosConfig = OPA_VIP_PAD;
    op_init_t.opaNegConfig = OPA_VIN_PAD;
    op_init_t.opaOutSelect = OPA_OUT_PAD;
    op_init_t.opaResConfig = OPA_RES_GND;
    op_init_t.opaGainLevel = OPA_GAIN_LEVEL0;
    op_init_t.opaVipSelect = OPA_VIP_VREF_NONE;

    OPA_Init(OP_1, &op_init_t);
    OPA_Enable(OP_1);

    // OP2 initialize
    op_init_t.opaBufferEn   = ENABLE;
    op_init_t.opaOutPadEn   = ENABLE;
    op_init_t.opaFeedbackEn = DISABLE;
    op_init_t.opaPosConfig = OPA_VIP_PAD;
    op_init_t.opaNegConfig = OPA_VIN_BUFFER;
    op_init_t.opaOutSelect = OPA_OUT_PAD;
    op_init_t.opaResConfig = OPA_RES_GND;
    op_init_t.opaGainLevel = OPA_GAIN_LEVEL0;
    op_init_t.opaVipSelect = OPA_VIP_VREF_NONE;

    OP2_VinPadConfig(OP2_VIN_PAD_PC07);
    OP2_VipPadConfig(OP2_VIP_PAD_PD00);
    OP2_OutPadConfig(OP2_OUT_PAD_PD01);

    OPA_Init(OP_2, &op_init_t);
    OPA_Enable(OP_2);
}

/**
 * @brief      SYS_ADC_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_ADC_Init(void)
{
    DMA_Init(); // Initialize DMA

    ANAREG_1->reg_adsar_top = SARADC_REF_SELECT_VDD; // reference voltage

    // PA00, SpO2 Sample pin
    ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_p = SARADC_AIN0_PA00;
    ANAREG_1->reg_adsar_mux_in_f.adc_ext_input_n = SARADC_INN_GND_PAD;
    ANAREG_1->reg_adsar_mux_in_f.adc_sar_sel = 1;

    ANAREG_1->reg_adc_conf1_f.adc_fifo_format = 0x01;  /* adc_fifo_format */
    ANAREG_1->reg_adc_conf1_f.adc_smp_cycle = 14;      /* adc_smp_cycle */
    ANAREG_1->reg_adc_conf1_f.adc_wait_cycle = 21;     /* adc_wait_cycle */
    ANAREG_1->reg_adc_conf1_f.adc_dma_en = 0x01;       /* adc_dma_en */
    ANAREG_1->reg_adc_conf1_f.adc_fifo_thrhold = 0x02; /* adc_fifo_thrhold */
    ANAREG_1->reg_adc_conf1_f.adc_trig_num = 0x02;     /* adc_trig_num */
    ANAREG_1->reg_adc_conf1_f.adc_scan_chnum = 0x00;   /* adc_scan_chnum */
    ANAREG_1->reg_adc_conf1_f.adc_irq_mode = 0;

    // FCLK for SAR, default is using 16M and fclk is enable
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_en_saradc = 1;
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_pclk_en_saradc = 1;
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_div_saradc = 0;
    SYSCTL->SARADC_CLKRST_CTRL_f.sw_fclk_sel_saradc = 0;

    PeripheralsRxDmaConfig( gDmaAdcBuf,
                            (uint32_t*)(&(ANAREG_1->reg_adc_conf8)),
                            DMA_LENTH,
                            SARADC_RX_DMA_CHANNEL,
                            dmaSaradc_rx,
                            1,
                            3,
                            1,
                            0);

    //ctrl_sar_8_enable + ctrl_adsar_chargepump_0_enable
    AON_ANACFG->reg_analog_block_enable &= ~(0x01 << 1); // disable sdadc
    AON_ANACFG->reg_analog_block_enable |= ((0x01 << 7) | (0x01 << 5));

    TriggerSARAdcStart();
}

/**
 * @brief      SYS_TIMER_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_TIMER_Init(void)
{
    /* FCLK 16M, 60us定时周期 */
    TIMER2->sw_force_con = 1;
    TIMER2->latched_cnt = 0;
    TIMER2->prescale = 0;
    TIMER2->period = 965;  //965
    TIMER2->repeat_value = 0;
}

/**
 * @brief      SYS_TIMER_PWM_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_TIMER_PWM_Init(void)
{
    timerconfig_t timerConfig;

    timerConfig.timer    = TIMER4;
    timerConfig.prescale = 0;
    timerConfig.channel  = 0x3; //CHA CHB

    timerConfig.period    = PWM_GAP_FRQ - 1;
    timerConfig.matchAOp  = 4;
    timerConfig.matchA0   = 0;
    timerConfig.repeatVal = 0;

    timerConfig.mode = 0x6;
    timerConfig.wave = 0;
    timerConfig.matchnum  = 0;/* 一个匹配点 */
    timerConfig.ece  = 0;

    SetTimer(&timerConfig);
    ADT_StartTimer(&timerConfig);
}


/**
 * @brief      SYS_SPI0_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_SPI0_Init(void)
{
    /* 标准4线全双工，主机,CPOL= 0,CPHA = 0,数据位8 */
    SPI_Config config =
    {
        .spi               = SPI0,
        .slave             = 0,
        .loopback          = 0,
        .CPOL              = 0,
        .CPHA              = 0,
        .frame_size        = 8,
        .imsc              = 0,            /* 中断关掉 */
        .dmaConfig.txDmaEn = 0,
        .dmaConfig.rxDmaEn = 0,
    };

    SPI_Init(config);
}

/**
 * @brief      SYS_PressKey_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_PressKey_Init(void)
{
    Cmp_Init_t cmp_init = {0};

    cmp_init.cmpDigFilterTime = 100;
    cmp_init.cmpPosChannel = CMP_PA06_CHANNEL6;
    cmp_init.cmpNegChannel = CMP_CHANNEL_VDAC;
    cmp_init.cmpIrqMode = CMP_IT_NONE;

    Cmp_CLKConfig(COMPARATOR1, CMP_FCLK_SRC_16M, CMP_FCLK_DIV2);
    Cmp_Init(COMPARATOR1, &cmp_init);
    Cmp_VrefConfig(COMPARATOR1, CMP_VREF_1V, CMP_DAC_DIN1, 0x7F);
    Cmp_Enable(COMPARATOR1);
}

/**
 * @brief      SYS_Buzz_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_Buzz_Init(void)
{
    timerconfig_t timerConfig;

    timerConfig.timer     = TIMER3;
    timerConfig.prescale  = 15;
    timerConfig.channel   = 0x2;   /* CHB */

    timerConfig.period    = 368;
    timerConfig.matchBOp  = 4;
    timerConfig.matchB0   = 0;
    timerConfig.repeatVal = 0;

    timerConfig.mode      = 0x5;
    timerConfig.wave      = 0;
    timerConfig.matchnum  = 0;   /* 一个匹配点 */
    timerConfig.ece       = 0;

    SetTimer(&timerConfig);
    ADT_StartTimer(&timerConfig);
}

/**
 * @brief      SYS_Hardware_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
void SYS_Hardware_Init(void)
{
    SYS_GPIO_Init();

    SYS_TIMER_Init();
    SYS_TIMER_PWM_Init();

    SYS_PressKey_Init();
    SYS_OPA_Init();
    SYS_ADC_Init();
}



#if (SYS_BLE_SELECTED != 0)
/**
 * @brief      SYS_USART_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
static void SYS_USART_Init(void)
{
    uartconfig_t uartCfg;

    uartCfg.uart           = UART1,
    uartCfg.baudRate       = 115200,
    uartCfg.wordLength     = UART_WORDLENGTH_8B,
    uartCfg.stopBits       = UART_STOPBITS_1,
    uartCfg.parity         = UART_PARITY_NONE,
    uartCfg.hwFlowCtl      = UART_HWCONTROL_NONE,
    uartCfg.txPolarity    = UART_TX_POL_POSITIVE,
    uartCfg.rxPolarity    = UART_RX_POL_POSITIVE,
    uartCfg.txFifoTriggerLevel = 0;
    uartCfg.rxFifoTriggerLevel = 1;
    uartCfg.uartIrqEn         = 0;
    uartCfg.dmaConfig.rxDmaEn = 0;
    uartCfg.dmaConfig.txDmaEn = 0;
    //lpuartCfg.pinmuxOption = 0;

    UART_Init(&uartCfg);
}
#endif

/**
 * @brief      SYS_ModuleDrive_Init
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
void SYS_ModuleDrive_Init(void)
{

    /* 通过宏定义或者TX线上的电平来判定BLE是否存在 */
#if (SYS_BLE_SELECTED != 0)

    SYS_USART_Init();
    gChkFunc.chkBLH = TRUE;

#endif

    //Restroe device setup
    Setup_RestoreDevice();

    //GUI display initialize
    Display_Init();
}

/**
 * @brief      SYS_Interrupt_Enable
 *
 * @param[in]
 *
 * @details
 *
 * @retval
 * @retval
 */
void SYS_Interrupt_Enable(void)
{
    /* 使能TIMER2中断源 */
    FIC->mask1 = ~(1 << (FIC_Timer2_IRQn - FIC_Ovf_IRQn));
    /* 使能IWDG中断源 */
    AIC->mask1 &= ~AIC_IWDG_MASK;

    /* 配置FIC，AIC，LIC中断优先级 */
    NVIC_SetPriority(Lic_IRQn, 1);
    NVIC_EnableIRQ(Lic_IRQn);
    NVIC_SetPriority(Aic_IRQn, 2);
    NVIC_EnableIRQ(Aic_IRQn);
    NVIC_SetPriority(Fic_IRQn, 0);
    NVIC_EnableIRQ(Fic_IRQn);

    /* 开启TIMER2 */
    TIMER2->cfg = 0x01;
    /* Enable WDT */
    IWDG_Enable();
}


void IwdgCallback(void)
{
    if(IWDG_GetITFlag(WDG_IOVF_FLAG))
    {
        IWDG_ClearITPending(WDG_IOVF_FLAG);
        GPIO_ClrAFMode(GPIOPortC, GPIOPin0);
        NVIC_SystemReset();
    }
}




