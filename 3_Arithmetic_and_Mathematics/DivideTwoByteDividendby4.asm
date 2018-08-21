/*
 * DivideTwoByteDividendby4.asm
 * This example divides the dividend by 4
 *  Created: 8/19/2018 5:48:11 PM
 * Author: Chris Darcy
 */ 
.def	AL=r16
.def	AH=r17
.set	dividend = 400
.org	0x0000
	rjmp	reset
reset:
	ldi		AL,low(dividend)
	ldi		AH,high(dividend)
	lsr		AH
	ror		AL
	lsr		AH
	ror		AL
wait:
	rjmp	wait