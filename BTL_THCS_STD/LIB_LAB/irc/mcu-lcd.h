/*
  *****************************************************************************
  * @file			mcu-lcd.h                                                              *
  * @author			Thai Salem                                               
  * @date			00/00/0000                                                         
  ******************************************************************************
*/

#ifndef MCU_LCD_H
#define MCU_LCD_H

/*
author: Thai MCU
*/

#include "stm32f10x.h"
/*ham cau hinh GPIO cho LCD*/
void LCD_ConfigIO(void);
/*ham cho phep LCD hoat dong*/
void LCD_Enable(void);
/**/
void LCD_Send4Bit(unsigned char Data);
/*ham gui lenh*/
void LCD_SendCommand(unsigned char command);
/*ham xoa man hinh*/
void LCD_Clear(void);
/*ham khoi tao LCD*/
void LCD_Init(void);
/*ham dich chuyen vi tri cua con tro chuot*/
void LCD_Gotoxy(unsigned char x, unsigned char y);
/*ham gui 1 ki tu*/
void LCD_PutChar(unsigned char Data);
/*ham gui 1 chuoi ki tu*/
void LCD_Puts(unsigned char *str);
/*tao ki tu k co san.*/
void LCD_Custom(unsigned char loc, unsigned char *msg);

void LCD_Posi_Char(unsigned char row, unsigned char column, unsigned char out_char);

void LCD_Posi_Str(unsigned char row, unsigned char column, unsigned char* text);
/**********************************************************/

///*ham cau hinh timer*/
//void Config_Timer(void);
///*---------------------------------------*/
//void Delay_1ms(void);
///*---------------------------------------*/
//void Delay_ms(unsigned int time_ms);
///*---------------------------------------*/
//void Delay_1us(void);
///*---------------------------------------*/
//void Delay_us(unsigned int time_us);

#endif
