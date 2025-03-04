/*
 * SHA-1_on_328p.c
 *
 * Created: 15.02.2025 13:08:27
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


#define EEPROM_HASH_ADRESS 0x00

// sha-1 initial values, saves values in eeprom for keeping ram free, if they are not there already; eeprom_update_block for minimizing eeprom write cycles
const uint32_t sha1_initial_values[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

void write_hash_eeprom(){
	eeprom_update_block(sha1_initial_values, EEPROM_HASH_ADRESS, sizeof(sha1_initial_values));
}



int main(void)
{
	write_hash_eeprom();
	usart_init(9600);
	//UART_init();
	DDRB |= (1 << PB5);  // Setze PB0 als Ausgang (LED-Pin)

	    SHA1_CTX ctx;
	    uint8_t digest[SHA1_BLOCK_SIZE];
	    const char *message = "abc";
		//const char *message = "Hello, AVR!";
				    
    while (1) {
		
		if(gotcalled){
			gotcalled = 0;
			save();
			//uart_sendByte(rxBuffer[rxBufPos-1]);
			//uart_sendArray(rxBuffer, rxBufPos);
		}
		if(startFlag){
			cli();
			startFlag=0;
			uart_sendString(message);
			sha1_init(&ctx);
			sha1_update(&ctx, message, 3);  // Length of "Hello, AVR!"
			sha1_final(&ctx, digest);
			memset(rxBuffer, 0, RX_BUFFER_SIZE);
			sei();
		}
		
		if(answearFlag){
			answearFlag = 0;
			    uart_sendString("SHA1: ");
			    for (uint8_t i = 0; i < 20; i++) {
				    uart_transmit_hex(digest[i]);
					uart_sendByte(' ');
					//uart_sendByte('\n');
			    }
			    uart_sendByte('\n');
			PORTB ^= ( 1 << PB5 );
		}
	
	}
}