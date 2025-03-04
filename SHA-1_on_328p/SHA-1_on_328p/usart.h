/*
 * usart.h
 *
 * Created: 24.02.2025 19:25:08
 *  Author: BigMac
 */ 


#ifndef USART_H_
#define USART_H_


#include <stdint-gcc.h>

//unsigned short int RX_BUFFER_SIZE;

void usart_init(unsigned int iBaud);
uint8_t check();
//void send();
void UART_transmit(char x);



#endif /* USART_H_ */