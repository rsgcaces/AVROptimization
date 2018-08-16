/*
 * VariablesFlash.asm
 *
 *  Created: 8/14/2018 6:29:39 AM
 *   Author: Chris Darcy
 */ 
.def	util=r16		;provide an alias for r16
.CSEG
.org	0x0000
	rjmp	reset
;Flash Variables...
.org	0x0030
vars:	.DB 'A', 255, 0b01010101, -128, 0xaa, "ab", 0
words:	.DW 0x5876, 0b1001110001010101, -32768, 65535
dwords:	.DD 0, 0xfadebeef, -2147483648, 1 << 30
qwords:	.DQ 0,0xfadefeeddeadbeef, 1 << 62
.org	0x0060
reset:
	ldi		ZH,high(vars<<1)	;the lpm instruction retrieves the byte pointed to by the Z register
	ldi		ZL,low(vars<<1)		;addresses are in words so mut be doubled for bytes
	lpm		util,Z+				;Step through these instructions with the Processor View open
	lpm		util,Z+				;watch r16 take on the respective byte values
	lpm		util,Z+				;lpm Z+ is the Load from Program Memory instruction with Post-Increment
	lpm		util,Z+				;which makes it convenient to undertake a byte walkthrough of Program Flash
	lpm		util,Z+				;
	lpm		util,Z+				;
	lpm		util,Z+				;
	lpm		util,Z+				;
	rjmp	reset				;redo


