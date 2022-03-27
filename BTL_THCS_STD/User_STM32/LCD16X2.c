#include "lcd16x2.h"


#define LCD_RS GPIO_Pin_3	// Ch�n A3 STM32 n�i voi ch�n RS
#define LCD_EN GPIO_Pin_4	// Ch�n A4 STM32 n�i voi ch�n EN
#define LCD_D4 GPIO_Pin_5	// Ch�n A5 STM32 n�i voi ch�n D4
#define LCD_D5 GPIO_Pin_6	// Ch�n A6 STM32 n�i voi ch�n D5
#define LCD_D6 GPIO_Pin_7	// Ch�n A7 STM32 n�i voi ch�n D6
#define LCD_D7 GPIO_Pin_0	// Ch�n B0 STM32 n�i voi ch�n D7

/*Khoi tao bien cau hinh LCD*/
GPIO_InitTypeDef GPIO_LCD_InitStruction;

/* H�m c�u h�nh ch�n LCD */
void GPIO_LCD_Config(void){

	/*Cap clock cho GPIO v� port su dung*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);

	GPIO_LCD_InitStruction.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_LCD_InitStruction.GPIO_Pin 	= LCD_D4|LCD_D5|LCD_D6|LCD_EN|LCD_RS;
	GPIO_LCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_LCD_InitStruction);
	
	GPIO_LCD_InitStruction.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_LCD_InitStruction.GPIO_Pin 	= LCD_D7;
	GPIO_LCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_LCD_InitStruction);
	
}

/* H�m tao xung ch�t gui du li�u */
void LCD_Enable(void){

	GPIO_SetBits(GPIOA, LCD_EN);  // K�o ch�n EN l�n muc cao
	delay_us(5);									// Tr� 5us
	GPIO_ResetBits(GPIOA, LCD_EN);// K�o ch�n EN l�n muc th�p
	delay_us(50);									// Tr� 50us
}

/* H�m ghi 4 bit du lieu v�o LCD */
void LCD_Send4Bit(unsigned char Data){
	
	// L�y v� ki�m tra bit th�p nh�t
	// n�u bit = 1 th� xu�t D4 muc cao
	// Nguoc lai th� xu�t D4 muc th�p
	GPIO_WriteBit(GPIOA, LCD_D4, Data & 0x01);
	
	// Tuong tu voi c�c bit ti�p theo
	GPIO_WriteBit(GPIOA, LCD_D5, (Data>>1)&1);
	GPIO_WriteBit(GPIOA, LCD_D6, (Data>>2)&1);
	GPIO_WriteBit(GPIOB, LCD_D7, (Data>>3)&1);
}

/* H�m gui l�nh ra m�n h�nh LCD */
void LCD_SendCommand(unsigned char command){

	LCD_Send4Bit(command >> 4); // Gui 4 bit cao truoc
	LCD_Enable();								// ch�t du lieu
	LCD_Send4Bit(command);			// gui 4 bit th�p
	LCD_Enable();								// ch�t du lieu
}

/* H�m x�a m�n h�nh LCD */
void LCD_Clear(void){

	LCD_SendCommand(0x01); // Gui l�nh x�a m�n h�nh
	delay_ms(2);					 // Tr� 10us
}

/* H�m khoi tao LCD */
void LCD_Init(void){
	
	// �at c�c ch�n RS, EN, RW ban d�u muc th�p
	GPIOA->BRR = LCD_RS;
	GPIOA->BRR = LCD_EN;
	
	
	LCD_Send4Bit(0x03); // Gui gi� tri 0x03 d� khoi tao
	delay_ms(5);				// Tr� 5ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x03); // Gui gi� tri 0x03 d� khoi tao
	delay_ms(5);				// Tr� 5ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x03); // Gui gi� tri 0x03 d� khoi tao
	delay_ms(1);				// Tr� 1ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x02); // Gui gi� tri 0x02 d� khoi tao
	LCD_Enable();
	
	LCD_SendCommand( 0x28 ); // LCD hi�n thi 2 d�ng, font 5x7, ch� do 4-bit
	delay_ms(1);
	LCD_SendCommand( 0x0C ); // cho phep hien thi man hinh va tat con tro
	delay_ms(1);
	LCD_SendCommand( 0x06 ); // Tu d�ng tang cursor (tu dong dich phai)
	delay_ms(1); 
	LCD_SendCommand( 0x01 ); // xoa toan bo khung hinh  
	delay_ms(2);
}

// H�m dat vi tr� con tro */
void LCD_Gotoxy(unsigned char x, unsigned char y){

	unsigned char address;
	if(y == 0)address=(0x80+x);				//
	else if(y == 1) address=(0xc0+x); //

	delay_ms(1);
	
	// ghi v�o DDRAM vi tr� cua con tro
	LCD_SendCommand(address);
	delay_ms(1);
}

/* H�m gii k� tu ra m�n h�nh LCD */
void LCD_PutChar(unsigned char Data){

	GPIO_SetBits(GPIOA, LCD_RS);
	LCD_SendCommand(Data); // Gui k� tu ra m�n h�nh LCD
	GPIO_ResetBits(GPIOA, LCD_RS);
}

/* H�m gui chu�i k� tu ra m�n h�nh LCD */
void LCD_Puts(char *s){

	// Trong khi k� tu kh�c 0 (null)
	while (*s){
		
		LCD_PutChar(*s); // Gui k� tu ra m�n h�nh
		s++;						 // Tang vi tr� k� tu th�m 1
	}
}
