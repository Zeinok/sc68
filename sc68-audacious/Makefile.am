# Time-stamp: <2011-08-26 09:41:54 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# | sc68aud -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |                  (C) 2007-2011 Benjamin Gerard                       |
# |           <benjihan -4t- users.sourceforge -d0t- net>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install

libdir               = @PLUGINDIR@

AM_CFLAGS            = $(ALL_CFLAGS) $(LIB_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(ALL_EFLAGS) $(LIB_EFLAGS) $(BIN_EFLAGS) $(EFLAGS)
AM_LDFLAGS           = $(ALL_LFLAGS) $(LIB_LFLAGS) $(BIN_LFLAGS) $(LFLAGS)
EFLAGS               = $(sc68_inc) $(file68_inc)

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA = $(sc68_lib) $(file68_lib)
endif

lib_LTLIBRARIES        = libsc68aud.la
libsc68aud_la_SOURCES  = sc68-audacious.c
libsc68aud_la_LIBADD   = $(SC68_LA)

# libsc68aud_la_CFLAGS   = $(ALL_CFLAGS) $(LIB_CFLAGS)
# libsc68aud_la_CPPFLAGS = $(ALL_EFLAGS) $(LIB_EFLAGS)
# libsc68aud_la_LDFLAGS  = $(ALL_LFLAGS) $(LIB_LFLAGS) $(LFLAGS)
LFLAGS                 = -avoid-version $(NOINSTALL)
