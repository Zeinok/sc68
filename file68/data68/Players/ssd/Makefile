# Makefile for sc68 SSD payer
#
# by Ben / OVR

VERSION=103
src=ssd_abd4.s

AS=vasmm68k_mot
TOSFLAGS=-m68000 -Ftos -devpac -no-opt
BINFLAGS=-m68000 -Fbin -devpac -no-opt

## You should not edit after this

tos=$(src:%.s=%.tos)
tgt=ssd-$(VERSION).bin

all: ssd1.bin

ssd1.bin: ssd1-sc68.s
	@echo "BIN $< -> $@"
	@$(AS) $(BINFLAGS) -o $@ $< >/dev/null

ssd1.bin: $(tgt)

$(tgt): $(tos)
	@echo "RAW $< -> $@"
	@dd status=none iflag=binary oflag=binary skip=28 bs=1 if=$< of=$@

%.tos: %.s
	@echo "TOS $< -> $@"
	@$(AS) $(TOSFLAGS) -o $@ $< >/dev/null

clean:
	-rm -f --  $(tgt) $(tos) ssd1.bin

.PHONY: all clean

.INTERMEDIATE: $(tos)

$(tgt): Makefile
