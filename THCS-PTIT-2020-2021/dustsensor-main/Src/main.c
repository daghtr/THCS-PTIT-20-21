#include <string.h>
#include <stdio.h>
#include "main.h"
#include "i2c-lcd.h"


ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer1;
TIM_HandleTypeDef htimer2;
GPIO_InitTypeDef gpio;
#define TRUE 1
#define FALSE 0
#define BIT_LED GPIO_PIN_13 // OUTPUT_PINC_13 de kich hoat led hong ngoai

int FLAG_START_LED=FALSE;
uint16_t Receive_ADC=0;
uint16_t ADC_Convert=0;
char buffer_volt[30];
char buffer_dust[30];
int count_display=0;
int Dust=0;




void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void Config_Timer1(void);
static void Config_Timer2(void);
void Delay_x_10us(int time);

int main(void)
{
 
  HAL_Init();

 
  SystemClock_Config();


  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
	Config_Timer1();
	Config_Timer2();
  MX_USART2_UART_Init();
	
	// test LCD
	lcd_init();
	//lcd_goto_XY(1,1);
	//lcd_send_string("hello");
	
	
	
	HAL_TIM_Base_Start_IT(&htimer2);

  while (1)
  {
			//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
			//HAL_Delay(1000);
			
		
			if(FLAG_START_LED==TRUE)
			{
					HAL_GPIO_WritePin(GPIOC,BIT_LED,GPIO_PIN_SET);
					Delay_x_10us(28);
					//bat dau set ADC
					HAL_ADC_Start_IT(&hadc1);
					
					Delay_x_10us(4); // De bi bug cho nay
					HAL_GPIO_WritePin(GPIOC,BIT_LED,GPIO_PIN_RESET);	
					ADC_Convert=(int)(Receive_ADC*0.805860); // tinh toan ra ADC_Convert= Voltage*1000 de hien thi cho de dang
					Dust=(int)(ADC_Convert*0.17)-0.1*1000; // tinh toan ra nong do bui Dust*1000 de hien thi cho de, dua vao Datasheet ta co phuong trinh Dust=0.17*Volt-0.1
					if(Dust<0) Dust=0;
					//ket thuc 1 qua trinh
					count_display++;
					FLAG_START_LED=FALSE;
					if(count_display>=50) //
					{
								// Hien thi LCD Volt
								count_display=0; //reset count
								lcd_goto_XY(1,0);
								sprintf(buffer_volt,"Volt=%d.%d%d(V)    ",ADC_Convert/1000,(ADC_Convert/100)%10,(ADC_Convert/10)%10);
								lcd_send_string(buffer_volt);
								// Hien thi LCD Dust
								lcd_goto_XY(2,0);
								sprintf(buffer_dust,"Dust=%d.%d%d(mg/m3)    ",Dust/1000,(Dust/100)%10,(Dust/10)%10);
								lcd_send_string(buffer_dust);
						
								//truyen UART len may tinh, can phai co module chuyen doi USB to COM (Vd: PL2303,......)
								HAL_UART_Transmit(&huart2,(uint8_t*)buffer_volt,strlen(buffer_volt),HAL_MAX_DELAY);
								HAL_UART_Transmit(&huart2,"\n",1,HAL_MAX_DELAY); //xuong dong
								HAL_UART_Transmit(&huart2,(uint8_t*)buffer_dust,strlen(buffer_dust),HAL_MAX_DELAY);
							  HAL_UART_Transmit(&huart2,"\n",1,HAL_MAX_DELAY);
								
							
								
					}
					
					
					
					
					
			}
		
		
		
  }
	

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
 
	
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
	__HAL_RCC_HSI_DISABLE();
	
	
	
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Config_Timer1(void)
{
		htimer1.Instance=TIM1;
		htimer1.Init.Prescaler=16-1; 
		htimer1.Init.Period=10-1;   
		HAL_TIM_Base_Init(&htimer1); // Timer1 tran sau moi 10^-5 s

}


static void Config_Timer2(void)
{
			htimer2.Instance=TIM2;
			htimer2.Init.Prescaler=160-1; // Da sua
			htimer2.Init.Period=1000-1;  // Timer2 tran sau moi 10ms
				
			HAL_TIM_Base_Init(&htimer2);
}



	


static void MX_ADC1_Init(void)
{

 
  ADC_ChannelConfTypeDef sConfig = {0};


  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;   
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
 
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5; //thoi gian lay mau la 1.5 chu ky
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_I2C1_Init(void)
{

 
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
 

}


static void MX_USART2_UART_Init(void)
{

  
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	gpio.Pin=GPIO_PIN_13|BIT_LED; // chan 13 la led hien thi, chan 14 la OUTPUT_PIN de kich hoat hong ngoai cua cam bien
	gpio.Mode=GPIO_MODE_OUTPUT_PP;
	gpio.Pull=GPIO_NOPULL;
	gpio.Speed=GPIO_SPEED_FREQ_LOW;
	
	
	HAL_GPIO_Init(GPIOC,&gpio);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET); // tat led bao
	HAL_GPIO_WritePin(GPIOC,BIT_LED,GPIO_PIN_RESET); // BIT_LED = 0

}


void Delay_x_10us(int time) //delay Xx10^-5s
{
		for(int i=0;i<time;i++)
	{
			HAL_TIM_Base_Start(&htimer1);
			while(!(TIM1->SR&0x0001));
			TIM1->SR=0;
			HAL_TIM_Base_Stop(&htimer1);
			
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance==TIM2)
		{
				//Bat flag cua Led hong ngoai thanh True
				FLAG_START_LED=TRUE;
				
						
		}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
		if(hadc->Instance==ADC1)
		{
					Receive_ADC=HAL_ADC_GetValue(&hadc1);
				
		}
}


void Error_Handler(void)
{
  while(1)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		HAL_Delay(500);// Trong truong hop loi
	}
}




