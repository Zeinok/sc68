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

SUBDIRS            = io68 emu68 @subdirs@
ACLOCAL_AMFLAGS    = -I m4 --install

AM_CFLAGS          = @LIBSC68_CFLAGS@ @LIBLIBSC68_CFLAGS@
AM_CPPFLAGS        = @LIBSC68_CPPFLAGS@ @LIBLIBSC68_CPPFLAGS@ $(SINCS)
AM_LDFLAGS         = @LIBSC68_LDFLAGS@ @LIBLIBSC68_LDFLAGS@


# ,----------------------------------------------------------------------.
# | script binary                                                        |
# `----------------------------------------------------------------------'

bin_SCRIPTS        = sc68-config


# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	$(MAKE) -C $(file68_dir) libfile68.la
else
FILE68_LA =
endif

SINCS              = -I$(srcdir)/sc68 $(file68_inc)

lib_LTLIBRARIES    = libsc68.la
libsc68_la_SOURCES = api68.c conf68.c mixer68.c libsc68.c
libsc68_la_LIBADD  = io68/libio68.la emu68/libemu68.la $(FILE68_LA)


# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

noinst_HEADERS     = sc68/conf68.h sc68/mixer68.h
pkginclude_HEADERS = sc68/sc68.h
pkgincludedir      = $(includedir)/sc68
