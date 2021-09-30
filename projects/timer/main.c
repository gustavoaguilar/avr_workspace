#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 1000000L // Internal 1Mhz clock

#define COUNT_VALUE 11 // the Timer counts 250ms (256-244)

int8_t counter = 0; // used to count multiple interrupts

ISR(TIMER0_OVF_vect){
    counter++;
    TCNT0 = COUNT_VALUE;
    if(counter == 2){// 500ms
        PORTB ^= (1 << PB1); // toggle pin
        counter = 0;
    }
}

void init_mcu(){
    DDRB |= (1 << PB1); // set PB1 as output

    TCCR0A = 0b00000000; // normal mode
    TCCR0B = 0b00000101; // 1024 prescaler
    TCNT0 = COUNT_VALUE;  // timer counter starts at COUNT_VALUE
    TIMSK |= (1 << TOIE0); // enables timer 0 interruption

    sei(); // Enable interruptions
}

int main(void){
    init_mcu();

    while(1){} // do nothing
    return 0;
}
