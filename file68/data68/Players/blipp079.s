; blippblop079 sc68 wrapper
; by ben(jamin) gerard <ben@sashipa.com>
;
; Play a .TUN file [fourCC = 'Elof']

	bra	m+0
	rts
	rts
	bra	m+16

m:
	incbin	"org/blipp079.bin"
tune:
	; Tune must be located here !!
