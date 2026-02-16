/*
* SHA-1_on_328p.c
*
* Created: 15.01.2026 13:08:27
* Author : BigMac
*/


#include "usart.h"
#include "sha1.h"

#define F_CPU 16000000UL

#include "util/delay.h"

#include <avr/io.h>
#include "stdlib.h"
#include "avr/interrupt.h"
#include "avr/eeprom.h"


int main(void)
{
	////algorythmus bekommt 512 bit lange kette. Message + padding  [1,0,0,.] + Messagelänge = 512 bit

	usart_init(9600);
	
	//UART_init();
	DDRB |= (1 << PB5);  // Setze PB0 als Ausgang (LED-Pin)

	SHA1_CTX ctx;
	
	//20 Byte = 160 Bit Ergebnis
	uint8_t digest[SHA1_BLOCK_SIZE];
	
	
	
	while (1) {
		
		if (startFlag) {
			uint16_t len;

			// Atomic getting len + delete Flag 
			cli();
			startFlag = 0;
			len = rxBufPos;
			sei();

			// deactivate RX-Interrupt to save the buffer while hashing
			UCSR0B &= ~(1 << RXCIE0);

			sha1_init(&ctx);
			sha1_update(&ctx, (const uint8_t*)rxBuffer, len);
			sha1_final(&ctx, digest);

			// Buffer reset
			cli();
			rxBufPos = 0;
			sei();

			// RX Interrupt back on
			UCSR0B |= (1 << RXCIE0);
		}

		if (answearFlag) {
			answearFlag = 0;
			uart_sendString((uint8_t*)"\r\n");
			uart_sendString((uint8_t*)"SHA1: ");
			for (uint8_t i = 0; i < 20; i++) uart_transmit_hex(digest[i]);
			uart_sendString((uint8_t*)"\r\n");
			//Blink Blink
			PINB = ( 1 << PB5 );
		}
	}
}