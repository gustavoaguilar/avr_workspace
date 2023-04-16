#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UART_MAX_BUFFER_TX_SIZE 126
#define UART_MAX_BUFFER_RX_SIZE 126

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

#define USART_BAUD_RATE 115200
#define MYUBRR ((F_CPU + USART_BAUD_RATE * 8L) / (USART_BAUD_RATE * 16L) - 1)


/* If I want to have multiple uarts with multiple different sizes of buffer
    * I'd have to make the buffer external and link here with a pointer.
    */
typedef struct{
    uint8_t buffer_tx[UART_MAX_BUFFER_TX_SIZE];
    uint8_t buffer_tx_start;
    uint8_t buffer_tx_end;
    uint8_t buffer_rx[UART_MAX_BUFFER_RX_SIZE];
    uint8_t buffer_rx_start;
    uint8_t buffer_rx_end;
}t_UART;

extern t_UART UART;
extern uint8_t pos;

void uart_init(){
    UBRR0H = (uint8_t) (MYUBRR >> 8);
    UBRR0L = (uint8_t) MYUBRR;

    UCSR0A = 0; // Reset Status Register
    /*  Enable TX 
        Enable RX;
        Enable Data Transmit Interrupt */
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(UDRIE0); //_BV(TXCIE0) |;
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // Async 1 stop 8 bits no parity
    
    sei();
}

void safe_increment_byte(uint8_t *value, uint8_t max_value){
    if(*value < max_value-1)
        (*value)++;
    else
        *value = 0;
}

void uart_send_byte(t_UART *uart,uint8_t byte){
    safe_increment_byte(&(uart->buffer_tx_end), UART_MAX_BUFFER_TX_SIZE);
    uart->buffer_tx[uart->buffer_tx_end] = byte; 
}

void uart_receive_byte(t_UART *uart, uint8_t byte){
    safe_increment_byte(&(uart->buffer_rx_end), UART_MAX_BUFFER_RX_SIZE);
    uart->buffer_rx[uart->buffer_rx_end] = byte; 
}

void uart_send_string(t_UART *uart, char* str, uint16_t size){
    while(*str != '\0' && size > 0){
        uart_send_byte(uart, *str);
        str++;
        size--;
    }
    /* I will leave it here, just for safety. Things can get nasty.
    
    uint8_t c, i;
    for(i = 0; i < UART_MAX_BUFFER_TX_SIZE; i++){
        c = str[i];
        if(c != 0 && size > 0){
            uart_send_byte(uart, c);
            size--;
        } else {
            break;
        }
    }
    */ 
}

void uart_send_start(){
    UCSR0B |= _BV(UDRIE0);
}

void uart_send_blocking(){

}
#endif