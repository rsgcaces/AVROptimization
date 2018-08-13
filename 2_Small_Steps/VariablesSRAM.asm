/*
 * VariablesSRAM.asm
 * Created: 8/12/2018 3:32:09 PM
 * Author: Chris D'Arcy
 */ 
.DSEG
count:
.BYTE	2				;reserve two bytes in SRAM
.def	util=r16		;provide an alias for r16
.CSEG
.org	0x0000
	rjmp	reset
.org	0x0100
reset:
	ldi		util,0xAB	;prepare a sample value for storage
	sts		count,util	;an 'assignemnt' statement
	lds		r17,count	;retrieve the value from SRAM
	swap	r17			;perform some verifiable operation
	sts		count+1,r17	;store the modified byte value
	rjmp	reset
