/*
 * uart.c
 *
 * Implementation of the UART API for several AVR controller
 *
 *  Created on: 26.03.2014
 *      Author: andreasbehnke
 */
#include <avr/io.h>
#include <util/delay.h>
#include "include/uart.h"

#ifdef UCSRA
#define UBRR0H UBRRH
#define UBRR0L UBRRL
#define UCSR0A UCSRA
#define UCSR0B UCSRB
#define UCSR0C UCSRC
#define RXEN0  RXEN
#define TXEN0  TXEN
#define UCSZ00 UCSZ0
#define UCSZ01 UCSZ1
#define RXCIE0 RXCIE
#define RXC0   RXC
#define UDR0   UDR
#define UDRIE0 UDRIE
#define UDRE0  UDRE
#define USART0_RX_vect   USART_RXC_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

#ifdef UCSR1A
#define UART_COUNT 2
#elif defined UCSR0A
#define UART_COUNT 1
#endif

void uart_init(uint8_t uart_index, uint16_t uart_baud) {
#ifdef UCSR1A
    switch (uart_index) {
        case 0:
#endif
            UBRR0H = (uint8_t) (uart_baud>>8);
            UBRR0L = (uint8_t) (uart_baud);
            UCSR0B = (1<<RXEN0)|(1<<TXEN0);
#ifdef URSEL
            UCSR0C = ( 1 << URSEL )|(1 << UCSZ01)|(1 << UCSZ00); // 8 bit and 1 stop bit
#else
            UCSR0C = (1 << UCSZ01)|(1 << UCSZ00); // 8 bit and 1 stop bit
#endif
#ifdef UCSR1A
            break;
        case 1:
            UBRR1H = (uint8_t) (uart_baud>>8);
            UBRR1L = (uint8_t) (uart_baud);
            UCSR1B = (1<<RXEN1)|(1<<TXEN1);
            UCSR1C = (1 << UCSZ11)|(1 << UCSZ10); // 8 bit and 1 stop bit
            break;
        default:
            break;
    }
#endif
}

/*
 * Sends one character to UART
 */
static int uart_put(char c, FILE *stream)
{
#ifdef UCSR1A
    uint8_t *uart_index = (uint8_t*)stream->udata;
    switch(*uart_index) {
        case 0:
#endif
            while (!(UCSR0A & (1<<UDRE0))) {}
            UDR0 = c;
#ifdef UCSR1A
            break;
        case 1:
            while (!(UCSR1A & (1<<UDRE1))) {}
            UDR1 = c;
            break;
    }
#endif

	return 0;
}

/*
 * Receive one character from UART.
 */
static int uart_get(FILE *stream)
{
#ifdef UCSR1A
    uint8_t *uart_index = (uint8_t*)stream->udata;
    switch(*uart_index) {
        case 0:
#endif
            while (!(UCSR0A & (1<<RXC0))) {}
            return UDR0;
#ifdef UCSR1A
        case 1:
            while (!(UCSR1A & (1<<RXC1))) {}
            return UDR1;
    }
    return EOF;
#endif
}

FILE *uart_open_stream(uint8_t uart_index) {
	FILE *stream = fdevopen (uart_put, uart_get);
	stream->udata = malloc(sizeof(uint8_t));
    *(uint8_t*)stream->udata = uart_index;
    return stream;
}
