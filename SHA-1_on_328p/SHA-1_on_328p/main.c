/*
 * SHA-1_on_328p.c
 *
 * Created: 15.02.2025 13:08:27
 * Author : BigMac
 */ 

#include <avr/io.h>
#include "stdlib.h"
#include "usart.h"

#define F_CPU 16000000UL
#define RX_BUFFER_SIZE ( 2048 ) / 2  // Bit Gr��e des Empfangs-Buffers (halber Ram)
#define TX_BUFFER_SIZE 160  // Bit Gr��e des Sende-Buffers (sha-1  l�nge, andere �bertragung noetig, ggf fehlerfall?)

char rxBuffer[RX_BUFFER_SIZE];

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

