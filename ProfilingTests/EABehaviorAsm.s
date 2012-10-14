
	PUBLIC	_moveTest

NUMITERATIONS = 10000
	
_moveTest:

	lea	buffer,a0
	move.l	d4,-(sp)
	move.l	d3,-(sp)
	move.l	d2,-(sp)
	moveq	#0,d4
	move.l	#NUMITERATIONS,d0
.iter:
	REPT	5
	addx.l	d1,d1			; pOEP-only
	ENDR
;	move.l	#1,4(a0,d4.w*8)
	move.w	#1,4(a0)
;	move.l	4(a0,d4.w*8),d2
	add.l	d3,d3
	REPT	4
	addx.l	d1,d1			; pOEP-only
	ENDR

	add.l	d1,d1			; pOEP|sOEP
	subq.l	#1,d0			; pOEP|sOEP
	bne.s	.iter			; is predicted and folded, takes 0 cycles

	move.l	(sp)+,d2
	move.l	(sp)+,d3
	move.l	(sp)+,d4
	
	move.l	#NUMITERATIONS,d0
	rts
	
	cnop	0,4
buffer	ds.l	2