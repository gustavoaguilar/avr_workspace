#define F_CPU 16000000L
#define __AVR_ATmega328P__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "i2c.h"

#define pin_set(port, pin) port |= _BV(pin)
#define pin_clear(port, pin) port &= ~_BV(pin)
#define pin_dir_output(port, pin) port |= _BV(pin)
#define pin_dir_input(port, pin) port &= ~_BV(pin)

UART_t UART;
Timer_t timer;

uint32_t period_timer;

void main(void){
    uint8_t i2c_led_value = 0;
    pin_dir_output(DDRB, PB5);
    pin_dir_output(DDRB, PB4);

    uart_init();
    timer_init();
    adc_init();
    i2c_init();

    sei();

    period_timer = timer_get();

    while(1){

        if(timer_wait(1, period_timer)){
            period_timer = timer_get();
            // uart_send_string(&UART, "Sensor AD: ", 12);
            // uart_send_integer(&UART, adc_get_value(7));
            // uart_send_byte(&UART, '\r');
            // uart_send_byte(&UART, '\n');
            // uart_send_start();

            i2c_write_byte(4, i2c_led_value);
            if(i2c_led_value)
                i2c_led_value = 0;
            else
                i2c_led_value = 1;
            _delay_ms(100);
            i2c_read_byte(4);
        }
        // _delay_ms(100);


        // if(UART.buffer_rx_end != UART.buffer_rx_start){
        //     safe_increment_byte(&(UART.buffer_rx_start), UART_MAX_BUFFER_RX_SIZE);
        //     // // uart_send_byte(&UART, UART.buffer_rx[UART.buffer_rx_start]);
        //     // uart_send_start();

        // }
    }

}

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

ISR(TIMER0_OVF_vect){
    TCNT0 = 6;
    timer.ms_counter++;
    if(timer.ms_counter >= 1000){
        timer.ms_counter = 0;
        timer.timestamp++;
    }
    PORTB ^= (1 << PB4); // toggle pin
}

// ISR(ADC_vect){
//     uart_send_integer(&UART, (uint32_t) ADC);
//     uart_send_byte(&UART, '\r');
//     uart_send_start();

//     ADCSRA |= (1 << ADSC); // Triggers the next convertion
// }