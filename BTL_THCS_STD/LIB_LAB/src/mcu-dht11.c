//#include "mcu-dht11.h"
//#include "mcu-lcd.h"
//#include "mcu-gpio.h"
////////////////////////////////////////////////////////////////////////////////////	 
//								  
////////////////////////////////////////////////////////////////////////////////////


//void DHT11_Rst(void){                 
//	DHT11_IO_OUT(); 	//SET OUTPUT
//  DHT11_DQ_OUT = 0; 	//DQ=0
//  Delay_ms(20);    	//18ms
//  DHT11_DQ_OUT = 1; 	//DQ=1 
//	Delay_us(30);     	//20~40us
//}

//u8 DHT11_Check(void){   
////	u8 retry=0;
////	DHT11_IO_IN();//SET INPUT	 
////    while (DHT11_DQ_IN&&retry<100){//keo xuong thap 40~80us
////		retry++;
////		Delay_us(1);
////	} 
////	if(retry>=100)return 1;
////	else retry=0;
////    while (!DHT11_DQ_IN&&retry<100){//keo xuong thap lan nua 40~80us
////		retry++;
////		Delay_us(1);
////	}
////	if(retry>=100)return 1;	    
////	return 0;
//	
//	u8 time=0;
//	DHT11_IO_IN();
//	while(DHT11_DQ_IN&&time<100){time++;Delay_us(1);}//cho xuong
//	if(time>=100) return 1;//loi
//	else time = 0;
//	while(!DHT11_DQ_IN&&time<100){time++;Delay_us(1);}// cho len
//	if(time>=100) return 1;//loi
//	else return 0;
//	
//	//return 0;
//	
//}



//u8 DHT11_Read_Bit(void){
//// 	u8 retry=0;
////	while(DHT11_DQ_IN && retry<100){//chow xuong thap
////		retry++;
////		Delay_us(1);
////	}
////	retry=0;
////	while(!DHT11_DQ_IN&&retry<100){//chow xuong thap
////		retry++;
////		Delay_us(1);
////	}
////	Delay_us(40);//40us
////	if(DHT11_DQ_IN)return 1;
////	else return 0;		   
//	
//	u8 time = 0;
//	//DHT11_IO_IN();
//	while(DHT11_DQ_IN && time <100){time++; Delay_1us();}
//	time = 0; 
//	while(!DHT11_DQ_IN && time <100){time++; Delay_1us();}
//	Delay_us(40);
//	if(DHT11_DQ_IN) return 1;
//	else return 0;
//	
//	return 0;
//}

//u8 DHT11_Read_Byte(void){        
//  u8 i,data = 0;  
//	for (i=0;i<8;i++){
//   		data<<=1; 
//	    data|=DHT11_Read_Bit();
//    }						    
//    return data;
//}
//// Nhiet Do: T *C ( 0 ~ 50 бу)
//// Humi: gia tri ( 20% ~ 90%)
//// Return: 0, binh  thuong; 1, that bai
//u8 DHT11_Read_Data(u8 *temp,u8 *humi){        
// 	u8 buf[5];
//	u8 i;
//	DHT11_Rst();
//	if(DHT11_Check()==0){
//		for(i=0;i<5;i++){
//			buf[i]=DHT11_Read_Byte();
//		}
//		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){
//			*humi=buf[0]; 
//			*temp=buf[2];
//		}
//	}else return 1;
//	return 0;	    
//}
//  	 
//u8 DHT11_Init(void){
//	pinMode(PA7,OUTPUT);
//	DHT11_DQ_OUT=1;
//	DHT11_Rst();
//	return DHT11_Check();
//}

#include "mcu-dht11.h"
#include "mcu-delay.h"
#include "mcu-lcd.h"
#include "stdio.h"

#define DHT11_PIN GPIO_Pin_4
#define DHT11_PORT GPIOA

