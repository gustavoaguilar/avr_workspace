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
#define SEG_6 0b01111101
#define SEG_7 0b00000111
#define SEG_8 0b01111111
#define SEG_9 0b01101111
#define SEG_0 0b00111111
#define SEG_DOT 0b10000000
#define SEG_BLANK 0b00000000

// inverted grid, idk why they made the module like that...
#define DISPLAY_1 0x0E
#define DISPLAY_2 0x0C
#define DISPLAY_3 0x0A
#define DISPLAY_4 0x08
#define DISPLAY_5 0x06
#define DISPLAY_6 0x04
#define DISPLAY_7 0x02
#define DISPLAY_8 0x00

#define LED_1 0x01
#define LED_2 0x03
#define LED_3 0x05
#define LED_4 0x07
#define LED_5 0x09
#define LED_6 0x0B
#define LED_7 0x0D
#define LED_8 0x0F

uint8_t value_to_seg[10] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7, SEG_8, SEG_9};

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

static void display_digit(int8_t addr, uint8_t digit){
	if(digit >= 0 && digit <= 9){
		send_data(CMD_SET_ADDR | addr, value_to_seg[digit]);
	}else {
		send_data(CMD_SET_ADDR | addr, SEG_BLANK);
	}
}

void display_number(uint32_t num){
	uint8_t disp = DISPLAY_1;
	
	// special cases
	if(num == 0){
		display_digit(disp, num);
		disp -= 0x02;
	} else if(num > 99999999){
		num = 0; // error, display only a Dot, to indicate error
		send_data(CMD_SET_ADDR | disp, SEG_DOT);
		disp -= 0x02;
	}

	// write all digitss
	while(num > 0){
		display_digit(disp, num%10);
        num /= 10;
		
		if(disp == 0x00){
			return;
		}
		disp -= 0x02; // jump to the next display
    }

	// clears the rest of the display
	while (disp >= 0x00){
		send_data(CMD_SET_ADDR | disp, SEG_BLANK);

		if(disp == 0x00){
			break;
		}
		disp -= 0x02;
	}
}

void write_led(uint8_t addr, uint8_t state){
	
	send_data(CMD_SET_ADDR | addr, state);
}

int main(void){
	mcu_init();
	
	send_cmd(CMD_DATA_COMMAND);
	send_cmd(CMD_DISP_CTRL);
	for(uint8_t i=0; i<16; i++){
		send_data(CMD_SET_ADDR|i, 0x00);
	}

	// display_number(99999999);
	write_led(LED_6, 1);

	/* loop */
	while (1) {
		// send_cmd(CMD_DATA_COMMAND);
		// send_cmd(CMD_DISP_CTRL);

		// send_cmd(CMD_DATA_COMMAND);
		// send_data(CMD_SET_ADDR, 5);
	}
}
