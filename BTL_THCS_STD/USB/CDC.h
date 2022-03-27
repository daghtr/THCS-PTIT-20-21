#ifndef _CDC_CLASS_
#define _CDC_CLASS_

#include "sys.h"
#include "hw_config.h"

	void Serial_begin(uint32_t baud_count);
	void Serial_end(void);

	int Serial_available(void);
	//virtual void accept(void);
	int Serial_peek(void);
	uint32_t Serial_readX(uint8_t * buf, uint32_t len);
	int Serial_readBytes(char *buf, const u8 len);
	int Serial_availableForWrite(void);
	int Serial_read(void);
	void Serial_flush(void);
	size_t Serial_write(uint8_t);
	size_t Serial_Puts(const void *buffer, size_t size);


//#include "Stream.h"
//#include "RingBuffer.h"

//================================================================================
//================================================================================
//	Serial over CDC (Serial1 is the physical port)

//class Serial_ : public Stream
//{
//private:
//	RingBuffer *_cdc_rx_buffer;
//public:
//	
//	void begin(uint32_t baud_count);
//	void end(void);

//	virtual int available(void);
//	virtual void accept(void);
//	virtual int peek(void);
//	virtual int read(void);
//	virtual void flush(void);
//	virtual size_t write(uint8_t);
//	virtual size_t write(const uint8_t *buffer, size_t size);
//	using Print::write; // pull in write(str) from Print
//	operator bool() { return true; };
//};
//extern Serial_ SerialUSB;


#endif // _CDC_CLASS_

