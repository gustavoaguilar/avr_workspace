#include "main.h"
#include "usart.h"

void main(void){
    int i = 0;
    DDRB |= (5 << PB5);
    init_usart(9600);
    
    while(1){
        // PORTB |= (1 << PB5);
        // _delay_ms(500);
        // PORTB &= ~(1 << PB5);
        _delay_ms(500);
        usart_print("tick");
    }
}
