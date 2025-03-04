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

uint8_t testarray[] = {'1','2','3','4'};
uint32_t count = 0;
uint32_t count2 = 0;

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

	
	
    while (1) {
		
		if(gotcalled){
			save();
			gotcalled = 0;
			//uart_sendByte(rxBuffer[rxBufPos-1]);
			//uart_sendArray(rxBuffer, rxBufPos);
		}
		
		if(answearFlag){
			uart_sendString(rxBuffer);
			PORTB ^= ( 1 << PB5 );
			answearFlag = 0;
		}
	
	}
}