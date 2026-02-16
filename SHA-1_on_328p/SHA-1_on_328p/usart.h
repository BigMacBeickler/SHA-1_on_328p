/*
 * usart.h
 *
 * Created: 24.01.2026 19:25:08
 *  Author: BigMac
 */ 


#ifndef USART_H_
#define USART_H_

#include <stdint-gcc.h>

#define F_CPU 16000000UL
#define RX_BUFFER_SIZE 1024		// Byte-size of receive buffer, half the ram, more possible?

extern volatile uint8_t rxBuffer[];
extern volatile uint16_t rxBufPos;
extern volatile uint8_t startFlag;
extern volatile uint8_t answearFlag;
extern volatile uint8_t gotcalled;

void usart_init(unsigned int iBaud);
void uart_sendByte(uint8_t data);
void uart_sendArray(uint8_t *array, uint16_t length);
void uart_sendString(const uint8_t *String);
void uart_transmit_hex(uint8_t byte);
void uart_DEBUG(const uint8_t *String);
void save();


#endif /* USART_H_ */