
#include "main.h"
#include "stm32f1xx_it.h"
extern TIM_HandleTypeDef htimer2;
extern ADC_HandleTypeDef hadc1;
void SysTick_Handler(void)
{
 
  HAL_IncTick();
  
}


void TIM2_IRQHandler(void)
{
		HAL_TIM_IRQHandler(&htimer2);

}

void ADC1_2_IRQHandler(void)
{
		HAL_ADC_IRQHandler(&hadc1);


}