#include "mcu-usart.h"
#include "mcu-gpio.h"
/*******************************************************************************************************/

#define MAX 100

/*******************************************************************************************************/
struct __FILE {
    int dummy;
};
extern FILE __stdout;
 
int fputc(int ch, FILE *f) {
 
    USARTx_Sendc(USART1, ch);
  
    return ch;
}
/*******************************************************************************************************/
void USARTx_Init(USART_TypeDef* USART_x, uint32_t USART_Baud){
extern GPIO_InitTypeDef gpio_typedef;
USART_InitTypeDef usart_typedef;
/*-------------------if use USART1-------------------*/
	if(USART_x == USART1){
		// enable clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		// congifgure pin Tx - A9;
		pinMode(PA9,AF_PP);		
		// configure pin Rx - A10;
		pinMode(PA10,INPUT);
		// usart configure
		usart_typedef.USART_BaudRate = USART_Baud;
		usart_typedef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart_typedef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
		usart_typedef.USART_Parity = USART_Parity_No;
		usart_typedef.USART_StopBits = USART_StopBits_1;
		usart_typedef.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART_x, &usart_typedef);
/*----------------------------------------------------------------------------------------*/		
		/*if use usart interrup */
		//  Enable RXNE interrupt 
		USART_ITConfig(USART_x, USART_IT_RXNE, ENABLE);
		 /* Enable USART1 global interrupt */
		NVIC_EnableIRQ(USART1_IRQn);
/*----------------------------------------------------------------------------------------*/		
		USART_Cmd(USART_x, ENABLE);
	}

/*-------------------if use USART2-------------------*/
	else if(USART_x == USART2){
		// enable clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		// congifgure pin Tx - A2;
		pinMode(PA2,AF_PP);		
		// configure pin Rx - A3;
		pinMode(PA3,INPUT);
		// usart configure
		usart_typedef.USART_BaudRate = USART_Baud;
		usart_typedef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart_typedef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
		usart_typedef.USART_Parity = USART_Parity_No;
		usart_typedef.USART_StopBits = USART_StopBits_1;
		usart_typedef.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART_x, &usart_typedef);
/*----------------------------------------------------------------------------------------*/		
		/*if use usart interrup */
		//  Enable RXNE interrupt 
		USART_ITConfig(USART_x, USART_IT_RXNE, ENABLE);
		 /* Enable USART1 global interrupt */
		NVIC_EnableIRQ(USART2_IRQn);
/*----------------------------------------------------------------------------------------*/		
		USART_Cmd(USART_x, ENABLE);
	}

/*-------------------if use USART3-------------------*/
	else if(USART_x == USART3){
		// enable clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		// congifgure pin Tx - B9;
		pinMode(PB9,AF_PP);		
		// configure pin Rx - B10;
		pinMode(PB10,INPUT);
		// usart configure
		usart_typedef.USART_BaudRate = USART_Baud;
		usart_typedef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart_typedef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
		usart_typedef.USART_Parity = USART_Parity_No;
		usart_typedef.USART_StopBits = USART_StopBits_1;
		usart_typedef.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART_x, &usart_typedef);
		
		/*if use usart interrup */
		//  Enable RXNE interrupt 
		USART_ITConfig(USART_x, USART_IT_RXNE, ENABLE);
		 /* Enable USART1 global interrupt */
		NVIC_EnableIRQ(USART3_IRQn);
	
		USART_Cmd(USART_x, ENABLE);
	}
}
/*******************************************************************************************************/
void USARTx_Sendc(USART_TypeDef* USART_x, char c){
	/*----------------------------------------------------------------------------------------*/
	USART_SendData(USART_x,c);
	while(USART_GetFlagStatus(USART_x, USART_FLAG_TXE)==RESET);
}
/*******************************************************************************************************/
void USARTx_Sends(USART_TypeDef* USART_x, char* s){
	/*----------------------------------------------------------------------------------------*/
	while(*s!='\0'){
		USARTx_Sendc(USART_x,*s++);
		while(USART_GetFlagStatus(USART_x, USART_FLAG_TXE)==RESET);
	}
}
/*******************************************************************************************************/
uint16_t UARTx_Getc(USART_TypeDef* USARTx){
	return USART_ReceiveData(USARTx);
}
/*******************************************************************************************************/

