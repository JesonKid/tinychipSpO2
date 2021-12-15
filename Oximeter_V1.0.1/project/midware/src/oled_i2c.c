#include "stdio.h"
#include "delay.h"
#include "oled_i2c.h"



void TM_I2C_Init(void)
{
    i2cconfig_t i2c0Cfg;
    
    i2c0Cfg.i2c =   I2C0;
    i2c0Cfg.pinmuxOption = 1;
    i2c0Cfg.slaveen = 0;  //master
    i2c0Cfg.icen = 0;
    i2c0Cfg.dmaConfig.txDmaEn    = 0;
    i2c0Cfg.dmaConfig.rxDmaEn    = 0;
    I2C_Init(&i2c0Cfg);

}

void TM_I2C_Write(T_I2C* I2Cx, uint8_t address, uint8_t reg, uint8_t data)
{
    /* start */
    I2Cx->I2CSCR =0x0018000|(address<<1)|((2 - 1) << 23); /* start = 1 restart = 1 target_rwn = 0 */
    /* data */
    I2Cx->I2CTDR = reg;
    I2Cx->I2CTDR = data;
    while((I2Cx->I2CSTR&I2C_I2CSTR_BUSY) == 1);
    /* end */
    I2Cx->I2CSCR |= 1<<17;

//  TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
//  TM_I2C_WriteData(I2Cx, reg);
//  TM_I2C_WriteData(I2Cx, data);
//  TM_I2C_Stop(I2Cx);
}


void TM_I2C_WriteMulti(T_I2C* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count)
{
    /* start */
    //I2Cx->I2CSCR =0x0018000|(address<<1)|((1-1) << 23); /* start = 1 restart = 1 target_rwn = 0 */
    /* data */
    //I2Cx->I2CTDR = reg;
    /* end */
    //I2Cx->I2CSCR |= 1<<17;

    while (count--)
    {
        /* start */
        I2Cx->I2CSCR =0x0018000|(address<<1)|((2-1) << 23); /* start = 1 restart = 1 target_rwn = 0 */
        /* data */
        I2Cx->I2CTDR = reg;
        I2Cx->I2CTDR = *data++;
        while((I2Cx->I2CSTR&I2C_I2CSTR_BUSY) == 1);
        /* end */
        I2Cx->I2CSCR |= 1<<17;
    }

//  TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
//  TM_I2C_WriteData(I2Cx, reg);
//  while (count--) {
//      TM_I2C_WriteData(I2Cx, *data++);
//  }
//  TM_I2C_Stop(I2Cx);
}

