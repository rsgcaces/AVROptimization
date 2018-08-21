/*
 * SingleByteSignedMultiplication.asm
 *
 *  Created: 8/19/2018 2:07:28 PM
 *   Author: Chris Darcy
 */ 
.def	A=r16
.def	B=r17
.org	0x0000
	rjmp	reset
reset:
	ldi		A,-5
	ldi		B,0xF0
	mul		A,B
	movw	A,r0
wait:
	rjmp	wait