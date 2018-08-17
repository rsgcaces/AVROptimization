/*
 * CBlink.c
 *
 * Created: 8/17/2018 9:07:44 AM
 *  Author: Chris Darcy
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL  // 16 MHz
#include <util/delay.h>
int main(void)
{
	DDRB |= 1<<PB5;
    while(1)
    {
		PORTB ^= 1<<PB5;
		_delay_ms(1000);
    }
}