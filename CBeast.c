/*
 * CBeast.c
 * Example of AVR-gcc C Code to present the value of pi on 'The Beast'.
 *'The Beast' is a PCB designed to present a 12-digit PoV display using 
 * a normal 595 shift registers for the segment and TPIC6C595 Power logic current sink 
 * Created: 8/10/2018 1:10:28 PM
 * Author: Chris Darcy
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL  // 16 MHz
#include <util/delay.h>

uint8_t latch = 1<<PB2;			//digital pin 10
uint8_t clock = 1<<PB3;			//digital pin 11
uint8_t data = 1<<PB4;			//digital pin 12

// Seven Segment Order:dGFEDCBA
uint8_t dp = 1<<7;
//Assemble the hexadecimal segment maps into single array...
uint8_t  segMaps[] = {0b00111111,0b00000110, 0b01011011, 0b01001111,0b01100110,0b01101101,0b01111100,0b00000111,0b01111111,0b01100111,
	                  0b01110111,0b01111100,0b00111001,0b01011110,0b01111001,0b01110001};

char display[] = "314159265539";  //Sample...

//my 'super' shiftout (23 bits are shifted in one go)
void shiftOut(uint8_t d, uint8_t c, uint8_t l, uint32_t segMap, uint32_t digit ){
	uint32_t bits = (segMap<<16) | digit;
	// assemble the 23-bit shift out value from the segMap and the respective digit
	//pull latch low...
	PORTB &= ~latch;
	//synchronously clock in the data bits 
	for (uint32_t mask=1L<<23; mask>0; mask>>=1)	{
		PORTB &= ~clock;
		// should the data bit be set or clear?
		if (bits & mask)
			PORTB |= data;
		else 
			PORTB &= ~data;
		PORTB |= clock;
	}	
	//pull latch high to present present flipflops on the output pins...
	PORTB |= latch;
}

int main(void)
{
  // Let's use three pins of portB for the shifting...
  DDRB = 0xFF;
  uint8_t i = 0;
  uint32_t segments; 
  while(1) {
	segments = segMaps[display[i]-48];
	if (!i) segments |= dp;  //add decimal point on the 3 for pi 
	shiftOut(data,clock,latch,segments,1L<<(11-i));
	i = (i+1) % 12;
  }
}
