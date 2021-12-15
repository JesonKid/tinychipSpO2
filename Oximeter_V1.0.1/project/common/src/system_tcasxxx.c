/*************************************************************************************
    Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
**************************************************************************************/
#include "tcasxxx.h"
#include "system_tcasxxx.h"
#include "uart.h"
#include "trim.h"
#include "flash.h"

 
uint32_t  SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

#if defined   ( __ICCARM__   ) /* iar */

#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = ".textrw"
#pragma section = ".textrw_init" 

void CopyDataRWtoImage(void)
{
    uint32_t n;

    // ¿½±´ .data ¶Î
    uint8_t *data_ram     = __section_begin(".data");
    uint8_t *data_rom     = __section_begin(".data_init");
    uint8_t *data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;
    if (data_ram != data_rom)
    {
        while (n)
        {
            *data_ram++ = *data_rom++;
            n--;
        }
    }

    // ÇåÁã .bss ¶Î
    uint8_t *bss_start = __section_begin(".bss");
    uint8_t *bss_end   = __section_end(".bss");
    n = bss_end - bss_start;
    while (n)
    {
        *bss_start++ = 0;
        n--;
    }

    // ¿½±´ .textrw ¶Î
    uint8_t *code_relocate_ram     = __section_begin(".textrw");
    uint8_t *code_relocate_rom     = __section_begin(".textrw_init");
    uint8_t *code_relocate_rom_end = __section_end(".textrw_init");
    n = code_relocate_rom_end - code_relocate_rom;
    while (n)
    {
        *code_relocate_ram++ = *code_relocate_rom++;
        n--;
    }
}

#elif defined (__CC_ARM) /* Keil ¦ÌVision 5.29.0.0 */

extern uint32_t Load$$RW_IRAM1$$Base;     /* Load Address of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$Base;    /* Exec Address of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$Length;  /* Length of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$ZI$$Base;
extern uint32_t Image$$RW_IRAM1$$ZI$$Limit;

void CopyDataRWtoImage(void)
{
    uint32_t *src;
    uint32_t *dst;
    uint32_t length;

    dst    = &(Image$$RW_IRAM1$$Base);
    src    = &(Load$$RW_IRAM1$$Base);
    length = (unsigned int)&(Image$$RW_IRAM1$$Length);
    length /= sizeof(unsigned int);

    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--;
        }
    }
}

#endif

uint32_t SystemGetHClkFreq(void)
{
    uint32_t u32Val = 0;
    uint8_t hclkSel = 0;
    hclkSel = ACLKRST->hclk_sel;
    switch (hclkSel)
    {
        // clk16m
        case 0x00:
        case 0x02:
        {
            if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_27_DATA)
                u32Val = 27000000ul;
            else if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_16_DATA)
                u32Val = 16000000ul;
            else
                u32Val = 16000000ul;
        }
        break;
        // clk32k
        case 0x04:
        case 0x05:
        {
            u32Val = 32768ul;
            break;
        }
        // fmult
        case 0x01:
        case 0x03:
        {
            if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_27_DATA)
                u32Val = 54000000ul;
            else if (ANAREG_TRIM_MISC->trim_oscrc16m_value == RC16M_TRIM_16_DATA)
                u32Val = 32000000ul;
            else
                u32Val = 32000000ul;
            break;
        }
        // dfll
        case 0x06:
        case 0x07:
        {
            u32Val = 32000000ul;
            break;
        }
        default:
            u32Val = 0u;
            break;
    }

    return u32Val;
}

void SystemCoreClockUpdate (void)
{
    /*Later update SystemCoreClock according to clock tree*/
    SystemCoreClock = SystemGetHClkFreq();
}

void SystemCoreSwitchHclkSrc(enumSystemHclkSrc_t hclkSrc)
{
    uint32_t i;
    /* The three bit should be changed only one bit a time */
    switch (hclkSrc)
    {
        case HCLK_SRC_RC16M:
            SYSCTL->SDM_f.sw_16m_sel = 0x00;
            ACLKRST->hclk_sel = 0x0;
            break;
        case HCLK_SRC_XTAL16M:
            SYSCTL->SDM_f.sw_16m_sel = 0x01;
            ACLKRST->hclk_sel = 0x0;
            break;
        case HCLK_SRC_FMULT:
            ACLKRST->hclk_sel = 0x1;
            break;
        case HCLK_SRC_RC32K:
            ACLKRST->clk_en_f.sw_32k_sel = 0x00;
            ACLKRST->hclk_sel = 0x4;
            break;
        case HCLK_SRC_OSC32K:
            ACLKRST->clk_en_f.sw_32k_sel = 0x01;
            ACLKRST->hclk_sel = 0x4;
            break;
        default:
            break;
    }
    for (i = 0; i < 16*64; i ++) __NOP();
}



void SystemConfigFlash(void)
{
    if(SystemCoreClock > 30000000ul)
    {
        Flash_ConfigRCTC(1);
    }
    else
    {
        Flash_ConfigRCTC(0);
    }
}




void SystemAnlogTrim()
{
#if 0
    //LPLDO trim
    AON_ANACFG->reg_vbias_ibias_top = 0x3D;
    //LPLDO trim
    AON_ANACFG_TRIM->trim_lpldo_value = 0x11;
    //RC16M trim
    ANAREG_TRIM_MISC->trim_oscrc16m_value = RC16M_TRIM_16_DATA;
    AON_ANACFG_TRIM->trim_oscrc32k_value = 0x385;
    //VDAC trim
    ANAREG_TRIM_MISC->trim_vdac_value = 0x840;
    AON_ANACFG->reg_ldo3_top = 0x103f;
#endif
}

boolean_t SysCheckTrimValid(uint32_t u32Addr)
{
    uint32_t nReadData = 0; 

    M0P_FLASH->ECONF_f.nvr_select = 1;        /* NVR selected */

    nReadData = *(volatile uint32_t *)(u32Addr);

    if(0x10800C10 == u32Addr)           //app_start_addr
    {
        if(nReadData == 0x10000000)
        {
            return TRUE;
        }
    }
    else if(0x10800C5C == u32Addr)       //app_sector_size
    {
        if((nReadData & 0x00FFFF00) == 0)
        {
            return TRUE;
        }
    }
    else if(0x10800C0C == u32Addr)       //app_start_index_disable
    {
        if((nReadData & 0x01000000) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

void SystemInit (void)
{
    SystemCoreSwitchHclkSrc(CONFIG_SYSTEM_CORE_CLK);
    if (((PMU->sleep_mode & 0x3) == 0x01) || ((PMU->sleep_mode & 0x3) == 0x02))
    {
        //sleepwalk or sleep mode, retention sram is on, keep old value
        //*((uint32_t *)(0x00023000))= 0x33333333; 
    }else{
    //*((uint32_t *)(0x00023000))= 0x11111111;
    CopyDataRWtoImage(); 
    }
    SystemAnlogTrim();
    SystemCoreClockUpdate();  
    SystemConfigFlash();
    
    if(!SysCheckTrimValid(0x10800C10)
       || !SysCheckTrimValid(0x10800C5C)
       || !SysCheckTrimValid(0x10800C0C))
    {
        while(1);
    }
	
	// Use Trim Value to calibrate 16M RC Oscillator
	ANAREG_TRIM_MISC->trim_oscrc16m_value = ((*(volatile uint32_t *)0x10800d18)&0xFFFF);
}
