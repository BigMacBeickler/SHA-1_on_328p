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


#define EEPROM_HASH_ADRESS 0x00

// sha-1 initial values, saves values in eeprom for keeping ram free, if they are not there already; eeprom_update_block for minimizing eeprom write cycles
const uint32_t sha1_initial_values[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

void write_hash_eeprom(){
	eeprom_update_block(sha1_initial_values, EEPROM_HASH_ADRESS, sizeof(sha1_initial_values));
}



int main(void)
{
	////algorythmus bekommt 512 bit lange kette. Message + padding  [1,0,0,.] + Messagelänge = 512 bit
	//
	//write_hash_eeprom();
	//usart_init(9600);
	//
//
	//// --- SHA1 SELFTEST: "abc" ---
	//uint8_t block_test[64] = {0};
	//block_test[0] = 'a';
	//block_test[1] = 'b';
	//block_test[2] = 'c';
	//block_test[3] = 0x80;
	//block_test[63] = 0x18;   // 24 bit
//
	//uint8_t digest_test[20];
	//sha1_hash_one_preprocessed_block(block_test, digest_test);
//
	//uart_sendString((uint8_t*)"SHA1(abc) = ");
	//for (uint8_t i = 0; i < 20; i++) {
		//uart_transmit_hex(digest_test[i]);
	//}
	//uart_sendString((uint8_t*)"\r\nEXPECTED = A9993E364706816ABA3E25717850C26C9CD0D89D\r\n");
	//// --- Ende SELFTEST ---

	//
	//uint8_t block[64] = {0};      // alles 0
	//block[0] = 0x61;              // 'a'
	//block[1] = 0x62;              // 'b'
	//block[2] = 0x63;              // 'c'
	//block[3] = 0x80;              // Padding-Bit
	//block[63] = 0x18;             // Länge = 24 bit (0x0000000000000018)
	
	//UART_init();
	DDRB |= (1 << PB5);  // Setze PB0 als Ausgang (LED-Pin)

	SHA1_CTX ctx;
	
	//20 Byte = 160 Bit Ergebnis
	uint8_t digest[SHA1_BLOCK_SIZE];
	//uint8_t *message = "";
	
				    
    while (1) {
		//if(gotcalled){
			//gotcalled = 0;
			//save();
		//}
		
		if (startFlag) {
			uint16_t len;

			// Atomar len holen + Flag löschen (rxBufPos ist 16-bit!)
			cli();
			startFlag = 0;
			len = rxBufPos;
			sei();

			// Während des Hashens RX-Interrupt aus (sonst überschreibt dir ISR den Buffer)
			UCSR0B &= ~(1 << RXCIE0);

			sha1_init(&ctx);
			sha1_update(&ctx, (const uint8_t*)rxBuffer, len);
			sha1_final(&ctx, digest);

			// Buffer reset
			cli();
			rxBufPos = 0;
			sei();

			// RX Interrupt wieder an
			UCSR0B |= (1 << RXCIE0);
		}

		if (answearFlag) {
			answearFlag = 0;
			uart_sendString((uint8_t*)"SHA1: ");
			for (uint8_t i = 0; i < 20; i++) uart_transmit_hex(digest[i]);
			uart_sendString((uint8_t*)"\r\n");
			//Blink Blink
			PORTB ^= ( 1 << PB5 );
		}
	}
}