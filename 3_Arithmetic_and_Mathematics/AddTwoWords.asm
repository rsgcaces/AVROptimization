/*
 * AddTwoWords.asm
 *
 *  Created: 8/20/2018 3:24:28 PM
 *   Author: Chris Darcy
 */ 
.set	opA = 0x0404
.set	opB = 0x0505
.def	AL=r18
.def	AH=r19
.def	BL=r20
.def	BH=r21
.org	0x0000
		rjmp	reset
.org	0x0100
reset:	
		ldi		AL,low(opA)
		ldi		AH,high(opA)
		ldi		BL,low(opB)
		ldi		BH,high(opB)
		add		BL,AL
		adc		BH,AH
hold:	rjmp	hold