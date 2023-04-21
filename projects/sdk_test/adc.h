#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

void adc_init(){
    PRR &= ~_BV(PRADC);
    ADCSRA = _BV(ADEN);
}

uint16_t adc_get_value(uint8_t channel){
    while(ADCSRA & _BV(ADSC)){} // wait for previous convertion;

    if(channel >= 9){ // check if the channel is valid
        channel = 15; // defaults to the GND
    }
    ADMUX = channel & 0x0F; // select channel
    
    ADCSRA |= _BV(ADSC); // starts a convertion
    
    while(ADCSRA & _BV(ADSC)){} // wait convertion to end

    return ADC; // return 16bit value
}

#endif