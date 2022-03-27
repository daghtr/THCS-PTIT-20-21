#include "mcu-adc.h"
#include "stm32f10x_gpio.h" 

/*cau hinh gpio cho kenh cua ADC*/
void ADC_GPIO_Config( GPIO_TypeDef* GPIOx, uint16_t PINx, uint16_t RCCx){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCCx, ENABLE);
	GPIO_InitStruct.GPIO_Pin = PINx;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*cau hinh tham so cho tung kenh cua ADC*/
void ADC_Channel_Init(ADC_TypeDef* ADCx, uint8_t channel){
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_0){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_0, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_1){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_1, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_2){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_2, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_3){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_3, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_4){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_4, RCC_APB2Periph_GPIOA);
		
	}if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_5){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_5, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_6){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_6, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_7){
		ADC_GPIO_Config(GPIOA, GPIO_Pin_7, RCC_APB2Periph_GPIOA);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_8){
		ADC_GPIO_Config(GPIOB, GPIO_Pin_8, RCC_APB2Periph_GPIOB);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_9){
		ADC_GPIO_Config(GPIOB, GPIO_Pin_9, RCC_APB2Periph_GPIOB);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_10){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_0, RCC_APB2Periph_GPIOC);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_11){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_1, RCC_APB2Periph_GPIOC);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_12){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_2, RCC_APB2Periph_GPIOC);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3) && channel == ADC_Channel_13){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_3, RCC_APB2Periph_GPIOC);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_14){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_4, RCC_APB2Periph_GPIOC);
	}
	
	if( (ADCx == ADC1 || ADCx == ADC2) && channel == ADC_Channel_15){
		ADC_GPIO_Config(GPIOC, GPIO_Pin_5, RCC_APB2Periph_GPIOC);
	}
}

void ADCx_Init(ADC_TypeDef* ADCx, uint8_t channel){
	
	ADC_InitTypeDef ADC_InitDef;
	//DMA_InitTypeDef DMA_InitStructure;
	
	if(ADCx == ADC1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);		
	} else if (ADCx == ADC2) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	} else if (ADCx == ADC3) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	}
	
	ADC_Channel_Init(ADCx, channel);

	ADC_InitDef.ADC_Mode = ADC_Mode_Independent;
  ADC_InitDef.ADC_ScanConvMode = DISABLE;
  ADC_InitDef.ADC_ContinuousConvMode = ENABLE;
  ADC_InitDef.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitDef.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitDef.ADC_NbrOfChannel = 1;	
 	ADC_Init(ADCx, &ADC_InitDef);
	
	ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADCx, ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


uint16_t ADCx_Read(ADC_TypeDef* ADCx, uint8_t channel) {
	return ADC_GetConversionValue(ADCx);
}

//long Map(long x, long in_min, long in_max, long out_min, long out_max) {
//  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//}

float Map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}