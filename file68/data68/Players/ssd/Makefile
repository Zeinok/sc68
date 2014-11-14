# Makefile for sc68 SSD payer
#
# by Ben / OVR

VERSION=105

AS=vasmm68k_mot
BINFLAGS=-m68000 -Fbin -devpac

## You should not edit after this

all: ssd1.bin

install: ssd1.bin
	@cp -v -- $< ../../Replay

ssd1.bin: ssd1-sc68.s ssd.bin
	@echo "ASM $< -> $@"
	@$(AS) $(BINFLAGS) -DVERSION=$(VERSION) -o $@ $< >/dev/null

ssd.bin: ssd.s
	@echo "ASM $< -> $@ ($(VERSION))"
	@$(AS) $(BINFLAGS) -o $@ $<
	@cp -- $@ ssd-$(VERSION).bin

clean:
	-rm -f -- ssd1.bin ssd.bin

.PHONY: all clean install

.INTERMEDIATE: $(tos)

$(tgt): Makefile
