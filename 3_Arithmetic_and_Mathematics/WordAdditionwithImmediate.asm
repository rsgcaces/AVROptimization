/*
 * WordAdditionwithImmediate.asm
 *
 *  Created: 8/20/2018 7:09:19 AM
 *   Author: Chris Darcy
 */ 
.def	total = r16
	rjmp	reset
.org	0x0020
matrix:
	.DB	0,0,0,0,0,0,0,0
	.DB	0,0,0,1,0,0,0,1
	.DB	0,0,1,1,1,1,0,4
	.DB	0,1,1,0,0,1,1,4
	.DB	1,1,1,1,1,1,1,7
	.DB	1,1,0,0,0,1,1,4
	.DB	1,1,0,0,0,1,1,4
	.DB	1,1,0,0,0,1,1,4
matrixEnd:
.org	0x0100
reset:
	ldi		ZL,low(matrix<<1)
	ldi		ZH,high(matrix<<1)
	ldi		YL,low(matrixEnd<<1)
	adiw	Z,7
	clr		total
next:
	lpm		r17,Z
	add		total,r17
	adiw	Z,8
	cp		ZL,YL			
	brpl	hold
	rjmp	next
hold:
	rjmp	hold