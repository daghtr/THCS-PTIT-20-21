/*
  *****************************************************************************
  * @file			mcu-tim.h                                                              *
  * @author			Thai Salem                                               
  * @date			00/00/0000                                                         
  ******************************************************************************
*/

#ifndef MCU_TIM_H
#define MCU_TIM_H

#include "stm32f10x.h"

/*
TIM1: CH1 - PA8
			CH2 - PA9
			CH3 - PA10
			CH4 - PA11

TIM2: CH1 - PA0
			CH2 - PA1
			CH3 - PA2
			CH4 - PA3
			
TIM3: CH1 - PA6
			CH2 - PA7
			CH3 - PB0
			CH4 - PB1
			
TIM4: CH1 - PB6
			CH2 - PB7
			CH3 - PB8
			CH4 - PB9
			
*/  


/* define counter mode */
#define			UP				 								TIM_CounterMode_Up                
#define 		DOWN					  					TIM_CounterMode_Down              
#define 		ALIGNED1									TIM_CounterMode_CenterAligned1    
#define 		ALIGNED2									TIM_CounterMode_CenterAligned2     
#define 		ALIGNED3									TIM_CounterMode_CenterAligned3 

typedef enum {
	Channel_1,
	Channel_2,
	Channel_3,
	Channel_4
} PWM_Channel_t;


void TIMER_InitBasic(TIM_TypeDef* TIMx,  uint16_t CounterMode, uint16_t Period, uint16_t Prescaler, uint8_t Key_EXT);
void PWM_Init(TIM_TypeDef * TIMx, PWM_Channel_t Channel, uint16_t Period, uint16_t Prescaler);
void PWM_OCx(TIM_TypeDef * TIMx, PWM_Channel_t Channel);
void PWM_PinChannel(TIM_TypeDef * TIMx, PWM_Channel_t Channel);


#endif
