#define F_CPU 1000000L

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define TM_STB PB2
#define TM_CLK PB1
#define TM_DIO PB0

#define SEG_1 0b00000110
#define SEG_2 0b01011011
#define SEG_3 0b01001111
#define SEG_4 0b01100110
#define SEG_5 0b01101101
#define SEG_6 0b01011111
#define SEG_7 0b00000111
#define SEG_8 0b01111111
#define SEG_9 0b01100111
#define SEG_0 0b00111111

// inverted grid, idk why they made the module like that...
#define GRID1_ADDR 0x0E
#define GRID2_ADDR 0x0C
#define GRID3_ADDR 0x0A
#define GRID4_ADDR 0x08
#define GRID5_ADDR 0x06
#define GRID6_ADDR 0x04
#define GRID7_ADDR 0x02
#define GRID8_ADDR 0x00

#define CMD_DATA_COMMAND 0x40
#define CMD_SET_ADDR 0xC0
#define CMD_DISP_CTRL 0x88

void mcu_init(void);
void transmit_byte(int8_t);


void mcu_init(){
	DDRB |= (1 << TM_DIO) | (1 << TM_CLK) | (1 << TM_STB);
	PORTB |= (1 << TM_CLK) | (1 << TM_STB); // Start at High

}

static void send_byte(int8_t byte){
	for (uint8_t i = 0; i < 8; ++i){
		PORTB &= ~(1 << TM_CLK);
		_delay_us(5);
		if(byte & 0x01){
			PORTB |= (1 << TM_DIO);
		} else {
			PORTB &= ~(1 << TM_DIO);
		}
		PORTB |= (1 << TM_CLK);
		
		byte >>= 1; // next bit
		_delay_us(5);
	}
}

static void send_cmd(int8_t cmd){
	PORTB &= ~(1 << TM_STB);

	send_byte(cmd);

	PORTB |= (1 << TM_STB);
}

static void send_data(int8_t byte, int8_t data){
	PORTB &= ~(1 << TM_STB);

	send_byte(byte);
	send_byte(data);

	PORTB |= (1 << TM_STB);
}



int main(void){
	mcu_init();
	
	send_cmd(CMD_DATA_COMMAND);
	send_cmd(CMD_DISP_CTRL);
	for(uint8_t i=0; i<16; i++){
		send_data(CMD_SET_ADDR|i, 0x00);
	}

	send_cmd(CMD_DATA_COMMAND);
	send_data(0x0A, SEG_1);

	/* loop */
	while (1) {
		// send_cmd(CMD_DATA_COMMAND);
		// send_cmd(CMD_DISP_CTRL);

		// send_cmd(CMD_DATA_COMMAND);
		// send_data(CMD_SET_ADDR, 5);
	}
}
