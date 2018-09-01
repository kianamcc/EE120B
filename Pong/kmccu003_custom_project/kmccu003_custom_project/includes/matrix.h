#ifndef MATRIX_H_
#define MATRIX_H_

#include <avr/io.h>
#include <stdbool.h>

#include "bit.h"

//use port b for matrix
#define DATA_HIGH() PORTB = SetBit(PORTB, 2, 1)
#define DATA_LOW()  PORTB = SetBit(PORTB, 2, 0)
#define CS_HIGH()   PORTB = SetBit(PORTB, 1, 1)
#define CS_LOW()    PORTB = SetBit(PORTB, 1, 0)
#define CLK_HIGH()  PORTB = SetBit(PORTB, 0, 1)
#define CLK_LOW()   PORTB = SetBit(PORTB, 0, 0)
#define INIT_PORT() DDRB |= SetBit(PORTB, 0, 1) | SetBit(PORTB, 1, 1) | SetBit(PORTB, 2, 1)

void MAX7219transmit_data(unsigned char data)
{
	for (unsigned char i = 0; i < 8; ++i, data <<= 1)  //transmit byte to MAX
	{
		CLK_LOW();
		
		if (data & 0x80) //upper nibble
		{
			DATA_HIGH();
		}
		
		else
		{
			DATA_LOW();
		}
		
		CLK_HIGH();
	}
}

void matrix_write(unsigned char x, unsigned char y) //turn on led at position
{
	CS_LOW();
	MAX7219transmit_data(x);
	MAX7219transmit_data(y);
	CS_HIGH();
}

//bool

void matrix_clear(void) //clear led matrix
{
	for (unsigned char i = 0; i < 8; ++i)
	{
		matrix_write(i + 1, 0);
	}
}

void matrix_init(void)
{
	INIT_PORT();
	matrix_write(0x09, 0); //binary -> led matrix
	matrix_write(0x0A, 1); //intensity/brightness
	matrix_write(0x0B, 7); //show all
	matrix_write(0x0C, 1); //start/power
	matrix_write(0x0F, 0); //display test
	matrix_clear();
}

#endif /* MATRIX_H_ */