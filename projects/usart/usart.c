#include "usart.h"

tUsart Serial;

void init_usart(int baudrate){
    UBRR0H = (uint8_t) (MYUBRR >> 8);
    UBRR0L = (uint8_t) MYUBRR;

    UCSR0A = 0; // Reset Status Register
    /*  Enable TX 
        Enable RX;
        Enable Data Empty Interrupt */
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // Async 1 stop 8 bits no parity
    
    sei();
}

void usart_send_byte(uint8_t data){
    if(Serial.end_index_tx >= USART_BUFFER_TX_SIZE){
        Serial.end_index_tx = 0;
    }
    Serial.buffer_tx[Serial.end_index_tx++] = data;
}

void usart_send_string(char* data){
    uint8_t i = 0;
    while(data[i] != '\0'){
        usart_send_byte(data[i]);
        i++;
        if(i == UINT8_MAX){
            break;
        }
    }
}

void usart_start_tx(void){
    UCSR0B |= _BV(UDRIE0);
}

void usart_stop_tx(void){
    UCSR0B &= ~_BV(UDRIE0);
}

// Just used for simple text
void usart_print(char* string){
    usart_send_string(string);
    usart_send_string("\r\n");
    usart_start_tx();
}

ISR(USART_UDRE_vect){
    if(Serial.start_index_tx != Serial.end_index_tx){
        UDR0 = Serial.buffer_tx[Serial.start_index_tx++];
    }else{
        UCSR0B |= ~_BV(UDRIE0);
    }
}
