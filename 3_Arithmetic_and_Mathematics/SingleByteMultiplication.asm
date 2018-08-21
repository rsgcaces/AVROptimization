/*
 * SingleByteMultiplication.asm
 *  Created: 8/19/2018 1:48:06 PM
 *   Author: Chris Darcy
 */ 
.def	A=r16
.def	B=r17
.org	0x0000
	rjmp	reset
reset:
	ldi		A,128
	ldi		B,129
	mul		A,B
wait:
	rjmp	wait



