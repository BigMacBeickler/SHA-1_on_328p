/*
* usart.c
*
* Created: 24.01.2026 19:26:08
*  Author: BigMac
*/

#include "usart.h"
#include "sha1.h"

#include "avr/io.h"
#include "avr/interrupt.h"
#include <string.h>
#include <stdint-gcc.h>


volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxBufPos = 0;
volatile uint8_t startFlag = 0;			// tell if algorithm can start
volatile uint8_t answearFlag = 0;		// tell if data can be send back
volatile uint8_t data;

/*initialize uart ;calculate presclaer and write values for dynamic baud rate;
enable tx & rx; Format: 8 data bit, no odd parity?, 1 stop bit; interrupt enabled
*/
void usart_init(unsigned int Baud){
	
	unsigned short int Baud_Prescale = (F_CPU / ( 16UL * Baud ) - 1);
	
	UBRR0H = Baud_Prescale >> 8;
	UBRR0L = Baud_Prescale;
	
	UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 ) | (1 << RXCIE0);
	
	UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00);
	
	sei();
}



void uart_sendByte(uint8_t data){
	while (!(UCSR0A & (1 << UDRE0)));	// Wait until buffer is empty
	UDR0 = data;						// Send data
}


void uart_sendString(const uint8_t *String)
{
	while(*String){
		uart_sendByte(*String++);
	}
}

//better human reable output. 
void uart_transmit_hex(uint8_t byte) {
	char hex[] = "0123456789ABCDEF";
	uart_sendByte(hex[byte >> 4]);  // Send higher byte
	uart_sendByte(hex[byte & 0x0F] );  // Send lower byte
}


//receive data, write in buffer if there is space, delete buffer if full, check for start or answearsignal

static inline void uart_sendByte_isr(uint8_t b)
{
	// Non-blocking: nur senden, wenn Datenregister frei
	if (UCSR0A & (1 << UDRE0)) {
		UDR0 = b;
	}
}



ISR(USART_RX_vect)
{
	uint8_t c = UDR0;

	//Echo in terminal to see what one is doing. Show enter, but don´t write it in the buffer.
	if (c == '\r' || c == '\n') {
		uart_sendByte_isr('\r');
		uart_sendByte_isr('\n');
		return;
	}

	// Make backspace nice again
	if (c == '\b' || c == 0x7F) {
		uart_sendByte_isr('\b');
		uart_sendByte_isr(' ');
		uart_sendByte_isr('\b');
		return;
	}

	// send to terminal
	uart_sendByte_isr(c);

	// write data in rxBuffer. Cancel everything if too much data. Setting the flags (Imagine little soldiers raising flags like https://de.wikipedia.org/wiki/Raising_the_Flag_on_Iwo_Jima
	if (rxBufPos >= RX_BUFFER_SIZE) {
		uart_sendString("Buffer overflow");
		rxBufPos = 0;
		startFlag = 0;
		answearFlag = 0;
		return;
	}

	rxBuffer[rxBufPos++] = c;

	if (rxBufPos >= 2 && rxBuffer[rxBufPos - 2] == '#') {
		if (rxBuffer[rxBufPos - 1] == '!') {
			rxBufPos -= 2;
			startFlag = 1;
			} else if (rxBuffer[rxBufPos - 1] == '$') {
			rxBufPos -= 2;
			answearFlag = 1;
		}
	}
}



