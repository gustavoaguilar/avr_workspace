#define F_CPU 16000000L
#define __AVR_ATmega328P__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "uart.h"

#define pin_set(port, pin) port |= _BV(pin)
#define pin_clear(port, pin) port &= ~_BV(pin)
#define pin_dir_output(port, pin) port |= _BV(pin)
#define pin_dir_input(port, pin) port &= ~_BV(pin)

UART_t UART;

ISR(USART_TX_vect){
    if(UART.buffer_tx_start < UART.buffer_tx_end){
        safe_increment_byte(&UART.buffer_tx_start, UART_MAX_BUFFER_TX_SIZE);
        UDR0 = UART.buffer_tx[UART.buffer_tx_start];
    }
}

ISR(USART_UDRE_vect){
    if(UART.buffer_tx_start != UART.buffer_tx_end){
        safe_increment_byte(&UART.buffer_tx_start, UART_MAX_BUFFER_TX_SIZE);
        UDR0 = UART.buffer_tx[UART.buffer_tx_start];
    }else{
        UCSR0B &= ~_BV(UDRIE0);
    }
}

ISR(USART_RX_vect){
    safe_increment_byte(&UART.buffer_rx_end, UART_MAX_BUFFER_RX_SIZE);
    UART.buffer_rx[UART.buffer_rx_end] = UDR0;
}

void main(void){
    uint32_t num = 0;
    pin_dir_output(DDRB, PB5);

    uart_init();

    sei();
        
    while(1){
        // _delay_ms(500);
        // uart_send_string(&UART, "________\r\n", 11);
        // uart_send_start();
        _delay_ms(1000);

        num++;
        uart_send_integer(&UART, num);
        uart_send_byte(&UART, '\r');
        uart_send_byte(&UART, '\n');
        uart_send_start();

        if(UART.buffer_rx_end != UART.buffer_rx_start){
            safe_increment_byte(&(UART.buffer_rx_start), UART_MAX_BUFFER_RX_SIZE);
            // // uart_send_byte(&UART, UART.buffer_rx[UART.buffer_rx_start]);
            // uart_send_start();

        }
    }

}