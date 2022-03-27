#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "string.h"
#include "stdio.h"
#include "mcu-delay.h"
#include "mcu-i2c-lcd.h"

struct __FILE {
  int dummy;
};

FILE __stdout;

void USART_SendChar(char c)
{
	USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
}
void USART_SendString(char *str)
{
	while(*str)
	{
		USART_SendChar(*str++);
	}
}
   
int fputc(int ch, FILE *f){
    /* Send your custom byte */
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
    /* If everything is OK, you have to return character written */
    return ch;
}


/*Khoi tao bien cau hinh*/
GPIO_InitTypeDef			GPIO_InitStructure;
USART_InitTypeDef			UART_InitStructure;
ADC_InitTypeDef 			ADC_InitStructure;

/*nhan du lieu tu adc*/
unsigned int 	adc_ch0=0;
float calcVoltage=0,dustDensity=0;
uint8_t test = 0;
char Value_ADC[10];
char Value_Sensor[10];
char Voltage[10];
void GPIO_Configuration(void);
void UART_Configuration (void);
void ADC_Configuration(void);
unsigned int sum_adc1;
unsigned int adc_tb;
unsigned int b;

int main(){
	
		UART_Configuration();
		Config_Timer();
		GPIO_Configuration();
//		GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
		// LCD I2C1
		I2Cx_Init(I2C_USED, I2C1_B89, I2C_CLOCK_STANDARD);
		I2Cx_LCD_Init();
		I2Cx_LCD_Clear();
		I2Cx_LCD_BackLight(1);
		/*ADC1_CHANNEL_1*/
		ADC_Configuration();
		 

		while (1)
		{		
		
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				Delay_us(280);
				/*doc du lieu tu ADC*/
//				adc_ch0=ADC_GetConversionValue(ADC1);
				
				for( b=0; b<10; b++)
				{
					adc_ch0 = ADC_GetConversionValue(ADC1);
					sum_adc1 = sum_adc1 + adc_ch0;
					Delay_ms(1);
				}
				adc_tb = sum_adc1/10;
				sum_adc1=0;
				Delay_us(40);
			
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
					
				Delay_us(9680);
				

			
				calcVoltage = adc_tb * (5.0 / 4095);
				

			
				dustDensity = (0.17*calcVoltage-0.1)*1000;
			
				if(dustDensity < 0 )  dustDensity = 0.00;
			

				
				sprintf(Value_ADC, "%d", adc_tb);
				sprintf(Value_Sensor, "%g", dustDensity);
				sprintf(Voltage, "%g", calcVoltage);
				
				I2Cx_LCD_Clear();
				I2Cx_LCD_Puts("Value ADC:");
				I2Cx_LCD_Puts(Value_ADC);
				I2Cx_LCD_Line2();
				I2Cx_LCD_Puts("Dust:");
				I2Cx_LCD_Puts(Value_Sensor);
				I2Cx_LCD_Puts("(ug/m3)");
				I2Cx_LCD_Line3();
				I2Cx_LCD_Puts("Voltage:");
				I2Cx_LCD_Puts(Voltage);
				I2Cx_LCD_Puts("(V)");
				
				USART_SendString("Value_ADC:");
				USART_SendString(Value_ADC);
				USART_SendString("\n");
				USART_SendString("Dust:");
				USART_SendString(Value_Sensor);
				USART_SendString("(ug/m3)");
				USART_SendString("\n");
				USART_SendString("Voltage:");
				USART_SendString(Voltage);
				USART_SendString("(V)");
				USART_SendString("\n");
				
				
				
				
				Delay_ms(500);
			
		}
}


void UART_Configuration(){
		/* Cau Tx mode AF_PP, Rx mode FLOATING  */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin 								= GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode								= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed								= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin 								= GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode 								= GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*Cau hinh USART*/
		UART_InitStructure.USART_BaudRate 						= 115200;
		UART_InitStructure.USART_WordLength 					= USART_WordLength_8b;
		UART_InitStructure.USART_StopBits 						= USART_StopBits_1;
		UART_InitStructure.USART_Parity 							= USART_Parity_No;
		UART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		UART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &UART_InitStructure);
		
		USART_Cmd(USART1 , ENABLE);
}
void GPIO_Configuration(){

		/*cau hinh chan led cam bien*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	

}
void ADC_Configuration(void)
	{
		/*cap clock cho chan GPIO va bo ADC1*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		/*cau hinh chan Input cua bo ADC1 la chan PA0*/
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/*cau hinh ADC1*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);
	
		/* Cau hinh chanel, rank, thoi gian lay mau */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
			/* Cho phep bo ADC1 hoa dong */
		ADC_Cmd(ADC1, ENABLE);   
		/* cho phep cam bien nhiet hoat dong */
		ADC_TempSensorVrefintCmd(ENABLE);
		/* Reset thanh ghi cablib  */   
		ADC_ResetCalibration(ADC1);
		/* Cho thanh ghi cablib reset xong */
		while(ADC_GetResetCalibrationStatus(ADC1));
		/* Khoi dong bo ADC */
		ADC_StartCalibration(ADC1);	
		/* Cho trang thai cablib duoc bat */
		while(ADC_GetCalibrationStatus(ADC1));
		/* Bat dau chuyen doi ADC */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  
	}


