	bra	init
	bra	crapman1+12
	bra	crapman1+8

init:
	bsr	crapman1
	bra	crapman1+4
	
crapman1:
	incbin	"org/crapman1"
	