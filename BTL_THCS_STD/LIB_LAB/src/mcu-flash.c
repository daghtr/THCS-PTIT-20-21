#include "mcu-flash.h"

void Flash_Unlock(void){
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

void Flash_Erase(volatile uint32_t u32StartAddr){
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	if((FLASH->CR&FLASH_CR_LOCK) == FLASH_CR_LOCK){
		Flash_Unlock();
	}
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = u32StartAddr;
	FLASH->CR |= FLASH_CR_STRT;
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->CR &= ~FLASH_CR_STRT;
}

FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t* u8BufferWrite, uint32_t length){
	unsigned int i = 0;
	if((u8BufferWrite == 0x00) || (length == 0) || (length%2 != 0)){
		return FLASH_ERRORS;
	}
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	if((FLASH->CR&FLASH_CR_LOCK) == FLASH_CR_LOCK){
		Flash_Unlock();
	}
	FLASH->CR |= FLASH_CR_PG;
	for(i = 0; i < (length/2); i++){
		*(uint16_t*)(u32StartAddr + i*2) = *(uint16_t*)(u8BufferWrite + i*2);
	}
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	FLASH->CR &= ~FLASH_CR_PG;
	return FLASH_NO_ERRORS;
}

FlashStatus Flash_Read(volatile uint32_t u32StartAddr, uint8_t* u8BufferRead, uint32_t length){
	uint32_t i = 0;
	if((u8BufferRead == 0) || length == 0){
		return FLASH_ERRORS;
	} 
	for(i = 0; i < length; i++){
		*(uint8_t*)(u8BufferRead + i) = *(uint8_t*)(u32StartAddr + i);
	}
	return FLASH_NO_ERRORS;
}

void Flash_Check(void){
	
}



