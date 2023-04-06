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

t_UART UART;

uint8_t data[5] = {'1','2','3','4','5'};
uint8_t pos = 0;

ISR(USART_TX_vect){
    if(UART.buffer_tx_start < UART.buffer_tx_end){
        safe_increment_byte(&UART.buffer_tx_start, UART_MAX_BUFFER_TX_SIZE);
        UDR0 = UART.buffer_tx[UART.buffer_tx_start];
    }
}


void main(void){
 
    pin_dir_output(DDRB, PB5);

    UCSR0A = 0; // not needed
    UCSR0B = 0b01001000;
    UCSR0C = 0b00000110;
    UBRR0H = 0;
    UBRR0L = 103;
    UART.buffer_rx_start =0;
    UART.buffer_rx_end =0;
    UART.buffer_tx_start = 0;
    UART.buffer_tx_end = 0;

    sei();
    // UDR0 = '0';
    uart_send_string(&UART, "Laurinha\r\n", 11);
    uart_send_start();
    
    while(1){
        pin_set(PORTB, PB5);
        _delay_ms(500);
        pin_clear(PORTB, PB5);
        _delay_ms(500);
        
        uart_send_string(&UART, "HIHI\r\n", 6);
        uart_send_start();
    }

}