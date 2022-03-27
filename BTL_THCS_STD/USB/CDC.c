
#include "sys.h"
#include "USBAPI.h"
#include "USBDesc.h"
#include "CDC.h"
#include "hw_config.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_init.h"


#ifdef CDC_ENABLED

#define CDC_SERIAL_BUFFER_SIZE	512

extern uint8_t packet_buffer[], packet_size;
extern uint8_t flag_TX_USB;
extern uint8_t flag_RX_USB;

/* For information purpose only since RTS is not always handled by the terminal application */
#define CDC_LINESTATE_DTR		0x01 // Data Terminal Ready
#define CDC_LINESTATE_RTS		0x02 // Ready to Send

#define CDC_LINESTATE_READY		(CDC_LINESTATE_RTS | CDC_LINESTATE_DTR)

//#if 0


//struct ring_buffer
//{
//	uint8_t buffer[CDC_SERIAL_BUFFER_SIZE];
//	volatile uint32_t head;
//	volatile uint32_t tail;
//};

//ring_buffer cdc_rx_buffer = { { 0 }, 0, 0};

//typedef struct
//{
//	uint32_t	dwDTERate;
//	uint8_t		bCharFormat;
//	uint8_t 	bParityType;
//	uint8_t 	bDataBits;
//	uint8_t		lineState;
//} LineInfo;

//static volatile LineInfo _usbLineInfo = { 
//    57600, // dWDTERate
//    0x00,  // bCharFormat
//    0x00,  // bParityType
//    0x08,  // bDataBits
//    0x00   // lineState
//};



int _serialPeek = -1;
void Serial_begin(uint32_t baud_count)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//PA11-A12
	
	Set_System();
	Set_USBClock();

	USB_Interrupts_Config();
	USB_Init();
	
	(void)baud_count;
}

void Serial_end(void)
{
	NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_DisableIRQ(USBWakeUp_IRQn);
	GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
}



/* Received data */
uint8_t vcomBufferRx[256];
/* Write index to vcomBufferRx */
 uint32_t rx_head=0;
/* Read index from vcomBufferRx */
 uint32_t rx_tail=0;

/* Non-blocking byte lookahead.
 *
 * Looks at unread bytes without marking them as read. */
u32 usb_cdcacm_peek(u8* buf, u32 len)
{
    u32 i;
    u32 tail = rx_tail;
		
	u32 rx_unread = (rx_head-tail) & 255;

    if (len > rx_unread) {
        len = rx_unread;
    }

    for (i = 0; i < len; i++) {
        buf[i] = vcomBufferRx[tail];//ASCII
        tail = (tail + 1) & 255;
    }

    return len;
}

/* Non-blocking byte receive.
 *
 * Copies up to len bytes from our private data buffer (*NOT* the PMA)
 * into buf and deq's the FIFO. */
u32 usb_cdcacm_rx(u8* buf, u32 len)
{
    /* Copy bytes to buffer. */
    u32 n_copied = usb_cdcacm_peek(buf, len);

    /* Mark bytes as read. */
	u16 tail = rx_tail; // load volatile variable
	tail = (tail + n_copied) & 255;
	rx_tail = tail; // store volatile variable

	u32 rx_unread = (rx_head - tail) & 255;
    // If buffer was emptied to a pre-set value, re-enable the RX endpoint
    if ( rx_unread == 0 ) { // experimental value, gives the best performance
       flag_RX_USB = 0;rx_tail=0;rx_head=0;
	}
    return n_copied;
}



int Serial_available(void)
{
	
	return  (rx_head - rx_tail) & 255;
}

int Serial_peek(void)
{
    u8 b;
	if (usb_cdcacm_peek(&b, 1)==1)
	{
		return b;
	}
	else
	{
		return -1;
	}
}

int Serial_availableForWrite(void) { return Serial_available(); }


u32 Serial_readX(u8 * buf, u32 len) {
    u32 rxed = 0;
    while (rxed < len) {
        rxed += usb_cdcacm_rx(buf + rxed, len - rxed);
    }

    return rxed;
}

void Serial_flush(void)
{
/*Roger Clark. Rather slow method. Need to improve this */
    u8 b;
	while(Serial_available())
	{
		Serial_readX(&b, 1);
	}
    return;
}

int Serial_readBytes(char *buf, const u8 len)
{
    size_t rxed=0;
    unsigned long startMillis;
    startMillis = millis();
    if (len <= 0) return 0;
    do {
        rxed += usb_cdcacm_rx((u8 *)buf + rxed, len - rxed);
        if (rxed == len) return rxed;
    } while(millis() - startMillis < 1000);
    return rxed;
}

int Serial_read(void)
{
	
	u8 b;
	
	if (usb_cdcacm_rx(&b, 1)==0)
	{
		return 0;
	}
	else
	{
		return b;
	}
	
}



size_t Serial_Puts(const void *buffer, size_t size)
{
	/* only try to send bytes if the high-level CDC connection itself
	 is open (not just the pipe) - the OS should set lineState when the port
	 is opened and clear lineState when the port is closed.
	 bytes sent before the user opens the connection or after
	 the connection is closed are lost - just like with a UART. */
	
	// TODO - ZE - check behavior on different OSes and test what happens if an
	// open connection isn't broken cleanly (cable is yanked out, host dies
	// or locks up, or host virtual serial port hangs)
	if (bDeviceState == CONFIGURED)
	{
		int r = CDC_Send_DATA (buffer,size);

		if (r > 0)
		{
			return r;
		} else
		{
			//setWriteError();
			return 0;
		}
	}
	//setWriteError();
	return 0;
}

size_t Serial_write(uint8_t c) {
	return Serial_Puts(&c, 1);
}

// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (lineState != 0) but not quite opened.


//Serial_ SerialUSB;

//#endif
#endif