GPIO_InitTypeDef gpio;
void DHT11_Init(void)
{
	TIM_TimeBaseInitTypeDef TM;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TM.TIM_CounterMode = TIM_CounterMode_Up;
	TM.TIM_ClockDivision = TIM_CKD_DIV1;
	TM.TIM_RepetitionCounter = 0;
	TM.TIM_Prescaler = 72-1;
	TM.TIM_Period = 0xFFFF;
	TIM_TimeBaseInit(TIM1,&TM);
	TIM_Cmd(TIM1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Pin = DHT11_PIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &gpio);
	GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

void DHT11_send(u8 data){
	u8 i;
	for(i = 0; i<8; i++){
		if(data & 0x80){
			GPIO_SetBits(DHT11_PORT, DHT11_PIN);
			Delay_ms(4);
			GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
			Delay_ms(1);
		}
		else{
			GPIO_SetBits(DHT11_PORT, DHT11_PIN);
			Delay_ms(1);
			GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
			Delay_ms(4);
		}
		data<<=1;
	}
}

uint8_t DHT11_Read(uint8_t *pu8Data)
{
		uint16_t Tim;
		uint8_t u8Buff[5];
		uint8_t u8CheckSum;
		uint8_t i;
	
	GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
	Delay_ms(20);
	GPIO_SetBits(DHT11_PORT, DHT11_PIN);
	
	/* cho chan DHT11_PIN len cao */
	TIM_SetCounter(TIM1, 0);
	while (TIM_GetCounter(TIM1) < 10) {
		if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
			break;
		}
	}
	Tim = TIM_GetCounter(TIM1);
	if (Tim >= 10) {
		return 0;
	}
	
	/* cho chan DHT11_PIN xuong thap */
	TIM_SetCounter(TIM1, 0);
	while (TIM_GetCounter(TIM1) < 45) {
		if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
			break;
		}
	}
	Tim = TIM_GetCounter(TIM1);
	if ((Tim >= 45) || (Tim <= 5)) {
		return 0;
	}
	
	/* cho chan DHT11_PIN len cao */
	TIM_SetCounter(TIM1, 0);
	while (TIM_GetCounter(TIM1) < 90) {
		if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
			break;
		}
	}
	Tim = TIM_GetCounter(TIM1);
	if ((Tim >= 90) || (Tim <= 70)) {
		return 0;
	}
	
	/* cho chan DHT11_PIN xuong thap */
	TIM_SetCounter(TIM1, 0);
	while (TIM_GetCounter(TIM1) < 95) {
		if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
			break;
		}
	}
	Tim = TIM_GetCounter(TIM1);
	if ((Tim >= 95) || (Tim <= 75)) {
		return 0;
	}
	
	/* nhan byte so 1 */
	for (i = 0; i < 8; ++i) {
		/* cho chan DHT11_PIN len cao */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 65) {
			if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 65) || (Tim <= 45)) {
			return 0;
		}
		
		/* cho chan DHT11_PIN xuong thap */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 80) {
			if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 80) || (Tim <= 10)) {
			return 0;
		}
		u8Buff[0] <<= 1;
		if (Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[0] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[0] &= ~1;
		}
	}
	
	/* nhan byte so 2 */
	for (i = 0; i < 8; ++i) {
		/* cho chan DHT11_PIN len cao */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 65) {
			if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 65) || (Tim <= 45)) {
			return 0;
		}
		
		/* cho chan DHT11_PIN xuong thap */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 80) {
			if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 80) || (Tim <= 10)) {
			return 0;
		}
		u8Buff[1] <<= 1;
		if (Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[1] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[1] &= ~1;
		}
	}
	
	/* nhan byte so 3 */
	for (i = 0; i < 8; ++i) {
		/* cho chan DHT11_PIN len cao */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 65) {
			if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 65) || (Tim <= 45)) {
			return 0;
		}
		
		/* cho chan DHT11_PIN xuong thap */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 80) {
			if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 80) || (Tim <= 10)) {
			return 0;
		}
		u8Buff[2] <<= 1;
		if (Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[2] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[2] &= ~1;
		}
	}
	
	/* nhan byte so 4 */
	for (i = 0; i < 8; ++i) {
		/* cho chan DHT11_PIN len cao */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 65) {
			if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 65) || (Tim <= 45)) {
			return 0;
		}
		
		/* cho chan DHT11_PIN xuong thap */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 80) {
			if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 80) || (Tim <= 10)) {
			return 0;
		}
		u8Buff[3] <<= 1;
		if (Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[3] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[3] &= ~1;
		}
	}
	
	/* nhan byte so 5 */
	for (i = 0; i < 8; ++i) {
		/* cho chan DHT11_PIN len cao */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 65) {
			if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 65) || (Tim <= 45)) {
			return 0;
		}
		
		/* cho chan DHT11_PIN xuong thap */
		TIM_SetCounter(TIM1, 0);
		while (TIM_GetCounter(TIM1) < 80) {
			if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
				break;
			}
		}
		Tim = TIM_GetCounter(TIM1);
		if ((Tim >= 80) || (Tim <= 10)) {
			return 0;
		}
		u8Buff[4] <<= 1;
		if (Tim > 45) {
			/* nhan duoc bit 1 */
			u8Buff[4] |= 1;
		} else {
			/* nhan duoc bit 0 */
			u8Buff[4] &= ~1;
		}
	}
	
	u8CheckSum = u8Buff[0] + u8Buff[1] + u8Buff[2] + u8Buff[3];
	if (u8CheckSum != u8Buff[4]) {
		return 0;
	}
	
	for (i = 0; i < 4; ++i) {
		pu8Data[i] = u8Buff[i];
	}
	
	return 1;
}






















