/*
 * usart.c
 *
 * Created: 24.01.2026 19:26:08
 *  Author: BigMac
 */ 

#include "usart.h"
#include "sha1.h"

#include "avr/io.h"
#include "avr/interrupt.h"
#include <string.h>
#include <stdint-gcc.h>


volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxBufPos = 0;
volatile uint8_t startFlag = 0;			// tell if algorithm can start
volatile uint8_t answearFlag = 0;		// tell if data can be send back
volatile uint8_t gotcalled = 0;
volatile uint8_t data;

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

/*
static inline void uart_sendByte_isr(uint8_t b)
{
	// Non-blocking: nur senden, wenn Datenregister frei
	if (UCSR0A & (1 << UDRE0)) {
		UDR0 = b;
	}
}



ISR(USART_RX_vect)
{
	uint8_t c = UDR0;

	// Echo im Terminal:
	// Enter hübsch darstellen, aber NICHT in den Hash-Buffer übernehmen
	if (c == '\r' || c == '\n') {
		uart_sendByte_isr('\r');
		uart_sendByte_isr('\n');
		return;
	}

	// Optional: Backspace hübsch behandeln
	if (c == '\b' || c == 0x7F) {
		uart_sendByte_isr('\b');
		uart_sendByte_isr(' ');
		uart_sendByte_isr('\b');
		// Wenn du auch im rxBuffer löschen willst, hier rxBufPos-- etc.
		return;
	}

	// Zeichen normal echoen
	uart_sendByte_isr(c);

	// --- dein bisheriger Code: in Buffer schreiben / Kommandos prüfen ---
	if (rxBufPos >= RX_BUFFER_SIZE) {
		rxBufPos = 0;
		startFlag = 0;
		answearFlag = 0;
		return;
	}

	rxBuffer[rxBufPos++] = c;

	if (rxBufPos >= 2 && rxBuffer[rxBufPos - 2] == '#') {
		if (rxBuffer[rxBufPos - 1] == '!') {
			rxBufPos -= 2;
			startFlag = 1;
			} else if (rxBuffer[rxBufPos - 1] == '$') {
			rxBufPos -= 2;
			answearFlag = 1;
		}
	}
}


*/

ISR(USART_RX_vect)
{
	uint8_t c = UDR0;

	// Optional: PuTTY-Zeilenenden ignorieren, damit Enter nicht "mitgehasht" wird
	if (c == '\r' || c == '\n') return;

	// Buffer overflow Schutz
	if (rxBufPos >= RX_BUFFER_SIZE) {
		rxBufPos = 0;
		startFlag = 0;
		answearFlag = 0;
		return;
	}

	rxBuffer[rxBufPos++] = c;

	// Kommandoerkennung: #! oder #$
	if (rxBufPos >= 2 && rxBuffer[rxBufPos - 2] == '#' ) {
		if (rxBuffer[rxBufPos - 1] == '!') {
			rxBufPos -= 2;      // Kommando NICHT in der Message behalten
			startFlag = 1;
			} else if (rxBuffer[rxBufPos - 1] == '$') {
			rxBufPos -= 2;      // Kommando NICHT in der Message behalten
			answearFlag = 1;
		}
	}
}