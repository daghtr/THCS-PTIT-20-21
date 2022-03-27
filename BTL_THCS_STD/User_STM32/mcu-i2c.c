#include <stm32f10x.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "mcu-i2c.h"

GPIO_InitTypeDef GPIO_I2C_InitStruct;

void I2Cx_Pinout(uint32_t Pinout){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	if (Pinout == I2C1_B67) {
		//                      SCL          SDA
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
	} 
	else if (Pinout == I2C1_B89) {
		//                      SCL          SDA
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	} 
	else if (Pinout == I2C2_B1011) {
		//                      SCL          SDA
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		
	}
	GPIO_I2C_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_I2C_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_I2C_InitStruct);
	
}

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

/* 
 *  See AN2824 STM32F10xxx I2C optimized examples
 *
 *  This code implements polling based solution
 *
 */

/**
 *  Names of events used in stdperipheral library
 *
 *      I2C_EVENT_MASTER_MODE_SELECT                          : EV5
 *      I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
 *      I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
 *      I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
 *     
 **/



/*
 *  Read process is documented in RM008
 *
 *   There are three cases  -- read  1 byte  AN2824 Figure 2 
 *                             read  2 bytes AN2824 Figure 2
 *                             read >2 bytes AN2824 Figure 1
 */  
 
uint8_t I2Cx_Write( uint8_t Address, uint8_t *pData, uint8_t length){
	return lib_I2Cx_Write(I2C_USED, pData, length, Address);
}

uint8_t I2Cx_Read(uint8_t Address, uint8_t *pData, uint8_t length){
	return lib_I2Cx_Read(I2C_USED, pData, length, Address);
}

Status lib_I2Cx_Read(I2C_TypeDef* I2Cx, uint8_t *buf,uint32_t nbyte, uint8_t SlaveAddress)
{
  __IO uint32_t Timeout = 0;

  //    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors 

  if (!nbyte)
    return Success;



  // Wait for idle I2C interface

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

  // Enable Acknowledgement, clear POS flag

  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

  // Intiate Start Sequence (wait for EV5

  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

  // Send Address

  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

  // EV6
  
  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));

  if (nbyte == 1)
    {

      // Clear Ack bit      

      I2C_AcknowledgeConfig(I2Cx, DISABLE);       

      // EV6_1 -- must be atomic -- Clear ADDR, generate STOP

      __disable_irq();
      (void) I2Cx->SR2;                           
      I2C_GenerateSTOP(I2Cx,ENABLE);      
      __enable_irq();

      // Receive data   EV7

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
      *buf++ = I2C_ReceiveData(I2Cx);

    }
  else if (nbyte == 2)
    {
      // Set POS flag

      I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);

      // EV6_1 -- must be atomic and in this order

      __disable_irq();
      (void) I2Cx->SR2;                           // Clear ADDR flag
      I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
      __enable_irq();

      // EV7_3  -- Wait for BTF, program stop, read data twice

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

      __disable_irq();
      I2C_GenerateSTOP(I2Cx,ENABLE);
      *buf++ = I2Cx->DR;
      __enable_irq();

      *buf++ = I2Cx->DR;

    }
  else 
    {
      (void) I2Cx->SR2;                           // Clear ADDR flag
      while (nbyte-- != 3)
	{
	  // EV7 -- cannot guarantee 1 transfer completion time, wait for BTF 
          //        instead of RXNE

	  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)); 
	  *buf++ = I2C_ReceiveData(I2Cx);
	}

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  

      // EV7_2 -- Figure 1 has an error, doesn't read N-2 !

      I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

      __disable_irq();
      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
      I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
      __enable_irq();

      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

      // wait for byte N

      Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
      *buf++ = I2C_ReceiveData(I2Cx);

      nbyte = 0;

    }

  // Wait for stop

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
  return Success;

 errReturn:

  // Any cleanup here
  return Error;

}

/*
 * Read buffer of bytes -- AN2824 Figure 3
 */

Status lib_I2Cx_Write(I2C_TypeDef* I2Cx, const uint8_t* buf,  uint32_t nbyte, uint8_t SlaveAddress){
    __IO uint32_t Timeout = 0;

    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    //    I2Cx->CR2 |= I2C_IT_ERR;

  if (nbyte){
		Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

		// Intiate Start Sequence

		I2C_GenerateSTART(I2Cx, ENABLE);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

		// Send Address  EV5

		I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		// EV6

		// Write first byte EV8_1

		I2C_SendData(I2Cx, *buf++);

		while (--nbyte) {

	  // wait on BTF

			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  
			I2C_SendData(I2Cx, *buf++);
		}

		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  
		I2C_GenerateSTOP(I2Cx, ENABLE);
		Timed(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
  }
    return Success;
 errReturn:
    return Error;
}

void I2Cx_Init(I2C_TypeDef* I2Cx, uint32_t Pinout, uint32_t ClockSpeed) {
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	if (I2Cx == I2C1) {
		/* Enable clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		/* Enable pins */	
		I2Cx_Pinout(Pinout);
		        /* I2C1 Reset */

		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = ClockSpeed;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C1_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = I2C1_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = I2C1_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = I2C1_ACK;
		I2C_InitStruct.I2C_DutyCycle = I2C1_DUTY_CYCLE;
	} 
	else if (I2Cx == I2C2) {
		/* Enable clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		/* Enable pins */
		I2Cx_Pinout(Pinout);
		
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);	
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = ClockSpeed;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C2_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = I2C2_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = I2C2_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = I2C2_ACK;
		I2C_InitStruct.I2C_DutyCycle = I2C2_DUTY_CYCLE;
	} 
	
	/* Disable I2C first */
	I2C_Cmd(I2Cx, DISABLE);
	/* Initialize I2C */
	I2C_Init(I2Cx, &I2C_InitStruct);
	/* Enable I2C */
	I2C_Cmd(I2Cx, ENABLE);
}

