
/*
  ******************************************************************************
  * @file		mcu-usart.h                                                              
  * @author		Thai Salem                                                
  * @date		69/96/6996                                                        
  ******************************************************************************
*/

#ifndef MCU_USART_H
#define MCU_USART_H
/*******************************************************************************************************/
	
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x_usart.h"	
#include "stdio.h"
#include "string.h"
	 
/*******************************************************************************************************/
/*
	PA9PA10,		//USART1
	PA2PA3,		  //USART2
	PB10PB11,		//USART3
*/
/*******************************************************************************************************/
/*usart configure*/
void USARTx_Init(USART_TypeDef* USART_x, uint32_t USART_Baud);
/*******************************************************************************************************/
/*The function passes a character*/
void USARTx_Sendc(USART_TypeDef* USART_x, char c);
/*******************************************************************************************************/
/*string transmission function*/
void USARTx_Sends(USART_TypeDef* USART_x, char* s);
/*******************************************************************************************************/
/*function recever a character*/
uint16_t USARTx_Getc(USART_TypeDef* USARTx);
/*******************************************************************************************************/
/*function convert int -> string*/
void USARTx_InStr(unsigned int vri_Number);
/*******************************************************************************************************/
/*the function compare two strings*/
int USARTx_Compare(char vrc_CpStr[],char vrc_CpQtr[]);


#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* THAI E&C *****END OF FILE****/




