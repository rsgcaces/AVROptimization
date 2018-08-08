/*
 * IfThenElse.asm
 *
 *  Created: 8/8/2018 3:19:27 PM
 *   Author: Chris Darcy
 */
.def	util = r16				;
.def	led = r17				;
.equ	COOL = 15				;
.equ	WARM = 25				;
.equ	temp = 10				;
.equ	red = 1<<PB2			;
.equ	blue = 1<<PB5			;
.equ	gnd = 1<<PB3			;

.org	0x0000
	rjmp	reset				;
.org	0x0100
reset:
	rcall	initPORT			;
again:
	rcall	getTemp				;
	cpi		r16,COOL			;
	brmi	sayCool				;
	cpi		r16,WARM			;
	brpl	sayWARM				;
	rjmp	again				;
sayCool:
	ldi		led,blue			;
	out		PORTB,led			;	
 	rjmp	again				;
sayWarm:
	ldi		led,red				;
	out		PORTB,led			;	
	rjmp	again				;

initPORT:
	ldi		util,red|gnd|blue	;
	out		DDRB,util			;
	ret	
getTemp:
	ldi		r16,temp			;
	ret
	