char 	vrc_Getc; 			// bien kieu char, dung de nhan du lieu tu PC gui xuong;
int		vri_Stt; 			// bien danh dau trang thai. 
int 	vri_Count = 0; 		// bien diem
char	vrc_Res[MAX]; 		//Mang kieu char vrc_Res[MAX]: mang luu tru tam thoi;

/*Use interrupt to receive data*/
void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET){
		vrc_Getc = UARTx_Getc(USART1);
		if(vrc_Getc == 35){ // 13: enter; bao hieu ket thuc chuoi
			vri_Stt = 1; // dieu kien kiem tra de thuc hien cac chuong trinh theo yeu cau.
			vrc_Res[vri_Count] = NULL;
			vri_Count = 0;
		}
		else{
			vrc_Res[vri_Count] = vrc_Getc;
			//USARTx_Sendc(USART1, vrc_Res[vri_Count]);
			vri_Count++;
		}
	}
}

/*******************************************************************************************************/

/*Use interrupt to receive data*/
void USART2_IRQHandler(){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		vrc_Getc = UARTx_Getc(USART2);
		if(vrc_Getc == 13){// 13: enter; bao hieu ket thuc chuoi
			vri_Stt = 1; // dieu kien kiem tra de thuc hien cac chuong trinh theo yeu cau.
			vrc_Res[vri_Count] = NULL;
			vri_Count = 0;
		}
		else{
			vrc_Res[vri_Count] = vrc_Getc;
			USARTx_Sendc(USART2, vrc_Res[vri_Count]);
			vri_Count++;
		}
	}
}
/*******************************************************************************************************/
/*Use interrupt to receive data*/
void USART3_IRQHandler(){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		vrc_Getc = UARTx_Getc(USART3);
		if(vrc_Getc == 13){// 13: enter; bao hieu ket thuc chuoi
			vri_Stt = 1; // dieu kien kiem tra de thuc hien cac chuong trinh theo yeu cau.
			vrc_Res[vri_Count] = NULL;
			vri_Count = 0;
		}
		else{
			vrc_Res[vri_Count] = vrc_Getc;
			USARTx_Sendc(USART3, vrc_Res[vri_Count]);
			vri_Count++;
		}
	}
}
/*******************************************************************************************************/

extern int 		vri_Number; 			// chua gia tri can chuyen doi.
extern int 		vri_CountPtr; 		// dem so luong cua mang vrc_Ptr.
extern int  	vri_CountArr; 		// dem so luong phan tu cua mang vrc_Arr.
extern int 		vri_Countemp;
char  	vrc_Ptr[100];			//mang nhan gia tri (chuan vi tri) sau khi chuyen doi int->string.
char  	vrc_Arr[100]; 		//mang nhan gia tri (sai vi tri) sau khi chuyen doi tu int->string.

// chuyen tu int -> string.
void USARTx_InStr(unsigned int vri_Number){
 int vri_CountArr = 0, vri_CountPtr = 0;
	while(vri_Number>0){
		vrc_Arr[vri_CountArr] = ((vri_Number % 10) + 48);
		vri_CountArr++;
		vri_Number /= 10;
	}
	vrc_Arr[vri_CountArr]=NULL;
	for(vri_CountArr = strlen(vrc_Arr)-1; vri_CountArr >= 0; vri_CountArr--){
		vrc_Ptr[vri_CountPtr] = vrc_Arr[vri_CountArr];
		vri_CountPtr++;
	}
	vrc_Ptr[vri_CountPtr]=NULL;
}
/*********************************************************************************************************/
/*the function compare two strings*/
int USARTx_Compare(char vrc_CpStr[],char vrc_CpQtr[]){
	if(strstr(vrc_CpStr,vrc_CpQtr)!=NULL){
		return 1;
	}
	else{ 
		return 0;
	}
}

