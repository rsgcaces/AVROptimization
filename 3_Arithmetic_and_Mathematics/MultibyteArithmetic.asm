/*
 * MultibyteArithmetic.asm
 *
 *  Created: 8/19/2018 8:45:18 AM
 *   Author: Chris Darcy
 */ 
.def	A=r16
.def	B=r17
.org	0x0000
	rjmp	reset
reset:
	ldi		A,128
	ldi		B,128
	add		A,B
	brvs	overflow
wait:
	rjmp	wait
overflow:
	rjmp	PC-1
