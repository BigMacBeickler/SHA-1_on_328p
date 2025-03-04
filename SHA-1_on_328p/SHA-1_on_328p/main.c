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

char test;

// sha-1 initial values, saves values in eeprom for keeping ram free, if they are not there already; eeprom_update_block for minimizing eeprom write cycles
const uint32_t sha1_initial_values[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

void write_hash_eeprom(){
	eeprom_update_block(sha1_initial_values, EEPROM_HASH_ADRESS, sizeof(sha1_initial_values));
}

int main(void)
{
	//interrupts enabled after init
	//sei();
	usart_init(9600);
	DDRB |= (1 << PB5);  // Setze PB0 als Ausgang (LED-Pin)

    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(500);
	UART_transmit(test);	
	PORTB ^= (1 << PB5); // Toggle LED
    }
}
