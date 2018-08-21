/*
 * SingleByteSubtraction.asm
 * Designed to generate underflow condition
 *  Created: 8/19/2018 9:47:05 AM
 *  Author: Chris Darcy
 */ 
.def	A=r16
.def	B=r17
.org	0x0000
	rjmp	reset
reset:
	ldi		A,128
	ldi		B,129
	sub		A,B
	brcs	underflow
wait:
	rjmp	wait
underflow:
	rjmp	PC-1

