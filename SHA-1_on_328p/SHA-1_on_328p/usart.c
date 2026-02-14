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


volatile char rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxBufPos = 0;
volatile uint8_t startFlag = 0;			// tell if algorithm can start
volatile uint8_t answearFlag = 0;		// tell if data can be send back
volatile uint8_t gotcalled = 0;
volatile char data;

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

//unnoettig?
void uart_sendArray(uint8_t *array, uint16_t length)
{
	for(uint16_t n = 0; n < length; n++){
		uart_sendByte(array[n]);
	}
}

void uart_sendString(const uint8_t *String)
{
	while(*String){
		uart_sendByte(*String++);
	}
}

void uart_transmit_hex(uint8_t byte) {
	char hex[] = "0123456789ABCDEF";
	uart_sendByte(hex[byte >> 4]);  // Send high nibble
	uart_sendByte(hex[byte & 0x0F] );  // Send low nibble
	//uart_sendByte(byte);
}

void uart_DEBUG(const uint8_t *String)
{
	uint8_t mes[]= "DEBUG:"; 
	uint8_t newline = '\n';
		
	uart_sendString(mes);
	
	uart_sendString(String);

	uart_sendByte(newline);
}

//receive data, write in buffer if there is space, delete buffer if full, check for start or answearsignal

void save(){
	//uart_sendByte(data);
		if(((rxBufPos + 1) % RX_BUFFER_SIZE) > 0){
			rxBuffer[rxBufPos] = data;				//geht hier das ++ in der abfrage oder muss das nach kommen?
			rxBufPos++;
		} else {
			memset(rxBuffer, 0, RX_BUFFER_SIZE);
			startFlag = 0;
			answearFlag = 0;
		}
		uart_sendArray(rxBuffer, rxBufPos);
		if (rxBufPos >= 2) {
			if (rxBuffer[rxBufPos - 2] == '#' && rxBuffer[rxBufPos - 1] == '!') {
				startFlag = 1;
			} else if (rxBuffer[rxBufPos - 2] == '#' && rxBuffer[rxBufPos - 1] == '$') {
				answearFlag = 1;
			PORTB ^= ( 1 << PB5 );
			}
		}
}

ISR(USART_RX_vect){
	data = UDR0;
	gotcalled = 1;
}
