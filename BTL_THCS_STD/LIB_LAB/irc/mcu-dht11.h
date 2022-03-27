//#ifndef _MCU_DHT11_H
//#define _MCU_DHT11_H 
//#include "stm32f10x.h"
//#include "mcu-gpio.h"

//////////////////////////////////////////////////////////////////////////////////////////
////xem tai lieu STM32----.html (2F1)
////IO - config for A0

//#define 		DHT11_IO_IN()  		{GPIOA->CRL&=0x0FFFFFFF;GPIOA->CRL|=8<<28;}// mode input pullup/pulldown
//#define 		DHT11_IO_OUT() 		{GPIOA->CRL&=0x0FFFFFFF;GPIOA->CRL|=3<<28;}// mode output push-pull

////IO										   
//#define			DHT11_DQ_OUT 			PAout(7) 
//#define			DHT11_DQ_IN  			PAin(7) 	

//////////////////////////////////////////////////////////////////////////////////////////

////#define DHT11_IO_IN()  {GPIOA->CRH&=0xFFFFFFF0;GPIOA->CRH|=8<<0;}
////#define DHT11_IO_OUT() {GPIOA->CRH&=0xFFFFFFF0;GPIOA->CRH|=3<<0;}

////#define	DHT11_DQ_OUT 		PAout(8) 
////#define	DHT11_DQ_IN  		PAin(8) 

//u8 DHT11_Init(void);
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);
//u8 DHT11_Read_Byte(void);
//u8 DHT11_Read_Bit(void);
//u8 DHT11_Check(void);
//void DHT11_Rst(void);  
//#endif

#ifndef MCU_DHT11_H_
#define MCU_DHT11_H_
//---------------------------------------------
#include "stm32f10x.h" 

void DHT11_Init(void);
void DHT11_send(u8 data);
uint8_t DHT11_Read(uint8_t *pu8Data);

#endif













