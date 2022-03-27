#include "stm32f10x_tim.h"
#include "mcu-tim.h"
#include "mcu-gpio.h"

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

TIM_OCInitTypeDef				pwm_typedef;
TIM_TimeBaseInitTypeDef timer_typedef;

void PWM_PinChannel(TIM_TypeDef * TIMx, PWM_Channel_t Channel){
	
	if(TIMx == TIM1){
		if (Channel == Channel_1){
			pinMode(PA8,AF_PP);	
		}
		else if(Channel == Channel_2){
			pinMode(PA9,AF_PP);	
		}
		else if(Channel == Channel_3){
			pinMode(PA10,AF_PP);	
		}
		else if(Channel == Channel_4){
			pinMode(PA11,AF_PP);	
		}
	}
	
	
	else if(TIMx == TIM2){
		if (Channel == Channel_1){
			pinMode(PA0,AF_PP);	
		}
		else if(Channel == Channel_2){
			pinMode(PA1,AF_PP);	
		}
		else if(Channel == Channel_3){
			pinMode(PA2,AF_PP);
		}
		else if(Channel == Channel_4){
			pinMode(PA3,AF_PP);
		}
	}
	
	
	else if(TIMx == TIM3){
		if (Channel == Channel_1){
			pinMode(PA6,AF_PP);	
		}
		else if(Channel == Channel_2){
			pinMode(PA7,AF_PP);	
		}
		else if(Channel == Channel_3){
			pinMode(PB0,AF_PP);
		}
		else if(Channel == Channel_4){
			pinMode(PB1,AF_PP);
		}
	}
	
  else if(TIMx == TIM4){
		if (Channel == Channel_1){
			pinMode(PB6,AF_PP);
		}
		else if(Channel == Channel_2){
			pinMode(PB7,AF_PP);
		}
		else if(Channel == Channel_3){
			pinMode(PB8,AF_PP);
		}
		else if(Channel == Channel_4){
			pinMode(PB9,AF_PP);
		}
	}
	
}


void PWM_OCx(TIM_TypeDef * TIMx, PWM_Channel_t Channel){
//	TIM_OCInitTypeDef				pwm_typedef;
	pwm_typedef.TIM_OCMode = TIM_OCMode_PWM1;
	pwm_typedef.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm_typedef.TIM_OutputState = TIM_OutputState_Enable;
//	pwm_typedef.TIM_Pulse = 0;
	
	if (Channel == Channel_1){
			TIM_OC1Init(TIMx, &pwm_typedef);
		//	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
		}
	else if (Channel == Channel_2){
			TIM_OC2Init(TIMx, &pwm_typedef);
		//	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
		}
	else if (Channel == Channel_3){
			TIM_OC3Init(TIMx, &pwm_typedef);
		//	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
		}
	else if (Channel == Channel_4){
			TIM_OC4Init(TIMx, &pwm_typedef);
		//	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
		}
}

/*
TIMER_Init: function using parameter configuration for TIMx
- Period: gia tri bo dem
- Prescaler: bo chia tan
- Key_EXT: bien cho phep su dung ngat hay k. 1: enable/ 0: disable
*/
void TIMER_InitBasic(TIM_TypeDef * TIMx,  uint16_t CounterMode, uint16_t Period, uint16_t Prescaler, uint8_t Key_EXT){
	
	TIM_TimeBaseInitTypeDef timer_typedef;
	
	if(TIMx == TIM1){
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		if(Key_EXT == 1){
			TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
			NVIC_EnableIRQ(TIM1_UP_IRQn); //TIM1 Update Interrupt  
		}
	}
	
	if(TIMx == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
		if(Key_EXT == 1){
			TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
			NVIC_EnableIRQ(TIM2_IRQn);
		}
	}
	if(TIMx == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		if(Key_EXT == 1){
			TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
			NVIC_EnableIRQ(TIM3_IRQn);
		}
	}
	if(TIMx == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		if(Key_EXT == 1){
			TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
			NVIC_EnableIRQ(TIM4_IRQn);
		}
	}
	
	timer_typedef.TIM_Period				 		= Period - 1;
	timer_typedef.TIM_Prescaler 		 		= Prescaler - 1;
	timer_typedef.TIM_CounterMode 	 		= CounterMode;
	timer_typedef.TIM_ClockDivision 		= 0;
	timer_typedef.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIMx,&timer_typedef);
	TIM_Cmd(TIMx,ENABLE);
}

void PWM_Init(TIM_TypeDef * TIMx, PWM_Channel_t Channel, uint16_t Period, uint16_t Prescaler){
//	TIM_TimeBaseInitTypeDef timer_typedef;
	
	 if(TIMx == TIM1){  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); }
	 if(TIMx == TIM2){  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); }
	 if(TIMx == TIM3){	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); }
	 if(TIMx == TIM4){	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); }
	
	PWM_PinChannel(TIMx,Channel);
	
	timer_typedef.TIM_Period				 		= Period - 1;
	timer_typedef.TIM_Prescaler 		 		= Prescaler - 1;
	timer_typedef.TIM_CounterMode 	 		= UP;
	timer_typedef.TIM_ClockDivision 		= 0;
	timer_typedef.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx,&timer_typedef);
	
	PWM_OCx(TIMx,Channel);
	TIM_CtrlPWMOutputs(TIMx,ENABLE);
	TIM_Cmd(TIMx,ENABLE);
}

