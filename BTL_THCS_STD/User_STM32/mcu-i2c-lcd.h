#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stm32f10x.h"
#include "mcu-i2c.h"
#include "mcu-delay.h"
#include <stdint.h>

#define I2C_LCD_ADDR 0x4E

#define I2C_LCD_Delay_Ms(u16DelayMs) Delay_Ms(u16DelayMs)

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3

void I2Cx_LCD_Init(void);
void I2Cx_LCD_Puts(char *szStr);
void I2Cx_LCD_Clear(void);
void I2Cx_LCD_Line2(void);
void I2Cx_LCD_Line3(void);
void I2Cx_LCD_Line4(void);
void I2Cx_LCD_BackLight(uint8_t u8BackLight);

#endif
