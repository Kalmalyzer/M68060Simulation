
		SECTION	code,code
_main:

_Painter_sortPrimsIntoBuckets:
		movem.l	d2-d4/a2-a3,-(sp)

		tst.w	d0
		beq.s	.skip

		lea		$12345678,a0
		move.l	$12345678,a1

		moveq	#0,d2
.sortPrimPass1:
		lea		(a1,d0.l),a2

		move.w	$1234(a2),d1
		move.b	$1234+1(a2),d2

		move.w	(a0,d2.l*2),d3
		move.w	d3,$1234(a2)
		move.w	d0,(a0,d2.l*2)
		move.w	d1,d0
		bne.s	.sortPrimPass1

		lea		$12345678,a2

		move.w	#256,d0
		moveq	#0,d1
		moveq	#0,d3
.sortPrimPass2_bucket
		move.w	(a0)+,d1
		beq.s	.sortPrimPass2_skipBucket

.sortPrimPass2_prim
		lea		(a1,d1.l),a3

		move.w	$1234(a3),d2
		move.b	$1234(a3),d3

		move.w	(a2,d3.l*2),d4
		move.w	d4,$1234(a3)
		move.w	d1,(a2,d3.l*2)

		move.w	d2,d1
		bne.s	.sortPrimPass2_prim

.sortPrimPass2_skipBucket
		subq.w	#1,d0
		bne.s	.sortPrimPass2_bucket

.skip
		movem.l	(sp)+,d2-d4/a2-a3
		rts


;-----------------------------------------------------------------------------
;--- Painter_renderPrimBuckets ------------------------------------------
;-----------------------------------------------------------------------------
; void Painter_renderPrimBuckets()

_Painter_renderPrimBuckets
		movem.l	d2-d7/a2-a6,-(sp)

		move.l	#$12345678,a0
.bucket:
		moveq	#0,d0
		move.w	-(a0),d0
		beq.s	.skipBucket

		move.l	a0,-(sp)
.prim:
		move.l	$12345678,a0
		add.w	d0,a0
		move.l	a0,-(sp)

		move.w	$1234(a0),d0
		lea		$12345678,a1
		jsr		([4,a1,d0.w*8])
		move.l	(sp)+,a0
		move.w	$1234(a0),d0
		bne.s	.prim

		move.l	(sp)+,a0
.skipBucket:
		cmp.l	#$12345678,a0
		bne.s	.bucket

		movem.l	(sp)+,d2-d7/a2-a6
		rts

