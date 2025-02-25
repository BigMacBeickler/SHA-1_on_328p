/*
 * usart.h
 *
 * Created: 24.02.2025 19:25:08
 *  Author: BigMac
 */ 


#ifndef USART_H_
#define USART_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include <string.h>
#include <stdint-gcc.h>


//unsigned short int RX_BUFFER_SIZE;

void usart_init(unsigned int iBaud);
int usart_start();



#endif /* USART_H_ */