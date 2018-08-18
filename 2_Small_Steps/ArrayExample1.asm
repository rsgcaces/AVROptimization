/*
 * ArrayExample1.asm
 *  Created: 8/18/2018 8:21:17 AM
 *   Author: Chris Darcy
 */ 
 .DSEG						;
 .BYTE	6					;reserve an even number of bytes in data segment (SRAM)  
 .CSEG
.org	0x0000
	rjmp	copy
.org	0x0030
A:	.DB	16,2,77,40,107,0	;define and initialize the array, A
Aend:
.org	0x0100
copy:						;since .BYTE does not permit initialization
	ldi		ZL,low(A<<1)	;we frst copy from program flash to SRAM
	ldi		ZH,high(A<<1)	;lpm instruction requires Z register
	ldi		YL,low(Aend<<1)	;point Y to the end of the array
	ldi		YH,high(Aend<<1);
	ldi		XL,0x00			;point X to the start of SRAM as the target for copy 
	ldi		XH,0x01			;
	lpm		r0,Z+			;Load from program memory and postincrement 
	st		X+,r0			;Store indirect and postincrement 
	cp		ZL,YL			;are we at the end of the array? Compare low bytes
	cpc		ZH,YH			;compare high bytes with carry 
	brne	PC-0x04			;Branch if not equal 
loop:						;leave X at the first address after the end of the array
	clr		r18				;zero a register for sum prior to accumulation loop 
	clr		ZL				;point Z to the beginning of the array (SRAM)
	ldi		ZH,0x01			;
	ld		r19,Z+			;get the (next) element of the array
	add		r18,r19			;add it to the running sum: sum += A[i];
	cp		ZL,XL			;are we at the end of the array? Compare low bytes 
	cpc		ZH,XH			;compare high bytes with carry 
	brne	PC-0x04			;branch if not at end
	st		Z,r18			;store sum in SRAM
wait:
	rjmp wait				;done...

