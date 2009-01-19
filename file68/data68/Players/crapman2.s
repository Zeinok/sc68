	bra	init
	bra	crapman2+12
	bra	crapman2+8

init:
	bsr	crapman2
	bra	crapman2+4
	
crapman2:
	incbin	"org/crapman2"
	