#ifndef __OLED_I2C_H__
#define __OLED_I2C_H__
#include "i2c.h"





void TM_I2C_Init(void);

void TM_I2C_Write(T_I2C* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

void TM_I2C_WriteMulti(T_I2C* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);


#endif
