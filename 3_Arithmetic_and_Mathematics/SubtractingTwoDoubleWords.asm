/*
 * SubtractingTwoDoubleWords.asm
 *
 *  Created: 8/20/2018 3:58:34 PM
 *   Author: Chris Darcy
 */ 

.set	opA = 0x09090909
.set	opB = 0x05050505
.def	A1=r18
.def	A2=r19
.def	A3=r20
.def	A4=r21
.def	B1=r22
.def	B2=r23
.def	B3=r24
.def	B4=r25
.org	0x0000
		rjmp	reset
.org	0x0100
reset:	
		ldi		A1,low(opA)
		ldi		A2,byte2(opA)
		ldi		A3,byte3(opA)
		ldi		A4,byte4(opA)
		ldi		B1,low(opB)
		ldi		B2,byte2(opB)
		ldi		B3,byte3(opB)
		ldi		B4,byte4(opB)
		sub		A1,B1
		sbc		A2,B2
		sbc		A3,B3
		sbc		A4,B4
hold:	rjmp	hold

