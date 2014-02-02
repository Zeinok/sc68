
# Makefile for sc68 tools.
#
# Time-stamp: <2013-09-27 09:51:54 ben>

CFLAGS   = -Wall -pedantic -g -O0

all: gen68 insttest68 texinfo2man quar

clean:
	rm -f -- gen68 insttest68 texinfo2man quar

LINES = ../libsc68/emu68/lines/

gen: gen68
	./gen68 $(GEN68OPT) -- all "$(LINES)"

.PHONY: all clean gen
