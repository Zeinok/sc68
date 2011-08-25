# ,----------------------------------------------------------------------.
# |                                                                      |
# |  libsc68 -- Makefile.am -- Use with automake to produce Makefile.in  |
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

SUBDIRS         = io68 emu68 $(subdirs)
ACLOCAL_AMFLAGS = -I m4 --install

AM_CFLAGS       = $(ALL_CFLAGS) $(LIB_CFLAGS)
AM_CPPFLAGS     = $(ALL_EFLAGS) $(LIB_EFLAGS) $(EFLAGS)
AM_LDFLAGS      = $(ALL_LFLAGS) $(LIB_LFLAGS) $(LFLAGS)
EFLAGS          = -DSC68_EXPORT=1 -I$(srcdir)/sc68 $(file68_inc)
LFLAGS          = -version-info 3:0:0

# ,----------------------------------------------------------------------.
# | script binary                                                        |
# `----------------------------------------------------------------------'

bin_SCRIPTS        = sc68-config

# ,----------------------------------------------------------------------.
# | pkg-config                                                           |
# `----------------------------------------------------------------------'

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = libsc68.pc

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	cd "$(file68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libfile68.la
else
FILE68_LA =
endif

lib_LTLIBRARIES    = libsc68.la
libsc68_la_SOURCES = api68.c conf68.c mixer68.c libsc68.c
libsc68_la_LIBADD  = io68/libio68.la emu68/libemu68.la $(FILE68_LA)

# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

noinst_HEADERS     = sc68/conf68.h sc68/mixer68.h sc68/trap68.h
pkginclude_HEADERS = sc68/sc68.h
pkgincludedir      = $(includedir)/sc68

# ,----------------------------------------------------------------------.
# | Assemble and transform TOS trap emulator                             |
# `----------------------------------------------------------------------'
$(srcdir)/sc68/trap68.h: $(srcdir)/trapfunc.s
	e=false; $(as68) -p $< -o trapfunc.bin && $(hexdump) -v -e '1/1 "0x%02x,\n" ' trapfunc.bin >$@ && e=true; rm trapfunc.bin; $$e

MAINTAINERCLEANFILES = $(srcdir)/sc68/trap68.h  