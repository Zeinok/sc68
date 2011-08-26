# ,----------------------------------------------------------------------.
# |                                                                      |
# |  cgdb68 -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |            Copyright (C) 2006-2009 Benjamin Gerard                   |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# $Id: Makefile.am 102 2009-03-14 17:21:58Z benjihan $
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install

AM_CFLAGS       = $(ALL_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS     = $(ALL_EFLAGS) $(BIN_EFLAGS) $(EFLAGS)
AM_LDFLAGS      = $(ALL_LFLAGS) $(BIN_LFLAGS)
EFLAGS          = $(sc68_inc) $(file68_inc) $(desa68_inc)

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_DESA68
DESA68_LA = $(desa68_dir)/libdesa68.la
@SET_MAKE@
$(desa68_dir)/libdesa68.la:
	cd "$(desa68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libdesa68.la
else
DESA68_LA = 
endif

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA =
endif

bin_PROGRAMS   = cdbg68
cdbg68_SOURCES = cdbg68.c register.c status.c disa.c mem.c cmd.c expr.c
cdbg68_LDADD   = $(SC68_LA) $(DESA68_LA)
noinst_HEADERS = register.h status.h disa.h mem.h cmd.h expr.h
