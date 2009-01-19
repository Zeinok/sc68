	bra	init
	bra	player+12
	bra	player+8

;inst:	move.l	$800.w,a2

init:
	lea	player(pc),a2
	move.l	#$4e714e71,d2
	move.l	d2,$ac(a2)
	move.l	d2,$8a2(a2)
	
	lea	$d92(a2),a3
	move.l	a3,$d9a(a2)
	move.l	a0,a2
player:
	incbin	"org/tao_digi"
	even
	dc.w	0
	
