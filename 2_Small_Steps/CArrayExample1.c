/*
 * ArrayExample1.c
 *
 * Created: 8/17/2018 8:16:22 AM
 *  Author: Chris Darcy
 */ 

#include <avr/io.h>
uint8_t A [] = {16,2,77,40,107};
uint8_t B [] = {0x20,0x30};
uint8_t i, sum = 0;
int main(void)
{
	for(i=0; i<5; i++)
		sum += A[i];
}