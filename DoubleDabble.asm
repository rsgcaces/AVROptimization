;https://en.wikipedia.org/wiki/Double_dabble
bin16BCD:						
	push	r16			;16 bit input parameter must be in r19:r18 
	push	r17			;5 digit BCD conversion will be in BCD4:BCD32:BCD10 = r22:r21:r20
	push	r18
	push	r19
	push	r23
	push	r24
	ldi		r23,0x03
	ldi		r24,0x30
	ldi		r17,16		;
	clr		r20			;
	clr		r21			;
	clr		r22				;
next:	
	lsl		r18				;Shift left input value Low byte
	rol		r19				;Shift left carry into input value High byte
	rol		r20				;Shift left carry into result byte 0 (two least significant digits).
	rol		r21				;Shift left carry into result byte 
	rol		r22				;Shift left carry into result byte 2 (most significant digit)
	dec		r17				;one less shift to perform
	breq	cya				;Done!
units:
	mov		r16,r20			;
	andi	r16,0x0F		;
	cpi		r16,0x05		;
	brlo	tens			;
	add		r20,r23			;
tens:
	cpi		r20,0x50		;
	brlo	hundreds		;
	add		r20,r24			;	
hundreds:
	mov		r16,r21			;
	andi	r16,0x0F		;
	cpi		r16,0x05		;
	brlo	thousands		;
	add		r21,r23			;
thousands:
	cpi		r21,0x50		;
	brlo	tenThousands	;
	add		r21,r24			;	
tenThousands:
	mov		r16,r22			;
	andi	r16,0x0F		;
	cpi		r16,0x05		;
	brlo	next			;
	add		r22,r23			;
	rjmp	next			;Repeat...
cya:	
	pop		r24	
	pop		r23	
	pop		r19	
	pop		r18	
	pop		r17
	pop		r16	
	ret						;return
