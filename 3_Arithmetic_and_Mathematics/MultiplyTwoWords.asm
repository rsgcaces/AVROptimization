/*
 * MultiplyTwoWords.asm
 *  Created: 8/21/2018 11:00:48 AM
 *  Author: Chris Darcy
 *	Adapted from https://sites.google.com/site/avrasmintro/home/2b-basic-math
 */ 
.def	zero = R2					;To hold Zero
.def	AL = R16					;To hold multiplicand
.def	AH = R17					;
.def	BL = R18					;To hold multiplier
.def	BH = R19					;
.def	ANS1 = R20					;LSB of 32-bit product
.def	ANS2 = R21					;
.def	ANS3 = R22					;
.def	ANS4 = R23					;MSB of 32-bit product
.set	multiplicand = 0x5050		;AxB=0x5050 * 0x4040
.set	multiplier = 0x4040			;	=0x1428_1400
.org	0x0000
	rjmp	reset
.org	0x0100
reset:
	ldi		AL,low(multiplicand)	;Load multiplicand into AH:AL
	ldi		AH,high(multiplicand)	;
	ldi		BL,low(multiplier)		;Load multiplier into BH:BL
	ldi		BH,high(multiplier)		;
mul16x16:
	clr		zero				;Set R2 to zero
	mul		AH,BH				;Multiply high bytes: AHxBH
	movw	ANS4:ANS3,r1:r0		;Move two-byte result into MSW of answer

	mul		AL,BL				;Multiply low bytes: ALxBL
	movw	ANS2:ANS1,R1:R0		;Move two-byte result into LSW of answer

	mul		AH,BL				;Multiply: AHxBL
	add		ANS2,R0				;Add result to answer
	adc		ANS3,R1				;
	adc		ANS4,ZERO			;Add the Carry Bit

	mul		BH,AL				;Multiply BHxAL
	add		ANS2,R0				;Add result to answer
	adc		ANS3,R1				;
	adc		ANS4,ZERO			;Add the Carry Bit
hold:
	rjmp	hold
