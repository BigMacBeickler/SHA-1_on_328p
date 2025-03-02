/*
 * SHA-1_on_328p.c
 *
 * Created: 15.02.2025 13:08:27
 * Author : BigMac
 */ 


#include "usart.h"
#include "sha1.h"

#include <avr/io.h>
#include "stdlib.h"
#include "avr/interrupt.h"
#include "avr/eeprom.h"

#define F_CPU 16000000UL
#define EEPROM_HASH_ADRESS 0x00

// sha-1 initial values, saves values in eeprom, if they are not there already; eeprom_update_block for minimizing eeprom write cycles
const uint32_t sha1_initial_values[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

void write_hash_eeprom(){
	eeprom_update_block(sha1_initial_values, EEPROM_HASH_ADRESS, sizeof(sha1_initial_values));
}

int main(void)
{
	//interrupts enabled after init
	//sei();
	usart_init(9600);

    /* Replace with your application code */
    while (1) 
    {
	send();	
    }
}
