# ,----------------------------------------------------------------------.
# |                                                                      |
# |  libsc68 -- Makefile.am -- Use with automake to produce Makefile.in  |
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id: Makefile.am,v 1.2 2007/01/12 09:28:14 benjihan Exp $

BN              = libsc68
ACLOCAL_AMFLAGS = -I m4 --install
SUBDIRS         = io68 emu68 @subdirs@


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

AM_CFLAGS           = @SC68_CFLAGS@ @LIBSC68_CFLAGS@
AM_CPPFLAGS         = @SC68_CPPFLAGS@ @LIBSC68_CPPFLAGS@

lib_LTLIBRARIES     = libsc68.la
libsc68_la_SOURCES  = api68.c conf68.c mixer68.c libsc68.c
libsc68_la_LIBADD   = io68/libio68.la emu68/libemu68.la $(FILE68_LA)
libsc68_la_LDFLAGS  = @LIBSC68_LDFLAGS@


# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

noinst_HEADERS          = conf68.h mixer68.h
pkginclude_HEADERS      = sc68.h
pkgincludedir           = $(includedir)/sc68
