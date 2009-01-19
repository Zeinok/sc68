;
; MMME from Lethal Xcess
;
	bra	init
	bra	RPL+12
	bra.s	RPL+8

; a0 = six
; d1 = 0:STE  1:STF
init:
	lea	RPL(pc),a1
RPL	incbin	"org/lx_mmme"
	even
	dc.l	0
