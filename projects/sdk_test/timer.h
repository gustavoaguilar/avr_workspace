#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

//Very early on time counting... this can change a lot in the future
typedef struct{
    volatile uint32_t timestamp; // it can count up to 136 years running non stop
    volatile uint16_t ms_counter;
}Timer_t;

extern Timer_t timer;

// equation:   time = prescaler*(max_register - value) * 1/CPU_F

void timer_init(void){
    TCCR0A = 0b00000000; // normal mode
    TCCR0B = 0b00000011; // 64 prescaler
    TCNT0 = 6;  // timer counter starts at COUNT_VALUE
    TIMSK0 |= (1 << TOIE0); // enables timer 0 interruption
}

uint8_t timer_wait(uint32_t ammount, uint32_t time_counter){
    if(time_counter + ammount <= timer.timestamp)
        return 1;
    else
        return 0;
}

// Maybe the code will be prettier
uint32_t timer_get(void){
    return timer.timestamp;
}

#endif