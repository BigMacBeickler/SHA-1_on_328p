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
#define RX_BUFFER_SIZE ( 2048 ) / 2		// Byte-size of receive buffer, half the ram
#define TX_BUFFER_SIZE 20				// Byte-size of transmit buffer, probably the size of the hashed value (160 bit)?

volatile uint8_t pos = 0;
volatile uint8_t error = 0;
volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t test = 'a';

/*initialize uart ;calculate presclaer and write values for dynamic baud rate;
enable tx & rx; Format: 8 data bit, odd parity, 1 stop bit; interrupt enabled
*/
void usart_init(unsigned int Baud){
	
	unsigned short int Baud_Prescaler = (F_CPU / ( 16UL * Baud ) - 1);
	
	UBRR0H = (unsigned char) Baud_Prescaler >> 8;
	UBRR0L = (unsigned char) Baud_Prescaler;
	
	UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 );
	
	UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00);
	UCSR0C = ( 1 << UPM01 );
	
	UCSR0B = 1 << RXCIE0;
	sei();
}

/* Interrupt for receiving data */
/* read from register, check if position is within buffer range, if yes, store in data array*/


//rewrite for both checks "#!" and "#$"
uint8_t check(){
	/* ' for char litarals aka asci chars and " for strings with \0 terminator */
	if(1){
		return (rxBuffer[pos-1] == '#' && rxBuffer[pos] == '!');
	}
	return 0;
}

/*
void send() {
	PORTB |= (0 << PB5); // Toggle LED
	while(!(UCSR0A & (1 << UDRE0)));
	PORTB |= (1 << PB5);
	UDR0 = test;

}*/

void UART_transmit(char x) {
	
	while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer

	UDR0 = (x+1); // Send data
}

