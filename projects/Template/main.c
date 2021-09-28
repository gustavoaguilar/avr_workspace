#define F_CPU 1000000L

#include <avr/io.h>
#include <util/delay.h>


void main(void){
 
    DDRB |= (1 << PB1);
    
    while(1){
        PORTB |= (1 << PB1);
        _delay_ms(500);
        PORTB &= ~(1 << PB1);
        _delay_ms(500);
    }

}
