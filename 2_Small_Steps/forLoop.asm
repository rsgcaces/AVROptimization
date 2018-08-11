/*
 * forLoop.asm
 * Performs 10 iterations (5 cycles) of Blinking LED on pin 13
 * Created: 8/11/2018 9:13:33 AM
 * Author: Chris Darcy
 */
.equ	START	= 0				;lower bound of for loop
.equ	END		= 10			;upper (inclusive) bound of for loop
.def	index	= r18			;index of the for loop (lcv)
.equ	PIN13	= 1<<PB5		;visual confirmation of iteration
.def	util	= r16			;generic utility register
.def	led		= r17			;led register for toggling purposes

.org	0x0000
	rjmp	setup				;let's use the Arduino C terminology 
.org	0x0100					;well past the interrupt jump vector table
setup:
	ldi		led,PIN13			;one-time code
	out		DDRB,led			;set PORTB bit 5 for output (pin 13)
loop:
	clr		util
	out		PORTB,util			;start with LED on pin 13 OFF
	ldi		index,START			;initialize loop control variable
forLoop:
	cpi		index,END			;are we finished?
	brge	exit				;if so, exit the for loop
	eor		util,led			;body of the for loop: toggle state of pin 13
	out		PORTB,util			;display it
	rcall	delay1s				;admire
	inc		index				;advance the loop control variable
	rjmp	forLoop				;back to the top of the for loop
exit:
	rcall	delay1s				;admire
	rcall	delay1s				;admire
	rcall	delay1s				;admire
	rjmp	loop
delay1s:
; Generated by delay loop calculator
; at http://www.bretmulvey.com/avrdelay.html
; Delay 16 000 000 cycles
; 1s at 16 MHz
    ldi  r19, 82
    ldi  r20, 43
    ldi  r21, 0
L1: dec  r21
    brne L1
    dec  r20
    brne L1
    dec  r19
    brne L1
    lpm
    nop
	ret