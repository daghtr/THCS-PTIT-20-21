#ifndef _MCU_I2C_H
#define _MCU_I2C_H

#include <stm32f10x_i2c.h>

/*
 *	Pinout
 *			
 *	I2CX	|SCL	SDA		|SCL	SDA		|
 *			  |						|						|
 *	I2C1	|PB6	PB7		|PB8	PB9		|
 *	I2C2	|PB10	PB11	|
 */
#ifndef I2C_USED
#define I2C_USED 										I2C1
#endif
 
#ifndef I2C1_B67
#define I2C1_B67 										(1u)
#endif

#ifndef I2C1_B89
#define I2C1_B89 										(2u)
#endif

#ifndef I2C2_B1011
#define I2C2_B1011									(3u)
#endif
 
// Timeout
#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT									20000
#endif

//I2C1 settings, change defines
#ifndef I2C1_ACKNOWLEDGED_ADDRESS
#define I2C1_ACKNOWLEDGED_ADDRESS		I2C_AcknowledgedAddress_7bit
#endif
#ifndef I2C1_MODE
#define I2C1_MODE										I2C_Mode_I2C
#endif
#ifndef I2C1_OWN_ADDRESS
#define I2C1_OWN_ADDRESS						0x00
#endif
#ifndef I2C1_ACK
#define I2C1_ACK										I2C_Ack_Disable
#endif
#ifndef I2C1_DUTY_CYCLE
#define I2C1_DUTY_CYCLE							I2C_DutyCycle_2
#endif

//I2C2 settings, change defines
#ifndef I2C2_ACKNOWLEDGED_ADDRESS
#define I2C2_ACKNOWLEDGED_ADDRESS		I2C_AcknowledgedAddress_7bit
#endif
#ifndef I2C2_MODE
#define I2C2_MODE										I2C_Mode_I2C
#endif
#ifndef I2C2_OWN_ADDRESS
#define I2C2_OWN_ADDRESS						0x00
#endif
#ifndef I2C2_ACK
#define I2C2_ACK										I2C_Ack_Disable
#endif
#ifndef I2C2_DUTY_CYCLE
#define I2C2_DUTY_CYCLE							I2C_DutyCycle_2
#endif

//I2C speed modes
#define I2C_CLOCK_STANDARD					100000
#define I2C_CLOCK_FAST_MODE					400000
#define I2C_CLOCK_FAST_MODE_PLUS		1000000
#define I2C_CLOCK_HIGH_SPEED				3400000



typedef enum {Error = 0, Success = !Error } Status;

void I2Cx_Init(I2C_TypeDef* I2Cx, uint32_t Pinout, uint32_t ClockSpeed); 
uint8_t I2Cx_Read(uint8_t Address, uint8_t *pData, uint8_t length);
uint8_t I2Cx_Write(uint8_t Address, uint8_t *pData, uint8_t length);
Status lib_I2Cx_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
Status lib_I2Cx_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);


#endif
