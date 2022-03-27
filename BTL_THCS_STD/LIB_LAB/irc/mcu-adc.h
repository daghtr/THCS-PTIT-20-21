
/*
  *****************************************************************************
  * @file				mcu-adc.h                                                              *
  * @author			Thai Salem                                               
  * @date				00/00/0000                                                         
  ******************************************************************************
*/

#ifndef MCU_ADC_H
#define MCU_ADC_H

#include "stm32f10x.h"

void ADC_GPIO_Config( GPIO_TypeDef* GPIOx, uint16_t PINx, uint16_t RCCx);
void ADC_Channel_Init(ADC_TypeDef* ADCx, uint8_t channel);
void ADCx_Init(ADC_TypeDef* ADCx, uint8_t channel);
uint16_t ADCx_Read(ADC_TypeDef* ADCx, uint8_t channel);

float Map(float x, float in_min, float in_max, float out_min, float out_max);
//long Map(long x, long in_min, long in_max, long out_min, long out_max);
#endif
