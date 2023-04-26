#ifndef I2C_H
#define I2C_H

#include<avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

#define I2C_MASTER_TWSR_START 0x8
#define I2C_MASTER_TWSR_START_REPEAT 0x10
#define I2C_MASTER_TWSR_SLA_W_ACK 0x18
#define I2C_MASTER_TWSR_SLA_W_NACK 0x20
#define I2C_MASTER_TWSR_SLA_R_ACK 0x40
#define I2C_MASTER_TWSR_SLA_R_NACK 0x48
#define I2C_MASTER_TWSR_DATA_TRANSMITTED_ACK 0x28
#define I2C_MASTER_TWSR_DATA_TRANSMITTED_NACK 0x30
#define I2C_MASTER_TWSR_DATA_RECEIVED_ACK 0x50
#define I2C_MASTER_TWSR_DATA_RECEIVED_NACK 0x58
#define I2C_MASTER_TWSR_ARBITRATION_LOST 0x38

void i2c_init(void){
    /*
        Frequency equation
        SCL_freq = CPU_F/(16 + (2 * (TWBR) * Prescaler))
        
        100k CPU_F = 16M, TWBR = 18, Prescaler = 4
    */ 
    TWBR = 18;
    TWCR = _BV(TWEN); //_BV(TWIE);
    TWSR |= _BV(TWPS0);
}

void i2c_write_byte(uint8_t addr, uint8_t byte){

    // uart_send_string(&UART, "Sending Start!\r\n", 17);
    TWCR |= _BV(TWINT) | _BV(TWSTA);
    
    //Wait start condition to be sent
    while(!(TWCR & _BV(TWINT))){}
    // uart_send_string(&UART, "Start SENT!\r\n", 14);

    if((TWSR & 0xF8) != I2C_MASTER_TWSR_START){
        //ERROR
        // uart_send_string(&UART, "Start ERROR!\r\n", 15);
        return;
    }

    // uart_send_string(&UART, "Sending Address!\r\n", 19);

    TWDR = (addr<<1);
    TWCR = _BV(TWINT) | _BV(TWEN);

    //Wait address to be sent
    while(!(TWCR & _BV(TWINT))){}
    // uart_send_string(&UART, "Address SENT!\r\n", 16);

    if((TWSR & 0xF8) != I2C_MASTER_TWSR_SLA_W_ACK){
        //ERROR
        // uart_send_string(&UART, "Address ERROR!\r\n", 17);
        return;
    }

    // uart_send_string(&UART, "Sending Data!\r\n", 16);
    TWDR = byte;
    TWCR = _BV(TWINT) | _BV(TWEN);

    while(!(TWCR & _BV(TWINT))){}

    // uart_send_string(&UART, "Data SENT!\r\n", 13);
    if((TWSR & 0xF8) != I2C_MASTER_TWSR_DATA_TRANSMITTED_ACK){
        // uart_send_string(&UART, "Data ERROR!\r\n", 14);
        return;
    }

    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

void i2c_read_byte(uint8_t addr){

    // uart_send_string(&UART, "Sending Start!\r\n", 17);
    TWCR |= _BV(TWINT) | _BV(TWSTA);
    
    //Wait start condition to be sent
    while(!(TWCR & _BV(TWINT))){}
    // uart_send_string(&UART, "Start SENT!\r\n", 14);

    if((TWSR & 0xF8) != I2C_MASTER_TWSR_START){
        //ERROR
        // uart_send_string(&UART, "Start ERROR!\r\n", 15);
        return;
    }

    // uart_send_string(&UART, "Sending Address!\r\n", 19);

    TWDR = (addr<<1) | 0x01;
    TWCR = _BV(TWINT) | _BV(TWEN);

    //Wait address to be sent
    while(!(TWCR & _BV(TWINT))){}
    // uart_send_string(&UART, "Address SENT!\r\n", 16);

    if((TWSR & 0xF8) != I2C_MASTER_TWSR_SLA_R_ACK){
        //ERROR
        // uart_send_string(&UART, "Address ERROR!\r\n", 17);
        return;
    }

    // uart_send_string(&UART, "Sending Data!\r\n", 16);
    TWDR = 0x00;
    TWCR = _BV(TWINT) | _BV(TWEN);

    while(!(TWCR & _BV(TWINT))){}
    
    uart_send_byte(&UART, TWDR);
    uart_send_start();

    // uart_send_string(&UART, "Data SENT!\r\n", 13);
    if((TWSR & 0xF8) != I2C_MASTER_TWSR_DATA_RECEIVED_NACK){
        // uart_send_string(&UART, "Data ERROR!\r\n", 14);
        return;
    }
    // uart_send_string(&UART, "Sending Stop!\r\n", 16);
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

#endif