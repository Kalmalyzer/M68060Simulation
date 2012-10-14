
; Example program
; This runs a 5-cycle code loop for 10.000 iterations, then returns

	PUBLIC	_runAsmCode

NUMITERATIONS = 10000
	
_runAsmCode:

	move.l	#NUMITERATIONS,d0
.iter:
	addx.l	d1,d1			; pOEP-only
	addx.l	d1,d1			; pOEP-only
	addx.l	d1,d1			; pOEP-only
	addx.l	d1,d1			; pOEP-only

	add.l	d1,d1			; pOEP|sOEP
	subq.l	#1,d0			; pOEP|sOEP
	bne.s	.iter			; is predicted and folded, takes 0 cycles
	
	move.l	#NUMITERATIONS,d0
	rts