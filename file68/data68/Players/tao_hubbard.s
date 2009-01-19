	bra	init
	bra	player+12
	bra	player+8

init:
	lea	player(pc),a2
	move.l	a0,$dd2(a2)
	move.l	#$4e714e71,$18(a2)
	subq	#1,d0
player:
	incbin	"org/tao_hubbard"
	even
	dc.w	0
	
