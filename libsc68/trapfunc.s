;;; Atari ST GEMDOS and XBIOS emulator
;;;
;;; (C) COPYRIGHT 2011 Benjamin Gerard
;;;
;;; Gemdos (trap #1) and Xbios (trap #14) functions

;;; Install trap vectors
;;; 
install_trap:
	movem.l	d0/a0-a2,-(a7)
	
	;; Install trap vectors
	lea	trap_illegal(pc),a0
	lea	$80.w,a1
	moveq	#15,d0
.copy:
	move.l	a0,(a1)+
	addq	#8,a0
	dbf	d0,.copy
	lea	gemdos(pc),a0
	move.l	a0,$84.w
	
	;; lea	xbios(pc),a0
	;; move.l	a0,$B8.w
	
	;; Init dummy malloc system
	lea	malloc(pc),a0
	lea	-2048(a7),a1	     ; a7 is at near the end of memory
	move.l	a1,-(a0)

	movem.l	(a7)+,d0/a0-a2
	rts


open:	macro
	{
	movem.l	d0-a6,-(a7)		; 15*4
	lea	6+15*4(a7),a6
	move.w	(a6)+,d0		; function
	}

close:	macro
	{
	movem.l	(a7)+,d0-a6
	rte
	}	

return_d0 macro
	{
	move.l	d0,(a7)
	}

;;; Gemdos functions
;;;
;;; 0(a7).w -> SR
;;; 2(a7).l -> PC
;;; 6(a7).w -> Function !!! Cou

	
trap_close:
	close

trap_illegal:
	move.l	#$DEAD0000,d1
	illegal
	move.l	#$DEAD0001,d1
	illegal
	move.l	#$DEAD0002,d1
	illegal
	move.l	#$DEAD0003,d1
	illegal
	move.l	#$DEAD0004,d1
	illegal
	move.l	#$DEAD0005,d1
	illegal
	move.l	#$DEAD0006,d1
	illegal
	move.l	#$DEAD0007,d1
	illegal
	move.l	#$DEAD0008,d1
	illegal
	move.l	#$DEAD0009,d1
	illegal
	move.l	#$DEAD0010,d1
	illegal
	move.l	#$DEAD0011,d1
	illegal
	move.l	#$DEAD0012,d1
	illegal
	move.l	#$DEAD0013,d1
	illegal
	move.l	#$DEAD0014,d1
	illegal
	move.l	#$DEAD0015,d1
	illegal

	
;;; ======================================================================
;;; TRAP #1 GEMDOS
;;; ======================================================================
	
gemdos:
	open
	move.l	#$DEAD0D05,d1
	
	cmp.w	#$30,d0
	beq.s	sversion

	cmp.w	#$48,d0
	beq.s	malloc

	cmp.w	#$49,d0
	beq.s	mfree

	move.l	#$DEAD0D05,d1
	illegal

	
;;; ======================================================================
;;; sversion()
;;; trap #1 function 48 ($30)
;; @retval
;; 0x1300	TOS 1.00, TOS 1.02
;; 0x1500	TOS 1.04, TOS 1.06
;; 0x1700	TOS 1.62
;; 0x1900	TOS 2.01, TOS 2.05, TOS 2.06, TOS 3.01, TOS 3.05, TOS 3.06
;; 0x3000	TOS 4.00, TOS 4.01, TOS 4.02, TOS 4.03, TOS 4.04,
;;	 	MultiTOS 1.00, MultiTOS 1.08
sversion:
	move.l	#$1500,d0
	return_d0
	close

;;; ======================================================================
;;; malloc(amount.l)
;;; trap #1 function 72 ($48)
	dc.l 0				; malloc_ptr
malloc:
	lea	malloc(pc),a0
	move.l	-(a0),d0		; read malloc_ptr
	sub.l	(a6)+,d0		; alloc amount
	bclr	#0,d0			; ensure even ptr
	move.l	d0,(a0)			; save malloc_ptr
	return_d0
	close

;;; ======================================================================
;;; mfree(ptr)
;;; trap #1 function 73 ($49)
mfree:
	moveq	#0,d0
	return_d0
	close
	
;;; ======================================================================
;;; mxmalloc(amount.l,mode.w)
;;; trap #1 function 68 ($44)


;;; ======================================================================
;;; TRAP #E XBIOS
;;; ======================================================================

xbios:
	open
	move.l	#$DEAD0014,d1
	
	cmp.w	#$80,d0
	beq.s	locksnd

	cmp.w	#$81,d0
	beq.s	unlocksnd

	move.l	#$DEADB105,d1
	illegal

;;; ======================================================================
;;; Locksnd()
;;; trap #14 function 128 ($80)
	dc.w	0
locksnd:
	move.w	#-129,d0		; SNDLOCKED (-129)
	lea	locksnd(pc),a0
	tas.b	-(a0)
	bne.s	.locked
	moveq	#1,d0
.locked:
	return_d0
	close

;;; ======================================================================
;;; Unlocksnd()
;;; trap #14 function 129 ($81)
unlocksnd:
	moveq	#-128,d0		; SNDNOTLOCK (-128)
	lea	locksnd(pc),a0
	tst.b	-(a0)
	beq.s	.notlocked
	clr.b	(a0)
	moveq	#0,d0
.notlocked:
	return_d0
	close
