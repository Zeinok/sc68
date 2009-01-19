	bra	init
	bra	replay+12
	bra.s	replay

init:
	moveq	#1,d0
	bsr.s	replay+4
	bra.s	replay+8
replay:
	incbin	"org/digitfmx"
	
