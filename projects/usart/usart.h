#ifndef USART_H
#define USART_H

#include "main.h"

#define USART_TX_ENABLE (UCSR0B |= _BV(TXEN0))
#define USART_TX_DISABLE (UCSR0B &= ~_BV(TXEN0))
#define USART_RX_ENABLE (UCSR0B |= _BV(RXEN0))
#define USART_RX_DISABLE (UCSR0B &= ~_BV(RXEN0))

#define USART_DATA_EMPTY_INTERRUPT_ENABLE (UCSR0B |= _BV(UDRIE0))
#define USART_DATA_EMPTY_INTERRUPT_DISABLE (UCSR0B &= ~_BV(UDRIE0))
#define USART_TX_INTERRUPT_ENABLE (UCSR0B |= _BV(TXCIE0))
#define USART_TX_INTERRUPT_DISABLE (UCSR0B &= ~_BV(TXCIE0))
#define USART_RX_INTERRUPT_ENABLE (UCSR0B |= _BV(RXCIE0))
#define USART_RX_INTERRUPT_DISABLE (UCSR0B &= ~_BV(RXCIE0))

#define USART_BAUD_RATE 9600
#define MYUBRR ((F_CPU + USART_BAUD_RATE * 8L) / (USART_BAUD_RATE * 16L) - 1)

#define USART_BUFFER_TX_SIZE 126
#define USART_BUFFER_RX_SIZE 126

typedef struct{
    uint8_t buffer_tx[USART_BUFFER_TX_SIZE];
    uint8_t start_index_tx;
    uint8_t end_index_tx;

    uint8_t buffer_rx[USART_BUFFER_RX_SIZE];
    uint8_t start_index_rx;
    uint8_t end_index_rx;
}tUsart;

#endif