
# Makefile for sc68 tools.
#

CFLAGS   = -Wall -pedantic -g -O0

all: gen68 insttest68 texinfo2man unquar

clean:
	rm -f -- gen68 insttest68 texinfo2man quar

LINES = ../libsc68/emu68/lines/

gen: gen68
	./gen68 $(GEN68OPT) -- all "$(LINES)"

.PHONY: all clean gen
