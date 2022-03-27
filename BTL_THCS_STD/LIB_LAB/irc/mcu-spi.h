#ifndef MCU_SPI_H
#define MCU_SPI_H

#include "stm32f10x.h"
#include "mcu-gpio.h"

#define MFRC522_SPI							SPI2

#define MFRC522_CS_PIN 					PB12
#define MFRC522_CS_LOW 					digitalWrite(MFRC522_CS_PIN,RESET)
#define MFRC522_CS_HIGH 				digitalWrite(MFRC522_CS_PIN,SET)


void SPIx_Init(SPI_TypeDef* SPIx, uint16_t SPI_BaudRate);
uint8_t SPIx_Transfer(SPI_TypeDef* SPIx, u8 Data);

#endif


