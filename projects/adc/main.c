/*
    Author: gustavoaguilar
    Date:   01/10/21

    This example is quite simple, just need to enable the ADC and it's
    interruptions, select a channel (Analog pin) and start a convertion
    with ADSC bit.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define THRESHOLD 50

ISR(ADC_vect){
    if(ADC >= THRESHOLD){
        PORTB |= (1 << PB1); // turn ON LED
    }else{
        PORTB &= ~(1 << PB1); // turn OFF LED
    }

    ADCSRA |= (1 << ADSC); // Triggers the next convertion
}

void init_mcu(){
    DDRB |= (1 << PB1);  // set PB1 as output
    
    ADCSRA = (1 << ADEN) | (1 << ADIE); // Enable ADC and its interruptions
    ADMUX = 0x02; // channel AD2
    
    ADCSRA |= (1 << ADSC); // starts a convertion

    sei(); // Enable interruptions
}

int main(void){
    init_mcu();

    while(1){} // do nothing

    return 0;
}
