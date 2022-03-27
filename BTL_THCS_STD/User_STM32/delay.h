#ifndef __DELAY_H
#define __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

void Timer2_StandardLibrary(void);

void delay_ms(unsigned long ms);
void delay_us(unsigned long us);

void Delay_ms(uint16_t time);

//void SysTick_Init(void);
//void SysTick_Handler(void);
//uint64_t SysTick64(void);
//uint32_t SysTick32(void);
//uint32_t SysTick24(void);
//uint64_t SysTick_Millis(void);
//uint64_t SysTick_Micros(void);
//void delay_us(unsigned long us);
//void delay_ms(unsigned long ms);


#endif
