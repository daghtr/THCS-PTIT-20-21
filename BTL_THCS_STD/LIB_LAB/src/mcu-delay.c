#include "mcu-delay.h"

void Config_Timer(void){
	TIM_TimeBaseInitTypeDef TIMER;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIMER.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER.TIM_Period = 65535;
	TIMER.TIM_Prescaler = 2;
	TIMER.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIMER);
	
}

void Delay_1ms(void){
		Config_Timer();
		TIM_Cmd(TIM2,ENABLE);
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2)<36000);
		TIM_Cmd(TIM2,DISABLE);
}

void Delay_ms(unsigned int time_ms){
	while(time_ms--){
		Delay_1ms();
	}
}

void Delay_1us(void){
		Config_Timer();
		TIM_Cmd(TIM2,ENABLE);
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2)<36);
		TIM_Cmd(TIM2,DISABLE);
}

void Delay_us(unsigned int time_us){
	while(time_us--){
		Delay_1us();
	}
}

