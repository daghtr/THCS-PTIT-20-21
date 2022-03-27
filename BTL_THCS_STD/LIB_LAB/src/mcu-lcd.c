#include "mcu-lcd.h"
#include "mcu-gpio.h"
#include "mcu-delay.h"
#include "string.h" 

/*
author: Thai MCU
*/

#define LCD_RS      GPIO_Pin_12    
#define LCD_EN      GPIO_Pin_5
#define LCD_D4      GPIO_Pin_6
#define LCD_D5      GPIO_Pin_7
#define LCD_D6      GPIO_Pin_8
#define LCD_D7      GPIO_Pin_9


GPIO_InitTypeDef GPIO_LCD_InitStruction;
void GPIO_LCD_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
  GPIO_LCD_InitStruction.GPIO_Pin     = LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_EN;
  GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_LCD_InitStruction);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
  GPIO_LCD_InitStruction.GPIO_Pin     = LCD_RS;
  GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_LCD_InitStruction);
	
}
void LCD_Enable(void)
{
  GPIO_SetBits(GPIOB, LCD_EN);
  Delay_us(3);
  GPIO_ResetBits(GPIOB, LCD_EN);
  Delay_us(50); 
}
 
void LCD_Send4Bit(unsigned char Data)
{
  GPIO_WriteBit(GPIOB, LCD_D4,  Data & 0x01);
  GPIO_WriteBit(GPIOB, LCD_D5, (Data>>1)&1);
  GPIO_WriteBit(GPIOB, LCD_D6, (Data>>2)&1);
  GPIO_WriteBit(GPIOB, LCD_D7, (Data>>3)&1);
}

void LCD_SendCommand(unsigned char command)
{
  LCD_Send4Bit(command >> 4);
  LCD_Enable();
  LCD_Send4Bit(command);
  LCD_Enable();

}

void LCD_Clear(){
  LCD_SendCommand(0x01);
  Delay_us(10);
}

void LCD_Init(){
	GPIO_LCD_Config();
  LCD_Send4Bit(0x00);
  Delay_ms(20);
  GPIOA->BRR = LCD_RS;
  //GPIOA->BRR = LCD_RW;
  LCD_Send4Bit(0x03);
  LCD_Enable();
  Delay_ms(5);
  LCD_Enable();
  Delay_us(100);
  LCD_Enable();
  LCD_Send4Bit(0x02);
  LCD_Enable();
  LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
  LCD_SendCommand(0x0c); // cho phep hien thi man hinh
  LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
  LCD_SendCommand(0x01); // xoa toan bo khung hinh
}

void LCD_Puts(unsigned char *s){
  while (*s){
    LCD_PutChar(*s);
    s++;
  }
}

void LCD_PutChar(unsigned char Data){
  GPIO_SetBits(GPIOA, LCD_RS);
  LCD_SendCommand(Data);
  GPIO_ResetBits(GPIOA, LCD_RS);
}

void LCD_Gotoxy(unsigned char x, unsigned char y){
  unsigned char address;
  if(y == 0)address=(0x80+x);
  else if(y == 1) address=(0xc0+x);
  Delay_ms(1);
  LCD_SendCommand(address);
  Delay_us(50);
}

/*tao ky tu dac biet*/
void LCD_Custom(unsigned char count, unsigned char *msg){
	unsigned char i;
	if(count < 8){
	/* Command 0x40 and onwards forces the device to point CGRAM address */
		LCD_SendCommand (0x40 + (count * 8));
		for(i=0;i<8;i++) /* Write 8 byte for generation of 1 character */
			LCD_PutChar(msg[i]);      
		}   
}

/*-------------------------------------*-
	Lcd_Chr
	- In ky tu ra man hinh tai (row, column)
-*-------------------------------------*/
void LCD_Posi_Char(unsigned char row, unsigned char column, unsigned char out_char){
	unsigned char add;
	add = (row==1?0x80:0xC0);
	add += (column - 1);
	LCD_SendCommand(add);
	LCD_PutChar(out_char);
}

/*-------------------------------------*-
	Lcd_Out
	- In chuoi (text) ra man hinh
	- Vi tri bat dau tai (row, column)
-*-------------------------------------*/
void LCD_Posi_Str(unsigned char row, unsigned char column, unsigned char* text){
	unsigned char add;
	add = (row==1?0x80:0xC0);
	add += (column - 1);
	LCD_SendCommand(add);
	LCD_Puts(text); 
}


