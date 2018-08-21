/*
 * MultiplySingleBytebyPowerof2.asm
 *	This example quadruples the multiplicand
 *  originally placed in the AH:AL register pair 
 *  Created: 8/19/2018 5:01:32 PM
 * Author: Chris Darcy
 */ 
.def	AL=r16
.def	AH=r17
.set	multiplicand = 100
.org	0x0000
	rjmp	reset
reset:
	ldi		AL,multiplicand
	clr		AH
	lsl		AL
	rol		AH
	lsl		AL
	rol		AH
wait:
	rjmp	wait