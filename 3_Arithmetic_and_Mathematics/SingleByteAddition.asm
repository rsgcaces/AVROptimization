/*
 * SingleByteAddition.asm
 * Designed to generate Carry Flag
 *  Created: 8/19/2018 9:45:58 AM
 *   Author: Chris Darcy
 */ 
.def	A=r16
.def	B=r17
.org	0x0000
	rjmp	reset
reset:
	ldi		A,128
	ldi		B,129
	add		A,B
	brcs	overflow
wait:
	rjmp	wait
overflow:
	rjmp	PC-1