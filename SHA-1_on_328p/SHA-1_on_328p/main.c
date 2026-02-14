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

static void sha1_selftest_abc(void)
{
	// "abc" als *vorverarbeiteter* 512-bit Block (64 Bytes)
	static const uint8_t block[64] = {
		0x61,0x62,0x63,0x80,
		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x18
	};

	uint32_t H[5];
	sha1_init(H);

	// >>> HIER rufst du die Funktion auf, die bei dir einen 64-Byte-Block verarbeitet:
	// z.B. sha1_process_block(H, block); oder sha1_compress(H, block);
	sha1_process_block(H, block);

	// Ausgabe als 5x32-bit in HEX (so wie du es eh machst)
	uart_puts("SHA1(abc)= ");
	for (uint8_t i=0;i<5;i++) uart_put_hex32(H[i]);
	uart_puts("\r\n");
}


int main(void)
{
	uint8_t block[64]={61626380000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018};
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
			sha1_update(&ctx, block, 63);  // Length of "Hello, AVR!"
			sha1_final(&ctx, digest);
			memset(rxBuffer, 0, RX_BUFFER_SIZE);
			rxBufPos = 0;
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