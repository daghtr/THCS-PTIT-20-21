#include "mcu-spi.h"
#include "mcu-gpio.h"

	void SPIx_Init(SPI_TypeDef* SPIx, uint16_t SPI_BaudRate){
	SPI_InitTypeDef SPI_InitStruct;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	
	if(SPIx == SPI1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);
		pinMode(PA5,AF_PP); //SCK
		pinMode(PA6,INPUT); //MISO
		pinMode(PA7,AF_PP);	//MOSI
	}
	
	if(SPIx == SPI2){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
		pinMode(PB13,AF_PP);
		pinMode(PB14,INPUT);
		pinMode(PB15,AF_PP);	
	}
		SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;					
		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRate;	
		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;		
		SPI_InitStruct.SPI_CRCPolynomial = 7;
		SPI_Init(SPIx, &SPI_InitStruct);			
		SPI_Cmd(SPIx, ENABLE);	
}

uint8_t SPIx_Transfer(SPI_TypeDef* SPIx, u8 Data){
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx,Data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPIx);
}