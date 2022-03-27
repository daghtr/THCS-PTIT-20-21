#include "mcu-keypad.h"
#include "mcu-gpio.h"
#include "string.h"
char Column[]={PA0,PA1,PA2};
char Row[]={PA3,PA4,PA5,PA6};
int i;

void KeyPad_Init(void){
	
	// cau hinh cot
	for(i = 0; i<3; i++){
		pinMode(Column[i], OUTPUT);
		digitalWrite(Column[i],LOW);
	}
	
	// cau hinh hang
	for(i = 0; i<4; i++){
		pinMode(Row[i], INPUT_PULLUP);
		digitalWrite(Row[i],HIGH);
	}
}

char KeyPad_Press(){
	GPIO_Write(GPIOA,0x7e);
	if(!digitalRead(PA6)){return '3';}
	if(!digitalRead(PA5)){return '6';}
	if(!digitalRead(PA4)){return '9';}
	if(!digitalRead(PA3)){return '#';}
	
	GPIO_Write(GPIOA,0x7d);
	if(!digitalRead(PA6)){return '2';}
	if(!digitalRead(PA5)){return '5';}
	if(!digitalRead(PA4)){return '8';}
	if(!digitalRead(PA3)){return '0';}
	
	GPIO_Write(GPIOA,0x7b);
	if(!digitalRead(PA6)){return '1';}
	if(!digitalRead(PA5)){return '4';}
	if(!digitalRead(PA4)){return '7';}
	if(!digitalRead(PA3)){return '*';}
	return 0x7F;
}

//uint8_t CheckPass(char *Str1, char *Str2){
//	if(strlen(Str1) == strlen(Str2)){
//		if(strstr(Str1,Str2)!=NULL){ 
//			return 1;
//		}
//	}
//	return 0;
//}