/*
 * usart.c
 *
 * Created: 24.02.2025 19:26:08
 *  Author: BigMac
 */ 

#include "usart.h"
#include "sha1.h"

#include "avr/io.h"
#include "avr/interrupt.h"
#include <string.h>
#include <stdint-gcc.h>


#define F_CPU 16000000UL
#define RX_BUFFER_SIZE ( 2048 ) / 2		// Byte-size of receive buffer, half the ram, more possible?
#define TX_BUFFER_SIZE 20				// Byte-size of transmit buffer, probably the size of the hashed value (160 bit)?

volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxBufPos = 0;

/*initialize uart ;calculate presclaer and write values for dynamic baud rate;
enable tx & rx; Format: 8 data bit, odd parity, 1 stop bit; interrupt enabled
*/
void usart_init(unsigned int Baud){
	
	unsigned short int Baud_Prescaler = (F_CPU / ( 16UL * Baud ) - 1);
	
	UBRR0H = Baud_Prescaler >> 8;
	UBRR0L = Baud_Prescaler;
	
	UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 ) | (RXCIE0 << 1);
	
	UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00) | ( 1 << UPM01 );
	
	sei();
}

void uart_sendByte(uint8_t data){
	while (!(UCSR0A & (1 << UDRE0)));	// Wait until buffer is empty
	UDR0 = data;						// Send data
}

//unnoettig?
void uart_sendArray(uint8_t *array, uint16_t length)
{
	for(uint8_t n = 0; n < length; n++){
		uart_sendByte(array[n]);
	}
}

void uart_sendString(const uint8_t *String)
{
	while(*String){
		uart_sendByte(*String++);
	}
}

void uart_DEBUG(const uint8_t *String)
{
	uint8_t mes[]= "DEBUG:"; 
	uint8_t newline = '\n';
		
	uart_sendString(mes);
	
	uart_sendString(String);

	uart_sendByte(newline);
}

