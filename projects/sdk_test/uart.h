#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UART_MAX_BUFFER_TX_SIZE 124
#define UART_MAX_BUFFER_RX_SIZE 124

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

void safe_increment_byte(uint8_t *value, uint8_t max_value){
    if(*value < max_value)
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
    while(size > 0){
        uart_send_byte(uart, *str);
        str++;
        size--;
    }
}

void uart_send_start(){
    safe_increment_byte(&UART.buffer_tx_start, UART_MAX_BUFFER_TX_SIZE);
    UDR0 = UART.buffer_tx[UART.buffer_tx_start];
}
#endif