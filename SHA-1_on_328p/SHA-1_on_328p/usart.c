/*
 * usart.c
 *
 * Created: 24.02.2025 19:26:08
 *  Author: BigMac
 */ 

#include "usart.h"

#define F_CPU 16000000UL

volatile char recieveByte;
volatile int pos = 0;
//usart initialisieren

void usart_init(unsigned int Baud){
	
	/*USART Prescaler berechnen und in Baud-Register schieben*/
	
	unsigned short int Baud_Prescaler = F_CPU / ( 16 * Baud ) - 1;
	
	UBRR0H = (unsigned char) Baud_Prescaler >> 8;
	UBRR0L = (unsigned char) Baud_Prescaler;
	
	/* Transmitter und Reciever aktivieren */
	
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 );
	
	/*Uebertragungsformat festlegen, ungerade Paritaet , Asynchron voreingestellt, 1 stop bit  */
	
	UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00);
	UCSR0C = ( 1 << UPM01 );
	
	/*Interrupts aktivieren*/
	UCSR0B = 1 << RXCIE0;
	sei();
}

/* Interrupt Routine bei Datenempfang */
/* Daten werden aus Register in recieveByte geschrieben, anschließend in den RxBuffer geschrieben, sollte dieser nicht voll sein.*/


ISR(USART_RX_vect){
	recieveByte = UDR0;
	rxBuffer[pos] = recieveByte;
	
	if(pos < RX_BUFFER_SIZE){
		pos ++;
	} else {
		while(pos > 0){
			rxBuffer[pos] = 0;
			pos --;
		}
	}
	
}