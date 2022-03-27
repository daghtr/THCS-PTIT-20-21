#include "lcd16x2.h"


#define LCD_RS GPIO_Pin_3	// Chân A3 STM32 nôi voi chân RS
#define LCD_EN GPIO_Pin_4	// Chân A4 STM32 nôi voi chân EN
#define LCD_D4 GPIO_Pin_5	// Chân A5 STM32 nôi voi chân D4
#define LCD_D5 GPIO_Pin_6	// Chân A6 STM32 nôi voi chân D5
#define LCD_D6 GPIO_Pin_7	// Chân A7 STM32 nôi voi chân D6
#define LCD_D7 GPIO_Pin_0	// Chân B0 STM32 nôi voi chân D7

/*Khoi tao bien cau hinh LCD*/
GPIO_InitTypeDef GPIO_LCD_InitStruction;

/* Hàm câu hình chân LCD */
void GPIO_LCD_Config(void){

	/*Cap clock cho GPIO và port su dung*/
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

/* Hàm tao xung chôt gui du liêu */
void LCD_Enable(void){

	GPIO_SetBits(GPIOA, LCD_EN);  // Kéo chân EN lên muc cao
	delay_us(5);									// Trê 5us
	GPIO_ResetBits(GPIOA, LCD_EN);// Kéo chân EN lên muc thâp
	delay_us(50);									// Trê 50us
}

/* Hàm ghi 4 bit du lieu vào LCD */
void LCD_Send4Bit(unsigned char Data){
	
	// Lây và kiêm tra bit thâp nhât
	// nêu bit = 1 thì xuât D4 muc cao
	// Nguoc lai thì xuât D4 muc thâp
	GPIO_WriteBit(GPIOA, LCD_D4, Data & 0x01);
	
	// Tuong tu voi các bit tiêp theo
	GPIO_WriteBit(GPIOA, LCD_D5, (Data>>1)&1);
	GPIO_WriteBit(GPIOA, LCD_D6, (Data>>2)&1);
	GPIO_WriteBit(GPIOB, LCD_D7, (Data>>3)&1);
}

/* Hàm gui lênh ra màn hình LCD */
void LCD_SendCommand(unsigned char command){

	LCD_Send4Bit(command >> 4); // Gui 4 bit cao truoc
	LCD_Enable();								// chôt du lieu
	LCD_Send4Bit(command);			// gui 4 bit thâp
	LCD_Enable();								// chôt du lieu
}

/* Hàm xóa màn hình LCD */
void LCD_Clear(void){

	LCD_SendCommand(0x01); // Gui lênh xóa màn hình
	delay_ms(2);					 // Trê 10us
}

/* Hàm khoi tao LCD */
void LCD_Init(void){
	
	// Ðat các chân RS, EN, RW ban dâu muc thâp
	GPIOA->BRR = LCD_RS;
	GPIOA->BRR = LCD_EN;
	
	
	LCD_Send4Bit(0x03); // Gui giá tri 0x03 dê khoi tao
	delay_ms(5);				// Trê 5ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x03); // Gui giá tri 0x03 dê khoi tao
	delay_ms(5);				// Trê 5ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x03); // Gui giá tri 0x03 dê khoi tao
	delay_ms(1);				// Trê 1ms
	LCD_Enable();
	
	
	LCD_Send4Bit(0x02); // Gui giá tri 0x02 dê khoi tao
	LCD_Enable();
	
	LCD_SendCommand( 0x28 ); // LCD hiên thi 2 dòng, font 5x7, chê do 4-bit
	delay_ms(1);
	LCD_SendCommand( 0x0C ); // cho phep hien thi man hinh va tat con tro
	delay_ms(1);
	LCD_SendCommand( 0x06 ); // Tu dông tang cursor (tu dong dich phai)
	delay_ms(1); 
	LCD_SendCommand( 0x01 ); // xoa toan bo khung hinh  
	delay_ms(2);
}

// Hàm dat vi trí con tro */
void LCD_Gotoxy(unsigned char x, unsigned char y){

	unsigned char address;
	if(y == 0)address=(0x80+x);				//
	else if(y == 1) address=(0xc0+x); //

	delay_ms(1);
	
	// ghi vào DDRAM vi trí cua con tro
	LCD_SendCommand(address);
	delay_ms(1);
}

/* Hàm gii ký tu ra màn hình LCD */
void LCD_PutChar(unsigned char Data){

	GPIO_SetBits(GPIOA, LCD_RS);
	LCD_SendCommand(Data); // Gui ký tu ra màn hình LCD
	GPIO_ResetBits(GPIOA, LCD_RS);
}

/* Hàm gui chuôi ký tu ra màn hình LCD */
void LCD_Puts(char *s){

	// Trong khi ký tu khác 0 (null)
	while (*s){
		
		LCD_PutChar(*s); // Gui ký tu ra màn hình
		s++;						 // Tang vi trí ký tu thêm 1
	}
}
