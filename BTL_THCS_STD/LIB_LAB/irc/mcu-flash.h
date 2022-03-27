#ifndef _MCU_FLASH_H
#define _MCU_FLASH_H
#include "stm32f10x_flash.h"

typedef enum {
	FLASH_ERRORS = 0,
	FLASH_NO_ERRORS
}FlashStatus;

void Flash_Unlock(void);
void Flash_Erase(volatile uint32_t u32StartAddr);
FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t *u8BufferWrite, uint32_t length);
FlashStatus Flash_Read(volatile uint32_t u32StartAddr, uint8_t *u8BufferRead, uint32_t length);

#endif