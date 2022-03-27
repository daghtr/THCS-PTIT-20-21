#ifndef MCU_DELAY_H
#define MCU_DELAY_H
#include "stm32f10x.h"
/*ham cau hinh timer*/
void Config_Timer(void);
/*---------------------------------------*/
void Delay_1ms(void);
/*---------------------------------------*/
void Delay_ms(unsigned int time_ms);
/*---------------------------------------*/
void Delay_1us(void);
/*---------------------------------------*/
void Delay_us(unsigned int time_us);


#endif



